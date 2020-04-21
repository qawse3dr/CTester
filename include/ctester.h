/**A test suite lib for c for personal use
 *Author: Lawrence Milne .AKA. qawse3dr*/

/**To use the tester run use ctest to run a test
 **Use startTest to start the unit tests should be called at the start.
 **Use startSection to start a new batch of tests ie ()
 **Use endSection to end a section and print results of section
 **Use endTest to finish all test and print results should be called at the end.*/

typedef struct result{
  int passed;
  int failed;
}result;

/**starts the unit tests
 *@param desc the description of the test*/
void startTest(char* desc);

/**prints the result of the test units
 *@return result the results of the tests*/
result endTest();

/**starts a section of testing
 *@param desc the desciption of the test section*/
void startTestSection(char* desc);

/**print the results from a section
 *@return the results of the section*/
result endTestSection();

/**function used for testing
 *@param desc the desciption of the test
 *@param testFun the function it is testing
*/
void ctest(char* desc,int(*testFun)());

/**sets the timeout for a single testfunction*/
void setcTestTimeout(long timeout);
