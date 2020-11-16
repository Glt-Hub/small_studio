#pragma once
#include "stdafx.h"



// CMyImageView

class CMyImageView : public CWnd
{
	DECLARE_DYNAMIC(CMyImageView)

public:
	CMyImageView();
	virtual ~CMyImageView();

private:
	CRect			m_DispRect;
	INT				m_DispID;
	int				m_rawWidth;
	int				m_rawHeight;

public:
	BOOL			m_Islarger;
	cv::Mat			m_OriMat; // OpenCV Mat
	cv::Mat			m_DrawMat;
	BITMAPINFO*		m_pBitmapInfo;
	int				m_showWidth; // ���������� Out�Ǵ� �̹����� Size
	int				m_showHeight;

public:
	void			largerScreen();
	void			pByteToMat(BYTE* imgbits, int width, int height); // m_pMat ���� �� ��ȯ
	void			cvCursorRGB(CPoint point, CPoint rectTopLeft, CPoint rectBottomRight);
	void			createBitmapInfo(cv::Mat mat);
	
// GDIPlus ���� : ��� ���� 20201113 ���Ѱ�
private:
	Bitmap*			m_pBitmap; // ���� �̹����� clone�� Bitmap* �ּ�
	void			MatToBitmap(); // Mat�� ������ Gdiplus::Bitmap���� ��ȯ (m_pBitmap�� �ּ�)
	void			cloneBitmap(Bitmap* oriBitmap);
	void			cursorRGB(CPoint point, CPoint rectTopLeft, CPoint rectBottomRight);	// ���� RGB���� ���콺 ������ ��ǥ ǥ��
////////////////////////////////////////////

protected:
	DECLARE_MESSAGE_MAP()
};


