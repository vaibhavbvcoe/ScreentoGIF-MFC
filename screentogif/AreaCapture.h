#pragma once


// CAreaCapture

class CAreaCapture : public CWnd
{
	DECLARE_DYNAMIC(CAreaCapture)

public:
	CAreaCapture();
	virtual ~CAreaCapture();

protected:
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	BOOL m_bDrag;
	//BOOL m_bDown;
	CPoint m_posStart;
	CRect m_OldRect;
	RECT m_rcRecordingRect;

	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	CDC *pdc;
};


