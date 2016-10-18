
// MFCwithOpenGLDlg.h : header file
//

#pragma once

#include <GL\glew.h>
#include <stdio.h>
#include <stdlib.h>

#include "OpenGLControl.h"
// CMFCwithOpenGLDlg dialog
class CMFCwithOpenGLDlg : public CDialogEx
{
// Construction
public:
	CMFCwithOpenGLDlg(CWnd* pParent = NULL);	// standard constructor
private:
	COpenGLControl m_oglWindow;

	

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCWITHOPENGL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
