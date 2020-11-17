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
	m_pBitmap = NULL;
	m_rawHeight = -1;
	m_rawWidth = -1;
	m_Islarger = FALSE;
	m_pBitmapInfo = NULL;
	m_largerTopLeft = cv::Point(-1, -1);
	m_RealLargeTopLeft = cv::Point(-1, -1);
	m_IsMove = FALSE;
	m_prevMouse = CPoint(0, 0);
}

CMyImageView::~CMyImageView()
{
	if (m_pBitmap != NULL)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}

	if (m_pBitmapInfo != NULL)
	{
		delete m_pBitmapInfo;
		m_pBitmapInfo = NULL;
	}
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

// Mat �� ���콺 �����Ͱ� ����Ű�� ��ġ�� RGB�� ���
void CMyImageView::cvCursorRGB(CPoint point, cv::Point textPoint , CPoint rectTopLeft, CPoint rectBottomRight)
{
	CString textToDraw = _T("");
	// �׷��� �̹��� ���κ� X,Y ��ǥ
	int fontSize = 0;
	cv::Point showpt;
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
	cv::Point insidePoint;
	cv::Point RealPoint;
	cv::Point imgPoint;

	if (point.x >= rectTopLeft.x && point.y >= rectTopLeft.y)
	{
		if (point.x <= rectBottomRight.x && point.y <= rectBottomRight.y)
		{
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
			RealPoint.x = int(imgPoint.x * insidePoint.x / (rectBottomRight.x - rectTopLeft.x));
			RealPoint.y = int(imgPoint.y * insidePoint.y / (rectBottomRight.y - rectTopLeft.y));

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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Gdiplus ����. . . ��� ���� ///////////////////////////////////////////////////////////////////////////////////////////////////////
// Gdiplus::Bitmap ���� 20201112 ���Ѱ�
void CMyImageView::cloneBitmap(Bitmap* oriBitmap)
{
	if (oriBitmap != NULL)
	{
		m_pBitmap = oriBitmap->Clone(0, 0, oriBitmap->GetWidth(), oriBitmap->GetHeight(), oriBitmap->GetPixelFormat());
	}
	else return;
}

// Image quality ������ �̻��.
// MemDC�� Ȱ��, text�� �̹��� ���� ����� 20201112 ���Ѱ�
void CMyImageView::cursorRGB(CPoint point ,CPoint rectTopLeft, CPoint rectBottomRight)
{
	if (m_pBitmap != NULL)
	{
		CPoint imgPoint;
		CPoint insidePoint;
		CPoint RealPoint;
		Color color;
		CString textToDraw = _T("");
		if (point.x >= rectTopLeft.x && point.y >= rectTopLeft.y)
		{
			if (point.x <= rectBottomRight.x && point.y <= rectBottomRight.y)
			{
				StringFormat SF;

				imgPoint.x = m_pBitmap->GetWidth();
				imgPoint.y = m_pBitmap->GetHeight();

				insidePoint.x = point.x - rectTopLeft.x;
				insidePoint.y = point.y - rectTopLeft.y;

				RealPoint.x = abs(imgPoint.x * insidePoint.x / rectBottomRight.x);
				RealPoint.y = abs(imgPoint.y * insidePoint.y / rectBottomRight.y);

				Graphics textG(m_pBitmap);
				textG.SetTextRenderingHint(TextRenderingHintAntiAlias);
				Gdiplus::Font F(L"Palatino Linotype Bold", 15, FontStyleBold, UnitPixel);
				RectF R(10, 10, 150, 20);


				SF.SetAlignment(StringAlignmentNear);
				SF.SetLineAlignment(StringAlignmentNear);

				SolidBrush B(Color(0, 0, 0));
				m_pBitmap->GetPixel(RealPoint.x, RealPoint.y, &color);
				textToDraw.Format(_T("%d, %d : %d"), RealPoint.x, RealPoint.y, color.GetR());
				textG.DrawString(textToDraw, -1, &F, R, &SF, &B);
			}
		}
	}
	else
		return;
}

void CMyImageView::MatToBitmap()
{
	if (m_pBitmap != NULL)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}

	if (m_DrawMat.empty())
	{
		return;
	}

	if (m_showWidth < 0 || m_showHeight < 0)
	{
		m_showHeight = m_DrawMat.cols;
		m_showHeight = m_DrawMat.rows;
	}
	Bitmap bitmap(m_showWidth, m_showHeight, (INT)m_DrawMat.step, PixelFormat8bppIndexed, m_DrawMat.data);
	m_pBitmap = bitmap.Clone(0, 0, bitmap.GetWidth(), bitmap.GetHeight(), bitmap.GetPixelFormat());
	CString debug;
	Color color;
	m_pBitmap->GetPixel(100, 100, &color);
	OutputDebugString(debug);
}
// Gdiplus ����. . . ��� ���� ///////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CMyImageView, CWnd)
END_MESSAGE_MAP()
// CMyImageView �޽��� ó�����Դϴ�.