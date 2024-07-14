#pragma once


// CRectRecord

class CRectRecord : public CWnd
{
	DECLARE_DYNAMIC(CRectRecord)

public:
	CRectRecord();
	virtual ~CRectRecord();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


