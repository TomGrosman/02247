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


namespace{
    class Stack {
    
        int m_lineNumber;
        std::vector<Value*> m_whatever;
    public:
        void setLine(int x){m_lineNumber=x;}
        int getLine(){return m_lineNumber;}
        void setVector(std::vector<Value*> x){
            
            std::vector<Value*> m_whatever =x;
        }
        
        std::vector<Value*> getVector(){
            return m_whatever;
        }
        
        void addToVector(Value* value){
            m_whatever.push_back(value);
        }
        
        void areTwoLinesIdentical(Stack l1, Stack l2){
            if(l1.getVector() == l2.getVector())
                errs()<<"Lines are Identical \n";
            else
                errs()<< "NOT IDENTICAL \n";
        }
        
        
    };
    
    struct CopyPaste3 : public ModulePass {
        
        static char ID;
        
        Instruction *SavedInstruction=NULL;
        
        CopyPaste3() : ModulePass(ID) {}
        
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
            if (Var != NULL) return "tmp";
            
            return Var->getName();
        }
        
        virtual bool runOnModule(Module &M) {
            //F.dump();
            // We assume there are limit of 50 lines in source code
            // We need to have the objects ready so we can easily access the members
            std::vector<Stack> objects;
            for (int i = 1; i < 51; i++)
                objects.push_back(Stack());
            
            for (auto &F :  M) {
                for (auto &B : F){
                    for(auto &I: B){
                    
                    LoadInst* loadInst;
                    
                    if((loadInst = dyn_cast<LoadInst>(&I))){
                        Value* operand = loadInst->llvm::User::getOperand(0);
                       
                        operand->dump();
                       
                        errs() << "Name: " << getOriginalName(operand) << "\n";
                        
                        if (DILocation *Loc = I.getDebugLoc())
                        { // Here I is an LLVM instruction
                            unsigned Line = Loc->getLine();
                            
                            errs()<< Line << "\n";
       //note : Pushing instruction to stack is not helpful because they will be pushed with registers and they are always different so instead we will push operand
                            static unsigned first=Line;
                            
                            
                            objects.at(1).setLine(first);
                            errs()<< objects.at(1).getLine() <<"first line\n";
                            
                            for(int i=2; i<20; i++)
                            {
                                objects.at(i).setLine(objects.at(i-1).getLine()+1);
                                
                                errs() << i <<" object line" << objects.at(i).getLine()<< "\n";
                                errs() << "vector size is: "<< objects.at(i).getVector().size()<<"\n";
                            }
                            
                         if(objects.at(1).getLine() ==Line)
                         {
                            objects.at(1).addToVector(operand);
                             errs()<< "we pushed operand to object line" << objects.at(1).getLine() << "\n";
                            errs() << "vector size is: "<<objects.at(1).getVector().size()<<"\n";
                            
                            for(int i=0; i<objects.at(1).getVector().size(); i++)
                                errs() << *objects.at(1).getVector().at(i) <<"\n";
                             
                         }
                        
                            for(int i=2; i<20; i++)
                            {
                                if(objects.at(i).getLine()== Line)
                                {
                                    objects.at(i).addToVector(operand);
                                    errs()<< "we pushed operand to object line: " << objects.at(i).getLine() << "\n";
                                    errs() << "vector size is: " << objects.at(i).getVector().size()<<"\n";
                                }
                            }
                            
                            
                            for(int i=0; i<objects.at(i).getVector().size(); i++)
                                errs() << *objects.at(i).getVector().at(i) <<"\n";
                            
            }
                            errs()<<"****** \n";
                        
        }
                    
                    
                    
                    
                    
                    if (isa<CallInst> (I)) {
                        
                     
                        CallInst *op = dyn_cast<CallInst>(&I);
                        Value *V  =	op->getCalledValue ();
                        
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
            objects.at(10).areTwoLinesIdentical(objects.at(10), objects.at(13));
            
            return false;  //No change to code
                }
        
        
    };
}
char CopyPaste3::ID = 0;static RegisterPass<CopyPaste3> Z("copypaste3","CopyPaste detection3");



namespace llvm {
    std::string getShortValueName(Value * v);
    
        Value * v1;
        Value * v2;
        Instruction::BinaryOps op;
        
        Expression::Expression (Instruction * I) {
            if (BinaryOperator * BO = dyn_cast<BinaryOperator>(I)) {
                this->v1 = BO->getOperand(0);
                this->v2 = BO->getOperand(1);
                this->op = BO->getOpcode();
            }
            else {
                errs() << "We're only considering BinaryOperators\n";
            }
        }
        
        // For two expressions to be equal, they must
        // have the same operation and operands.
    bool Expression::operator== (const Expression &e2) const {
            return this->v1 == e2.v1 && this->v2 == e2.v2 && this->op == e2.op;
        }
        
        // Less than is provided here in case you want
        // to use STL maps, which use less than for
        // equality checking by default
    bool Expression::operator< (const Expression &e2) const {
            if (this->v1 == e2.v1)
                if (this->v2 == e2.v2)
                    if (this->op == e2.op)
                        return false;
                    else
                        return this->op < e2.op;
                    else
                        return this->v2 < e2.v2;
                    else
                        return this->v1 < e2.v1;
        }
        
        // A pretty printer for Expression objects
        // Feel free to alter in any way you like
    std::string Expression::toString() const {
            std::string op = "?";
            switch (this->op) {
                case Instruction::Add:
                case Instruction::FAdd: op = "+"; break;
                case Instruction::Sub:
                case Instruction::FSub: op = "-"; break;
                case Instruction::Mul:
                case Instruction::FMul: op = "*"; break;
                case Instruction::UDiv:
                case Instruction::FDiv:
                case Instruction::SDiv: op = "/"; break;
                case Instruction::URem:
                case Instruction::FRem:
                case Instruction::SRem: op = "%"; break;
                case Instruction::Shl: op = "<<"; break;
                case Instruction::AShr:
                case Instruction::LShr: op = ">>"; break;
                case Instruction::And: op = "&"; break;
                case Instruction::Or: op = "|"; break;
                case Instruction::Xor: op = "xor"; break;
            }
            return getShortValueName(v1) + " " + op + " " + getShortValueName(v2);
        }
        
        // Silly code to print out a set of expressions in a nice
        // format
        void printSet(std::vector<Expression> * x) {
            bool first = true;
            outs() << "{";
            for (std::vector<Expression>::iterator it=x->begin(), iend=x->end(); it!=iend; ++it) {
                if (!first) {
                    outs() << ", ";
                }
                else { first = false; }
                outs() << (it->toString());
            }
            outs() << "}\n";
        }
        
        
        std::string getShortValueName(Value * v) {
            if (v->getName().str().length() > 0) {
                return "%" + v->getName().str();
            }
            else if (isa<Instruction>(v)) {
                std::string s = "";
                raw_string_ostream * strm = new raw_string_ostream(s);
                v->print(*strm);      
                std::string inst = strm->str();
                size_t idx1 = inst.find("%");
                size_t idx2 = inst.find(" ",idx1);
                if (idx1 != std::string::npos && idx2 != std::string::npos) {
                    return inst.substr(idx1,idx2-idx1);
                }
                else {
                    return "\"" + inst + "\"";
                }
            }
            else if (ConstantInt * cint = dyn_cast<ConstantInt>(v)) {
                std::string s = "";
                raw_string_ostream * strm = new raw_string_ostream(s);
                cint->getValue().print(*strm,true);      
                return strm->str();
            }
            else {
                std::string s = "";
                raw_string_ostream * strm = new raw_string_ostream(s);
                v->print(*strm);      
                std::string inst = strm->str();
                return "\"" + inst + "\"";
            }
        }

    
    struct CopyPaste4 : public FunctionPass {
        
        static char ID;
        
        Instruction *SavedInstruction=NULL;
        
        
        CopyPaste4() : FunctionPass(ID) {}
        
        
        virtual bool runOnFunction(Function &F) override {
            //F.dump();
            // We assume there are limit of 50 lines in source code
            // We need to have the objects ready so we can easily access the members
            
            for (auto &B :  F) {
                for (auto &I : B) {
                    //getShortValueName(v1);
                    I.dump();
                    Instruction::BinaryOps op;
                    Expression  e = Expression(&I);
                    
                    //e.operator==(e);
                    //e.toString();
                    
                        
                    
                    
                    
                    
                    if (isa<CallInst> (I)) {
                        
                        
                        CallInst *op = dyn_cast<CallInst>(&I);
                        Value *V  =	op->getCalledValue ();
                        
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
                        
                        
                        
                    }}}
            
            return false;  //No change to code
        }
        
        
    };
}
char CopyPaste4::ID = 0;static RegisterPass<CopyPaste4> H("copypaste4","CopyPaste detection4");



