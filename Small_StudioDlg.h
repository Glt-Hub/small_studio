
// Small_StudioDlg.h : 헤더 파일
//

#pragma once
#include "CrevisCtrl.h"
#include "LightCtrl.h"
#include "Small_StudioDlg.h"
#include "Small_Studio.h"
#include "OptionDlg.h"
#include "MyImageView.h"


// CSmall_StudioDlg 대화 상자
class CSmall_StudioDlg : public CDialogEx
{
// 생성입니다.
public:
	CSmall_StudioDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SMALL_STUDIO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
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
	
//	BOOL			DIBMake(int dispNum);
//	BOOL			hbitmap2CImage(int dispNum);
	BOOL			LightCtrl(int dispNum);
	BOOL			LightSend(int dispNum, BOOL OnOff);
	BOOL			RawToGDIPBmp(int dispNum, int width, int height, BYTE* buffer);
	LRESULT			OnMyMsg(WPARAM length, LPARAM lpara);
	BOOL			m_IsOverlay[MAXCAM];
	CPoint			m_CurSor;

public:
	bool			m_IsSystemInit;		// Camera SDK Init 상태 점검입니다.
	BOOL			m_IsOpen[MAXCAM];	// 카메라 Open 여부
	
	
	BOOL			m_IsPlay[MAXCAM];	// Play 여부 (Grab & Show)
	INT32			m_vidwidth[MAXCAM];
	INT32			m_vidheight[MAXCAM];
	BOOL			m_IsSerialOpen;		// Seiral Port Open여부
	CCrevisCtrl*	m_pCamCtrl[MAXCAM]; // 카메라용 Class 객체 포인터입니다. 
	CMyImageView*	m_pImageView[MAXCAM];
	INT32			m_statusCode;		// 프로그램 에러 코드를 저장하는 변수입니다.
	CString			m_strErr;			// 출력할 에러 String입니다

	CString			m_optionPath;		// option.ini 파일의 path (초기 실행시 한 번만 받아옴)
	
	BYTE*			m_pBit[MAXCAM];		// 이미지버퍼
	HBITMAP			m_hBmp[MAXCAM];
//	CImage*			m_pCOriImage[MAXCAM]; // 이미지 객체
	CLightCtrl*		m_pLightCtrl;		 // Serial port Control용 객체

	HANDLE			m_hPlayThread[MAXCAM];		// Continuous mode용 thread
	HANDLE			m_hPlayTerminate[MAXCAM];

	HANDLE			m_hOpenThread[MAXCAM];
	// GDI+
	CRect			m_rcDisp[MAXCAM];
	HDC				m_hDC[MAXCAM];
	Graphics*		m_pGraphics[MAXCAM];
	Bitmap*			m_pBitmap[MAXCAM];
	////////////////////////////////////////////////////////////////////////////////////////////////

	BOOL			m_optionmodal;
	//option.ini 파일에서 read 할 변수 20201103 장한결
	CString			m_ComPort;			// 조명 Controller에서 사용할 port number입니다.
	CString			m_BaudRate;			// 조명 Controller에서 사용할 baud rate입니다.
	CString			m_OptionBright[LIGHTCH];
	CString			m_CamIP[MAXCAM];	// IP 주소 String입니다.
	DOUBLE			m_CamExposure[MAXCAM]; // Exposure time입니다.
	INT32			m_CamTrig[MAXCAM]; // 카메라의 Trigger mode 입니다.
	CString			m_CamLightCh[MAXCAM]; // 카메라 별 사용할 Light Channel number입니다.
	////////////////////////////////////////////////////////////////////////////////////////////////
	CRITICAL_SECTION mSc;
	BOOL			DrawImageContinuous(int dispNum);
	BOOL			GrabImageSWTrigger(int dispNum);
	BOOL			DrawSingleImage(int dispNum);
	void			thread1proc(); // Screen1ThreadProc에서 실행되는 스레드 프로세스 (1번 Screen grab -> draw)
	void			thread2proc(); // Screen2ThreadProc에서 실행되는 스레드 프로세스 (2번 Screen grab -> draw)
	void			Cam1OpenProc();
	void			Cam2OpenProc();
	
	afx_msg void OnBnClickedDebugdragon();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnStnClickedPic1();
	afx_msg void OnStnClickedPic2();
};
