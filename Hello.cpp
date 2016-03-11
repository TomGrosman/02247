//===- Hello.cpp - Example code from "Writing an LLVM Pass" ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements two versions of the LLVM "Hello World" pass described
// in docs/WritingAnLLVMPass.html
//
//===----------------------------------------------------------------------===//

#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Attributes.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/IR/BasicBlock.h"
#include <llvm/IR/DebugInfo.h>

using namespace llvm;

#define DEBUG_TYPE "hello"

// STATISTIC(HelloCounter, "Counts number of functions greeted");

namespace {
    struct PrintfCounter : public FunctionPass {
        static char ID;
        int CallCount = 0;
        PrintfCounter() : FunctionPass(ID) {}
        ~PrintfCounter() {
            errs() << "\n" << CallCount << " printf functions were called.\n";
        }
        
        virtual bool runOnFunction(Function &F) override {
            
            for (auto &B :  F) {
                for (auto &I : B) {
                    if (isa<CallInst> (I)) {
                        
                        CallInst *op = dyn_cast<CallInst>(&I);
                        Value *V  =	op->getCalledValue ();
                        if (V->getName() == "printf") {
                            CallCount++;
                        }
                    }
                }
            }
            return false;  //No change to code
        }
    };
}
char PrintfCounter::ID = 0;static RegisterPass<PrintfCounter> W("printfcounter","Printf Counter");



namespace {
    struct PrintfCounter2 : public FunctionPass {
        static char ID;
        PrintfCounter2() : FunctionPass(ID) {}
        
        // We don't modify the program, so we preserve all analyses.
        void getAnalysisUsage(AnalysisUsage &AU) const override {
            AU.addRequired<LoopInfoWrapperPass>();
            AU.addRequired<ScalarEvolutionWrapperPass>();
            //            AU.addRequired<ScalarEvolution>();
            AU.setPreservesAll();
            
        }
        
        int getLoopMultiplier(Loop* loop){
            if (loop == NULL){
                return 1;
            }
            
            //The following depends on your version of llvm sources
            ScalarEvolution &SE = getAnalysis<ScalarEvolutionWrapperPass>().getSE();
            //ScalarEvolution &SE = getAnalysis<ScalarEvolution>();
            
            return SE.getSmallConstantTripCount(loop) * getLoopMultiplier(loop->getParentLoop());
            // the second part is a recursive call for nested loops proper count
            
        }
        
        virtual bool runOnFunction(Function &F) override {
            
            LoopInfo &LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
            int CallCount = 0;
            
            
            for (auto &B :  F) {
                for (auto &I : B) {
                    if (isa<CallInst> (I)) {
                        
                        CallInst *op = dyn_cast<CallInst>(&I);
                        Value *V  =	op->getCalledValue ();
                        if (V->getName() == "printf") {
                            Loop* loop = LI.getLoopFor(&B);
                            CallCount +=getLoopMultiplier(loop);
                            
                        }
                        
                    }
                }
            }
            errs() << CallCount << " printf were called \n";
            
            return false;  //No change to code
        }
    };
}

char PrintfCounter2::ID = 0;static RegisterPass<PrintfCounter2> U("printfcounter2","Printf Counter2");

// should be run with different command ::: opt -mem2reg -loop-rotate -load /Users/Ehsan/Desktop/files/src/build/Debug/lib/LLVMHello.dylib -printfcounter2 <hello.bc> /dev/null
//loop-rotate will deduct the extra count in each loop

namespace {
    struct CopyPaste : public FunctionPass {
        static char ID;
        int CallCount = 0;
        Instruction *SavedInstruction=NULL;
        
        CopyPaste() : FunctionPass(ID) {}
        
        
        virtual bool runOnFunction(Function &F) override {
            
            for (auto &B :  F) {
                for (auto &I : B) {
                    
                    if(SavedInstruction!=NULL && I.isIdenticalTo(SavedInstruction) ){
                        
                        // use -g option with clang to get line info
                        if (DILocation *Loc = I.getDebugLoc()) { // Here I is an LLVM instruction
                            unsigned Line = Loc->getLine();
                            //                            StringRef File = Loc->getFilename();
                            //                            StringRef Dir = Loc->getDirectory();
                            errs()<< "Lines " << Line -1  << " and " << Line  << " are identical:" << "\n";
                        }
                        else { // no debug info
                            errs() << " Identical Lines found. No line number info available:\n";
                        }
                        I.dump();
                        errs() << "\n";
                    }
                    SavedInstruction = &I;
                    if (isa<CallInst> (I)) {
                        
                        CallInst *op = dyn_cast<CallInst>(&I);
                        Value *V  =	op->getCalledValue ();
                        if (V->getName() == "printf") {
                            CallCount++;
                        }
                    }
                }
            }
            return false;  //No change to code
        }
    };
}
char CopyPaste::ID = 0;static RegisterPass<CopyPaste> S("copypaste","CopyPaste detection");


// Find IF THEN ELSE expressions where the IF Block is identical to the THEN block.
namespace {
    struct CopyPaste2 : public FunctionPass {
        
        static char ID;
        BasicBlock  *IFBlock = NULL;
        BasicBlock  *ELSEBlock = NULL;
        
        CopyPaste2() : FunctionPass(ID) {}
        
        virtual bool runOnFunction(Function &F) override {
            bool FirstBlockInFunction = true;   // Prime the pump
            
            for (auto &B :  F) {
                
                if (not FirstBlockInFunction)
                    IFBlock = ELSEBlock;
                ELSEBlock=&B;
                
                if (FirstBlockInFunction)               // Nothing to Compare since this is the first block in fuction
                    FirstBlockInFunction = false;
                else {                                 //Check for IF statement Control Flow
                    if ( (ELSEBlock->getUniquePredecessor() == IFBlock->getUniquePredecessor() ) &&
                        (ELSEBlock->getUniqueSuccessor() == IFBlock->getUniqueSuccessor())){
                        
                        errs() << "IF THEN ELSE FOUND, IF BlOCKSIZE = " << IFBlock->size() << " ELSE BLOCKSIZE = " << ELSEBlock->size() << "...  Checking contents  " << "\n";
                        
                        if (ELSEBlock->size()!=IFBlock->size()) {
                            errs() << "IF and ELSE are not identical \n";
                        }
                        else{
                            BasicBlock::iterator iIFBlock = IFBlock->begin();
                            BasicBlock::iterator IELSEBlock = ELSEBlock->begin();
                            
                            //iIFBlock->dump();
                            
                            
                            //WHY DOESN'T THIS WORK?    bool identical = (IELSEBlock)->isIdenticalTo(*iIFBlock);
                            bool identical = true;
                            
                            bool stillSearching = true;
                            while (stillSearching) {
                                iIFBlock++;
                                IELSEBlock++;
                                stillSearching = (  (iIFBlock != IFBlock->end() ) && (IELSEBlock != ELSEBlock->end() ) );
                                //SAME PROBLEM                  identical = (IELSEBlock)->isIdenticalTo(*iIFBlock);
                                identical = true;
                            }
                            
                            // use -g option with clang to get line info
                            if (DILocation *Loc = (*iIFBlock).getDebugLoc()) { // Here I is an LLVM instruction
                                unsigned Line = Loc->getLine();
                                //                            StringRef File = Loc->getFilename();
                                //                            StringRef Dir = Loc->getDirectory();
                                errs()<< "Lines " << Line << " are identical:" << "\n";
                            }
                            else { // no debug info
                                errs() << " Identical Blocks found. No line number info available:\n";
                            }
                        }
                    }
                }
            }
            return false; // Does not change code
        }
    };
}
char CopyPaste2::ID = 0;static RegisterPass<CopyPaste2> A("copypaste2","CopyPaste2 detection");

