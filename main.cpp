#include "llvm/ADT/APInt.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/Target/TargetMachine.h"
#include <memory>
using namespace llvm;

static std::unique_ptr<LLVMContext> TheContext;
static std::unique_ptr<Module> TheModule;
static std::unique_ptr<IRBuilder<>> Builder;

static void InitializeModule() {
  // Open a new context and module.
  TheContext = std::make_unique<LLVMContext>();
  TheModule = std::make_unique<Module>("top", *TheContext);

  // to execute on M2
  TheModule->setDataLayout(StringRef("e-m:o-i64:64-i128:128-n32:64-S128"));
  TheModule->setTargetTriple(StringRef("arm64-apple-macosx13.0.0"));
  TheModule->setSDKVersion(VersionTuple(13, 1));

  // Create a new builder for the module.
  Builder = std::make_unique<IRBuilder<>>(*TheContext);
}
int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "%s: invalid number of arguments\n", argv[0]);
    return 1;
  }
  InitializeModule();
  Function *main = Function::Create(
      FunctionType::get(Type::getInt32Ty(*TheContext), false),
      Function::ExternalLinkage, Twine("main"), TheModule.get());
  Builder->SetInsertPoint(BasicBlock::Create(*TheContext, "", main));
  Builder->CreateRet(
      ConstantInt::get(*TheContext, APInt(32, atoi(argv[1]), true)));
  TheModule->print(outs(), nullptr);
}
