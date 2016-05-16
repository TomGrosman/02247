#include <stdio.h>

void func(int x, int y,int z){
  printf("hello\n");
}
int main(){

  printf("hello\n");//compare with compareLines
  printf("hello\n");
  int x=0;
  int y=9;
  int z=8;
  x= x+y;
  y++;
  //these four lines should be identical with compareMap
  int x=0;
  int y=9;
  x= x+y;
  y++;

  func(x,x+x,y);// to test if the two lines are equals for the pass
  func(x,x+x,y);
  if (x==9){
    y=y+4; //compareMap(lineMap, operators, 3) should return at least 4 lines identical beginning at 13 & 21
    x=x+y; //for operands, there should be a missmatching on the func(x,x+x,y) line
    y=y+4;
    x=x+y;
    func(x,x+x,y);
    x=10; // see if the lines are the same
  }
  else{
    y=y+4;
    x=x+y;
    y=y+4;
    x=x+y;
    func(y,y+y,x);
    x=10;
  }
  if (x==9){ // to see the behaviour
    while(x!=10){ 
      x++;
    }
  }
  
  if (x==9){ 
    for (int f=0;f<10;f++){
      x++;
    }
  }
  
  
  if (x==9){ //nested if loop not managed by our pass
    x=10;
  }else if{x==98){
    x=y;
  }else{
    y=3;
  }

  x=x-y;
  x-=y;//should be the same compareRangesWithOperand(51,2,52,lineMap,operands)
  x=x-z;//same operator compareRangesWithOperator(51,2,52,lineMap,operands)
  x=x+y; //same operands compareRangesWithOperand(51,2,52,lineMap,operands)

  return 0;
}
