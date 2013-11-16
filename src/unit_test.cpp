#include "unit_test.h"

UnitTest::UnitTest(const std::string &nm) : name(nm), runner(nullptr){
	this->getAllTests().push_back(this);
}
UnitTest::~UnitTest(){
	int i = 0;
	for(auto test : this->getAllTests()){
		if(test == this){
			this->getAllTests().erase(
				this->getAllTests().begin() + i,
				this->getAllTests().begin() + i + 1);
			break;
		}
		i++;
	}
}
const std::string& UnitTest::getName() const{
	return this->name;
};
void UnitTest::performTest(UnitTestRunner* newRunner){
	runner = newRunner;
	this->initialise();
	this->runTest();
	this->shutdown();
};
std::vector<UnitTest*>& UnitTest::getAllTests(){
	static std::vector<UnitTest*> tests;
	return tests;
};

void UnitTest::initialise(){};
void UnitTest::shutdown(){};

void UnitTest::beginTest(const std::string &testName){
	this->runner->beginNewTest(this, testName);
};
void UnitTest::expect(bool testResult, const std::string &failMessage){
	testResult ?
		this->runner->addPass() :
		this->runner->addFail(failMessage);

};
void UnitTest::logMessage(const std::string &message){
	this->runner->logMessage(message);
};

UnitTestRunner::UnitTestRunner() : currentTest(nullptr){};
UnitTestRunner::~UnitTestRunner(){};

std::string UnitTestRunner::to_string(int input){
	std::ostringstream ss;
	ss << input;
	return ss.str();
};

void UnitTestRunner::runTests(const std::vector<UnitTest*>& tests){
	this->results.clear();
	this->resultsUpdated();

	for(auto test: tests){
		try{
			test->performTest(this);
		}catch(...){
			this->addFail("An unhandled exception was thrown");
		}
	}

	this->endTest();
};
void UnitTestRunner::runAllTests(){
	this->runTests(UnitTest::getAllTests());
};

int UnitTestRunner::getNumResults() const{
	return this->results.size();
};
const UnitTestRunner::TestResult* UnitTestRunner::getResult(int index) const{
	return this->results.at(index);
};
void UnitTestRunner::resultsUpdated(){};
void UnitTestRunner::logMessage(const std::string &message){
	std::cout << message << std::endl;
};
void UnitTestRunner::beginNewTest(UnitTest* test, const std::string &subCategory){
	this->endTest();
	this->currentTest = test;

	TestResult* r = new TestResult();
	this->results.push_back(r);
	r->unitTestName = test->getName();
	r->subCategoryName = subCategory;
	r->passes = 0;
	r->failures = 0;

	this->logMessage("---------------------------------------------------------------");
	this->logMessage("Starting test: " + r->unitTestName + " / " + subCategory + "...");

	this->resultsUpdated();	
};
void UnitTestRunner::endTest(){
	if(this->results.size() > 0){
		const TestResult* r = this->results.at(this->results.size() - 1);

		if(r->failures > 0){
			std::string m("FAILED!! ");
			m.append(this->to_string(r->failures));
			m.append((r->failures == 1 ? " test" : " tests"));
			m.append(" failed out of a total of ");
			m.append(this->to_string(r->passes + r->failures));

			this->logMessage("");
			this->logMessage(m);
			this->logMessage("");
		}else{
			this->logMessage("All tests completed successfully");
		}
	}
};

void UnitTestRunner::addPass(){
	TestResult* r = this->results.at(this->results.size() - 1);
	r->passes++;

	std::string message("Test ");
	message.append(this->to_string(r->failures + r->passes));
	message.append(" passed");
	this->logMessage(message);

	this->resultsUpdated();
};
void UnitTestRunner::addFail(const std::string& failMessage){
	TestResult* r = this->results.at(this->results.size() - 1);
	r->failures++;

	std::string message("!!! Test ");
	message.append(this->to_string(r->failures + r->passes));
	message.append(" failed : ");
	message.append(failMessage);

	r->messages.push_back(message);
	this->logMessage(message);

	this->resultsUpdated();
};
