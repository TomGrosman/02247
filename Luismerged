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
#include "llvm/available-support.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/IR/InstIterator.h"
#include <vector>

using namespace llvm;
using namespace std;

namespace {
    
    int BlockTableIteratorCounter = 0;
    class BlockHolder {
        
    public:
        int startThenBlock, startElseBlock, blockLength;
        
        BlockHolder(int startThen, int startElse, int length) {
            startThenBlock = startThen;
            startElseBlock = startElse;
            blockLength = length;
        }
        
        void printTable() {
            errs() << "start THEN line: " << startThenBlock << "  start ELSE line: "
            << startElseBlock << "  Block Length: " << blockLength << "\n";
            
        }
    };
    
    int thenFirstLine, elseFirstline;
    
    std::map<int, BlockHolder> BlockTable;
    
    // vector to hold the operands in a sourceline
    typedef std::vector<Value*> LineInfo;
    
    enum CompareType {
        operands, operators, operandsANDoperators
    };
    
    class ProcessedLine {
    public:
        int lineNumber;             // source line number
        std::string functionName;        // function where the line is found
        LineInfo lineOperands;      // all operands in the sourceline
        std::string lineOperators;
        StringRef lineConstant;
        
        ProcessedLine(int ln, std::string fn, LineInfo lo, std::string lop = "",
                      StringRef co = "") {
            lineNumber = ln;
            functionName = fn;
            lineOperands = lo;
            lineOperators = lop;
            lineConstant = co;
        }
        //method to identify fully identical lines , considering operands and operators
        bool compareLines(ProcessedLine compareTo) {
            if (lineOperands == compareTo.lineOperands
                && lineOperators == compareTo.lineOperators) {
                return true;
            } else
                //errs() << "Lines " << lineNumber << " and " << compareTo.lineNumber << "are not Identical. \n";
                return false;
        }
        
        //function to detect more complicated patterns.refer to section 4.2 in pdf a4 from the lecture
        //first Example
        bool compareLinesStructural(ProcessedLine compareTo) {
            if (lineOperands.size() == compareTo.lineOperands.size()
                && lineOperators == compareTo.lineOperators)
                //errs() << "Lines " << lineNumber << " and " << compareTo.lineNumber << "are structuraly identical. \n";
                return true;
            else
                return false;
        }
        
        // utility method to print out a ProcessedLine object
        void printALine() {
            errs() << "Source Line=>" << lineNumber << "  Function=>"
            << functionName << "  Operands=> \n";
            for (const auto &p : lineOperands) {
                errs() << *p << "\n";
            }
            errs() << "\n";
        }
        
        // utility method to print out a operators of a line
        void printOperatorsLine() {
            errs() << "Source Line=>" << lineNumber << "  Operators=> \n";
            for (const auto &p : lineOperators) {
                errs() << p;
            }
            errs() << "\n";
        }
    };
    struct CopyPaste5: public ModulePass {
        
        static char ID;
        
        CopyPaste5() :
        ModulePass(ID) {
        }
        
        const Function* findEnclosingFunc(const Value* V) {
            if (const Argument* Arg = dyn_cast < Argument > (V)) {
                return Arg->getParent();
            }
            if (const Instruction* I = dyn_cast < Instruction > (V)) {
                return I->getParent()->getParent();
            }
            return NULL;
        }
        
        const DIVariable* findVar(const Value* V, const Function* F) {
            for (const_inst_iterator Iter = inst_begin(F), End = inst_end(F);
                 Iter != End; ++Iter) {
                const Instruction* I = &*Iter;
                if (const DbgDeclareInst* DbgDeclare = dyn_cast < DbgDeclareInst
                    > (I)) {
                    if (DbgDeclare->getAddress() == V)
                        return DbgDeclare->getVariable();
                } else if (const DbgValueInst* DbgValue = dyn_cast < DbgValueInst
                           > (I)) {
                    if (DbgValue->getValue() == V)
                        return DbgValue->getVariable();
                }
            }
            return NULL;
        }        
        StringRef getOriginalName(const Value* V) {
            const Function* F = findEnclosingFunc(V);
            if (!F)
                return V->getName();
            const DIVariable* Var = findVar(V, F);
            if (Var != NULL)
                return Var->getName();
            return Var->getName();
        }
        
        void compareRangesForOperands(int start1, int lenght, int start2,
                                      std::map<int, ProcessedLine> testMap, CompareType type) {
            float counter = 0.0;
            for (int i = 0; i < lenght; i++)
                if (testMap.at(start1 + i).lineOperands
                    == testMap.at(start2 + i).lineOperands)
                    counter++;
            errs() << "Ranges of lines beginning at " << start1 << " and " << start2
            << " are Identical " << (int) (counter / ((float) lenght) * 100)
            << "% operand wise. " "\n";
        }
        
        void compareRangesForOperators(int start1, int lenght, int start2,
                                       std::map<int, ProcessedLine> testMap, CompareType type) {
            float counter = 0.0;
            for (int i = 0; i < lenght; i++)
                if (testMap.at(start1 + i).lineOperators
                    == testMap.at(start2 + i).lineOperators)
                    counter++;
            errs() << "Ranges of lines beginning at " << start1 << " and " << start2
            << " are Identical " << (int) (counter / ((float) lenght) * 100)
            << "% operator wise. " "\n";
        }
        
        // compare Map LUIS
        
        void compareMap(std::map<int, ProcessedLine> testMap, CompareType type,
                        int k) {
            std::map<int, ProcessedLine>::iterator itdeb = testMap.begin();
            int i = itdeb->first;
            int fin = testMap.size() + i - 1;
            switch (type) {
                case operands:
                    while (i != fin - k) {
                        int j = i + k;
                        while (j < (fin - k)
                               && testMap.at(i).lineOperands
                               != testMap.at(j).lineOperands) {
                            j++;
                        }
                        int n = 1;
                        int cpt = 1;
                        while ((j + n) < fin && n < (j - i)
                               && (testMap.at(i + n).lineOperands
                                   == testMap.at(j + n).lineOperands)) {
                                   cpt++;
                                   n++;
                               }
                        if (cpt >= k)
                            errs() << cpt << " lines beginning at " << i << " and " << j  << " match with respect to their operands\n";
                        i++;
                    }
                    break;
                case operators:
                    while (i != fin - k) {
                        int j = i + k;
                        while ((j < fin - k)
                               && !testMap.at(i).compareLinesStructural(testMap.at(j)))
                            j++;
                        int n = 1;
                        int cpt = 1;
                        while ((j + n) < fin && n < (j - i)
                               && (testMap.at(i + n).compareLinesStructural(testMap.at(j + n)))) {
                            cpt++;
                            n++;
                        }
                        if (cpt >= k)
                            errs() << cpt << " lines beginning at " << i << " and " << j
                            << " match with respect to their operators\n";
                        i++;
                    }
                    break;
                case operandsANDoperators:
                    while (i != fin - k) {
                        int j = i + k;
                        while ((j < fin - k)
                               && !testMap.at(i).compareLines(testMap.at(j)))
                            j++;
                        if (j != fin - k) {
                            int n = 1;
                            int cpt = 1;
                            //while ((j+n)<fin && n<(j-i) && (testMap.at(i+n).lineOperators == testMap.at(j+n).lineOperators) && (testMap.at(i+n).lineOperands == testMap.at(j+n).lineOperands)){
                            while ((j + n) < fin && n < (j - i)
                                   && (testMap.at(i + n).compareLines(
                                                                      testMap.at(j + n)))) {
                                cpt++;
                                n++;
                            }
                            if (cpt >= k)
                                errs() << cpt << " lines beginning at " << i << " and "
                                << j
                                << " match with respect to their operands and operators\n";
                        }
                        
                        i++;
                    }
                    break;
            }
        }
        virtual bool runOnModule(Module &M) {
            errs()<< "\n******* This section is output from the section CopyPaste *******\n\n";
            std::map<int, ProcessedLine> lineMap; // Map containing all source lines processed
            std::map<int, ProcessedLine>::iterator itr;
            itr = lineMap.begin();
            for (auto &F : M) {
                for (auto &B : F) {
                    for (auto &I : B) {      
                        LoadInst* loadInst;
                        StoreInst* storeInst;
                        if ((loadInst = dyn_cast < LoadInst > (&I))) {
                            Value* operand = loadInst->llvm::User::getOperand(0);
                            if (DILocation *Loc = I.getDebugLoc()) { // Here I is an LLVM instruction
                                unsigned Line = Loc->getLine();
                                int lineNumber = 0;
                                lineNumber = Line;
                                itr = lineMap.find(lineNumber); // Find the line in the map if it exists already
                                if (itr == lineMap.end()) { // This is the first operand in this source line
                                    // create and initialize the map
                                    LineInfo v = { operand };
                                    lineMap.emplace(lineNumber,ProcessedLine(lineNumber, F.getName(),v));
                                } else { // line has already been seen and initialized, so just add the latest operand
                                    itr->second.lineOperands.push_back(operand);
                                }
                            }
                            
                        } else if ((storeInst = dyn_cast < StoreInst > (&I))) {
                                if (DILocation *Loc = I.getDebugLoc()) { // Here I is an LLVM instruction
                                unsigned Line = Loc->getLine();
                                int lineNumber = 0;
                                lineNumber = Line;
                                itr = lineMap.find(lineNumber); // Find the line in the map if it exists already
                                if (itr == lineMap.end()) {        //filling up the table with dummy information 
                                    lineMap.emplace(lineNumber, ProcessedLine(lineNumber, F.getName(),{ }));
                                } 
                            }
                        }
                        else {
                            if (DILocation *Loc = I.getDebugLoc()) { // Here I is an LLVM instruction
                                unsigned Line = Loc->getLine();
                                int lineNumber = 0;
                                lineNumber = Line;
                                itr = lineMap.find(lineNumber); // Find the line in the map if it exists already
                                std::string str = I.getOpcodeName();
                                itr->second.lineOperators.append(str);
                            }
                        }
                    }
                }
            }
            errs() << "\nSimilarities between the different lines of the blocks identified above\n";
            for (auto &iterator : BlockTable) {
                compareRangesForOperands(iterator.second.startThenBlock,
                                         iterator.second.blockLength + 1,
                                         iterator.second.startElseBlock, lineMap, operands);
                compareRangesForOperators(iterator.second.startThenBlock,
                                          iterator.second.blockLength + 1,
                                          iterator.second.startElseBlock, lineMap, operands);
            }
            errs() << "\nSimilarities between the different lines of the program\n";
            compareMap(lineMap, operators, 3);
            compareMap(lineMap, operandsANDoperators, 2);
            return false;  //No change to code
        }
    };
}
char CopyPaste5::ID = 0;
static RegisterPass<CopyPaste5> M("copypaste5", "CopyPaste detection5");

namespace {
    
    struct IFBLOCKS: public FunctionPass {
        
        static char ID;
        BasicBlock *THENBlock = NULL;
        BasicBlock *ELSEBlock = NULL;
        bool firstFunctionCall =true;
        IFBLOCKS() : FunctionPass(ID) {
        }
        
        virtual bool runOnFunction(Function &F) override {
            bool FirstBlockInFunction = true;   // Prime the pump
            if (firstFunctionCall==true){
            errs() << "\n********** This section is output from the section BlockFind ***********\n";
                firstFunctionCall=false;
            }
            for (auto &B : F) {
                
                if (not FirstBlockInFunction)
                    THENBlock = ELSEBlock;
                ELSEBlock = &B;
                
                if (FirstBlockInFunction) { // Nothing to Compare since this is the first block in fuction
                    FirstBlockInFunction = false;
                } else {                     //Check for THEN statement Control Flow
                    if ((ELSEBlock->getUniquePredecessor()== THENBlock->getUniquePredecessor())&& 
                        (ELSEBlock->getUniqueSuccessor()== THENBlock->getUniqueSuccessor())) {
                            
                            errs() << "if-then-else found, \n  then blocksize = "<< THENBlock->size() 
                                   << "\n  else block size = "<< ELSEBlock->size() << "\n";
                            
                            if (ELSEBlock->size() != THENBlock->size())
                                errs() << " Different sized Blocks found. THEN Block is not identical to ELSE Block.
                                         No Comparison will be performed. \n\n";
                            else {
                                BasicBlock::iterator iTHENBlock = THENBlock->begin();
                                if (DILocation *Loc = iTHENBlock->getDebugLoc()) {
                                    unsigned Line = Loc->getLine();
                                    errs() << "then block starts at line " << Line << "\n";
                                    thenFirstLine = Line;
                                }
                                
                                BasicBlock::iterator iELSEBlock = ELSEBlock->begin();
                                
                                if (DILocation *Loc = iELSEBlock->getDebugLoc()) {                                     			    unsigned Line = Loc->getLine();
                                    errs() << "else block starts at line " << Line << "\n";
                                    elseFirstline = Line;
                                }
                                bool stillSearching = true;
                                while (stillSearching) {
                                    iTHENBlock++;
                                    iELSEBlock++;
                                    stillSearching = ((iTHENBlock != THENBlock->end()) and (iELSEBlock != ELSEBlock->end()));
                                }
                                iTHENBlock--;
                                std::string opcode = iTHENBlock->getOpcodeName();
                                BlockTableIteratorCounter++;
                                BlockTable.emplace(BlockTableIteratorCounter,
                                                   BlockHolder(thenFirstLine, elseFirstline,elseFirstline - thenFirstLine - 1));
                            }
                        }
                }
            }
            for (auto &iterator : BlockTable)
                iterator.second.printTable();
            return false; // Does not change code
        }
    };
}
char IFBLOCKS::ID = 0;
static RegisterPass<IFBLOCKS> B("ifblocks", "IFBLOCKS detection");
