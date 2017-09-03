#pragma once

typedef unsigned short ushort;		//2字节定长字符
typedef unsigned int uint;			//4字节定长字符

class nchar
{
public:
	nchar() :ucs(0) {};
	nchar(int rc) :ucs(rc) {};
	nchar(short rc) :ucs(rc) {};
	nchar(char rc) :ucs(rc) {};
	~nchar();
public:
	friend inline bool operator==(nchar c1, nchar c2) { return c1.ucs == c2.ucs; }
	friend inline bool operator< (nchar c1, nchar c2) { return c1.ucs < c2.ucs; }

	friend inline bool operator!=(nchar c1, nchar c2) { return !operator==(c1, c2); }
	friend inline bool operator>=(nchar c1, nchar c2) { return !operator< (c1, c2); }
	friend inline bool operator> (nchar c1, nchar c2) { return  operator< (c2, c1); }
	friend inline bool operator<=(nchar c1, nchar c2) { return !operator< (c2, c1); }

public:
	inline char toLatin1() const { return ucs > 0xff ? '\0' : char(ucs); }

public:
	inline bool isLetter() const { return nchar::isLetter(ucs); }
	inline bool isNumber() const { return nchar::isNumber(ucs); }
	inline bool isLetterOrNumber() const { return nchar::isLetterOrNumber(ucs); }
 	inline bool isDigit() const { return nchar::isDigit(ucs); }
	inline bool isLower() const { return nchar::isLower(ucs); }
	inline bool isUpper() const { return nchar::isUpper(ucs); }
	inline ushort unicode() const { return ucs; }

public:
	static inline bool isNumber(uint ucs4)
	{
		return ucs4 <= '9' && ucs4 >= '0';
	}
	static inline bool isDigit(uint ucs4) 
	{
		return ucs4 <= '9' && ucs4 >= '0';
	}

	static inline bool isLetter(uint ucs4)
	{
		return ucs4 >= 'A' && ucs4 <= 'z' && (ucs4 >= 'a' || ucs4 <= 'Z');
	}
	static inline bool isLower(uint ucs4) 
	{
		return ucs4 <= 'z' && ucs4 >= 'a';
	}
	static inline bool isUpper(uint ucs4)  
	{
		return ucs4 <= 'Z' && ucs4 >= 'A';
	}
	static inline bool isLetterOrNumber(uint ucs4)
	{
		return (ucs4 >= 'A' && ucs4 <= 'z' && (ucs4 >= 'a' || ucs4 <= 'Z'))
			|| (ucs4 >= '0' && ucs4 <= '9');
	}

	static uint toLower(uint ucs4) ;
	static uint toUpper(uint ucs4) ;

public:


private:
	ushort ucs;
};

