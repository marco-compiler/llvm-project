//===- SectionPriorities.cpp ----------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// This is based on the ELF port, see ELF/CallGraphSort.cpp for the details
/// about the algorithm.
///
//===----------------------------------------------------------------------===//

#include "SectionPriorities.h"
#include "BPSectionOrderer.h"
#include "Config.h"
#include "InputFiles.h"
#include "Symbols.h"
#include "Target.h"

#include "lld/Common/Args.h"
#include "lld/Common/CommonLinkerContext.h"
#include "lld/Common/ErrorHandler.h"
#include "lld/Common/Utils.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/MapVector.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/TimeProfiler.h"
#include "llvm/Support/raw_ostream.h"

#include <numeric>

using namespace llvm;
using namespace llvm::MachO;
using namespace llvm::sys;
using namespace lld;
using namespace lld::macho;

PriorityBuilder macho::priorityBuilder;

namespace {
struct Edge {
  int from;
  uint64_t weight;
};

struct Cluster {
  Cluster(int sec, size_t s) : next(sec), prev(sec), size(s) {}

  double getDensity() const {
    if (size == 0)
      return 0;
    return double(weight) / double(size);
  }

  int next;
  int prev;
  uint64_t size;
  uint64_t weight = 0;
  uint64_t initialWeight = 0;
  Edge bestPred = {-1, 0};
};

class CallGraphSort {
public:
  CallGraphSort(const MapVector<SectionPair, uint64_t> &profile);

  DenseMap<const InputSection *, int> run();

private:
  std::vector<Cluster> clusters;
  std::vector<const InputSection *> sections;
};
// Maximum amount the combined cluster density can be worse than the original
// cluster to consider merging.
constexpr int MAX_DENSITY_DEGRADATION = 8;
} // end anonymous namespace

// Take the edge list in callGraphProfile, resolve symbol names to Symbols, and
// generate a graph between InputSections with the provided weights.
CallGraphSort::CallGraphSort(const MapVector<SectionPair, uint64_t> &profile) {
  DenseMap<const InputSection *, int> secToCluster;

  auto getOrCreateCluster = [&](const InputSection *isec) -> int {
    auto res = secToCluster.try_emplace(isec, clusters.size());
    if (res.second) {
      sections.push_back(isec);
      clusters.emplace_back(clusters.size(), isec->getSize());
    }
    return res.first->second;
  };

  // Create the graph
  for (const std::pair<SectionPair, uint64_t> &c : profile) {
    const auto fromSec = c.first.first->canonical();
    const auto toSec = c.first.second->canonical();
    uint64_t weight = c.second;
    // Ignore edges between input sections belonging to different output
    // sections.  This is done because otherwise we would end up with clusters
    // containing input sections that can't actually be placed adjacently in the
    // output.  This messes with the cluster size and density calculations.  We
    // would also end up moving input sections in other output sections without
    // moving them closer to what calls them.
    if (fromSec->parent != toSec->parent)
      continue;

    int from = getOrCreateCluster(fromSec);
    int to = getOrCreateCluster(toSec);

    clusters[to].weight += weight;

    if (from == to)
      continue;

    // Remember the best edge.
    Cluster &toC = clusters[to];
    if (toC.bestPred.from == -1 || toC.bestPred.weight < weight) {
      toC.bestPred.from = from;
      toC.bestPred.weight = weight;
    }
  }
  for (Cluster &c : clusters)
    c.initialWeight = c.weight;
}

// It's bad to merge clusters which would degrade the density too much.
static bool isNewDensityBad(Cluster &a, Cluster &b) {
  double newDensity = double(a.weight + b.weight) / double(a.size + b.size);
  return newDensity < a.getDensity() / MAX_DENSITY_DEGRADATION;
}

// Find the leader of V's belonged cluster (represented as an equivalence
// class). We apply union-find path-halving technique (simple to implement) in
// the meantime as it decreases depths and the time complexity.
static int getLeader(std::vector<int> &leaders, int v) {
  while (leaders[v] != v) {
    leaders[v] = leaders[leaders[v]];
    v = leaders[v];
  }
  return v;
}

static void mergeClusters(std::vector<Cluster> &cs, Cluster &into, int intoIdx,
                          Cluster &from, int fromIdx) {
  int tail1 = into.prev, tail2 = from.prev;
  into.prev = tail2;
  cs[tail2].next = intoIdx;
  from.prev = tail1;
  cs[tail1].next = fromIdx;
  into.size += from.size;
  into.weight += from.weight;
  from.size = 0;
  from.weight = 0;
}

// Group InputSections into clusters using the Call-Chain Clustering heuristic
// then sort the clusters by density.
DenseMap<const InputSection *, int> CallGraphSort::run() {
  const uint64_t maxClusterSize = target->getPageSize();

  // Cluster indices sorted by density.
  std::vector<int> sorted(clusters.size());
  // For union-find.
  std::vector<int> leaders(clusters.size());

  std::iota(leaders.begin(), leaders.end(), 0);
  std::iota(sorted.begin(), sorted.end(), 0);

  llvm::stable_sort(sorted, [&](int a, int b) {
    return clusters[a].getDensity() > clusters[b].getDensity();
  });

  for (int l : sorted) {
    // The cluster index is the same as the index of its leader here because
    // clusters[L] has not been merged into another cluster yet.
    Cluster &c = clusters[l];

    // Don't consider merging if the edge is unlikely.
    if (c.bestPred.from == -1 || c.bestPred.weight * 10 <= c.initialWeight)
      continue;

    int predL = getLeader(leaders, c.bestPred.from);
    // Already in the same cluster.
    if (l == predL)
      continue;

    Cluster *predC = &clusters[predL];
    if (c.size + predC->size > maxClusterSize)
      continue;

    if (isNewDensityBad(*predC, c))
      continue;

    leaders[l] = predL;
    mergeClusters(clusters, *predC, predL, c, l);
  }
  // Sort remaining non-empty clusters by density.
  sorted.clear();
  for (int i = 0, e = (int)clusters.size(); i != e; ++i)
    if (clusters[i].size > 0)
      sorted.push_back(i);
  llvm::stable_sort(sorted, [&](int a, int b) {
    return clusters[a].getDensity() > clusters[b].getDensity();
  });

  DenseMap<const InputSection *, int> orderMap;

  // Sections will be sorted by decreasing order. Absent sections will have
  // priority 0 and be placed at the end of sections.
  int curOrder = -clusters.size();
  for (int leader : sorted) {
    for (int i = leader;;) {
      orderMap[sections[i]] = curOrder++;
      i = clusters[i].next;
      if (i == leader)
        break;
    }
  }
  if (!config->printSymbolOrder.empty()) {
    std::error_code ec;
    raw_fd_ostream os(config->printSymbolOrder, ec, sys::fs::OF_None);
    if (ec) {
      error("cannot open " + config->printSymbolOrder + ": " + ec.message());
      return orderMap;
    }
    // Print the symbols ordered by C3, in the order of decreasing curOrder
    // Instead of sorting all the orderMap, just repeat the loops above.
    for (int leader : sorted)
      for (int i = leader;;) {
        const InputSection *isec = sections[i];
        // Search all the symbols in the file of the section
        // and find out a Defined symbol with name that is within the
        // section.
        for (Symbol *sym : isec->getFile()->symbols) {
          if (auto *d = dyn_cast_or_null<Defined>(sym)) {
            if (d->isec() == isec)
              os << sym->getName() << "\n";
          }
        }
        i = clusters[i].next;
        if (i == leader)
          break;
      }
  }

  return orderMap;
}

std::optional<int>
macho::PriorityBuilder::getSymbolOrCStringPriority(const StringRef key,
                                                   InputFile *f) {

  auto it = priorities.find(key);
  if (it == priorities.end())
    return std::nullopt;
  const SymbolPriorityEntry &entry = it->second;
  if (!f)
    return entry.anyObjectFile;
  // We don't use toString(InputFile *) here because it returns the full path
  // for object files, and we only want the basename.
  StringRef filename;
  if (f->archiveName.empty())
    filename = path::filename(f->getName());
  else
    filename = saver().save(path::filename(f->archiveName) + "(" +
                            path::filename(f->getName()) + ")");
  return std::min(entry.objectFiles.lookup(filename), entry.anyObjectFile);
}

std::optional<int>
macho::PriorityBuilder::getSymbolPriority(const Defined *sym) {
  if (sym->isAbsolute())
    return std::nullopt;
  return getSymbolOrCStringPriority(utils::getRootSymbol(sym->getName()),
                                    sym->isec()->getFile());
}

void macho::PriorityBuilder::extractCallGraphProfile() {
  TimeTraceScope timeScope("Extract call graph profile");
  bool hasOrderFile = !priorities.empty();
  for (const InputFile *file : inputFiles) {
    auto *obj = dyn_cast_or_null<ObjFile>(file);
    if (!obj)
      continue;
    for (const CallGraphEntry &entry : obj->callGraph) {
      assert(entry.fromIndex < obj->symbols.size() &&
             entry.toIndex < obj->symbols.size());
      auto *fromSym = dyn_cast_or_null<Defined>(obj->symbols[entry.fromIndex]);
      auto *toSym = dyn_cast_or_null<Defined>(obj->symbols[entry.toIndex]);
      if (fromSym && toSym &&
          (!hasOrderFile ||
           (!getSymbolPriority(fromSym) && !getSymbolPriority(toSym))))
        callGraphProfile[{fromSym->isec(), toSym->isec()}] += entry.count;
    }
  }
}

void macho::PriorityBuilder::parseOrderFile(StringRef path) {
  assert(callGraphProfile.empty() &&
         "Order file must be parsed before call graph profile is processed");
  std::optional<MemoryBufferRef> buffer = readFile(path);
  if (!buffer) {
    error("Could not read order file at " + path);
    return;
  }

  int prio = std::numeric_limits<int>::min();
  MemoryBufferRef mbref = *buffer;
  for (StringRef line : args::getLines(mbref)) {
    StringRef objectFile, symbolOrCStrHash;
    line = line.take_until([](char c) { return c == '#'; }); // ignore comments
    line = line.ltrim();

    CPUType cpuType = StringSwitch<CPUType>(line)
                          .StartsWith("i386:", CPU_TYPE_I386)
                          .StartsWith("x86_64:", CPU_TYPE_X86_64)
                          .StartsWith("arm:", CPU_TYPE_ARM)
                          .StartsWith("arm64:", CPU_TYPE_ARM64)
                          .StartsWith("ppc:", CPU_TYPE_POWERPC)
                          .StartsWith("ppc64:", CPU_TYPE_POWERPC64)
                          .Default(CPU_TYPE_ANY);

    if (cpuType != CPU_TYPE_ANY && cpuType != target->cpuType)
      continue;
    // Drop the CPU type as well as the colon
    if (cpuType != CPU_TYPE_ANY)
      line = line.drop_until([](char c) { return c == ':'; }).drop_front();

    constexpr std::array<StringRef, 2> fileEnds = {".o:", ".o):"};
    for (StringRef fileEnd : fileEnds) {
      size_t pos = line.find(fileEnd);
      if (pos != StringRef::npos) {
        // Split the string around the colon
        objectFile = line.take_front(pos + fileEnd.size() - 1);
        line = line.drop_front(pos + fileEnd.size());
        break;
      }
    }

    // The rest of the line is either <symbol name> or
    // CStringEntryPrefix<cstring hash>
    line = line.trim();
    if (line.starts_with(CStringEntryPrefix)) {
      StringRef possibleHash = line.drop_front(CStringEntryPrefix.size());
      uint32_t hash = 0;
      if (to_integer(possibleHash, hash))
        symbolOrCStrHash = possibleHash;
    } else
      symbolOrCStrHash = utils::getRootSymbol(line);

    if (!symbolOrCStrHash.empty()) {
      SymbolPriorityEntry &entry = priorities[symbolOrCStrHash];
      if (!objectFile.empty())
        entry.objectFiles.insert(std::make_pair(objectFile, prio));
      else
        entry.anyObjectFile = std::min(entry.anyObjectFile, prio);
    }

    ++prio;
  }
}

DenseMap<const InputSection *, int>
macho::PriorityBuilder::buildInputSectionPriorities() {
  DenseMap<const InputSection *, int> sectionPriorities;
  if (config->bpStartupFunctionSort || config->bpFunctionOrderForCompression ||
      config->bpDataOrderForCompression) {
    TimeTraceScope timeScope("Balanced Partitioning Section Orderer");
    sectionPriorities = runBalancedPartitioning(
        config->bpStartupFunctionSort ? config->irpgoProfilePath : "",
        config->bpFunctionOrderForCompression,
        config->bpDataOrderForCompression,
        config->bpCompressionSortStartupFunctions,
        config->bpVerboseSectionOrderer);
  } else if (config->callGraphProfileSort) {
    // Sort sections by the profile data provided by __LLVM,__cg_profile
    // sections.
    //
    // This first builds a call graph based on the profile data then merges
    // sections according to the C³ heuristic. All clusters are then sorted by a
    // density metric to further improve locality.
    TimeTraceScope timeScope("Call graph profile sort");
    sectionPriorities = CallGraphSort(callGraphProfile).run();
  }

  if (priorities.empty())
    return sectionPriorities;

  auto addSym = [&](const Defined *sym) {
    std::optional<int> symbolPriority = getSymbolPriority(sym);
    if (!symbolPriority)
      return;
    int &priority = sectionPriorities[sym->isec()];
    priority = std::min(priority, *symbolPriority);
  };

  // TODO: Make sure this handles weak symbols correctly.
  for (const InputFile *file : inputFiles) {
    if (isa<ObjFile>(file))
      for (Symbol *sym : file->symbols)
        if (auto *d = dyn_cast_or_null<Defined>(sym))
          addSym(d);
  }

  return sectionPriorities;
}

std::vector<StringPiecePair> macho::PriorityBuilder::buildCStringPriorities(
    ArrayRef<CStringInputSection *> inputs) {
  // Split the input strings into hold and cold sets.
  // Order hot set based on -order_file_cstring for performance improvement;
  // TODO: Order cold set of cstrings for compression via BP.
  std::vector<std::pair<int, StringPiecePair>>
      hotStringPrioritiesAndStringPieces;
  std::vector<StringPiecePair> coldStringPieces;
  std::vector<StringPiecePair> orderedStringPieces;

  for (CStringInputSection *isec : inputs) {
    for (const auto &[stringPieceIdx, piece] : llvm::enumerate(isec->pieces)) {
      if (!piece.live)
        continue;

      std::optional<int> priority = getSymbolOrCStringPriority(
          std::to_string(piece.hash), isec->getFile());
      if (!priority)
        coldStringPieces.emplace_back(isec, stringPieceIdx);
      else
        hotStringPrioritiesAndStringPieces.emplace_back(
            *priority, std::make_pair(isec, stringPieceIdx));
    }
  }

  // Order hot set for perf
  llvm::stable_sort(hotStringPrioritiesAndStringPieces);
  for (auto &[priority, stringPiecePair] : hotStringPrioritiesAndStringPieces)
    orderedStringPieces.push_back(stringPiecePair);

  // TODO: Order cold set for compression

  orderedStringPieces.insert(orderedStringPieces.end(),
                             coldStringPieces.begin(), coldStringPieces.end());

  return orderedStringPieces;
}
