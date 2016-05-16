void compareMap(std::map<int, ProcessedLine > testMap, CompareType type, int k){
  switch (type){
  case operand:

    std::map<int,int>::iterator itfin=first.end();
    std::map<int,int>::iterator itdeb=first.begin();
    int i = itdeb->first;
    int fin = itfin->first ;
    while (i != fin){
      int j = i+k;
      while (testMap[i].lineOperands != testMap[j].lineOperands && j<fin-k)      j++;
      int n =1;
      int cpt=1;
      while ((testMap[i+n].lineOperands == testMap[j+n].lineOperands) && j<fin){
	      cpt++;
	      n++;
      }
      if (cpt >=k)      errs() << cpt << " lines coincidence between the blocks begining at " << i << " and" << j<< endl;
    }
    i++;
    break;
  case operator:

    std::map<int,int>::iterator itfin=first.end();
    std::map<int,int>::iterator itdeb=first.begin();
    int k =2;
    int i = itdeb->first;
    int fin = itfin->first ;
    while (i != fin){
      int j = i+k;
      while (testMap[i].lineOperators != testMap[j].lineOperators && j<fin-k)      j++;
      int n =1;
      int cpt=1;
      while ((testMap[i+n].lineOperators == testMap[j+n].lineOperators) && j<fin){
	      cpt++;
	      n++;
      }
      if (cpt >=k)      errs() << cpt << " lines coincidence between the blocks begining at " << i << " and" << j<< endl;
    }
    i++;
    break;
  case operandsANDoperators:

    std::map<int,int>::iterator itfin=first.end();
    std::map<int,int>::iterator itdeb=first.begin();
    int k =2;
    int i = itdeb->first;
    int fin = itfin->first ;
    while (i != fin){
      int j = i+k;
      while (testMap[i].lineOperators != testMap[j].lineOperators && (testMap[i].lineOperands == testMap[j].lineOperands) && j<fin-k)      j++;
      int n =1;
      int cpt=1;
      while ((testMap[i+n].lineOperators == testMap[j+n].lineOperators) && (testMap[i+n].lineOperands == testMap[j+n].lineOperands) && j<fin){
	      cpt++;
	      n++;
      }
      if (cpt >=k)      errs() << cpt << " lines coincidence between the blocks begining at " << i << " and" << j << endl;
    }
    i++;
    break;
  }
}

void compareRangesForOperands(int start1, int lenght, int start2, std::map<int, ProcessedLine  > testMap, CompareType type){
switch (type){
	case operand:
	float counter = 0.0;
	for(int i=0; i<lenght; i++)
	   if(testMap.at(start1+i).lineOperands == testMap.at(start2+i).lineOperands)	      counter++;
	   errs() << "Ranges of lines are Identical operand wise " << (int)(counter/((float)lenght)*100)<< "%" << "\n";
	   break;
	case operator:
	float counter = 0.0;
	for(int i=0; i<lenght; i++)
	    if(testMap.at(start1+i).lineOperator == testMap.at(start2+i).lineOperator)     counter++;
	    errs() << "Ranges of lines are Identical operator wise " << (int)(counter/((float)lenght)*100)<< "%" << "\n";
	    break;
  	case operandsANDoperators:
	float counter = 0.0;
	for(int i=0; i<lenght; i++)
	    if((testMap.at(start1+i).lineOperator == testMap.at(start2+i).lineOperator) && (testMap.at(start1+i).lineOperands == testMap.at(start2+i).lineOperands))
	       counter++;
	    errs() << "Ranges of lines are Identical operator and operand wise " << (int)(counter/((float)lenght)*100)<< "%" << "\n";
	    break;
	
}
}
