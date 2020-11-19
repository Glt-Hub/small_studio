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
	int				m_rawWidth;		// original Mat width
	int				m_rawHeight;	// original Mat Height

public:
	BOOL			m_Islarger;		// �̹��� Ȯ�뿩��
	BOOL			m_IsMove;		// �̹��� �巡�� ����
	BOOL			m_IsCursorLarger;// Ŀ�� ����Ű�� �κ� Ȯ�� ����
	BOOL			m_IsCursorBin;	// Ŀ�� ����Ű�� �κ� Ȯ���� ����ȭ ����
	BOOL			m_IsOverlay;	// �̹��� ��� ��� �� overlay ��ɵ� �������
	cv::Mat			m_OriMat;		// ���� �̹��� ��� Mat
	cv::Mat			m_DrawMat;		// �̹��� ���� Mat
	BITMAPINFO*		m_pBitmapInfo;	// picture control draw�� bitmapinfo
	cv::Point		m_largerTopLeft;// Ȯ��� ���ʻ�� ��ǥ (CreateDIBitmap�� ��ȯ�� ��ǥ)
	cv::Point		m_RealLargeTopLeft; // Ȯ��� ���ʻ�� ��ǥ (����)
	CPoint			m_prevMouse; // ���콺 ������ �̵��� �Ǵ��� ��ǥ��
	int				m_showWidth; // ���������� Out�Ǵ� �̹����� Size
	int				m_showHeight;

public:
	void			pByteToMat(BYTE* imgbits, int width, int height);							// m_pOriMat ���� (Image BYTE -> cv::Mat m_pOriMat)
	cv::Point		cursorPtToImage(CPoint point, CPoint rectTopLeft, CPoint rectBottomRight);	// Ŭ���̾�Ʈ ���콺 ��ǥ -> �̹��� ������ǥ ȯ���
	void			cursorLarger(CPoint mpoint, CPoint rectTopLeft, CPoint rectBottomRight);	// Ŀ�� ����Ű�� �κ� Ȯ�� ���
	void			largerTopLeftMover(CPoint mpoint);											// �巡�׽� Ȯ���� �κ� ���� ��� ��ǥ ����
	void			largerScreen(cv::Point pt);													// Ȯ���� �� CreateDIBitmap ��� �����ϵ��� ��ǥ ��ȯ
	void			cvCursorRGB(CPoint point, cv::Point textPoint, CPoint rectTopLeft, CPoint rectBottomRight); // cursor�κ� Gray LV value ���
	void			createBitmapInfo(cv::Mat mat);												// CreateDIBitmap�� bitmapinfo ����
	void			ImageViewerReset();															// BOOL���� ���� FALSE SET (m_IsOverlay ����)

protected:
	DECLARE_MESSAGE_MAP()
};


