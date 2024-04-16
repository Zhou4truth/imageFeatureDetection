
// IFDS04.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CIFDS04App:
// See IFDS04.cpp for the implementation of this class
//

class CIFDS04App : public CWinApp
{
public:
	CIFDS04App();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CIFDS04App theApp;
