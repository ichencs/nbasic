/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
Regex::RegexInterpretor

Classes:
	RichInterpretor					��������ʽ��ȫģ����
***********************************************************************/

#ifndef VCZH_REGEX_REGEXRICH
#define VCZH_REGEX_REGEXRICH

#include "RegexAutomaton.h"

 
	namespace regex_internal
	{
		class CaptureRecord
		{
		public:
			nint								capture;
			nint								start;
			nint								length;

			bool								operator==(const CaptureRecord& record)const;
		};
	}

	template<>
	struct POD<regex_internal::CaptureRecord>
	{
		static const bool Result=true;
	};

	namespace regex_internal
	{
		class RichResult
		{
		public:
			nint								start;
			nint								length;
			NList<CaptureRecord>	captures;
		};

		class RichInterpretor : public Object
		{
		public:
		protected:
			class UserData
			{
			public:
				bool							NeedKeepState;
			};

			Automaton::Ref						dfa;
			UserData*							datas;
		public:
			RichInterpretor(Automaton::Ref _dfa);
			~RichInterpretor();

			bool								MatchHead(const wchar_t* input, const wchar_t* start, RichResult& result);
			bool								Match(const wchar_t* input, const wchar_t* start, RichResult& result);
			const NList<WString>&	CaptureNames();
		};
	};

#endif