#pragma once
#include "stdafx.h"



// CMyImageView

class CMyImageView : public CWnd
{
	DECLARE_DYNAMIC(CMyImageView)

public:
	CMyImageView();
	virtual ~CMyImageView();

public:
	Bitmap*			m_pBitmap; // ���� �̹����� clone�� Bitmap* �ּ�

public:
	void			cursorRGB(CPoint point, CPoint rectTopLeft, CPoint rectBottomRight);	// ���� RGB���� ���콺 ������ ��ǥ ǥ��
	Bitmap*			returnBitmap(); // �̹��� ��� �۾��� �Ϸ�� �̹����� �ּ� ��ȯ
	void			cloneBitmap(Bitmap* oriBitmap);

protected:
	DECLARE_MESSAGE_MAP()
};


