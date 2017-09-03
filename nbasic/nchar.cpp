#include "stdafx.h"
#include "nchar.h"




nchar::~nchar()
{
}

uint nchar::toLower(uint ucs4)
{
	if (isUpper(ucs4))
	{
		return ucs4 - ('A' - 'a');
	}
	return ucs4;
}

uint nchar::toUpper(uint ucs4)
{
	if (isLower(ucs4))
	{
		return ucs4 - ('a' - 'A');
	}
	return ucs4;
}
