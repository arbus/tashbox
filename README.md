#Tashbox
#### Your C++ Stashbox

Modules included in tashbox currently:
+ UnitTest

Modules waiting to be ported:
+ String 
+ File
+ Array
+ ScopedLocks

To build, `make library`

## UnitTest

This module enables you to to write unit tests in the same file as the implementation of what you are testing. It is designed to be as flexible as possible in terms of the actual tests.

To write tests, inherit the UnitTest class and implement the `runTest` function. Inside the test, start each block of tests with a `beingTest("Name of the subsection")`. Assertions are done with the `expect(result, failMessage)` function. The result should be a of `bool` type and the `failMessage` should be an unique string that can be used to identify the  failing test. Once the class is done, an static instance of it should be instantiated to initilize the test.

```
class Calculator{
public:
	int add(int a, int b){
		return a + b;
	};
	int subtract(int a, int b){
		return a - b;
	};
};

#ifdef TESTS
class CalculatorTests : public UnitTest, public Calculator{
public:
	CalculatorTests : UnitTest("Calculator Tests"){};
	void runTest(){
		beginTest("add tests");
		expect(add(1,1) == 2, "It should be able to add 2 positive numbers");
		expect(add(-1,1) == 0, "It should be able to add when one of the numbers is negative");
		expect(add(1.2, 1.2) == 2, "It should be able to add 2 floats");

		beginTest("subtract tests");
		expect(subtract(1,1) == 0, "It should be able to minus 2 numbers");
	};
}

static CalculatorTests calculatorTests;
#endif
```

As you can see, the test and implementation of the Calculator class is done within the same file. To actually run the tests:
```
#define TESTS
#include "calculator.h"


int main(void){
	UnitTestRunner runner;
	runner.runAllTests();
	return 0;
}
```
