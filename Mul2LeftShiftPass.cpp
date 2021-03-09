#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

using namespace llvm;
using namespace std;

struct Mul2LeftShiftPass : public PassInfoMixin<Mul2LeftShiftPass> {
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {
        for (auto &BB:F) {
            for (auto I = BB.begin(), IE = BB.end(); I != IE; ++I) {
                if (I->getOpcode() == Instruction::Mul) {
                    if (isa<ConstantInt>(I->getOperand(0))) {
                        change(BB, I, I->getOperand(1), dyn_cast<ConstantInt>(I->getOperand(0)));
                    }
                    if (isa<ConstantInt>(I->getOperand(1))) {
                        change(BB, I, I->getOperand(0), dyn_cast<ConstantInt>(I->getOperand(1)));
                    }
                }
            }
        }
        return PreservedAnalyses::all();
    }

public:
    static void change(BasicBlock &BB, BasicBlock::iterator &I, Value *a, ConstantInt *n) {
        auto &constValue = n->getValue();
        IRBuilder<> builder(dyn_cast<BinaryOperator>(I));
        SmallVector<Value *, 64> shifts;
        for (int i = 0; i < constValue.getBitWidth(); i++) {
            if (constValue[i]) {
                auto *bits = ConstantInt::get(I->getType(), i);
                auto *shift = builder.CreateShl(a, bits);
                shifts.emplace_back(shift);
            }
        }
        if (!shifts.empty()) {
            auto *result = shifts[0];
            for (int i = 1; i < shifts.size(); i++) {
                result = builder.CreateAdd(result, shifts[i]);
            }
            ReplaceInstWithValue(BB.getInstList(), I, result);
        }
    }
};

extern "C" LLVM_ATTRIBUTE_WEAK llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {
    return {
            LLVM_PLUGIN_API_VERSION, "Mul2LeftShift",
            LLVM_VERSION_STRING, [](PassBuilder &PB) {
                PB.registerPipelineParsingCallback(
                        [](StringRef Name, FunctionPassManager &FPM,
                           ArrayRef<PassBuilder::PipelineElement>) {
                            if (Name == "mul2leftshift") {
                                FPM.addPass(Mul2LeftShiftPass());
                                return true;
                            }
                            return false;
                        });
            }
    };
}