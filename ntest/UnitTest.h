#pragma once
class UnitTest
{
public:
	UnitTest();
	~UnitTest();
};


#define TEST_CHECK_ERROR(CONDITION,DESCRIPTION) do{if(!(CONDITION))throw Error(DESCRIPTION);}while(0)

#define TEST_ASSERT(CONDITION) do{TEST_CHECK_ERROR(CONDITION,L"");}while(0)
// #define TEST_CASE(NAME)\
// 		extern void TESTCASE_##NAME();														\
// 			class TESTCASE_RUNNER_##NAME													\
// 			{																				\
// 			public:																			\
// 				static void RunUnitTest()													\
// 				{																			\
// 					UnitTest::PrintMessage(L_(#NAME));						\
// 					TESTCASE_##NAME();														\
// 				}																			\
// 				TESTCASE_RUNNER_##NAME()													\
// 				{																			\
// 					UnitTest::PushTest(&TESTCASE_RUNNER_##NAME::RunUnitTest);	\
// 				}																			\
// 			} TESTCASE_RUNNER_##NAME##_INSTANCE;											\
// 		void TESTCASE_##NAME()

extern void TestDateTime();
extern void TestAutoPointer();
extern void StringLeftRightSub();
