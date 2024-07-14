// RectRecord.cpp : implementation file
//

#include "stdafx.h"
#include "screentogif.h"
#include "RectRecord.h"


// CRectRecord

IMPLEMENT_DYNAMIC(CRectRecord, CWnd)

CRectRecord::CRectRecord()
{

}

CRectRecord::~CRectRecord()
{
}


BEGIN_MESSAGE_MAP(CRectRecord, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CRectRecord message handlers




void CRectRecord::OnPaint()
{
	PAINTSTRUCT ps; 
	CDC* pDC = BeginPaint( &ps);
	HDC  hdc = pDC->GetSafeHdc();
	HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
	RECT rect;
	GetClientRect(&rect);
	Rectangle(pDC->GetSafeHdc(), 1, 1, rect.right, rect.bottom);
	SelectObject(hdc, hOldBrush);
	SelectObject(hdc, hOldPen);
	EndPaint(&ps);


}
