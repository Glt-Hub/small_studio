
// Small_StudioDlg.h : ��� ����
//

#pragma once
#include "CrevisCtrl.h"
#include "LightCtrl.h"
#include "Small_StudioDlg.h"
#include "Small_Studio.h"
#include "OptionDlg.h"
#include "MyImageView.h"
#include "afxwin.h"

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

//////////////////////////////////// �Լ� (private) ////////////////////////////////////////////////////
private:
	BOOL			camOpenSeq(int dispNum); // ī�޶� open ������
	CString			GetExePath();			 // option.ini ��� ���� �������� ��ġ ���
	BOOL			GetOptionValue(int mode);// option�� ȹ�� (����)
	BOOL			GetOptionValue(int mode, int dispNum); // option�� ȹ�� (play / open�� �ʿ��� ����)
	BOOL			LightSend(int dispNum, BOOL OnOff); // ���� ��� ����
	LRESULT			OnMyMsg(WPARAM length, LPARAM lpara); // RS232 ��� ���ű�
	CPoint			m_CurSor; // �� Ŀ����ġ
/////////////////////////////////////////////////////////////////////////////////////////////////////////


public:
//////////////////////////////////// ī�޶� ���� ���� ////////////////////////////////////////////////////
	bool			m_IsSystemInit;		// Camera SDK Init ���� �����Դϴ�.
	BOOL			m_IsOpen[MAXCAM];	// ī�޶� Open ����
	BOOL			m_IsPlay[MAXCAM];	// Play ���� (Grab & Show)
	CCrevisCtrl*	m_pCamCtrl[MAXCAM]; // ī�޶�� Class ��ü �������Դϴ�. 
	HANDLE			m_hPlayThread[MAXCAM];		// Continuous mode�� thread
	HANDLE			m_hPlayTerminate[MAXCAM];	// Continuous mode stop�� event
	HANDLE			m_hOpenThread[MAXCAM];		// open �۾��� thread
	DOUBLE			m_CamExposure[MAXCAM]; // Exposure time�Դϴ�.
	CString			m_CamIP[MAXCAM];	// IP �ּ� String�Դϴ�.
	INT32			m_CamTrig[MAXCAM]; // ī�޶��� Trigger mode �Դϴ�.
	CString			m_CamLightCh[MAXCAM]; // ī�޶� �� ����� Light Channel number�Դϴ�.
///////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////�̹��� ��� ���� ����/////////////////////////////////////////////////////
	CMyImageView*	m_pImageView[MAXCAM];// �̹��� ��� ��ü
	INT32			m_statusCode;		// ���α׷� ���� �ڵ带 �����ϴ� �����Դϴ�.
	CString			m_strErr;			// ����� ���� String�Դϴ�
	CString			m_optionPath;		// option.ini ������ path (�ʱ� ����� �� ���� �޾ƿ�)
	CString			m_exePath;
	CClientDC*		m_pDC[MAXCAM];
	CRect			m_rcDisp[MAXCAM];
	HDC				m_hDC[MAXCAM];
///////////////////////////////////���� ��Ʈ�ѷ� ���� ����/////////////////////////////////////
	CLightCtrl*		m_pLightCtrl;		 // Serial port Control�� ��ü
	BOOL			m_IsSerialOpen;		// Seiral Port Open����
	BOOL			m_optionmodal;
	CString			m_ComPort;			// ���� Controller���� ����� port number�Դϴ�.
	CString			m_BaudRate;			// ���� Controller���� ����� baud rate�Դϴ�.
	CString			m_OptionBright[LIGHTCH];
////////////////////////////////////////////////////////////////////////////////////////////////

	

///////////////////////////////////���� �˰��� ���� ����/////////////////////////////////////
	BOOL			m_IsAlgoMod[MAXCAM];		// �˰��� ��� OnOff
	CHVisionLibWrapper* m_pHVSWrapper[MAXCAM];			// HVS Wrapper
//////////////////////////////////////////////////////////////////////////////////////////

	CRITICAL_SECTION mSc; // ������ ���� Critical section

	BOOL			DrawImageContinuous(int dispNum);	// continuous mode�� ������
	BOOL			GrabImageSWTrigger(int dispNum);	// SW Trigger �̿� �̹��� Grab
	BOOL			DrawSingleImage(int dispNum);		// single frame draw
	BOOL			DrawProcessed(int dispNum);			// �̹��� ��� ��� ����� Image draw
	void			thread1proc();						// Screen1ThreadProc���� ����Ǵ� ������ ���μ��� (1�� Screen grab -> draw)
	void			thread2proc();						// Screen2ThreadProc���� ����Ǵ� ������ ���μ��� (2�� Screen grab -> draw)
	void			Cam1OpenProc();						// 1�� cam open�� ������ ���μ���
	void			Cam2OpenProc();						// 2�� cam open�� ������ ���μ���
	
	// Context menu Ŭ���� ���۵�
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
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnBnClickedAlgomod();
	afx_msg void OnBnClickedAlgomod2();
	afx_msg void OnBnClickedAlgoadd1();
	afx_msg void OnBnClickedAlgoedit1();
	afx_msg void OnBnClickedAlgodel1();
	CListBox m_AlgoList1;
	CListBox m_TacTimeList1;
	CListBox m_OutputList1;
	afx_msg void OnBnClickedAlgoadd2();
	afx_msg void OnBnClickedAlgoedit2();
	afx_msg void OnBnClickedAlgodel2();
	afx_msg void OnBnClickedRun2();
	afx_msg void OnBnClickedSavercd2();
	afx_msg void OnBnClickedClearrcd2();
	afx_msg void OnBnClickedLoadrcd1();
	afx_msg void OnBnClickedLoadrcd2();
	CListBox m_AlgoLlist2;
	CListBox m_OutputList2;
	CListBox m_TacTimeList2;
	
};
