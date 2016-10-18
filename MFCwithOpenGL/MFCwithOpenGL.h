
// MFCwithOpenGL.h : main header file for the PROJECT_NAME application
//
#pragma once
#include <GL\glew.h>


#include <stdio.h>
#include <stdlib.h>
#include "OpenGLControl.h"

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMFCwithOpenGLApp:
// See MFCwithOpenGL.cpp for the implementation of this class
//

class CMFCwithOpenGLApp : public CWinApp
{
public:
	CMFCwithOpenGLApp();
	static void error_callback(int error, const char* description);

	void oglInitialize(void);

// Overrides
public:
	virtual BOOL InitInstance() override;

// Implementation

	DECLARE_MESSAGE_MAP()

	
};

extern CMFCwithOpenGLApp theApp;