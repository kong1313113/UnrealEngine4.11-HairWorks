// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "CorePrivatePCH.h"

// 4 lines of 64 chars each, plus a null terminator
template <>
const ANSICHAR TCStringSpcHelper<ANSICHAR>::SpcArray[MAX_SPACES + 1] =
	"                                                                "
	"                                                                "
	"                                                                "
	"                                                               ";

template <>
const WIDECHAR TCStringSpcHelper<WIDECHAR>::SpcArray[MAX_SPACES + 1] =
	L"                                                                "
	L"                                                                "
	L"                                                                "
	L"                                                               ";

template <>
const ANSICHAR TCStringSpcHelper<ANSICHAR>::TabArray[MAX_TABS + 1] = 
	"\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t"
	"\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t"
	"\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t"
	"\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";

template <>
const WIDECHAR TCStringSpcHelper<WIDECHAR>::TabArray[MAX_TABS + 1] = 
	L"\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t"
	L"\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t"
	L"\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t"
	L"\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";

bool FToBoolHelper::FromCStringAnsi( const ANSICHAR* String )
{
	return FToBoolHelper::FromCStringWide( ANSI_TO_TCHAR(String) );
}

bool FToBoolHelper::FromCStringWide( const WIDECHAR* String )
{
	if (
			FCStringWide::Stricmp(String, (L"True") )==0 
		||	FCStringWide::Stricmp(String, (L"Yes"))==0
		||	FCStringWide::Stricmp(String, (L"On"))==0
		||	FCStringWide::Stricmp(String, *(GTrue.ToString()))==0
		||	FCStringWide::Stricmp(String, *(GYes.ToString()))==0)
	{
		return true;
	}
	else if(
			FCStringWide::Stricmp(String, (L"False"))==0
		||	FCStringWide::Stricmp(String, (L"No"))==0
		||	FCStringWide::Stricmp(String, (L"Off"))==0
		||	FCStringWide::Stricmp(String, *(GFalse.ToString()))==0
		||	FCStringWide::Stricmp(String, *(GNo.ToString()))==0)
	{
		return false;
	}
	else
	{
		return FCStringWide::Atoi(String) ? true : false;
	}
}
