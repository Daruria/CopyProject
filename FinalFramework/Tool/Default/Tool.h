
// Tool.h : main header file for the Tool application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CToolApp:
// See Tool.cpp for the implementation of this class
//

class CToolApp : public CWinApp
{
public:
	CToolApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CToolApp theApp;
