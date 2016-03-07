// AllInOneDll.h : main header file for the AllInOneDll DLL
//
#ifndef _ALLINONE_H
#define _ALLINONE_H
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols



// CAllInOneDllApp
// See AllInOneDll.cpp for the implementation of this class
//

class CAllInOneDllApp : public CWinApp
{
public:
	CAllInOneDllApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
#endif
