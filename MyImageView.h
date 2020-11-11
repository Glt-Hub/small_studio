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
	Graphics*		m_pGraphics;
	Bitmap*			m_pBitmap; // ���� �̹����� clone�� Bitmap* �ּ�
	CRect			m_rcDisp;

public:
	Bitmap*			pointerRGB();	// RGB���� ���콺 ������ ��ǥ�� ǥ�õ� �̹����� �����͸� ��ȯ

protected:
	DECLARE_MESSAGE_MAP()
};


