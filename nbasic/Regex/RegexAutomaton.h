/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
Regex::RegexAutomaton

Classes:
	State						��״̬
	Transition					��ת��
	Automaton					��״̬��

Functions:
	EpsilonNfaToNfa				��ȥEpsilon
	NfaToDfa					��NFAתDFA
***********************************************************************/

#ifndef VCZH_REGEX_REGEXAUTOMATON
#define VCZH_REGEX_REGEXAUTOMATON

#include "RegexData.h"

 

	namespace regex_internal
	{
		class State;
		class Transition;

		class Transition
		{
		public:
			enum Type
			{
				Chars,				//rangeΪ�ַ���Χ
				Epsilon,
				BeginString,
				EndString,
				Nop,				//�޶�������������epsilon�������������ȼ���
				Capture,			//captureΪ����Ƶ��
				Match,				//captureΪ����Ƶ����indexΪƥ���λ�ã�-1����ƥ��Ƶ�������������Ŀ
				Positive,			//����ƥ��
				Negative,			//����ƥ��
				NegativeFail,		//����ƥ��ʧ��
				End					//Capture, Position, Negative
			};

			State*								source;
			State*								target;
			CharRange							range;
			Type								type;
			nint								capture;
			nint								index;
		};

		class State
		{
		public:
			NList<Transition*>		transitions;
			NList<Transition*>		inputs;
			bool								finalState;
			void*								userData;
		};

		class Automaton
		{
		public:
			typedef Ptr<Automaton>		Ref;

			NList<Ptr<State>>		states;
			NList<Ptr<Transition>>	transitions;
			NList<WString>			captureNames;
			State*								startState;

			Automaton();

			State*								NewState();
			Transition*							NewTransition(State* start, State* end);
			Transition*							NewChars(State* start, State* end, CharRange range);
			Transition*							NewEpsilon(State* start, State* end);
			Transition*							NewBeginString(State* start, State* end);
			Transition*							NewEndString(State* start, State* end);
			Transition*							NewNop(State* start, State* end);
			Transition*							NewCapture(State* start, State* end, nint capture);
			Transition*							NewMatch(State* start, State* end, nint capture, nint index=-1);
			Transition*							NewPositive(State* start, State* end);
			Transition*							NewNegative(State* start, State* end);
			Transition*							NewNegativeFail(State* start, State* end);
			Transition*							NewEnd(State* start, State* end);
		};

		extern bool								PureEpsilonChecker(Transition* transition);
		extern bool								RichEpsilonChecker(Transition* transition);
		extern bool								AreEqual(Transition* transA, Transition* transB);
		extern Automaton::Ref					EpsilonNfaToNfa(Automaton::Ref source, bool(*epsilonChecker)(Transition*), Dictionary<State*, State*>& nfaStateMap);
		extern Automaton::Ref					NfaToDfa(Automaton::Ref source, Group<State*, State*>& dfaStateMap);
	}


#endif