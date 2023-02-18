#include "llvm/ADT/APInt.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include <algorithm>
#include <memory>
#include <utility>
using namespace llvm;

static std::unique_ptr<LLVMContext> TheContext;
static std::unique_ptr<Module> TheModule;
static std::unique_ptr<IRBuilder<>> Builder;

static void InitializeModule() {
  // Open a new context and module.
  TheContext = std::make_unique<LLVMContext>();
  TheModule = std::make_unique<Module>("top", *TheContext);

  // Initialize the target registry etc.
  InitializeNativeTarget();
  auto TargetTriple = sys::getDefaultTargetTriple();
  TheModule->setTargetTriple(TargetTriple);
  std::string Error;
  auto Target = TargetRegistry::lookupTarget(TargetTriple, Error);
  if (!Target) {
    errs() << Error;
    exit(1);
  }
  TargetOptions opt;
  auto RM = Optional<Reloc::Model>();
  auto TargetMachine = Target->createTargetMachine(
      TargetTriple, /*CPU=*/"generic", /*Features=*/"", opt, RM);
  TheModule->setDataLayout(TargetMachine->createDataLayout());
  // Create a new builder for the module.
  Builder = std::make_unique<IRBuilder<>>(*TheContext);
}
int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "%s: invalid number of arguments\n", argv[0]);
    return 1;
  }
  char *p = argv[1];
  InitializeModule();
  Function *main = Function::Create(
      FunctionType::get(Type::getInt32Ty(*TheContext), false),
      Function::ExternalLinkage, Twine("main"), TheModule.get());
  Builder->SetInsertPoint(BasicBlock::Create(*TheContext, "", main));
  Builder->SetInsertPoint(BasicBlock::Create(*TheContext, "second", main));
  Builder->CreateRet(Builder->getInt32(atoi(argv[1])));
  TheModule->print(outs(), nullptr);
}
