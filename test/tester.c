/**A test suite lib for c for personal use
 *Author: Lawrence Milne .AKA. qawse3dr*/


#include "ctester.h"
#include <stdio.h>
int testPass(){

  return 1;
}

int testFail(){
  return 0;
}

int testTimeout(){
  while(1){}
  return 1;
}

int testError(){
  return 0/0;
}

//should error out but c is jank
int testMemError(){
  int* num = NULL;

  printf("%d",num[10000]);

  return 1;
}
int main(){

  startTest("Test for ctester");

    startTestSection("Testing Working tests");
      ctest("This test will pass",testPass);
      ctest("This test will pass",testPass);
      ctest("This test will fail",testFail);
      ctest("This test will fail",testFail);
      ctest("This test will timeout",testTimeout);
      setcTestTimeout(10);
      ctest("This test will timeout in 10 seconds",testTimeout);
    endTestSection();

    startTestSection("test section 2 the electric boogaloo");
      ctest("This test will pass",testPass);
      ctest("This test will error out",testError);
      ctest("This test will segfault",testMemError);
    endTestSection();

  endTest();

  return 0;
}
