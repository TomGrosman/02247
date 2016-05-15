void compareMap(std::map<int, ProcessedLine > testMap, CompareType type, int k){ 
//later we should make a case for each copareType. For the moment I have just implemented the operand case
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
}

