void compareMap(std::map<int, ProcessedLine > testMap, CompareType type, int k){
  std::map<int,int>::iterator itfin=first.end();
  std::map<int,int>::iterator itdeb=first.begin();
  int i = itdeb->first;
  int fin = itfin->first ;
  switch (type){
  case operand:
    while (i != fin){
      int j = i+k;
      while (testMap[i].lineOperands != testMap[j].lineOperands && j<fin-k)      j++;
      int n =1;
      int cpt=1;
      while ((testMap[i+n].lineOperands == testMap[j+n].lineOperands) && j<fin){
	      cpt++;
	      n++;
      }
      if (cpt >=k)      errs() << cpt << " lines begining at " << i << " and" << j<< " match with respect to their operands" endl;
    }
    i++;
    break;
  case operator:
    while (i != fin){
      int j = i+k;
      while (testMap[i].lineOperators != testMap[j].lineOperators && j<fin-k)      j++;
      int n =1;
      int cpt=1;
      while ((testMap[i+n].lineOperators == testMap[j+n].lineOperators) && j<fin){
	      cpt++;
	      n++;
      }
      if (cpt >=k)      errs() << cpt << " lines begining at " << i << " and" << j<< " match with respect to their operators" endl;
    }
    i++;
    break;
  case operandsANDoperators:
    while (i != fin){
      int j = i+k;
      while (testMap[i].lineOperators != testMap[j].lineOperators && (testMap[i].lineOperands == testMap[j].lineOperands) && j<fin-k)      j++;
      int n =1;
      int cpt=1;
      while ((testMap[i+n].lineOperators == testMap[j+n].lineOperators) && (testMap[i+n].lineOperands == testMap[j+n].lineOperands) && j<fin){
	      cpt++;
	      n++;
      }
      if (cpt >=k)      errs() << cpt << " lines begining at " << i << " and" << j<< " match with respect to their operands and operators" endl;
    }
    i++;
    break;
  }
}

void compareRanges(int start1, int lenght, int start2, std::map<int, ProcessedLine  > testMap, CompareType type){
switch (type){
	case operand:
	  int end=start1+length;
	  float counter = 0.0;
	  int i=0;
	  while (it<=end){
	    if(testMap.at(start1).lineOperands == testMap.at(start2).lineOperands)	      counter++;
	    start1++;
	    start2++;
	  }
	  errs() << "Ranges of lines are Identical operand wise " << (int)(counter/((float)lenght)*100)<< "%" << "\n";
	  break;
	case operator:
	  int end=start1+length;
	  float counter = 0.0;
	  int i=0;
	  while (it<=end){
	    if(testMap.at(start1).lineOperators == testMap.at(start2).lineOperators)	      counter++;
	    start1++;
	    start2++;
	  }
	  errs() << "Ranges of lines are Identical operator wise " << (int)(counter/((float)lenght)*100)<< "%" << "\n";
	  break;
  	case operandsANDoperators:
	  int end=start1+length;
	  float counter = 0.0;
	  int i=0;
	  while (it<=end){
	    if((testMap.at(start1).lineOperands == testMap.at(start2).lineOperands) && (testMap.at(start1).lineOperators == testMap.at(start2).lineOperators))	      counter++;
	    start1++;
	    start2++;
	  }
	  errs() << "Ranges of lines are Identical operator and operand wise " << (int)(counter/((float)lenght)*100)<< "%" << "\n";
	  break;
	}
}
