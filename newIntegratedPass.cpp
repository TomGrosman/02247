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
        
        
        
        ProcessedLine(int ln, std::string fn, LineInfo lo, std::string lop="later"){
            lineNumber = ln;
            functionName = fn;
            lineOperands = lo ;
            lineOperators = lop;
        }
        
        void compareLines (ProcessedLine compareTo){
            if (lineOperands == compareTo.lineOperands)
                    errs() << "Lines " << lineNumber << " and " << compareTo.lineNumber << "are Identical. \n";
                else
                    errs() << "Lines " << lineNumber << " and " << compareTo.lineNumber << "are not Identical. \n";
        }
        
        
        //function to detect more complicated patterns.refer to section 4.2 in pdf a4 from the lecture
        // It should be noted that this has conflicts with what we had before
        void compareMethods (ProcessedLine compareTo){
            if (lineOperands.size() == compareTo.lineOperands.size())
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
        
        
        
        void compareRanges(int start1, int end, int start2, std::map<int, ProcessedLine  > testMap, CompareType type){
            float counter = 0.0;
            for(int i=0; i<end-start1; i++)
                    if(testMap.at(start1+i).lineOperands == testMap.at(start2+i).lineOperands)
                        counter++;
            errs() << "Ranges of lines are Identical " << (int)(counter/((float)end-(float)start1)*100)<< "%" << "\n";
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
                        }
                        
                       
                        /*
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
                        */
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
            }
            lineMap.at(18).compareLines (lineMap.at(19));
            //lineMap.at(18).compareMethods (lineMap.at(19));
            compareRanges(12,15, 16,lineMap, operands);
            
            return false;  //No change to code
        }
        
        
    };
}
char CopyPaste5::ID = 0;static RegisterPass<CopyPaste5> M("copypaste5","CopyPaste detection5");

