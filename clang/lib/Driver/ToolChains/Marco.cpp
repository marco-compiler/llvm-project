//===-- Marco.cpp - Marco+LLVM ToolChain Implementations --------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Marco.h"
#include "CommonArgs.h"

#include "clang/Driver/Options.h"

#include <cassert>
#include <iostream>
#include <ostream>

using namespace clang::driver;
using namespace clang::driver::tools;
using namespace clang;
using namespace llvm::opt;

Marco::Marco(const ToolChain &TC) : Tool("marco", "marco frontend", TC) {}

Marco::~Marco() {}

void Marco::addMarcoOptions(const ArgList &Args, ArgStringList &CmdArgs) const {
  Args.addAllArgs(CmdArgs, {options::OPT_g_Flag,
                            options::OPT_mcmodel_EQ,
                            options::OPT_mlarge_data_threshold_EQ,
                            options::OPT_meabi,
                            options::OPT_tune_cpu,
                            options::OPT_target_abi,
                            options::OPT_target_sdk_version_EQ,
                            options::OPT_darwin_target_variant_sdk_version_EQ,
                            options::OPT_darwin_target_variant_triple,
                            options::OPT_target_cpu,
                            options::OPT_target_feature,
                            options::OPT_triple,
                            options::OPT_target_linker_version,
                            options::OPT_triple_EQ,
                            options::OPT_mfpmath,
                            options::OPT_fpadding_on_unsigned_fixed_point,
                            options::OPT_Xomc,
                            options::OPT_omc_path,
                            options::OPT_omc_bypass,
                            options::OPT_model,
                            options::OPT_solver,
                            options::OPT_variable_filter,
                            options::OPT_bit_width});
}

void Marco::ConstructJob(Compilation &C, const JobAction &JA,
                         const InputInfo &Output, const InputInfoList &Inputs,
                         const ArgList &Args, const char *LinkingOutput) const {
  const auto &TC = getToolChain();
  const Driver &D = TC.getDriver();
  const llvm::Triple &Triple = TC.getEffectiveTriple();
  const std::string &TripleStr = Triple.getTriple();

  ArgStringList CmdArgs;

  // Invoke ourselves in -mc1 mode.
  CmdArgs.push_back("-mc1");

  if (isa<CompileJobAction>(JA)) {
    if (JA.getType() == types::TY_AST) {
      CmdArgs.push_back("-emit-ast");
    } else if (JA.getType() == types::TY_BaseModelica) {
      CmdArgs.push_back("-emit-base-modelica");
    } else if (JA.getType() == types::TY_MLIR) {
      CmdArgs.push_back("-emit-mlir");
    } else if (JA.getType() == types::TY_MLIR_Modelica) {
      CmdArgs.push_back("-emit-mlir-modelica");
    } else if (JA.getType() == types::TY_MLIR_LLVM) {
      CmdArgs.push_back("-emit-mlir-llvm");
    } else if (JA.getType() == types::TY_LLVM_IR ||
               JA.getType() == types::TY_LTO_IR) {
      CmdArgs.push_back("-emit-llvm");
    } else if (JA.getType() == types::TY_LLVM_BC ||
               JA.getType() == types::TY_LTO_BC) {
      CmdArgs.push_back("-emit-llvm-bc");
    } else {
      // Assembly files can also be emitted by the frontend, but the task is
      // left to clang to reuse its optimizations.
      llvm_unreachable("Unexpected output type!");
    }
  } else {
    // The Backend and Assemble actions are not checked as they should be
    // handled by clang, even though marco is theoretically capable of doing it
    // as well.
    llvm_unreachable("Unexpected action class for marco tool.");
  }

  addMarcoOptions(Args, CmdArgs);

  // Color diagnostics are parsed by the driver directly from argv and later
  // re-parsed to construct this job; claim any possible color diagnostic here
  // to avoid warn_drv_unused_argument.
  Args.getLastArg(options::OPT_fcolor_diagnostics,
                  options::OPT_fno_color_diagnostics);

  if (D.getDiags().getDiagnosticOptions().ShowColors)
    CmdArgs.push_back("-fcolor-diagnostics");

  // Forward -mllvm options to the LLVM option parser. In practice, this means
  // forwarding to `-mc1` as that's where the LLVM parser is run.
  for (const Arg *A : Args.filtered(options::OPT_mllvm)) {
    A->claim();
    A->render(Args, CmdArgs);
  }

  for (const Arg *A : Args.filtered(options::OPT_mmlir)) {
    A->claim();
    A->render(Args, CmdArgs);
  }

  // Optimization level for CodeGen.
  if (const Arg *A = Args.getLastArg(options::OPT_O_Group)) {
    if (A->getOption().matches(options::OPT_O4)) {
      CmdArgs.push_back("-O3");
      D.Diag(diag::warn_O4_is_O3);
    } else {
      A->render(Args, CmdArgs);
    }
  }

  // Target-specific arguments.
  CmdArgs.push_back("-triple");
  CmdArgs.push_back(Args.MakeArgString(TripleStr));

  std::string CPU = getCPUName(D, Args, Triple, false);

  if (!CPU.empty()) {
    CmdArgs.push_back("-target-cpu");
    CmdArgs.push_back(Args.MakeArgString(CPU));
  }

  getTargetFeatures(D, Triple, Args, CmdArgs, false);

  // Output file.
  if (Output.isFilename()) {
    CmdArgs.push_back("-o");
    CmdArgs.push_back(Output.getFilename());
  } else {
    assert(Output.isNothing() && "Invalid output.");
  }

  // Forward -Xmarco arguments to -mc1.
  for (auto *Arg : Args.filtered(options::OPT_Xmarco)) {
    Arg->claim();
    CmdArgs.push_back(Arg->getValue());
  }

  // Input files.
  for (auto &input : Inputs) {
    CmdArgs.push_back(input.getFilename());
  }

  // Build the command.
  const char *Exec = Args.MakeArgString(D.GetProgramPath("marco", TC));
  C.addCommand(std::make_unique<Command>(JA, *this,
                                         ResponseFileSupport::AtFileUTF8(),
                                         Exec, CmdArgs, Inputs, Output));
}
