#include <iostream>
#include <cstdlib>
#include <emscripten/emscripten.h>
using namespace std;


EXTERN EMSCRIPTEN_KEEPALIVE int find_max_min(int num1, int num2, int num3){
  if(num1<=num2 && num1<=num3){
    return num1;
  } else if(num2<=num1 && num2<=num3){
    return num2;
  } else {
    return num3;
  }
}
int main() {
  return 0;
}