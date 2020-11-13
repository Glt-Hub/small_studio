// MyImageView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Small_Studio.h"
#include "MyImageView.h"


// CMyImageView

IMPLEMENT_DYNAMIC(CMyImageView, CWnd)

CMyImageView::CMyImageView(CRect dispRect, INT dispID)
{
	m_DispRect = dispRect;
	m_DispID = dispID;
	m_OriMat = cv::Mat();
	m_DrawMat = cv::Mat();
	m_pBitmap = NULL;
	m_rawHeight = -1;
	m_rawWidth = -1;
	m_larger = FALSE;
}

CMyImageView::~CMyImageView()
{
	if (m_pBitmap != NULL)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}
}

// ������ �� ������ ���÷��̿� Mat ��� 20201113 ���Ѱ�
BOOL CMyImageView::MatToScreen()
{
	BITMAPINFO* pBitmapInfo = NULL;
	int w = m_DrawMat.cols;
	int h = m_DrawMat.rows;
	int bpp = 8;

	if (bpp == 8)
		pBitmapInfo = (BITMAPINFO *) new BYTE[sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD)];
	else // 24 or 32bit
		pBitmapInfo = (BITMAPINFO *) new BYTE[sizeof(BITMAPINFO)];

	pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pBitmapInfo->bmiHeader.biPlanes = 1;
	pBitmapInfo->bmiHeader.biBitCount = bpp;
	pBitmapInfo->bmiHeader.biCompression = BI_RGB;
	pBitmapInfo->bmiHeader.biSizeImage = 0;
	pBitmapInfo->bmiHeader.biXPelsPerMeter = 0;
	pBitmapInfo->bmiHeader.biYPelsPerMeter = 0;
	pBitmapInfo->bmiHeader.biClrUsed = 0;
	pBitmapInfo->bmiHeader.biClrImportant = 0;

	if (bpp == 8)
	{
		for (int i = 0; i < 256; i++)
		{
			pBitmapInfo->bmiColors[i].rgbBlue = (BYTE)i;
			pBitmapInfo->bmiColors[i].rgbGreen = (BYTE)i;
			pBitmapInfo->bmiColors[i].rgbRed = (BYTE)i;
			pBitmapInfo->bmiColors[i].rgbReserved = 0;
		}
	}

	pBitmapInfo->bmiHeader.biWidth = w;
	pBitmapInfo->bmiHeader.biHeight = -h;

	CClientDC dc(GetDlgItem(IDC_PIC1));

	CRect rect;
	GetDlgItem(IDC_PIC1)->GetClientRect(&rect);

	SetStretchBltMode(dc.GetSafeHdc(), COLORONCOLOR);
	StretchDIBits(dc.GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, m_DrawMat.cols, m_DrawMat.rows, m_DrawMat.data, pBitmapInfo, DIB_RGB_COLORS, SRCCOPY);

	if (pBitmapInfo != NULL)
	{
		delete pBitmapInfo;
		pBitmapInfo = NULL;
	}

	return TRUE;
}

// BYTE Grayscale �� -> Mat 20201113 ���Ѱ�
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

// Mat �� ���콺 �����Ͱ� ����Ű�� ��ġ�� RGB�� ���
void CMyImageView::cvCursorRGB(CPoint point, CPoint rectTopLeft, CPoint rectBottomRight)
{
	CString textToDraw = _T("");
	if (m_OriMat.empty())
	{
		return;
	}
	else
	{
		if (m_DrawMat.empty())
		{ 
			m_DrawMat = m_OriMat.clone();
		}
	}

	if (m_larger)
	{

	}
	else
	{
		cv::Point imgPoint;
		cv::Point insidePoint;
		cv::Point RealPoint;

		if (point.x >= rectTopLeft.x && point.y >= rectTopLeft.y)
		{
			if (point.x <= rectBottomRight.x && point.y <= rectBottomRight.y)
			{
				// �׷��� �̹��� ���κ� X,Y ��ǥ
				imgPoint.x = m_DrawMat.cols;
				imgPoint.y = m_DrawMat.rows;

				// ���콺 ��ǥ - ���÷��� ������� ��ǥ = �̹��� ���� ��ǥ
				insidePoint.x = point.x - rectTopLeft.x;
				insidePoint.y = point.y - rectTopLeft.y;

				// ���÷��� ��ǥ != �̹��� ��ǥ�̹Ƿ� ȯ����� �ʿ���
				// �׷��� �̹��� ���� : ���÷��� ���� = �̹������� ��ǥ (���� ��) : ������ ��ǥ
				RealPoint.x = abs(imgPoint.x * insidePoint.x / rectBottomRight.x);
				RealPoint.y = abs(imgPoint.y * insidePoint.y / rectBottomRight.y);
				
				int rgbv = (int)m_OriMat.at<char>(RealPoint);

				CString stringToDraw;
				stringToDraw.Format(_T("%d %d / : %d"), RealPoint.x, RealPoint.y, rgbv);
				cv::String str = CT2A(stringToDraw);
				cv::putText(m_DrawMat, str, cv::Point(100, 100), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(255, 255, 255), 3);
			}
		}
	}


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
void CMyImageView::cursorRGB(CPoint point, CPoint rectTopLeft, CPoint rectBottomRight)
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