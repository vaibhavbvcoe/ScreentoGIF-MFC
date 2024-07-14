
// screentogifDlg.h : header file
//

#pragma once
#include"RectRecord.h"
#include"AreaCapture.h"
#include"GifEncoder.h"
#include "afxwin.h"
#include "afxcmn.h"
// CscreentogifDlg dialog
class CscreentogifDlg : public CDialogEx
{
// Construction
public:
	CscreentogifDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SCREENTOGIF_DIALOG };

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
	afx_msg void OnBnClickedAreatocapture();
	afx_msg void OnBnClickedRecord();
	afx_msg void OnBnClickedStoprecord();
	static LPCWSTR lpszLayerWindowClass;
	static LPCWSTR lpszRectangleWindowClass;
	
	static HWND hLayerWnd;

	afx_msg LRESULT TriggerRectangle(WPARAM wparam,LPARAM lparam);
	CRectRecord * m_pRectWnd;
	CAreaCapture* m_AreaCptWnd;
	RECT *m_rcRecordingRect;
	afx_msg void OnActivateApp(BOOL bActive, DWORD dwThreadID);

	CGifEncoder *m_pGIFEncoder;
	BOOL m_bRecording;
	CEdit m_FrameRateEdt;
	CSliderCtrl m_sliderFrameRate;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeFrameRate();
	void UpdateCTRLs(bool bEnable);
	//afx_msg void OnNMCustomdrawSliderFramerate(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//ULONG_PTR gdiToken;
	afx_msg void OnDestroy();
};
