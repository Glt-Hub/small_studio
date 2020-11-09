
// Small_StudioDlg.h : ��� ����
//

#pragma once
#include "CrevisCtrl.h"
#include "LightCtrl.h"
#include "Small_StudioDlg.h"
#include "Small_Studio.h"
#include "OptionDlg.h"


// CSmall_StudioDlg ��ȭ ����
class CSmall_StudioDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CSmall_StudioDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SMALL_STUDIO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCam1open();
	afx_msg void OnBnClickedCam2open();
	afx_msg void OnBnClickedOptionbtn();
	afx_msg void OnBnClickedLightopen();
	afx_msg void OnBnClickedCam1play();
	afx_msg void OnBnClickedCam2play();
	afx_msg void OnDestroy();

private:
	BOOL			camOpenSeq(int dispNum);
	CString			GetExePath();
	BOOL			GetOptionValue(int mode);
	BOOL			GetOptionValue(int mode, int dispNum);
	BOOL			DrawImageSeq(int dispNum);
//	BOOL			DIBMake(int dispNum);
//	BOOL			hbitmap2CImage(int dispNum);
	BOOL			LightCtrl(int dispNum);
	BOOL			LightSend(int dispNum, BOOL OnOff);
	BOOL			RawToGDIPBmp(int dispNum, int width, int height, BYTE* buffer);
	LRESULT			OnReceive(WPARAM length, LPARAM lpara);

public:
	bool			m_IsSystemInit;		// Camera SDK Init ���� �����Դϴ�.
	BOOL			m_DrawCommand[MAXCAM]; // invalidate ȣ��� BOOL
	BOOL			m_IsOpen[MAXCAM];	// ī�޶� Open ����
	BOOL			m_IsPlay[MAXCAM];	// Play ���� (Grab & Show)
	INT32			m_vidwidth[MAXCAM];
	INT32			m_vidheight[MAXCAM];
	BOOL			m_IsSerialOpen;		// Seiral Port Open����
	CCrevisCtrl*	m_pCamCtrl[MAXCAM]; // ī�޶�� Class ��ü �������Դϴ�. 
	INT32			m_statusCode;		// ���α׷� ���� �ڵ带 �����ϴ� �����Դϴ�.
	CString			m_strErr;			// ����� ���� String�Դϴ�

	CString			m_optionPath;		// option.ini ������ path (�ʱ� ����� �� ���� �޾ƿ�)

	BYTE*			m_pBit[MAXCAM];		// �̹�������
	HBITMAP			m_hBmp[MAXCAM];
//	CImage*			m_pCOriImage[MAXCAM]; // �̹��� ��ü
	CLightCtrl*		m_pLightCtrl;		 // Serial port Control�� ��ü

	HANDLE			m_hContThread;		// Continuous mode�� thread
	HANDLE			m_TriggerThread;	// Trigger Mode�� thread

	// GDI+
	CRect			m_rcDisp[MAXCAM];
	HDC				m_hDC[MAXCAM];
	Graphics*		m_pGraphics[MAXCAM];
	Bitmap*			m_pBitmap[MAXCAM];
	////////////////////////////////////////////////////////////////////////////////////////////////

	BOOL			m_optionmodal;
	//option.ini ���Ͽ��� read �� ���� 20201103 ���Ѱ�
	CString			m_ComPort;			// ���� Controller���� ����� port number�Դϴ�.
	CString			m_BaudRate;			// ���� Controller���� ����� baud rate�Դϴ�.
	CString			m_OptionBright[LIGHTCH];
	CString			m_CamIP[MAXCAM];	// IP �ּ� String�Դϴ�.
	DOUBLE			m_CamExposure[MAXCAM]; // Exposure time�Դϴ�.
	INT32			m_CamTrig[MAXCAM]; // ī�޶��� Trigger mode �Դϴ�.
	CString			m_CamLightCh[MAXCAM]; // ī�޶� �� ����� Light Channel number�Դϴ�.
	////////////////////////////////////////////////////////////////////////////////////////////////
	CRITICAL_SECTION mSc;
	afx_msg void OnBnClickedDebugdragon();
};
