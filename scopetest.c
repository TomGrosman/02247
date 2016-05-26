#include <stdio.h>
#include <stdlib.h>
void func(int x, int y) {
  printf("Hello\n");
}
int main() {
  int a, b, c, x, y;
  a = x + y;
  b = x - b;
  b = x - b;
  {a = x + y;
    b = x - b;
    b = x - b;}
  {int x = 19;
    a = x + y;
    b = x - b;
    b = x - b;}
  printf("%d\n", b);
  func(x, y);
  func(x * x, y * y);
  func(y, x);
  printf("%d\n", x);
  func(x, y);
  func(x * x, y * y);
  func(x, y);
  if (x == y) {
    a = x + y;
    b = x - b;
  } else {a = x + y;
    b = x - y;}
  if (x == y) {
    a = x - y;
    b = x - b;
  } else {a = x - y;
    b = x - b;}
  if (x == y) {
    a = x + a + y;
    b = x - b;
    a = a + b;
  } else {a = b + y - x;
    b = x - b;
    func(x, y);}
  a = x + y;
  b = x - b;
  b = x - b;
  func(y, x);
  printf("%d\n", x);

  return (0);
}
