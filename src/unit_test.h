#ifndef UNIT_TEST_H
#define UNIT_TEST_H
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

class UnitTestRunner;
/**
 * This is the base UnitTest. All unit tests extend this call and implement its virtual functions to run its tests
 */
class UnitTest{
public: 
	explicit UnitTest(const std::string &name);
	virtual ~UnitTest();

	const std::string& getName() const;
	void performTest(UnitTestRunner* runner);
	static std::vector<UnitTest*>& getAllTests();

	virtual void initialise();
	virtual void shutdown();
	virtual void runTest() = 0;

	void beginTest(const std::string &testName);
	void expect(const bool testResult, const std::string &failMessage = std::string(""));
	void logMessage(const std::string &message);
private:
	const std::string name;
	UnitTestRunner* runner;
};
class UnitTestRunner{
public:
	UnitTestRunner();
	virtual ~UnitTestRunner();

	void runTests(const std::vector<UnitTest*>& tests);
	void runAllTests();

	struct TestResult{
		std::string unitTestName;
		std::string subCategoryName;

		int passes;
		int failures;

		std::vector<std::string> messages;
	};

	int getNumResults() const;
	const TestResult* getResult(int index) const;
protected:
	virtual void resultsUpdated();
	virtual void logMessage(const std::string &message);
private:
	friend class UnitTest;

	UnitTest* currentTest;
	std::string currentSubCategory;
	std::vector<TestResult*> results;

	void beginNewTest(UnitTest* test, const std::string &subCategory);
	void endTest();

	void addPass();
	void addFail(const std::string& failMessage);

	std::string to_string(int input);
};
#endif // UNIT_TEST_H