// MyImageView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Small_Studio.h"
#include "MyImageView.h"


// CMyImageView

IMPLEMENT_DYNAMIC(CMyImageView, CWnd)

CMyImageView::CMyImageView()
{
	m_OriMat = cv::Mat();
	m_DrawMat = cv::Mat();
	m_rawHeight = -1;
	m_rawWidth = -1;
	m_Islarger = FALSE;
	m_pBitmapInfo = NULL;
	m_largerTopLeft = cv::Point(-1, -1);
	m_RealLargeTopLeft = cv::Point(-1, -1);
	m_IsMove = FALSE;
	m_prevMouse = CPoint(0, 0);
	m_IsCursorBin = FALSE;
	m_IsCursorLarger = FALSE;
	m_IsOverlay = TRUE;
}

CMyImageView::~CMyImageView()
{
	if (m_pBitmapInfo != NULL)
	{
		delete m_pBitmapInfo;
		m_pBitmapInfo = NULL;
	}
}
BEGIN_MESSAGE_MAP(CMyImageView, CWnd)
END_MESSAGE_MAP()

// �̹��� re capture�� BOOL ���� Reset
void CMyImageView::ImageViewerReset()
{
	m_Islarger = FALSE;
	m_IsMove = FALSE;
	m_IsCursorLarger = FALSE;
	m_IsCursorBin = FALSE;
}

// BYTE (�̹���) -> Mat 20201113 ���Ѱ�
void CMyImageView::pByteToMat(BYTE* imgbits, int width, int height)
{
	m_OriMat = cv::Mat();
	if (imgbits != NULL)
	{
		m_OriMat = cv::Mat(height, width, CV_8UC1, imgbits).clone();
		m_rawWidth = width;
		m_rawHeight = height;
	}
}

// �̹��� Drag �� ��ǥ�� "�̵�" ���ִ� �Լ� 20201117 ���Ѱ�
void CMyImageView::largerTopLeftMover(CPoint mpoint)
{
	if (m_Islarger && m_IsMove)
	{
		if (mpoint.x > m_prevMouse.x)
		{
			m_RealLargeTopLeft.x += (mpoint.x - m_prevMouse.x);
			
		}
		if (mpoint.y > m_prevMouse.y)
		{
			m_RealLargeTopLeft.y += (mpoint.y - m_prevMouse.y);
		}
		if (mpoint.x < m_prevMouse.x)
		{
			m_RealLargeTopLeft.x -= (m_prevMouse.x - mpoint.x);
		}
		if (mpoint.y < m_prevMouse.y)
		{
			m_RealLargeTopLeft.y -= (m_prevMouse.x - mpoint.x);
		}
		if (m_RealLargeTopLeft.x < 0)
		{
			m_RealLargeTopLeft.x = 0;
		}
		if (m_RealLargeTopLeft.y < 0)
		{
			m_RealLargeTopLeft.y = 0;
		}
		if (m_RealLargeTopLeft.x > m_DrawMat.cols / 2)
		{
			m_RealLargeTopLeft.x = m_DrawMat.cols / 2;
		}

		if (m_RealLargeTopLeft.y > m_DrawMat.rows / 2)
		{
			m_RealLargeTopLeft.y = m_DrawMat.rows / 2;
		}

	}
	m_prevMouse = mpoint;
}

// �̹��� Ȯ�� �� ��ǥ�� "��ȯ" ���ִ� �Լ� 20201117 ���Ѱ�
void CMyImageView::largerScreen(cv::Point pt)
{
	if (m_OriMat.empty())
	{
		return;
	}
	else
	{
		if (pt.x < 0)
		{
			m_RealLargeTopLeft.x = 0;
			m_RealLargeTopLeft.y = 0;
			m_largerTopLeft.x = 0;
			m_largerTopLeft.y = m_DrawMat.rows / 2;
		}
		else
		{
			// ���� Ȯ�� ������ Rect ���� ��� ��ǥ
			m_RealLargeTopLeft.x = pt.x;
			m_RealLargeTopLeft.y = pt.y;

			// ���� �� ������� ���� ��� ��ǥ�� ���� �� �ִ� �ִ� (Lock)
			if (m_RealLargeTopLeft.x > m_DrawMat.cols / 2)
			{
				m_RealLargeTopLeft.x = m_DrawMat.cols / 2;
			}

			if (m_RealLargeTopLeft.y > m_DrawMat.rows / 2)
			{
				m_RealLargeTopLeft.y = m_DrawMat.rows / 2;
			}

			// ���� StretchDIBits���� ���� ��ǥ�� (ȯ���� �ʿ���)
			m_largerTopLeft.x = m_RealLargeTopLeft.x;
			m_largerTopLeft.y = m_DrawMat.rows / 2 - m_RealLargeTopLeft.y;
		}
	}
}

// ���÷��� ���� ���콺 ��ǥ -> �̹��� ���� ��ǥ ��ȯ��
cv::Point CMyImageView::cursorPtToImage(CPoint point, CPoint rectTopLeft, CPoint rectBottomRight)
{
	cv::Point insidePoint;
	cv::Point imgPoint;
	// �̹��� Ȯ�� ��
	if (m_Islarger)
	{
		// Ȯ�� �������� ~ �̹��� ������ / 2 ���� ��|����
		insidePoint.x = point.x - rectTopLeft.x + m_RealLargeTopLeft.x;
		insidePoint.y = point.y - rectTopLeft.y + m_RealLargeTopLeft.y;
		imgPoint.x = m_DrawMat.cols / 2;
		imgPoint.y = m_DrawMat.rows / 2;
	}
	else
	{
		// ���콺 ��ǥ - ���÷��� ������� ��ǥ = �̹��� ���� ��ǥ
		insidePoint.x = point.x - rectTopLeft.x;
		insidePoint.y = point.y - rectTopLeft.y;
		imgPoint.x = m_DrawMat.cols;
		imgPoint.y = m_DrawMat.rows;
	}

	// ���÷��� ��ǥ != �̹��� ��ǥ�̹Ƿ� ȯ����� �ʿ���
	// �׷��� �̹��� ���� : ���÷��� ���� = �̹������� ��ǥ (���� ��) : ������ ��ǥ
	// �̹������� ��ǥ = ������ ��ǥ * �׷��� �̹��� ���� / ���÷��� ����

	return cv::Point
		(
		int(imgPoint.x * insidePoint.x / (rectBottomRight.x - rectTopLeft.x)), // ��ȯ�� x
		int(imgPoint.y * insidePoint.y / (rectBottomRight.y - rectTopLeft.y))  // ��ȯ�� y
		);
}

// Mat �� ���콺 �����Ͱ� ����Ű�� ��ġ�� RGB�� ��� 20201116 ���Ѱ�
void CMyImageView::cvCursorRGB(CPoint point, cv::Point textPoint , CPoint rectTopLeft, CPoint rectBottomRight)
{
	CString textToDraw = _T("");
	// �׷��� �̹��� ���κ� X,Y ��ǥ
	int fontSize = 0;
	if (m_OriMat.empty())
	{
		return;
	}
	else
	{
		if (m_Islarger == FALSE)
		{
			m_DrawMat = m_OriMat.clone();
			m_showWidth = m_rawWidth;
			m_showHeight = m_rawHeight;
			fontSize = 2;
		}
		else
		{
			m_DrawMat = m_OriMat.clone();
			fontSize = 1;
		}
	}
	
	if (point.x >= rectTopLeft.x && point.y >= rectTopLeft.y)
	{
		if (point.x <= rectBottomRight.x && point.y <= rectBottomRight.y)
		{
			cv::Point RealPoint = cursorPtToImage(point, rectTopLeft, rectBottomRight);
			int rgbv = m_OriMat.at<uchar>(RealPoint);

			CString stringToDraw;
			stringToDraw.Format(_T("%d %d / : %d"), RealPoint.x, RealPoint.y,rgbv);
			cv::String str = CT2A(stringToDraw);
			cv::putText(m_DrawMat, str, textPoint, cv::FONT_HERSHEY_SIMPLEX, fontSize, cv::Scalar(155, 0, 0), fontSize);
		}
	}

}

// ������ �� ������ ���÷��̿� Mat ��� �� �ʿ��� bitmapinfo 20201113 ���Ѱ�
void CMyImageView::createBitmapInfo(cv::Mat mat)
{
	if (m_pBitmapInfo != NULL)
	{
		delete m_pBitmapInfo;
		m_pBitmapInfo = NULL;
	}
	int w = mat.cols;
	int h = mat.rows;
	int bpp = 8;

	if (bpp == 8)
		m_pBitmapInfo = (BITMAPINFO *) new BYTE[sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD)];
	else // 24 or 32bit
		m_pBitmapInfo = (BITMAPINFO *) new BYTE[sizeof(BITMAPINFO)];

	m_pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pBitmapInfo->bmiHeader.biPlanes = 1;
	m_pBitmapInfo->bmiHeader.biBitCount = bpp;
	m_pBitmapInfo->bmiHeader.biCompression = BI_RGB;
	m_pBitmapInfo->bmiHeader.biSizeImage = 0;
	m_pBitmapInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pBitmapInfo->bmiHeader.biYPelsPerMeter = 0;
	m_pBitmapInfo->bmiHeader.biClrUsed = 0;
	m_pBitmapInfo->bmiHeader.biClrImportant = 0;

	if (bpp == 8)
	{
		for (int i = 0; i < 256; i++)
		{
			m_pBitmapInfo->bmiColors[i].rgbBlue = (BYTE)i;
			m_pBitmapInfo->bmiColors[i].rgbGreen = (BYTE)i;
			m_pBitmapInfo->bmiColors[i].rgbRed = (BYTE)i;
			m_pBitmapInfo->bmiColors[i].rgbReserved = 0;
		}
	}
	m_pBitmapInfo->bmiHeader.biWidth = w;
	m_pBitmapInfo->bmiHeader.biHeight = -h;
}

// Ŀ���κ� Ȯ�밡 Ȱ��ȭ �� �� �۵� 20201118 ���Ѱ�
void CMyImageView::cursorLarger(CPoint mpoint, CPoint rectTopLeft, CPoint rectBottomRight)
{
	int width = 100;
	int height = 50;
	int resizeVal = 1;

	if (m_Islarger)
	{
		resizeVal = 3;
	}
	else
	{
		resizeVal = 6;
	}

	if (m_OriMat.empty())
	{
		return;
	}
	cv::Point RealPoint = cursorPtToImage(mpoint, rectTopLeft, rectBottomRight);
	cv::Rect ROIrect(RealPoint.x, RealPoint.y, width, height);
	cv::Mat subImage = m_OriMat(ROIrect);
	cv::resize(subImage, subImage, cv::Size(), resizeVal, resizeVal);

	if (m_IsCursorBin)
	{
		cv::threshold(subImage, subImage, 127, 255, cv::THRESH_BINARY);
	}
	
	cv::copyMakeBorder(subImage, subImage, 2, 2, 2, 2, cv::BORDER_CONSTANT, cv::Scalar(155, 0, 0));

	for (int i = 0; i < subImage.cols; i++)
	{
		for (int j = 0; j < subImage.rows; j++)
		{
			int x = i + RealPoint.x + 50;
			int y = j + RealPoint.y + 50;

			m_DrawMat.at<uchar>(y,x) = subImage.at<uchar>(j,i);
		}
	}
	

	
}