void compareMap(std::map<int, ProcessedLine > testMap, CompareType type, int k){
  switch (type){
  case operand:

    std::map<int,int>::iterator itfin=first.end();
    std::map<int,int>::iterator itdeb=first.begin();
    int k =2;
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
      while (testMap[i].lineOperandstors != testMap[j].lineOperators && j<fin-k)      j++;
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
      while (testMap[i].lineOperandstors != testMap[j].lineOperators && (testMap[i].lineOperands == testMap[j].lineOperands) && j<fin-k)      j++;
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
