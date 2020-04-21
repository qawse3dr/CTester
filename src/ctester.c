/**A test suite lib for c for personal use
 *Author: Lawrence Milne .AKA. qawse3dr*/

 /**To use the tester run use ctest to run a test
  **Use startTest to start the unit tests should be called at the start.
  **Use startSection to start a new batch of tests ie ()
  **Use endSection to end a section and print results of section
  **Use endTest to finish all test and print results should be called at the end.*/

 /*typedef struct result{
   int passed;
   int failed;
 }result;*/
#include "ctester.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

/*static vars to keep track of testing*/
static long timeout = 2;
static int testCount;
static int testPassed;
static int sectionTestCount;
static int sectionTestPassed;
static int sectionCount = 0;
static char* results = NULL;

//define colours
#define RED printf("\x1B[31m");
#define NORMAL printf("\x1B[37m");
#define GREEN printf("\x1B[32m");
//static as it should never be touched by the user
static void printHeader(char* desc,int isSectionHeader){
  //prints the top of the header
  for(int i = 0; i < 65;i++)printf("-");
  if(isSectionHeader){
    printf("\n|%25sSECTION %-30d|\n","",sectionCount);\
  } else {
    printf("\n");
  }
  while(*desc){
    printf("|     ");
    for(int i = 0; i < 55; i++){
      if(*desc){
        printf("%c",*desc);
        desc++;
      } else{
        printf(" ");
      }
    }
    printf("   |\n");
  }
  //print footer
  for(int i = 0; i < 65;i++)printf("-");
  printf("\n\n");
}
 /**starts the unit tests
  *@param desc the description of the test*/
void startTest(char* desc){
  //resets testing vars
  testCount = 0;
  testPassed = 0;
  sectionTestCount = 0;
  sectionTestPassed = 0;
  //create result String
  results = malloc(sizeof(char));
  *results = '\0';
  printHeader(desc,0);
}

 /**prints the result of the test units
  *@return result the results of the tests*/
result endTest(){
  result res;
  res.passed = sectionTestPassed;
  res.failed = sectionCount-sectionTestPassed;
  if(!results){
    fprintf(stderr,"no test is currently running");
  }
  printHeader("Test Results",0);
  printf("%s",results);
  printf("Total results: %d/%d\n",testPassed,testCount);
  if(testCount != 0){
    printf("Mark: %.2f%\n",100*((float)testPassed/testCount));
  }
  free(results);
  results = NULL;
  return res;
 }

 /**starts a section of testing
  *@param desc the desciption of the test section*/
void startTestSection(char* desc){
  sectionTestCount = 0;
  sectionTestPassed = 0;
  sectionCount++;

  //runs till the full string was printed within the box
  printHeader(desc,1);
}

 /**print the results from a section
  *@return the results of the section*/
result endTestSection(){

  //concat the new result onto the total for printing at the end
  char tempRes[1000];
  sprintf(tempRes,"Section %d Results: %d/%d\n",sectionCount,sectionTestPassed,sectionTestCount);
  results = realloc(results,sizeof(char)*(1 + strlen(tempRes)+ strlen(results)));
  strcat(results,tempRes);

  printf("%s\n",tempRes);

  //returns the tes results
  result res;
  res.passed = sectionTestPassed;
  res.failed = sectionCount-sectionTestPassed;
  return res;
 }

 /**function used for testing
  *@param desc the desciption of the test
  *@param testFun the function it is testing
 */
void ctest(char* desc,int(*testFun)()){
  testCount++;
  sectionTestCount++;
  int status;
  int child_pid;
  //print description; have to flush
  //to make sure it prints before fork
  printf("Test %d (%s): ",sectionTestCount,desc);
  fflush(stdout);

  //child
  if((child_pid = fork()) == 0){
    exit(testFun());
  }else{
    //parent lets it run for timeout then kills it
    //sleep(timeout);
    int i;
    for(i = 0; i < timeout;i++){
      if(waitpid(child_pid, &status, WNOHANG)){
        break;
      }else{
        sleep(1);
      }
    }
    if( i == timeout){
      kill(child_pid,SIGKILL);
      //timed out
      status = SIGKILL;
    }
  }
  //checks static of child
  if (WIFEXITED(status)){
    if(WEXITSTATUS(status) == 1){
      //didnt fail
      testPassed++;
      sectionTestPassed++;
      //sets colours and prints
      GREEN printf("PASSED\n"); NORMAL
    } else{//failed
      RED printf("Failed with status %d\n", WEXITSTATUS(status)); NORMAL
    }
  //failed
  } else if(WIFSIGNALED(status)){
    RED
    psignal(WTERMSIG(status), "Exit signal");
    NORMAL
  }

}

/**timeout in seconds*/
void setcTestTimeout(long newTimeout){
 timeout = newTimeout;
}
