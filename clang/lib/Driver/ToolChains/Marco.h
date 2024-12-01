//===--- Flang.h - Flang Tool and ToolChain Implementations ====-*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_MARCO_H
#define LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_MARCO_H

#include "clang/Driver/Action.h"
#include "clang/Driver/Compilation.h"
#include "clang/Driver/Tool.h"
#include "clang/Driver/ToolChain.h"
#include "llvm/Option/ArgList.h"
#include "llvm/Support/Compiler.h"

namespace clang {
namespace driver {

namespace tools {

/// Marco compiler tool.
class LLVM_LIBRARY_VISIBILITY Marco : public Tool {
private:
  /// Extract marco dialect options from the driver arguments and add them to
  /// the list of arguments for the generated command/job.
  ///
  /// \param [in] Args The list of input driver arguments
  /// \param [out] CmdArgs The list of output command arguments
  void addMarcoOptions(const llvm::opt::ArgList &Args,
                       llvm::opt::ArgStringList &CmdArgs) const;

public:
  Marco(const ToolChain &TC);
  ~Marco() override;

  bool hasGoodDiagnostics() const override { return true; }
  bool hasIntegratedAssembler() const override { return false; }
  bool hasIntegratedBackend() const override { return false; }
  bool canEmitIR() const override { return true; }
  bool hasIntegratedCPP() const override { return false; }

  void ConstructJob(Compilation &C, const JobAction &JA,
                    const InputInfo &Output, const InputInfoList &Inputs,
                    const llvm::opt::ArgList &TCArgs,
                    const char *LinkingOutput) const override;
};

} // end namespace tools

} // end namespace driver
} // end namespace clang

#endif // LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_MARCO_H
