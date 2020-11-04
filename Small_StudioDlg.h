
// Small_StudioDlg.h : ��� ����
//

#pragma once
#include "CrevisCtrl.h"
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
	BOOL			DIBMake(int dispNum);
	BOOL			hbitmap2CImage(int dispNum);

public:
	bool			m_IsSystemInit;		// Camera SDK Init ���� �����Դϴ�.
	BOOL			m_IsOpen[MAXCAM];	// ī�޶� Open ����
	BOOL			m_IsPlay[MAXCAM];	// Play ���� (Grab & Show)
	CCrevisCtrl*	m_pCamCtrl[MAXCAM]; // ī�޶�� Class ��ü �������Դϴ�. 
	INT32			m_statusCode;		// ���α׷� ���� �ڵ带 �����ϴ� �����Դϴ�.
	CString			m_strErr;			// ����� ���� String�Դϴ�

	CString			m_optionPath;		// option.ini ������ path (�ʱ� ����� �� ���� �޾ƿ�)

	HBITMAP			m_hBmp[MAXCAM];
	CImage*			m_pCOriImage[MAXCAM]; // �̹��� ��ü

	//option.ini ���Ͽ��� read�� ���� 20201103 ���Ѱ�
	CString			m_ComPort;			// ���� Controller���� ����� port number�Դϴ�.
	CString			m_BaudRate;			// ���� Controller���� ����� baud rate�Դϴ�.
	CString			m_CamIP[MAXCAM];	// IP �ּ� String�Դϴ�.
	DOUBLE			m_CamExposure[MAXCAM]; // Exposure time�Դϴ�.
	INT32			m_CamTrig[MAXCAM]; // ī�޶��� Trigger mode �Դϴ�.



	CRITICAL_SECTION mSc;
};
