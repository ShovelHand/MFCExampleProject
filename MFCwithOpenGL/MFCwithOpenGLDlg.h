
// MFCwithOpenGLDlg.h : header file
//

#pragma once

#include <GL\glew.h>
#include <stdio.h>
#include <stdlib.h>

#include "OpenGLControl.h"
#include "afxwin.h"
#include "afxcmn.h"
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
public:
	afx_msg void OnBnClickedCheck1();
	CButton m_enableDistortCheck;
	BOOL m_enableDistortCheckVal;
	afx_msg void OnBnClickedButton1();
	CEdit m_iSizeControl;
	int m_iSizeVal;
	afx_msg void OnEnUpdateEdit2();
	CEdit m_iRandControl;
	int m_iRandVal;
	afx_msg void OnEnChangeEdit3();
	
	CSliderCtrl m_DistortSlider;
	CSliderCtrl m_SliderH;
	
	//slider integers
	int m_iDistortVal;
	int m_iOffset;
	int m_iH;
	int m_iPeriodVal;
	int m_iLacunarity;
	int m_iOctaves;

	bool m_bDistort;
	float m_fDistortVal;
	float m_fOffset;
	float m_fH;
	float m_fLacunarity;  


	afx_msg void OnTRBNThumbPosChangingSlider4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CSliderCtrl m_SliderOctaves;
	CSliderCtrl m_SliderLacunarity;
	CSliderCtrl m_SliderPeriod;
	CSliderCtrl m_SliderOffset;

	void UpdateTerrainBuilder();

	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedButton2();
	CSliderCtrl m_SliderIntensity;
	float m_fIntensity;
	CSliderCtrl m_SliderAmbient;
	float m_fAmbient;
	afx_msg void OnEnChangeEdit4();
	CEdit M_fLightX;
	float m_fLightX;
	CEdit m_LightY;
	float m_fLightY;
	CEdit m_LightZ;
	float m_fLightZ;
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit5();
	CButton m_RadioRidges;
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio1();
};
