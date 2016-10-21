
// MFCwithOpenGLDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFCwithOpenGL.h"
#include "MFCwithOpenGLDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCwithOpenGLDlg dialog



CMFCwithOpenGLDlg::CMFCwithOpenGLDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCWITHOPENGL_DIALOG, pParent)
	, m_enableDistortCheckVal(FALSE)
	, m_fDistortVal(0.1f)
	, m_iRandVal(19)
	, m_iSizeVal(40)
	, m_iLacunarity(2)
	, m_fLacunarity(2.0f)
	, m_iOctaves(7)
	, m_iH(5)
	, m_fH(0.5f)
	, m_iPeriodVal(64)
	, m_iOffset(7)
	, m_fOffset(0.7f)
	, m_bDistort(false)
	
	, m_fIntensity(0)
	, m_fAmbient(0)
	, m_fLightX(110)
	, m_fLightY(40)
	, m_fLightZ(11)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCwithOpenGLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_enableDistortCheck);
	DDX_Check(pDX, IDC_CHECK1, m_enableDistortCheckVal);


	DDX_Control(pDX, IDC_EDIT3, m_iRandControl);
	DDX_Text(pDX, IDC_EDIT3, m_iRandVal);

	DDX_Text(pDX, IDC_EDIT2, m_iSizeVal);

	//light pos inputs
	DDX_Text(pDX, IDC_EDIT1, m_fLightX);
	DDX_Text(pDX, IDC_EDIT4, m_fLightY);
	DDX_Text(pDX, IDC_EDIT5, m_fLightZ);




	DDX_Slider(pDX, IDC_SLIDERLACUNARIRTY, m_iLacunarity);
	DDV_MinMaxInt(pDX, m_iLacunarity, 0, 4);
	DDX_Slider(pDX, IDC_SLIDEROCTAVES, m_iOctaves);
	DDV_MinMaxInt(pDX, m_iOctaves, 1, 10);
	DDX_Control(pDX, IDC_SLIDER_DISTORT, m_DistortSlider);
	DDX_Control(pDX, IDC_SLIDERH, m_SliderH);
	DDX_Slider(pDX, IDC_SLIDERH, m_iH);
	DDV_MinMaxInt(pDX, m_iH, 1, 10);
	DDX_Control(pDX, IDC_SLIDEROCTAVES, m_SliderOctaves);
	DDX_Control(pDX, IDC_SLIDERLACUNARIRTY, m_SliderLacunarity);
	DDX_Control(pDX, IDC_SLIDERPERIOD, m_SliderPeriod);
	DDX_Slider(pDX, IDC_SLIDERPERIOD, m_iPeriodVal);
	DDV_MinMaxInt(pDX, m_iPeriodVal, 1, 256);
	DDX_Control(pDX, IDC_SLIDER6, m_SliderOffset);
	DDX_Slider(pDX, IDC_SLIDER_DISTORT, m_iDistortVal);
	DDV_MinMaxInt(pDX, m_iDistortVal, 0, 10);
	DDX_Slider(pDX, IDC_SLIDER6, m_iOffset);
	DDV_MinMaxInt(pDX, m_iOffset, 0, 10);
	DDX_Control(pDX, IDC_SLIDER1, m_SliderIntensity);

	DDV_MinMaxFloat(pDX, m_fIntensity, 0, 100);
	DDX_Control(pDX, IDC_SLIDER2, m_SliderAmbient);

	DDV_MinMaxFloat(pDX, m_fAmbient, 0.0, 10.0);
	DDX_Control(pDX, IDC_EDIT1, M_fLightX);
	DDX_Text(pDX, IDC_EDIT1, m_fLightX);
	DDX_Control(pDX, IDC_EDIT4, m_LightY);
	DDX_Text(pDX, IDC_EDIT4, m_fLightY);
	DDX_Control(pDX, IDC_EDIT5, m_LightZ);
	DDX_Text(pDX, IDC_EDIT5, m_fLightZ);
	DDX_Control(pDX, IDCANCEL, m_RadioRidges);
}

BEGIN_MESSAGE_MAP(CMFCwithOpenGLDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHECK1, &CMFCwithOpenGLDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCwithOpenGLDlg::OnBnClickedButton1)
	ON_EN_UPDATE(IDC_EDIT2, &CMFCwithOpenGLDlg::OnEnUpdateEdit2)
	ON_EN_CHANGE(IDC_EDIT3, &CMFCwithOpenGLDlg::OnEnChangeEdit3)
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_CHECK2, &CMFCwithOpenGLDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCwithOpenGLDlg::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_EDIT4, &CMFCwithOpenGLDlg::OnEnChangeEdit4)
	ON_EN_CHANGE(IDC_EDIT1, &CMFCwithOpenGLDlg::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT5, &CMFCwithOpenGLDlg::OnEnChangeEdit5)
	ON_BN_CLICKED(IDC_RADIO2, &CMFCwithOpenGLDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CMFCwithOpenGLDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO1, &CMFCwithOpenGLDlg::OnBnClickedRadio1)
END_MESSAGE_MAP()


// CMFCwithOpenGLDlg message handlers

BOOL CMFCwithOpenGLDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	ShowWindow(SW_NORMAL);

	// TODO: Add extra initialization here
	//show the window which displays all the openGL stuff. 
	CRect rect;

	// Get size and position of the picture control
	GetDlgItem(IDC_OPENGL)->GetWindowRect(rect);

	// Convert screen coordinates to client coordinates
	ScreenToClient(rect);

	// Create OpenGL Control window
	m_oglWindow.oglCreate(rect, this);

	// Setup the OpenGL Window's timer to render
	m_oglWindow.m_unpTimer = m_oglWindow.SetTimer(1, 1, 0);
	

	//control initializations here
	//sliders
	m_DistortSlider.SetRange(1, 10);
	m_DistortSlider.SetPos(1);
	m_DistortSlider.EnableWindow(FALSE);
	m_SliderPeriod.SetRange(1, 256);
	m_SliderPeriod.SetPos(64);
	m_SliderH.SetRange(1, 10);
	m_SliderH.SetPos(5);
	m_SliderLacunarity.SetRange(1, 6);
	m_SliderLacunarity.SetPos(2);
	m_SliderOctaves.SetRange(1, 10);
	m_SliderOctaves.SetPos(7);
	m_SliderOffset.SetRange(0, 10);
	m_SliderOffset.SetPos(7);

	m_SliderIntensity.SetRange(1, 100);
	m_SliderIntensity.SetPos(50);
	m_oglWindow.m_fIntensity = 50.0f;

	m_SliderAmbient.SetRange(0, 10);
	m_SliderAmbient.SetPos(1);
	m_oglWindow.m_fAmbient = 0.1f;

	
	
	UpdateTerrainBuilder();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCwithOpenGLDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCwithOpenGLDlg::OnPaint()
{

	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCwithOpenGLDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCwithOpenGLDlg::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_enableDistortCheckVal) {
		m_DistortSlider.EnableWindow(TRUE);
		m_bDistort = true;
	}
	else {
		m_DistortSlider.EnableWindow(FALSE);
		m_bDistort = false;
	}
	UpdateTerrainBuilder();
}


void CMFCwithOpenGLDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	m_oglWindow.Generate();
}


void CMFCwithOpenGLDlg::OnEnUpdateEdit2()
{
	m_iSizeVal = GetDlgItemInt(IDC_EDIT2);
	m_oglWindow.SetSize(m_iSizeVal);
}


void CMFCwithOpenGLDlg::OnEnChangeEdit3()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	m_iRandVal = GetDlgItemInt(IDC_EDIT3);
	m_oglWindow.SetRandomSeed(m_iRandVal);


}



//Distortion slider
void CMFCwithOpenGLDlg::OnTRBNThumbPosChangingSlider4(NMHDR *pNMHDR, LRESULT *pResult)
{
	// This feature requires Windows Vista or greater.
	// The symbol _WIN32_WINNT must be >= 0x0600.
	NMTRBTHUMBPOSCHANGING *pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING *>(pNMHDR);
	// TODO: Add your control notification handler code here
	m_fDistortVal = float(m_DistortSlider.GetPos() / 10.0f);
	*pResult = 0;
}

void CMFCwithOpenGLDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
	if (pScrollBar == (CScrollBar *)&m_DistortSlider)
	{
		m_fDistortVal = float(m_DistortSlider.GetPos() / 10.0f);
	}
	else if (pScrollBar == (CScrollBar *)&m_SliderH)
	{
		m_fH = float(m_SliderH.GetPos() / 10.0f);
	}
	else if (pScrollBar == (CScrollBar *)&m_SliderLacunarity)
	{
		m_fLacunarity = float(m_SliderLacunarity.GetPos() / 10.0f);
	}
	else if (pScrollBar == (CScrollBar *)&m_SliderOctaves)
	{
		m_iOctaves = m_SliderOctaves.GetPos();
	}
	else if (pScrollBar == (CScrollBar *)&m_SliderPeriod)
	{
		m_iPeriodVal = m_SliderPeriod.GetPos();
	}
	else if (pScrollBar == (CScrollBar *)&m_SliderIntensity)
	{
		m_oglWindow.m_fIntensity = (float)m_SliderIntensity.GetPos();
	}
	else if (pScrollBar == (CScrollBar *)&m_SliderAmbient)
	{
		m_oglWindow.m_fAmbient = (float)m_SliderAmbient.GetPos() / 10.0f;
	}
	UpdateTerrainBuilder();
	UpdateData(TRUE);
}

void CMFCwithOpenGLDlg::UpdateTerrainBuilder()
{
	m_oglWindow.values.H = m_fH;
	m_oglWindow.values.lac = m_fLacunarity;
	m_oglWindow.values.octaves = m_iOctaves;
	m_oglWindow.values.offset = m_iOffset;
	m_oglWindow.values.period = m_iPeriodVal;
	m_oglWindow.values.distortVal = m_fDistortVal;
	m_oglWindow.values.distort = m_bDistort;
	m_oglWindow.UpdateGenerator();
}


void CMFCwithOpenGLDlg::OnBnClickedCheck2()
{
	m_oglWindow.m_bShowHeightMap = !m_oglWindow.m_bShowHeightMap;
}


void CMFCwithOpenGLDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	m_oglWindow.ResetCamera();
}


void CMFCwithOpenGLDlg::OnEnChangeEdit1()
{
	m_fLightX = GetDlgItemInt(IDC_EDIT1);
	m_oglWindow.lightPos.x = m_fLightX;
}

void CMFCwithOpenGLDlg::OnEnChangeEdit4()
{
	m_fLightY = GetDlgItemInt(IDC_EDIT4);
	m_oglWindow.lightPos.y = m_fLightY;
}

void CMFCwithOpenGLDlg::OnEnChangeEdit5()
{
	m_fLightZ = GetDlgItemInt(IDC_EDIT5);
	m_oglWindow.lightPos.z = m_fLightZ;
}

//hills
void CMFCwithOpenGLDlg::OnBnClickedRadio1()
{
	m_oglWindow.SetTerrainType(COpenGLControl::hills);
}

//ridges
void CMFCwithOpenGLDlg::OnBnClickedRadio2()
{
	m_oglWindow.SetTerrainType(COpenGLControl::ridges);
}

//plain
void CMFCwithOpenGLDlg::OnBnClickedRadio3()
{
	m_oglWindow.SetTerrainType(COpenGLControl::plain);
}


