
// SmartBox MasterKey Application.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CSmartBoxMasterKeyApp:
// See SmartBox MasterKey Application.cpp for the implementation of this class
//

class CSmartBoxMasterKeyApp : public CWinApp
{
public:
	CSmartBoxMasterKeyApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CSmartBoxMasterKeyApp theApp;
