// AreaCapture.cpp : implementation file
//

#include "stdafx.h"
#include "screentogif.h"
#include "AreaCapture.h"

#include<dwmapi.h>
#pragma comment(lib,"dwmapi")
	
// CAreaCapture

IMPLEMENT_DYNAMIC(CAreaCapture, CWnd)

	CAreaCapture::CAreaCapture()
{
	m_bDrag=FALSE;
	//m_bDown=FALSE;
	ZeroMemory( &m_posStart,sizeof(CPoint));
	ZeroMemory( &m_OldRect,sizeof(CRect));
	//ZeroMemory( &m_rcRecordingRect,sizeof(RECT));
	::SetRect(&m_rcRecordingRect, 0, 0, GetSystemMetrics(SM_CXVIRTUALSCREEN), GetSystemMetrics(SM_CYVIRTUALSCREEN));
::OffsetRect(&m_rcRecordingRect, GetSystemMetrics(SM_XVIRTUALSCREEN), GetSystemMetrics(SM_YVIRTUALSCREEN));

	

}

CAreaCapture::~CAreaCapture()
{
}


BEGIN_MESSAGE_MAP(CAreaCapture, CWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


void CAreaCapture::OnLButtonDown(UINT nFlags, CPoint point)
{
	
	pdc = GetDC();
	SetCapture(); 
	memcpy(&m_posStart,&point,sizeof(CPoint));
	return CWnd::OnLButtonDown(nFlags, point);

	
}


void CAreaCapture::OnLButtonUp(UINT nFlags, CPoint point)
{


	// TODO: Add your message handler code here and/or call default
	if (GetCapture() == this) 
	{
		pdc->DeleteDC();
		ReleaseCapture();
		if (m_bDrag) 
		{
			m_bDrag = FALSE;
			RECT rect ;
			rect.left=  min(point.x, m_posStart.x);
			rect.top =  min(point.y, m_posStart.y);
			rect.right =  max(point.x, m_posStart.x);
			rect.bottom =  max(point.y, m_posStart.y);
			ShowWindow( SW_HIDE);
			m_rcRecordingRect = rect;
		}
		else 
		{
			ShowWindow( SW_HIDE);
			CWnd* hTargetWnd = WindowFromPoint(m_posStart);
			hTargetWnd = hTargetWnd->GetAncestor(GA_ROOT);
			if (hTargetWnd) 
			{
				RECT rect;
				if (DwmGetWindowAttribute(hTargetWnd->GetSafeHwnd(), DWMWA_EXTENDED_FRAME_BOUNDS, &rect, sizeof(rect)) != S_OK) 
				{
					::GetWindowRect(hTargetWnd->GetSafeHwnd(), &rect);
				}
				m_rcRecordingRect = rect;
			}
		}
		::SendMessage( ::GetParent(m_hWnd), WM_APP, 0, (LPARAM)&m_rcRecordingRect);
		
	}


	CWnd::OnLButtonUp(nFlags, point);
}


void CAreaCapture::OnMouseMove(UINT nFlags, CPoint point)
{
	if ( GetCapture() == this)
	{
		if (!m_bDrag) 
		{
			if (abs(point.x - m_posStart.x) > GetSystemMetrics(SM_CXDRAG) && 
				abs(point.y - m_posStart.y) > GetSystemMetrics(SM_CYDRAG)) 
			{
				m_bDrag = TRUE;
			}
		}
		else 
		{
			CRgn rgn1,rgn2;
			
			rgn1.CreateRectRgnIndirect(&m_OldRect);
			
			CString buf;
			buf.Format(TEXT("@@@@@ OLD RectRgn LEFT = %d TOP = %d RIGHT = %d BOTTOM =%d@@@@@"),m_OldRect.left,m_OldRect.top,m_OldRect.right,m_OldRect.bottom);
			OutputDebugString(buf);
			
			CRect newRect ;
			newRect.left=  min(point.x, m_posStart.x);
			newRect.top =  min(point.y, m_posStart.y);
			newRect.right = max(point.x, m_posStart.x);
			newRect.bottom = max(point.y, m_posStart.y);
			ScreenToClient(&newRect);
			//::ScreenToClient(m_hWnd,((LPPOINT)&rect) + 1); 

			buf.Format(TEXT("##### NEW RectRgn LEFT = %d TOP = %d RIGHT = %d BOTTOM =%d##### "),newRect.left,newRect.top,newRect.right,newRect.bottom);
			OutputDebugString(buf);

			
			
			rgn2.CreateRectRgnIndirect(&newRect);
			
			rgn1.CombineRgn(&rgn1, &rgn2, RGN_DIFF); 
		
			CBrush brush;			
			brush.CreateSolidBrush(RGB(192, 192, 192));
			pdc->FillRgn(&rgn1,&brush);
			
			CBrush pBrush2;
			pBrush2.CreateSolidBrush(RGB(255, 0, 0));		
			
			pdc->FillRect( &newRect,&pBrush2);
			m_OldRect=newRect;
			
		}
		
	}
	return CWnd::OnMouseMove(nFlags, point);	
}


void CAreaCapture::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	GetWindowRect( &m_rcRecordingRect);
	ShowWindow( SW_HIDE);
	::SendMessage(::GetParent(m_hWnd), WM_APP, 0, (LPARAM)&m_rcRecordingRect);
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CAreaCapture::OnRButtonDown(UINT nFlags, CPoint point)
{
	GetWindowRect( &m_rcRecordingRect);
	ShowWindow( SW_HIDE);
	::SendMessage(::GetParent(m_hWnd), WM_APP, 0, (LPARAM)&m_rcRecordingRect);

	CWnd::OnRButtonDown(nFlags, point);
}
