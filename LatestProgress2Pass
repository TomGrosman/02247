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


namespace{
    // vector to hold the operands in a sourceline
    typedef std::vector<Value*>   LineInfo;
    
    enum CompareType{operands,
                     operators};
    
    class ProcessedLine {
    public:
        int lineNumber;             // source line number
        std::string functionName;        // function where the line is found
        LineInfo lineOperands;      // all operands in the sourceline
        std::string lineOperators;
        StringRef lineConstant;
        
        
        ProcessedLine(int ln, std::string fn, LineInfo lo , std::string lop="", StringRef co=""){
            lineNumber = ln;
            functionName = fn;
            lineOperands = lo ;
            lineOperators = lop;
            lineConstant = co;
        }
        //method to identify fully identical lines , considering operands and operators
        void compareLines (ProcessedLine compareTo){
            if (lineOperands == compareTo.lineOperands && lineOperators==compareTo.lineOperators)
                    errs() << "Lines " << lineNumber << " and " << compareTo.lineNumber << "are Identical. \n";
                else
                    errs() << "Lines " << lineNumber << " and " << compareTo.lineNumber << "are not Identical. \n";
        }
        
        
        //function to detect more complicated patterns.refer to section 4.2 in pdf a4 from the lecture
        //first Example
        void compareMethods (ProcessedLine compareTo){
            if (lineOperands.size() == compareTo.lineOperands.size() && lineOperators ==compareTo.lineOperators)
                errs() << "Methods in lines " << lineNumber << " and " << compareTo.lineNumber << "are Identical. \n";
            else
                errs() << "Methods in lines " << lineNumber << " and " << compareTo.lineNumber << "are not Identical. \n";
        }
        
        // utility method to print out a ProcessedLine object
        void printALine(){
            errs() << "Source Line=>"<< lineNumber << "  Function=>" << functionName << "  Operands=> \n";
            for (const auto &p : lineOperands){
                errs() << *p << "\n";
            }
            errs() << "\n";
        }
        
        // utility method to print out a operators of a line
        void printOperatorsLine(){
            errs() << "Source Line=>"<< lineNumber  << "  Operators=> \n";
            for (const auto &p : lineOperators){
                errs() << p ;
            }
            errs() << "\n";
        }

        
    };
    struct CopyPaste5 : public ModulePass {
        
        static char ID;
        
        
        CopyPaste5() : ModulePass(ID) {}
        
        const Function* findEnclosingFunc(const Value* V) {
            if (const Argument* Arg = dyn_cast<Argument>(V)) {
                return Arg->getParent();
            }
            if (const Instruction* I = dyn_cast<Instruction>(V)) {
                return I->getParent()->getParent();
            }
            return NULL;
        }
        
        const DIVariable* findVar(const Value* V, const Function* F) {
            for (const_inst_iterator Iter = inst_begin(F), End= inst_end(F); Iter !=End; ++Iter) {
                const Instruction* I = &*Iter;
                if (const DbgDeclareInst* DbgDeclare = dyn_cast<DbgDeclareInst>(I)) {
                    if (DbgDeclare->getAddress() == V) return DbgDeclare->getVariable();
                } else if (const DbgValueInst* DbgValue = dyn_cast<DbgValueInst>(I)) {
                    if (DbgValue->getValue() == V) return DbgValue->getVariable();
                }
            }
            return NULL;
        }
        
        StringRef getOriginalName(const Value* V) {
            // TODO handle globals as well
            
            const Function* F = findEnclosingFunc(V);
            if (!F) return V->getName();
            
            const DIVariable* Var = findVar(V, F);
            if (Var != NULL) return Var->getName();
            
            return Var->getName();
        }
        
        
        
        void compareRangesForOperands(int start1, int lenght, int start2, std::map<int, ProcessedLine  > testMap, CompareType type){
            float counter = 0.0;
            for(int i=0; i<lenght; i++)
                    if(testMap.at(start1+i).lineOperands == testMap.at(start2+i).lineOperands)
                        counter++;
            errs() << "Ranges of lines are Identical operand wise" << (int)(counter/((float)lenght)*100)<< "%" << "\n";
        }
        
        void compareRangesForOperators(int start1, int lenght, int start2, std::map<int, ProcessedLine  > testMap, CompareType type){
            float counter = 0.0;
            for(int i=0; i<lenght; i++)
                if(testMap.at(start1+i).lineOperators == testMap.at(start2+i).lineOperators)
                    counter++;
            errs() << "Ranges of lines are Identical operator wise " << (int)(counter/((float)lenght)*100)<< "%" << "\n";
        }

        
        virtual bool runOnModule(Module &M) {
            std::map<int, ProcessedLine> lineMap;   // Map containing all source lines processed
            std::map<int, ProcessedLine>::iterator itr;
            itr = lineMap.begin();
            //M.dump();
            for (auto &F :  M) {
                for (auto &B : F){
                    for(auto &I: B){
                        
                        LoadInst* loadInst;
                        StoreInst* storeInst;
                        
                        if((loadInst = dyn_cast<LoadInst>(&I))){
                            Value* operand = loadInst->llvm::User::getOperand(0);
                            
                            operand->dump();
                           
                            errs() << "Name: " << getOriginalName(operand) << "\n";
                            
                            if (DILocation *Loc = I.getDebugLoc())
                            { // Here I is an LLVM instruction
                                unsigned Line = Loc->getLine();
                                
                                errs()<< Line << "\n";
                                
                                int lineNumber = 0;
                                
                                
                                    lineNumber=Line;
                                    itr=lineMap.find(lineNumber); // Find the line in the map if it exists already
                                    
                                    if (itr == lineMap.end()){  // This is the first operand in this source line
                                        // so create and initialize the map entry with the key (line number), and a new ProcessedLine object w initialized with the line number, function name, and first operand in the lineOperands vector.
                                        
                                        
                                        LineInfo v = {operand};
                                        
                                        
                                        lineMap.emplace(lineNumber, ProcessedLine(lineNumber,F.getName(),v));
                                        
                                    } else { // line has already been seen and initialized, so just add the latest operand
                                        itr->second.lineOperands.push_back(operand);
                                        
                                    }
                                
                                }
                            
                        }else if((storeInst = dyn_cast<StoreInst>(&I))){
                            StringRef constant = storeInst->llvm::User::getName();
                            //errs() << "hello  we are here " << "\n";
                            errs() << constant << "\n";
                            //errs() << "hello  we are here 2 " << "\n";
                            
                            //errs() << "Name: " << getOriginalName(constant) << "\n";
                            
                            if (DILocation *Loc = I.getDebugLoc())
                            { // Here I is an LLVM instruction
                                unsigned Line = Loc->getLine();
                                
                                errs()<< Line << "\n";
                                
                                int lineNumber = 0;
                                
                                
                                lineNumber=Line;
                                itr=lineMap.find(lineNumber); // Find the line in the map if it exists already
                                
                                if (itr == lineMap.end()){  // This is the first operand in this source line
                                    // so create and initialize the map entry with the key (line number), and a new ProcessedLine object w initialized with the line number, function name, and first operand in the lineOperands vector.
                                    
                                    
                                    StringRef c = constant;
                                    
                                    
                                    //lineMap.emplace(lineNumber, ProcessedLine(lineNumber,F.getName(),c));
                                    
                                } else { // line has already been seen and initialized, so just add the latest operand
                                    //itr->second.lineOperands.push_back(constant);
                                    
                                }
                                
                            }
                        }
                        
                        else{
                            errs() << I.getOpcodeName() << "\n";
                            if (DILocation *Loc = I.getDebugLoc())
                            { // Here I is an LLVM instruction
                                unsigned Line = Loc->getLine();
                                
                                errs()<< Line << "\n";
                                
                                int lineNumber = 0;
                                
                                
                                lineNumber=Line;
                                itr=lineMap.find(lineNumber); // Find the line in the map if it exists already
                            //errs() << lineMap.at(lineNumber) << "\n";
                                std::string str = I.getOpcodeName();
                                itr-> second.lineOperators.append(str);
                            
                            
                            
                            }}
                        
                            errs()<<"****** \n";
                    
                        
                    
                    if (isa<CallInst> (I)) {
                            
                            /*
                            CallInst *op = dyn_cast<CallInst>(&I);
                            Value *V  =	op->getCalledValue ();
                            if (V->getName() != "if.else") {
                                errs() << V->getName() <<"\n" ;
                            }
                            */
                        
                            //errs() << "Name: " << getOriginalName(V) << "\n";
                            /*
                             if (V->getName() != "llvm.dbg.declare") {
                             
                             //I.dump();
                             
                             DILocalVariable *see;
                             see->getMetadataID();
                             
                             errs()<<"We have available Expression "<< "\n";
                             errs()<< V->getNumUses() << "\n";
                             //errs()<< getShortValueName(V);
                             
                             }
                             */
                            
                            
                        }
                        
                    }}}
            for (auto &p : lineMap) {
                errs() << "KEY=>" << p.first <<   "  ";
                p.second.printALine();
                p.second.printOperatorsLine();
            }
            
            
    //these are the tests that you need to uncomment to check and modify the inputs based on your program
            
            //lineMap.at(22).compareLines (lineMap.at(23));
            //lineMap.at(18).compareMethods (lineMap.at(19));
            //compareRangesForOperands(12, 3, 16,lineMap, operands);
            //compareRangesForOperators(12, 3, 16,lineMap, operators);
            
            return false;  //No change to code
        }
        
        
    };
}
char CopyPaste5::ID = 0;static RegisterPass<CopyPaste5> M("copypaste5","CopyPaste detection5");


namespace {
    
    int BlockTableIteratorCounter = 0;
    class BlockHolder {
        int startLine, endLine,blockLength;
        
    public:
        BlockHolder(int start, int end, int length) {
            startLine = start;
            endLine = end;
            blockLength = length;
        }
        
        void printTable(){
            errs()  << "start line: " << startLine
            << "  end line: " << endLine
            << "  Block Length: " << blockLength << "\n";
            
            
            
        }
    };
    
    int thenFirstLine,elseFirstline,blockLength;
    
    std::map<int,BlockHolder> BlockTable;
    
    struct IFBLOCKS: public FunctionPass {
        
        enum Equivalency {
            Identical, Literal, Structural, None
        };
        // Identical = literally and structurally
        // Literal = same literals but not same variables (dTHENferent semantics)
        // Structural = same statement form, but dTHENferent variables and/or literals
        
        bool compareBlocks(BasicBlock block1, BasicBlock block2,
                           Equivalency compareType) {
            return true;
        }
        
        static char ID;
        BasicBlock *THENBlock = NULL;
        BasicBlock *ELSEBlock = NULL;
        
        IFBLOCKS() :
        FunctionPass(ID) {
        }
        
        virtual bool runOnFunction(Function &F) override {
            bool FirstBlockInFunction = true;   // Prime the pump
            
            for (auto &B : F) {
                
                if (not FirstBlockInFunction)
                    THENBlock = ELSEBlock;
                ELSEBlock = &B;
                
                if (FirstBlockInFunction) // Nothing to Compare since this is the first block in fuction
                    FirstBlockInFunction = false;
                else {                       //Check for THEN statement Control Flow
                    if ((ELSEBlock->getUniquePredecessor()
                         == THENBlock->getUniquePredecessor())
                        && (ELSEBlock->getUniqueSuccessor()
                            == THENBlock->getUniqueSuccessor())) {
                            
                            errs() << "IF THEN ELSE FOUND, \n  THEN BLOCKSIZE = "
                            << THENBlock->size() << "\n  ELSE BLOCKSIZE = "
                            << ELSEBlock->size() << "\n";
                            
                            if (ELSEBlock->size() != THENBlock->size())
                                errs()
                                << " Different sized Blocks found. THEN Block is not identical to ELSE Block. No Comparison will be performed. \n\n";
                            
                            else {
                                
                                errs() << "   Checking contents...  " << "\n";
                                
                                BasicBlock::iterator iTHENBlock = THENBlock->begin();
                                if (DILocation *Loc = iTHENBlock->getDebugLoc()) {
                                    unsigned Line = Loc->getLine();
                                    
                                    errs() << "THEN BLOCK  startsss at line " << Line << "\n";
                                    thenFirstLine = Line;
                                }
                                
                                BasicBlock::iterator iELSEBlock = ELSEBlock->begin();
                                
                                if (DILocation *Loc = iELSEBlock->getDebugLoc()) { // Here I is an LLVM instruction
                                    unsigned Line = Loc->getLine();
                                    
                                    errs() << "Else BLOCK  startssss at line " << Line << "\n";
                                    elseFirstline = Line;
                                    
                                }
                                
                                bool stillSearching = true;
                                while (stillSearching) {
                                    iTHENBlock++;
                                    iELSEBlock++;
                                    stillSearching = ((iTHENBlock != THENBlock->end())
                                                      and (iELSEBlock != ELSEBlock->end()));
                                }
                                
                                iTHENBlock--;
                                std::string opcode = iTHENBlock->getOpcodeName();
                                errs() << opcode << " Found\n";
                                if (DILocation *Loc = iTHENBlock->getDebugLoc()) { // Here I is an LLVM instruction
                                    unsigned Line = Loc->getLine();
                                    errs() << "THEN BLOCK  ENDZ at line " << Line
                                    << "\n";
                                    
                                    
                                }
                                BlockTableIteratorCounter++;
                                
                                
                                
                                BlockTable.emplace(BlockTableIteratorCounter,BlockHolder(thenFirstLine,elseFirstline,elseFirstline -  thenFirstLine-1));
                            }
                        }
                }
            }
            for(auto &iterator : BlockTable)
                iterator.second.printTable();
            return false; // Does not change code
        }
    };
}
char IFBLOCKS::ID = 0;
static RegisterPass<IFBLOCKS> B("ifblocks", "IFBLOCKS detection");

