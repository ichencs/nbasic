/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
Regex::RegexInterpretor

Classes:
	PureInterpretor					：正则表达式纯模拟器
***********************************************************************/

#ifndef VCZH_REGEX_REGEXPURE
#define VCZH_REGEX_REGEXPURE

#include "RegexAutomaton.h"


namespace regex_internal
{
	class PureResult
	{
		public:
			nint				start;
			nint				length;
			nint				finalState;
			nint				terminateState;
	};

	class PureInterpretor : public Object
	{
		protected:
			static const nint	SupportedCharCount = 0x10000;		// UTF-16

			nint				charMap[SupportedCharCount];		// char -> char set index
			nint**				transition;							// (state * char set index) -> state*
			bool*				finalState;							// state -> bool
			nint*				relatedFinalState;					// sate -> (finalState or -1)
			nint				stateCount;
			nint				charSetCount;
			nint				startState;
		public:
			PureInterpretor(Automaton::Ref dfa, CharRange::List& subsets);
			~PureInterpretor();

			bool				MatchHead(const wchar_t* input, const wchar_t* start, PureResult& result);
			bool				Match(const wchar_t* input, const wchar_t* start, PureResult& result);

			nint				GetStartState();
			nint				Transit(wchar_t input, nint state);
			bool				IsFinalState(nint state);
			bool				IsDeadState(nint state);

			void				PrepareForRelatedFinalStateTable();
			nint				GetRelatedFinalState(nint state);
	};
}

#endif