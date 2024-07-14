
// screentogifDlg.cpp : implementation file
//

#include "stdafx.h"
#include "screentogif.h"
#include "screentogifDlg.h"
#include"AreaCapture.h"
#include "RectRecord.h"




//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CscreentogifDlg dialog




CscreentogifDlg::CscreentogifDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CscreentogifDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);


}

void CscreentogifDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FRAME_RATE, m_FrameRateEdt);
	DDX_Control(pDX, IDC_SLIDER_FRAMERATE, m_sliderFrameRate);
}

BEGIN_MESSAGE_MAP(CscreentogifDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_AREATOCAPTURE, &CscreentogifDlg::OnBnClickedAreatocapture)
	ON_BN_CLICKED(IDC_RECORD, &CscreentogifDlg::OnBnClickedRecord)
	ON_BN_CLICKED(IDC_STOPRECORD, &CscreentogifDlg::OnBnClickedStoprecord)
	ON_MESSAGE(WM_APP,&CscreentogifDlg::TriggerRectangle)
	ON_WM_ACTIVATEAPP()
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_FRAME_RATE, &CscreentogifDlg::OnEnChangeFrameRate)
	//ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_FRAMERATE, &CscreentogifDlg::OnNMCustomdrawSliderFramerate)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

LRESULT CscreentogifDlg::TriggerRectangle(WPARAM wparam,LPARAM lparam)
{
	m_rcRecordingRect = (RECT*)lparam;
	if(m_pRectWnd)
	{
		delete m_pRectWnd;
		m_pRectWnd=NULL;


	}
	if (!m_pRectWnd) 
	{
		m_pRectWnd = new CRectRecord();


		m_pRectWnd->CreateEx(WS_EX_LAYERED | WS_EX_TOPMOST,AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_CROSS)),NULL,WS_POPUP,CRect(0,0,0,0),(CWnd*)this,NULL,NULL);

		m_pRectWnd->SetLayeredWindowAttributes(RGB(0, 0, 0), 255, LWA_ALPHA | LWA_COLORKEY);
	}
	m_pRectWnd->SetWindowPos(&CWnd::wndTop, m_rcRecordingRect->left - 2, m_rcRecordingRect->top - 2, 
		m_rcRecordingRect->right - m_rcRecordingRect->left + 4, m_rcRecordingRect->bottom - m_rcRecordingRect->top + 4, SWP_NOACTIVATE);
	m_pRectWnd->ShowWindow(SW_NORMAL);
	m_pRectWnd->UpdateWindow();

	return 0L;
}

// CscreentogifDlg message handlers

BOOL CscreentogifDlg::OnInitDialog()
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

	m_pRectWnd=NULL;
	m_AreaCptWnd=NULL; 

	m_pGIFEncoder = NULL;
	m_bRecording = FALSE;
	//ZeroMemory(m_rcRecordingRect,sizeof(RECT));
	m_rcRecordingRect=NULL;
	/*Gdiplus::GdiplusStartupInput gdiSI;
	Gdiplus::GdiplusStartup(&gdiToken, &gdiSI, NULL);*/



	m_sliderFrameRate.SetRange(30, 60, TRUE);
	m_sliderFrameRate.SetPos(30);


	m_FrameRateEdt.SetWindowTextW(L"30");

	RECT rt={0};

	SendMessage(WM_APP, 0, (LPARAM)&rt);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CscreentogifDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CscreentogifDlg::OnPaint()
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
HCURSOR CscreentogifDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CscreentogifDlg::OnBnClickedAreatocapture()
{

	CRect mainWndRect;
	GetWindowRect(&mainWndRect);
	SetWindowPos(&CWnd::wndBottom,mainWndRect.left,mainWndRect.top,mainWndRect.Width(),mainWndRect.Height(),0);




	if (m_pRectWnd->GetSafeHwnd()) 
	{ 
		m_pRectWnd->ShowWindow( SW_HIDE);
	}

	m_AreaCptWnd = new CAreaCapture();

	m_AreaCptWnd->CreateEx(WS_EX_LAYERED | WS_EX_TOPMOST,AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_CROSS)),NULL,WS_POPUP,CRect(0,0,0,0),(CWnd*)this,NULL,NULL);

	m_AreaCptWnd->SetLayeredWindowAttributes( RGB(192, 192,192), 50,  LWA_ALPHA |LWA_COLORKEY);

	m_AreaCptWnd->SetWindowPos(&CWnd::wndTopMost, ::GetSystemMetrics(SM_XVIRTUALSCREEN), ::GetSystemMetrics(SM_YVIRTUALSCREEN), ::GetSystemMetrics(SM_CXVIRTUALSCREEN), ::GetSystemMetrics(SM_CYVIRTUALSCREEN), SWP_NOSENDCHANGING);
	m_AreaCptWnd->ShowWindow(SW_NORMAL);
	m_AreaCptWnd->UpdateWindow();



}


void CscreentogifDlg::OnBnClickedRecord()
{
	if (m_pGIFEncoder == 0 && m_bRecording == FALSE) 
	{
		UpdateCTRLs(TRUE);
		m_pGIFEncoder = new CGifEncoder();
		if (m_pGIFEncoder) 
		{
			m_pGIFEncoder->SetFrameSize(m_rcRecordingRect->right - m_rcRecordingRect->left, 
				m_rcRecordingRect->bottom - m_rcRecordingRect->top);
			int dwTick = m_sliderFrameRate.GetPos();
			m_pGIFEncoder->SetFrameRate((float)dwTick);

			WCHAR szFilePath[MAX_PATH];
			GetModuleFileName(NULL, szFilePath, MAX_PATH);
			PathRemoveFileSpec(szFilePath);
			WCHAR szFileName[MAX_PATH];
			SYSTEMTIME st;
			GetLocalTime(&st);
			wsprintf(szFileName, L"%04d%02d%02d%02d%02d%02d%03d.gif", 
				st.wYear, 
				st.wMonth, 
				st.wDay, 
				st.wHour, 
				st.wMinute, 
				st.wSecond, 
				st.wMilliseconds);
			PathAppend(szFilePath, szFileName);

			m_pGIFEncoder->StartEncoder(std::wstring(szFilePath));
			m_bRecording = TRUE;
			SetTimer(1, 1000 / dwTick, NULL);
			UpdateCTRLs(TRUE);	
		}
	}
	else 
	{
		m_bRecording = FALSE;
		KillTimer( 1);
		if (m_pGIFEncoder) 
		{
			m_pGIFEncoder->FinishEncoder();
			delete m_pGIFEncoder;
			m_pGIFEncoder = NULL;
			UpdateCTRLs(FALSE);

		}

	}

	// TODO: Add your control notification handler code here
}

void CscreentogifDlg::UpdateCTRLs(bool nEnableDisable)
{
	GetDlgItem(IDC_AREATOCAPTURE)->EnableWindow(nEnableDisable);
	GetDlgItem(IDC_RECORD)->EnableWindow(!nEnableDisable);
	GetDlgItem(IDC_STOPRECORD)->EnableWindow(nEnableDisable);
	GetDlgItem(IDC_SLIDER_FRAMERATE)->EnableWindow(nEnableDisable);
	GetDlgItem(IDC_FRAME_RATE)->EnableWindow(nEnableDisable);


}

void CscreentogifDlg::OnBnClickedStoprecord()
{
	m_bRecording = FALSE;
	KillTimer( 1);
	if (m_pGIFEncoder) 
	{
		m_pGIFEncoder->FinishEncoder();
		delete m_pGIFEncoder;
		m_pGIFEncoder = NULL;
	}
	if(m_AreaCptWnd)
	{
		delete m_AreaCptWnd;
		m_AreaCptWnd=NULL;
	}
	if(m_pRectWnd)
	{
		delete m_pRectWnd;
		m_pRectWnd=NULL;
	}
	UpdateCTRLs(true);
	GetDlgItem(IDC_RECORD)->EnableWindow(TRUE);

	// TODO: Add your control notification handler code here
}



void CscreentogifDlg::OnActivateApp(BOOL bActive, DWORD dwThreadID)
{
	CDialogEx::OnActivateApp(bActive, dwThreadID);
	if(bActive==FALSE)
	{
		if (m_AreaCptWnd) 
		{
			m_AreaCptWnd->ShowWindow(SW_HIDE);
		}
		if (m_pRectWnd && m_bRecording == FALSE) 
		{
			m_pRectWnd->ShowWindow(SW_HIDE);
		}
		else 
		{
			if (m_pRectWnd) 
			{
				m_pRectWnd->ShowWindow(SW_NORMAL);
			}

			// TODO: Add your message handler code here
		}
	}
}

void CscreentogifDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	// You can have multiple sliders: Check which one sent the notification  
	if (IDC_SLIDER_FRAMERATE == pScrollBar->GetDlgCtrlID())
	{
		CString strSliderValue;
		int iValue = m_sliderFrameRate.GetPos();
		strSliderValue.Format(L"%d", iValue); 
		m_FrameRateEdt.SetWindowText(strSliderValue); // Change CEdit Value
	}


	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CscreentogifDlg::OnEnChangeFrameRate()
{
	CString strEditValue;
	m_FrameRateEdt.GetWindowText(strEditValue);
	int iCeditValue = _wtoi(strEditValue.GetBuffer());
	strEditValue.ReleaseBuffer();
	if(iCeditValue >60)
	{
		AfxMessageBox(TEXT("Dont excced the farme rate above 60 FPS"),MB_ICONERROR);
		m_FrameRateEdt.SetWindowTextW(L"30");
		iCeditValue=30;

	}
	m_sliderFrameRate.SetPos(iCeditValue);
}


void CscreentogifDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==1)
	{
		if (m_pGIFEncoder) 
		{ 
			Gdiplus::Bitmap* bmp = new Gdiplus::Bitmap(m_rcRecordingRect->right - m_rcRecordingRect->left, 
				m_rcRecordingRect->bottom - m_rcRecordingRect->top);
			if (bmp) 
			{
				Gdiplus::Graphics g(bmp);
				{ 
					const HDC hdc = g.GetHDC();
					HWND hDesktopWnd = ::GetDesktopWindow(); 
					{ 
						HDC hDC = ::GetDC(hDesktopWnd); 
						BitBlt(hdc, 0, 0, m_rcRecordingRect->right - m_rcRecordingRect->left, 
							m_rcRecordingRect->bottom - m_rcRecordingRect->top, 
							hDC, m_rcRecordingRect->left, m_rcRecordingRect->top, SRCCOPY); 
						::ReleaseDC(hDesktopWnd, hDC); 
						CURSORINFO cursor = { sizeof(cursor) }; 
						GetCursorInfo(&cursor); 
						if (cursor.flags == CURSOR_SHOWING) 
						{ 
							ICONINFO info = { sizeof(info) }; 
							GetIconInfo(cursor.hCursor, &info); 
							const int x = cursor.ptScreenPos.x - m_rcRecordingRect->left - info.xHotspot; 
							const int y = cursor.ptScreenPos.y - m_rcRecordingRect->top - info.yHotspot; 
							BITMAP bmpCursor = { 0 }; 
							GetObject(info.hbmColor, sizeof(bmpCursor), &bmpCursor); 
							DrawIconEx(hdc, x, y, cursor.hCursor, bmpCursor.bmWidth, bmpCursor.bmHeight, 0, NULL, DI_NORMAL); 
						} 
					} 
					g.ReleaseHDC(hdc); 
				} 
				m_pGIFEncoder->AddFrame(bmp); 
				delete bmp; 
			}
		}





	}
	CDialogEx::OnTimer(nIDEvent);
}


void CscreentogifDlg::OnDestroy()
{

	m_bRecording = FALSE;
	KillTimer(1);
	if (m_pGIFEncoder) 
	{
		m_pGIFEncoder->FinishEncoder();
		delete m_pGIFEncoder;
		m_pGIFEncoder = NULL;
	}

	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
}
