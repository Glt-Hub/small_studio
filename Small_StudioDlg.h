
// Small_StudioDlg.h : ��� ����
//

#pragma once
#include "CrevisCtrl.h"
#include "LightCtrl.h"
#include "Small_StudioDlg.h"
#include "Small_Studio.h"
#include "OptionDlg.h"
#include "MyImageView.h"


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
	BOOL			LightCtrl(int dispNum);
	BOOL			LightSend(int dispNum, BOOL OnOff);
	LRESULT			OnMyMsg(WPARAM length, LPARAM lpara);
	CPoint			m_CurSor;

public:
	bool			m_IsSystemInit;		// Camera SDK Init ���� �����Դϴ�.
	BOOL			m_IsOpen[MAXCAM];	// ī�޶� Open ����
	BOOL			m_IsContext[MAXCAM];
	BOOL			m_IsPlay[MAXCAM];	// Play ���� (Grab & Show)
	BOOL			m_IsSerialOpen;		// Seiral Port Open����
	CCrevisCtrl*	m_pCamCtrl[MAXCAM]; // ī�޶�� Class ��ü �������Դϴ�. 
	CMyImageView*	m_pImageView[MAXCAM];
	INT32			m_statusCode;		// ���α׷� ���� �ڵ带 �����ϴ� �����Դϴ�.
	CString			m_strErr;			// ����� ���� String�Դϴ�

	CString			m_optionPath;		// option.ini ������ path (�ʱ� ����� �� ���� �޾ƿ�)

	HBITMAP			m_hBmp[MAXCAM];
	CLightCtrl*		m_pLightCtrl;		 // Serial port Control�� ��ü
	CClientDC*		m_pDC[MAXCAM];

	HANDLE			m_hPlayThread[MAXCAM];		// Continuous mode�� thread
	HANDLE			m_hPlayTerminate[MAXCAM];

	HANDLE			m_hOpenThread[MAXCAM];

	CRect			m_rcDisp[MAXCAM];
	HDC				m_hDC[MAXCAM];

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
	BOOL			DrawImageContinuous(int dispNum);
	BOOL			GrabImageSWTrigger(int dispNum);
	BOOL			DrawSingleImage(int dispNum);
	BOOL			DrawProcessed(int dispNum);
	void			thread1proc(); // Screen1ThreadProc���� ����Ǵ� ������ ���μ��� (1�� Screen grab -> draw)
	void			thread2proc(); // Screen2ThreadProc���� ����Ǵ� ������ ���μ��� (2�� Screen grab -> draw)
	void			Cam1OpenProc();
	void			Cam2OpenProc();
	void			OnCtxtClickedLarg1();
	void			OnCtxtClickedBin1();
	void			OnCtxtClickedLarg2();
	void			OnCtxtClickedBin2();
	void			OnCtxtClickedOvrly1();
	void			OnCtxtClickedOvrly2();
	
	afx_msg void OnBnClickedDebugdragon();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};
