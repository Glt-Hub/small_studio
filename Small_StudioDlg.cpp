
// Small_StudioDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "Small_Studio.h"
#include "Small_StudioDlg.h"
#include "afxdialogex.h"

// GDI+ ����� ���� ���� 20201106 ���Ѱ�
/*
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
*/
// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

// 1�� Screen (����)���� Play �Ǵ� �̹����� ������ ���� 20201110 ���Ѱ�
void Screen1ThreadProc(CSmall_StudioDlg* pPrivate)
{
	pPrivate->thread1proc();
}
void Screen2ThreadProc(CSmall_StudioDlg* pPrivate)
{
	pPrivate->thread2proc();
}


void Cam1OpenThreadProc(CSmall_StudioDlg* pPrivate)
{
	pPrivate->Cam1OpenProc();
}

void Cam2OpenThreadProc(CSmall_StudioDlg* pPrivate)
{
	pPrivate->Cam2OpenProc();
}

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSmall_StudioDlg ��ȭ ����



CSmall_StudioDlg::CSmall_StudioDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSmall_StudioDlg::IDD, pParent)
	, m_IsSystemInit(false)
	, m_statusCode(-99999)
	, m_strErr(_T(""))
	, m_optionPath(_T(""))
	, m_pLightCtrl(NULL)
	, m_BaudRate(_T(""))
	, m_ComPort(_T(""))
	, m_IsSerialOpen(FALSE)
	, m_optionmodal(FALSE)
	, m_CurSor(NULL)
	, m_exePath(_T(""))

{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	for (int i = 0; i < MAXCAM; i++)
	{
		CString eventName;
		eventName.Format(_T("TERMINATE_%d"), i);
		m_pCamCtrl[i] = NULL;
		m_CamIP[i] = _T("");
		m_CamExposure[i] = 0.0;
		m_IsOpen[i] = FALSE;
		m_IsPlay[i] = FALSE;
		m_hPlayThread[i] = NULL;
		m_hPlayTerminate[i] = CreateEvent(NULL, true, false, eventName);
		m_hOpenThread[i] = NULL;
		m_pImageView[i] = NULL;
		m_IsAlgoMod[i] = FALSE;
		m_pHVSWrapper[i] = NULL;
	}

	for (int i = 0; i < LIGHTCH; i++)
	{
		m_OptionBright[i] = _T("");
	}
}

void CSmall_StudioDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ALGOLIST1, m_AlgoList1);
	DDX_Control(pDX, IDC_TACTIMELIST1, m_TacTimeList1);
	DDX_Control(pDX, IDC_ALGOLIST2, m_AlgoLlist2);
	DDX_Control(pDX, IDC_OUTPUTLIST2, m_OutputList2);
	DDX_Control(pDX, IDC_TACTIMELIST2, m_TacTimeList2);
	DDX_Control(pDX, IDC_OUTPUTLIST1, m_OutputList1);
}

BEGIN_MESSAGE_MAP(CSmall_StudioDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CAM1OPEN, &CSmall_StudioDlg::OnBnClickedCam1open)
	ON_BN_CLICKED(IDC_CAM2OPEN, &CSmall_StudioDlg::OnBnClickedCam2open)
	ON_BN_CLICKED(IDC_OPTIONBTN, &CSmall_StudioDlg::OnBnClickedOptionbtn)
	ON_BN_CLICKED(IDC_LIGHTOPEN, &CSmall_StudioDlg::OnBnClickedLightopen)
	ON_BN_CLICKED(IDC_CAM1PLAY, &CSmall_StudioDlg::OnBnClickedCam1play)
	ON_BN_CLICKED(IDC_CAM2PLAY, &CSmall_StudioDlg::OnBnClickedCam2play)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_MYRECEIVE, OnMyMsg)
	ON_BN_CLICKED(IDC_DEBUGDRAGON, &CSmall_StudioDlg::OnBnClickedDebugdragon)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDB_CONTEXTOVRLY1, &CSmall_StudioDlg::OnCtxtClickedOvrly1)
	ON_COMMAND(IDB_CONTEXTOVRLY2, &CSmall_StudioDlg::OnCtxtClickedOvrly2)
	ON_COMMAND(IDB_CONTEXTLARG1, &CSmall_StudioDlg::OnCtxtClickedLarg1)
	ON_COMMAND(IDB_CONTEXTBIN1, &CSmall_StudioDlg::OnCtxtClickedBin1)
	ON_COMMAND(IDB_CONTEXTLARG2, &CSmall_StudioDlg::OnCtxtClickedLarg2)
	ON_COMMAND(IDB_CONTEXTBIN2, &CSmall_StudioDlg::OnCtxtClickedBin2)
	ON_BN_CLICKED(IDC_ALGOMOD, &CSmall_StudioDlg::OnBnClickedAlgomod)
	ON_BN_CLICKED(IDC_ALGOMOD2, &CSmall_StudioDlg::OnBnClickedAlgomod2)
	ON_BN_CLICKED(IDC_ALGOADD1, &CSmall_StudioDlg::OnBnClickedAlgoadd1)
	ON_BN_CLICKED(IDC_ALGOEDIT1, &CSmall_StudioDlg::OnBnClickedAlgoedit1)
	ON_BN_CLICKED(IDC_ALGODEL1, &CSmall_StudioDlg::OnBnClickedAlgodel1)
	ON_BN_CLICKED(IDC_ALGOADD2, &CSmall_StudioDlg::OnBnClickedAlgoadd2)
	ON_BN_CLICKED(IDC_ALGOEDIT2, &CSmall_StudioDlg::OnBnClickedAlgoedit2)
	ON_BN_CLICKED(IDC_ALGODEL2, &CSmall_StudioDlg::OnBnClickedAlgodel2)
	ON_BN_CLICKED(IDC_RUN2, &CSmall_StudioDlg::OnBnClickedRun2)
	ON_BN_CLICKED(IDC_SAVERCD2, &CSmall_StudioDlg::OnBnClickedSavercd2)
	ON_BN_CLICKED(IDC_CLEARRCD2, &CSmall_StudioDlg::OnBnClickedClearrcd2)
	ON_BN_CLICKED(IDC_LOADRCD1, &CSmall_StudioDlg::OnBnClickedLoadrcd1)
	ON_BN_CLICKED(IDC_LOADRCD2, &CSmall_StudioDlg::OnBnClickedLoadrcd2)
	ON_BN_CLICKED(IDC_CLEARRCD1, &CSmall_StudioDlg::OnBnClickedClearrcd1)
	ON_BN_CLICKED(IDC_SAVERCD1, &CSmall_StudioDlg::OnBnClickedSavercd1)
	ON_BN_CLICKED(IDC_RECIPEUP2, &CSmall_StudioDlg::OnBnClickedRecipeup2)
	ON_BN_CLICKED(IDC_RECIPEDOWN2, &CSmall_StudioDlg::OnBnClickedRecipedown2)
	ON_BN_CLICKED(IDC_RECIPEUP1, &CSmall_StudioDlg::OnBnClickedRecipeup1)
	ON_BN_CLICKED(IDC_RECIPEDOWN1, &CSmall_StudioDlg::OnBnClickedRecipedown1)
END_MESSAGE_MAP()


// CSmall_StudioDlg �޽��� ó����

BOOL CSmall_StudioDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	::InitializeCriticalSection(&mSc);

	for (int i = 0; i < MAXCAM; i++)
	{
		ResetEvent(m_hPlayTerminate[i]);
		
		if (i == 0)
		{
			m_pDC[i] = new CClientDC(GetDlgItem(IDC_PIC1)); 
			m_hDC[i] = m_pDC[i]->GetSafeHdc();
			GetDlgItem(IDC_PIC1)->GetWindowRect(m_rcDisp[i]);
			ScreenToClient(&m_rcDisp[i]);
			m_pImageView[i] = new CMyImageView();
			m_pHVSWrapper[i] = new CHVisionLibWrapper();
		}
		if (i == 1)
		{
			m_pDC[i] = new CClientDC(GetDlgItem(IDC_PIC2));
			m_hDC[i] = m_pDC[i]->GetSafeHdc();
			GetDlgItem(IDC_PIC2)->GetWindowRect(m_rcDisp[i]);
			ScreenToClient(&m_rcDisp[i]);
			m_pImageView[i] = new CMyImageView();
			m_pHVSWrapper[i] = new CHVisionLibWrapper();
		}
	}

	m_optionPath.Format(_T("%s\\option.ini"), GetExePath());
	m_exePath = GetExePath();
	GetOptionValue(OPT_READ_ALL);

	if (ST_InitSystem() != MCAM_ERR_SUCCESS)
	{
		AfxMessageBox(_T("���� ! Crevis Camera ���̺귯���� �ʱ�ȭ �� �� �����ϴ�."));
		m_IsSystemInit = false;
	}
	else
	{
		m_IsSystemInit = true;
	}

	// 1�� Display�� �˰��� ȭ�� (Disable)
	GetDlgItem(IDC_ALGOADD1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ALGODEL1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ALGOEDIT1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CLEARRCD1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_SAVERCD1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RUN1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_LOADRCD1)->ShowWindow(SW_HIDE);


	GetDlgItem(IDC_ALGOLIST1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_OUTPUTLIST1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TACTIMELIST1)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_RECIPESTATIC1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_OUTPUTSTATIC1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TACTIMESTATIC1)->ShowWindow(SW_HIDE);

	// 2�� Display�� �˰��� ȭ�� (Disable)
	GetDlgItem(IDC_ALGOADD2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ALGODEL2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ALGOEDIT2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CLEARRCD2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_SAVERCD2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RUN2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_LOADRCD2)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_ALGOLIST2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_OUTPUTLIST2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TACTIMELIST2)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_RECIPESTATIC2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_OUTPUTSTATIC2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TACTIMESTATIC2)->ShowWindow(SW_HIDE);

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.
	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.
	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CSmall_StudioDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CSmall_StudioDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		for (int i = 0; i < MAXCAM; i++)
		{
			if (m_IsPlay[i] && m_CamTrig[i] == CAMERA_TRIG_CONTINUOUS)
			{
				DrawImageContinuous(i);
			}
			else if (m_IsPlay[i] && m_CamTrig[i] == CAMERA_TRIG_SW)
			{
				if (m_pCamCtrl[i] != NULL)
				{
					DrawSingleImage(i);
				}
			}
			else
			{
				if (m_pCamCtrl[i] != NULL && m_pImageView[i]->m_IsOverlay)
				{
					DrawProcessed(i);
				}
				else if (m_pCamCtrl[i] != NULL && !m_pImageView[i]->m_IsOverlay)
				{
					DrawSingleImage(i);
				}
			}
		}
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CSmall_StudioDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CSmall_StudioDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	if (m_pLightCtrl != NULL)
	{
		m_pLightCtrl->Close();
		if (!m_pLightCtrl->m_bIsOpenned)
		{
			delete m_pLightCtrl;
			m_pLightCtrl = NULL;
		}
	}

	for (int i = 0; i < MAXCAM; i++)
	{
		if (m_pImageView[i] != NULL)
		{
			delete m_pImageView[i];
			m_pImageView[i] = NULL;
		}

		if (m_pHVSWrapper[i] != NULL)
		{
			delete m_pHVSWrapper[i];
			m_pHVSWrapper[i] = NULL;
		}

		if (m_IsOpen[i])
		{
			if (m_hPlayThread[i] != NULL)
			{
				// Thread Suspend
				SetEvent(m_hPlayTerminate[i]);
				WaitForSingleObject(m_hPlayThread[i], INFINITE);
				CloseHandle(m_hPlayThread[i]);
				m_hPlayThread[i] = NULL;
			}
			delete m_pCamCtrl[i];
		}

		if (m_pDC[i] != NULL)
		{
			delete m_pDC[i];
			m_pDC[i] = NULL;
		}
	}
	if (m_IsSystemInit)
	{
		ST_FreeSystem();
	}
	::DeleteCriticalSection(&mSc);
}

// ī�޶� Open �� ���� ������ 20201118 ���Ѱ�
BOOL CSmall_StudioDlg::camOpenSeq(int dispNum)
{
	if (!m_IsSystemInit)
	{
		AfxMessageBox(_T("System Init�� ���� �ʾҽ��ϴ�. ī�޶� ����� �� �����ϴ�."));
		return FALSE;
	}
	
	m_pCamCtrl[dispNum] = new CCrevisCtrl(m_CamIP[dispNum]);
	m_statusCode = m_pCamCtrl[dispNum]->OpenDevice();
	m_strErr.Format(_T("!!!!!!!!!!!!!!!!!!!!! DEBUG %d Cam status : %d !!!!!!!!!!!!!!!!!\n"), dispNum, m_statusCode);
	OutputDebugString(m_strErr);
	m_pCamCtrl[dispNum]->SetSWTrigger();

	if (m_statusCode != CAMERA_OPEN_SUCCESS)
	{
		delete m_pCamCtrl[dispNum];
		m_CamIP[dispNum] = _T("");
		m_IsOpen[dispNum] = FALSE;
		return FALSE;
	}

	if (!m_pCamCtrl[dispNum]->SetDeviceExposure(m_CamExposure[dispNum]))
	{		
		m_IsOpen[dispNum] = FALSE;
		return FALSE;
	}
	m_IsOpen[dispNum] = TRUE;
	return TRUE;
}

// Cam 1,2 �� Open. option.ini ������ ������ �о open�մϴ�. 20201030 ���Ѱ�
void CSmall_StudioDlg::OnBnClickedCam1open()
{
	int dispNum = 0;
	if (m_hOpenThread[dispNum] != NULL)
	{
		CloseHandle(m_hOpenThread[dispNum]);
		m_hOpenThread[dispNum] = NULL;
	}
	m_hOpenThread[dispNum] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Cam1OpenThreadProc, this, 0, NULL);
}


void CSmall_StudioDlg::OnBnClickedCam2open()
{
	int dispNum = 1;
	if (m_hOpenThread[dispNum] != NULL)
	{
		CloseHandle(m_hOpenThread[dispNum]);
		m_hOpenThread[dispNum] = NULL;
	}
	m_hOpenThread[dispNum] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Cam2OpenThreadProc, this, 0, NULL);
}


void CSmall_StudioDlg::OnBnClickedOptionbtn()
{
	COptionDlg optiondlg;
	GetOptionValue(OPT_READ_ALL);
	
	for (int i = 0; i < MAXCAM; i++)
	{
		optiondlg.m_CamIP[i] = m_CamIP[i];
		optiondlg.m_CamExposure[i].Format(_T("%lf"), m_CamExposure[i]);
		optiondlg.m_CamTrig[i].Format(_T("%d"), m_CamTrig[i]);
		optiondlg.m_CamLightCh[i] = m_CamLightCh[i];
	}

	for (int i = 0; i < LIGHTCH; i++)
	{
		optiondlg.m_OptionBright[i] = m_OptionBright[i];
	}

	optiondlg.m_BaudRate = m_BaudRate;
	optiondlg.m_ComPort = m_ComPort;
	optiondlg.m_optionPath = m_optionPath;

	if (m_pLightCtrl != NULL)
	{
		optiondlg.m_pLightCtrl = m_pLightCtrl;
	}
	else
	{
		optiondlg.m_pLightCtrl = NULL;
	}
	m_optionmodal = TRUE;

	if (IDOK == optiondlg.DoModal())
	{
		// �ɼ� setting �Ϸ��. ini�� ����� �� ���α׷����� �ݿ�. 20201103 ���Ѱ�
		m_optionmodal = FALSE;
		// ī�޶� �ɼ� �ݿ� 20201105 ���Ѱ�
		for (int i = 0; i < MAXCAM; i++)
		{
			// ��Ÿ ������ Replay�� Ȥ�� ReOpen�� �ݿ� 20201103 ���Ѱ�
			m_CamIP[i] = optiondlg.m_CamIP[i];
			m_CamExposure[i] = _ttof(optiondlg.m_CamExposure[i]);
			m_CamTrig[i] = _ttoi(optiondlg.m_CamTrig[i]);
			m_CamLightCh[i] = optiondlg.m_CamLightCh[i];

			if (m_IsPlay[i])
			{
				LightSend(i, TRUE);
			}

			if (m_pCamCtrl[i] != NULL)
			{
				// Exposure time�� ��� �ٷ� �ݿ��˴ϴ�. 20201103 ���Ѱ�
				if (m_IsOpen[i])
				{
					m_pCamCtrl[i]->SetDeviceExposure(m_CamExposure[i]);
				}
			}
		}

		// ���� �ɼ� �ݿ� 20201105 ���Ѱ�
		m_BaudRate = optiondlg.m_BaudRate;
		m_ComPort = optiondlg.m_ComPort;
		for (int i = 0; i < LIGHTCH; i++)
		{
			m_OptionBright[i] = optiondlg.m_OptionBright[i];
		}
	}
	else
	{
		m_optionmodal = FALSE;
	}
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

// Light Open ��ư ���� 20201104 ���Ѱ�
void CSmall_StudioDlg::OnBnClickedLightopen()
{
	if (!m_IsSerialOpen)
	{
		m_pLightCtrl = new CLightCtrl(m_ComPort, m_BaudRate, _T("None"), _T("8 Bit"), _T("1 Bit"));

		if (m_pLightCtrl->Create(GetSafeHwnd()) != 0)
		{
			m_pLightCtrl->Clear();
		}
		else
		{
			AfxMessageBox(_T("���� Controller ���� ����!"));
			m_IsSerialOpen = FALSE;

			GetDlgItem(IDC_LIGHTOPEN)->SetWindowTextW(_T("Light Closed"));
			delete m_pLightCtrl;
			m_pLightCtrl = NULL;
			return;
		}
		m_IsSerialOpen = TRUE;
		GetDlgItem(IDC_LIGHTOPEN)->SetWindowTextW(_T("Light Open"));
	}
	else
	{
		m_pLightCtrl->Close();
		if (!m_pLightCtrl->m_bIsOpenned)
		{
			delete m_pLightCtrl;
			m_pLightCtrl = NULL;
		}
		GetDlgItem(IDC_LIGHTOPEN)->SetWindowTextW(_T("Light Closed"));
		m_IsSerialOpen = FALSE;
	}
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CSmall_StudioDlg::OnBnClickedCam1play()
{
	int dispNum = 0;
	GetOptionValue(OPT_READ_PLAY, dispNum);
	if (m_IsPlay[dispNum])
	{
		GetDlgItem(IDC_CAM1PLAY)->SetWindowTextW(_T("Play"));
		
		if (m_hPlayThread[dispNum] != NULL)
		{
			// Thread Suspend
			SetEvent(m_hPlayTerminate[dispNum]);
			WaitForSingleObject(m_hPlayThread[dispNum], INFINITE);
			CloseHandle(m_hPlayThread[dispNum]);
			m_hPlayThread[dispNum] = NULL;

		}
		LightSend(dispNum, FALSE);
		m_IsPlay[dispNum] = FALSE;
	}

	else
	{
		if (m_hPlayThread[dispNum] != NULL)
		{
			// Thread Suspend
			SetEvent(m_hPlayTerminate[dispNum]);
			WaitForSingleObject(m_hPlayThread[dispNum], INFINITE);
			CloseHandle(m_hPlayThread[dispNum]);
			m_hPlayThread[dispNum] = NULL;
		}
		m_IsPlay[dispNum] = TRUE;
		GetDlgItem(IDC_CAM1PLAY)->SetWindowTextW(_T("Stop"));
		ResetEvent(m_hPlayTerminate[dispNum]);
		m_hPlayThread[dispNum] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Screen1ThreadProc, this, 0, NULL);
		// ������ ����
	} // SW Trigger �� ���� ����� �����
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CSmall_StudioDlg::OnBnClickedCam2play()
{
	int dispNum = 1;
	GetOptionValue(OPT_READ_PLAY, dispNum);
	if (m_IsPlay[dispNum] && m_CamTrig[dispNum] == CAMERA_TRIG_CONTINUOUS)
	{
		GetDlgItem(IDC_CAM2PLAY)->SetWindowTextW(_T("Play"));
		if (m_hPlayThread[dispNum] != NULL)
		{
			// Thread Suspend
			SetEvent(m_hPlayTerminate[dispNum]);
			WaitForSingleObject(m_hPlayThread[dispNum], INFINITE);
			CloseHandle(m_hPlayThread[dispNum]);
			m_hPlayThread[dispNum] = NULL;
		}
		LightSend(dispNum, FALSE);
		m_IsPlay[dispNum] = FALSE;
	}

	else
	{
		if (m_hPlayThread[dispNum] != NULL)
		{
			// Thread Suspend
			SetEvent(m_hPlayTerminate[dispNum]);
			WaitForSingleObject(m_hPlayThread[dispNum], INFINITE);
			CloseHandle(m_hPlayThread[dispNum]);
			m_hPlayThread[dispNum] = NULL;
		}
		m_IsPlay[dispNum] = TRUE;
		GetDlgItem(IDC_CAM2PLAY)->SetWindowTextW(_T("Stop"));
		ResetEvent(m_hPlayTerminate[dispNum]);
		m_hPlayThread[dispNum] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Screen2ThreadProc, this, 0, NULL);
		// ������ ����
	} // SW Trigger �� ���� ����� �����
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

// Grab -> Mat -> ȭ�鿡 Draw�ϴ� ����
// 20201105 ���Ѱ�
BOOL CSmall_StudioDlg::DrawImageContinuous(int dispNum)
{
	// continuous mode 20201112 ���Ѱ�
	::EnterCriticalSection(&mSc);
	if (!m_pCamCtrl[dispNum]->GrabImageContinuous())
	{
		::LeaveCriticalSection(&mSc);
		return FALSE;
	}
	m_pImageView[dispNum]->ImageViewerReset();
	m_pImageView[dispNum]->pByteToMat(m_pCamCtrl[dispNum]->m_pImage, m_pCamCtrl[dispNum]->m_camWidth, m_pCamCtrl[dispNum]->m_camHeight);
	m_pImageView[dispNum]->createBitmapInfo(m_pImageView[dispNum]->m_OriMat);
	SetStretchBltMode(m_pDC[dispNum]->GetSafeHdc(), COLORONCOLOR);
	StretchDIBits(m_pDC[dispNum]->GetSafeHdc(), 0, 0, m_rcDisp[dispNum].Width(), m_rcDisp[dispNum].Height(), 0, 0,
		m_pImageView[dispNum]->m_OriMat.cols, m_pImageView[dispNum]->m_OriMat.rows, m_pImageView[dispNum]->m_OriMat.data,
		m_pImageView[dispNum]->m_pBitmapInfo, DIB_RGB_COLORS, SRCCOPY);
	::LeaveCriticalSection(&mSc);
	return TRUE;
}
// SW Trigger mode (single frame grab)
// 20201112 ���Ѱ�
BOOL CSmall_StudioDlg::GrabImageSWTrigger(int dispNum)
{
	// Software Trigger
	::EnterCriticalSection(&mSc);
	if (dispNum == 0)
	{
		GetDlgItem(IDC_CAM1PLAY)->EnableWindow(FALSE);
	}
	else if (dispNum == 1)
	{
		GetDlgItem(IDC_CAM2PLAY)->EnableWindow(FALSE);
	}
	// ���� On
	LightSend(dispNum, TRUE);
	// Image Grab
	if (!m_pCamCtrl[dispNum]->GrabImageSW())
	{
		::LeaveCriticalSection(&mSc);
		return FALSE;
	}
	Sleep(5);
	// ���� Off
	LightSend(dispNum, FALSE);
	m_pImageView[dispNum]->pByteToMat(m_pCamCtrl[dispNum]->m_pImage, m_pCamCtrl[dispNum]->m_camWidth, m_pCamCtrl[dispNum]->m_camHeight);
	::LeaveCriticalSection(&mSc);
	return TRUE;
}

// �̹��� ���� draw 20201118 ���Ѱ�
BOOL CSmall_StudioDlg::DrawSingleImage(int dispNum)
{
	::EnterCriticalSection(&mSc);
	m_pImageView[dispNum]->ImageViewerReset();
	m_pImageView[dispNum]->createBitmapInfo(m_pImageView[dispNum]->m_OriMat);
	SetStretchBltMode(m_pDC[dispNum]->GetSafeHdc(), COLORONCOLOR);
	StretchDIBits(m_pDC[dispNum]->GetSafeHdc(), 0, 0, m_rcDisp[dispNum].Width(), m_rcDisp[dispNum].Height(), 0, 0,
		m_pImageView[dispNum]->m_OriMat.cols, m_pImageView[dispNum]->m_OriMat.rows, m_pImageView[dispNum]->m_OriMat.data,
		m_pImageView[dispNum]->m_pBitmapInfo, DIB_RGB_COLORS, SRCCOPY);
	m_IsPlay[dispNum] = FALSE;
	::LeaveCriticalSection(&mSc);
	return TRUE;
}

// �̹��� ��� ��� ���� �̹��� draw 20201118 ���Ѱ�
BOOL CSmall_StudioDlg::DrawProcessed(int dispNum)
{
	int imgCol;
	int imgRow;
	CPoint imgTopLeft;
	cv::Point GrayLvdrawPoint;

	// Ŀ�� gray scale level�� ǥ�� �� (�� �� �� �� ��)
	if (!m_pImageView[dispNum]->m_Islarger)
	{
		imgTopLeft = CPoint(0, 0);
		imgCol = m_pImageView[dispNum]->m_DrawMat.cols;
		imgRow = m_pImageView[dispNum]->m_DrawMat.rows;
		GrayLvdrawPoint = cv::Point(100, 100);
	}

	// Ȯ��� (x 2 . 0)
	if (m_pImageView[dispNum]->m_Islarger)
	{
		m_pImageView[dispNum]->m_DrawMat = m_pImageView[dispNum]->m_OriMat.clone(); // ���� ���׷� ���� Ȯ��ÿ��� �����̹��� ����
		m_pImageView[dispNum]->largerScreen(m_pImageView[dispNum]->m_RealLargeTopLeft);
		imgTopLeft = CPoint(m_pImageView[dispNum]->m_largerTopLeft.x, m_pImageView[dispNum]->m_largerTopLeft.y);
		imgCol = m_pImageView[dispNum]->m_DrawMat.cols / 2;
		imgRow = m_pImageView[dispNum]->m_DrawMat.rows / 2;
		GrayLvdrawPoint = cv::Point(m_pImageView[dispNum]->m_RealLargeTopLeft.x + 50, m_pImageView[dispNum]->m_RealLargeTopLeft.y + 50);
	}

	// ���� Issue�� ���� ��Ȯ��ø� cursor RGB, Ȯ������
	if (!m_pImageView[dispNum]->m_Islarger)
	{
		// Ŀ���κ� Gray lv ǥ��
		m_pImageView[dispNum]->cvCursorRGB(m_CurSor, GrayLvdrawPoint, m_rcDisp[dispNum].TopLeft(), m_rcDisp[dispNum].BottomRight());
		// Ŀ���κ� Ȯ�� ������
		if (m_pImageView[dispNum]->m_IsCursorLarger)
		{
			m_pImageView[dispNum]->cursorLarger(m_CurSor, m_rcDisp[dispNum].TopLeft(), m_rcDisp[dispNum].BottomRight());
		}
		m_pImageView[dispNum]->createBitmapInfo(m_pImageView[dispNum]->m_DrawMat);

	}
	SetStretchBltMode(m_pDC[dispNum]->GetSafeHdc(), COLORONCOLOR);
	StretchDIBits
		(
		m_pDC[dispNum]->GetSafeHdc(), // Hdc
		0, 0, // ���÷��� ���� �׸��� ������ ��ǥ
		m_rcDisp[dispNum].Width(), m_rcDisp[dispNum].Height(), // ���÷��� ��, ����
		imgTopLeft.x, imgTopLeft.y, // ���� ������ ��ǥ
		imgCol, imgRow, // ���� ��, ����
		m_pImageView[dispNum]->m_DrawMat.data, // �̹��� ������
		m_pImageView[dispNum]->m_pBitmapInfo, // Bitmap �׸� �� �ʿ��� info (�׻� ������) 
		DIB_RGB_COLORS, SRCCOPY // ��Ÿ����
		);
	
	return TRUE;
}


// Option�� ������ ���� ���� �������� ��� ��ȯ 20201030 ���Ѱ�
CString CSmall_StudioDlg::GetExePath()
{
	static TCHAR pBuf[256] = { 0, };
	memset(pBuf, NULL, sizeof(pBuf));
	GetModuleFileName(NULL, pBuf, sizeof(pBuf));
	CString strFilePath;
	strFilePath.Format(_T("%s"), pBuf);
	strFilePath = strFilePath.Left(strFilePath.ReverseFind(_T('\\')));
	return strFilePath;
}

// option ��ư ���� ���� APP ���� �� ����
// option.ini ���Ͽ��� ��� �� �о���� �Լ� 20201030 ���Ѱ�
BOOL CSmall_StudioDlg::GetOptionValue(int mode)
{
	if (mode == OPT_READ_ALL)
	{
		LPWSTR cBuf;
		cBuf = new WCHAR[256];
		for (int i = 0; i < MAXCAM; i++)
		{
			CString camnum;
			camnum.Format(_T("CAMERA%d"), i + 1);
			// IP�ּ� Read
			GetPrivateProfileStringW(camnum, _T("IP"), _T(""), cBuf, 256, m_optionPath);
			m_CamIP[i] = cBuf;

			// Camera Open�Ͻ� Device���� ���� Exposure time�� Trigger Mode�� ������ �ɴϴ�.
			if (m_IsOpen[i])
			{
				Sleep(50);
				m_CamExposure[i] = m_pCamCtrl[i]->GetDeviceExposure();
			}
			else
			{
				GetPrivateProfileStringW(camnum, _T("Exposure"), _T("50000"), cBuf, 256, m_optionPath);
				m_CamExposure[i] = _ttof(cBuf);
			}
			// Grab mode read
			CString str;
			str.Format(_T("%d"), CAMERA_TRIG_SW);
			GetPrivateProfileStringW(camnum, _T("GRAB_MODE"), str, cBuf, 256, m_optionPath);
			m_CamTrig[i] = _ttoi(cBuf);
			GetPrivateProfileStringW(camnum, _T("LIGHTCH"), _T("1"), cBuf, 256, m_optionPath);
			m_CamLightCh[i] = cBuf;
		}
		GetPrivateProfileStringW(_T("LLIGHT4CH"), _T("PORT"), _T("COM1"), cBuf, 256, m_optionPath);
		m_ComPort = cBuf;
		GetPrivateProfileStringW(_T("LLIGHT4CH"), _T("BAUD"), _T("300"), cBuf, 256, m_optionPath);
		m_BaudRate = cBuf;

		CString chnum;
		for (int i = 0; i < LIGHTCH; i++)
		{
			chnum.Format(_T("CH%d"), i+1);
			GetPrivateProfileStringW(_T("LLIGHT4CH"), chnum, _T("0"), cBuf, 256, m_optionPath);
			m_OptionBright[i] = cBuf;
		}

		delete cBuf;
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

// camera open ���۽� ���
// option.ini ���Ͽ��� ���÷��� ��ȣ�� ���� ���� ������ �Լ� 20201030 ���Ѱ�
// GetOptionValue() Override
BOOL CSmall_StudioDlg::GetOptionValue(int mode, int dispNum)
{
	LPWSTR cBuf;
	cBuf = new WCHAR[256];
	CString camnum;
	camnum.Format(_T("CAMERA%d"), dispNum + 1);
	CString chnum;


	CString str;
	str.Format(_T("%d"), CAMERA_TRIG_SW);
	// Cam Open�� Read
	if (mode == OPT_READ_CAM)
	{
		GetPrivateProfileStringW(camnum, _T("IP"), _T(""), cBuf, 256, m_optionPath);
		m_CamIP[dispNum] = cBuf;
		GetPrivateProfileStringW(camnum, _T("Exposure"), _T("50000"), cBuf, 256, m_optionPath);
		m_CamExposure[dispNum] = _ttof(cBuf);
		GetPrivateProfileStringW(camnum, _T("GRAB_MODE"), str, cBuf, 256, m_optionPath);
		m_CamTrig[dispNum] = _ttoi(cBuf);
	}
	// Play button�� read
	else if (mode == OPT_READ_PLAY)
	{
		GetPrivateProfileStringW(camnum, _T("Exposure"), _T("50000"), cBuf, 256, m_optionPath);
		m_CamExposure[dispNum] = _ttof(cBuf);
		GetPrivateProfileStringW(camnum, _T("GRAB_MODE"), str, cBuf, 256, m_optionPath);
		m_CamTrig[dispNum] = _ttoi(cBuf);
		GetPrivateProfileStringW(camnum, _T("LIGHTCH"), _T("1"), cBuf, 256, m_optionPath);
		m_CamLightCh[dispNum] = cBuf;
		GetPrivateProfileStringW(_T("LLIGHT4CH"), m_CamLightCh[dispNum], _T("0"), cBuf, 256, m_optionPath);
		m_OptionBright[dispNum] = cBuf;
	}
	// Light open�� read
	else if (mode == OPT_READ_LIGHT)
	{
		GetPrivateProfileStringW(_T("LLIGHT4CH"), _T("PORT"), _T(""), cBuf, 256, m_optionPath);
		m_ComPort = cBuf;
		GetPrivateProfileStringW(_T("LLIGHT4CH"), _T("BAUD"), _T(""), cBuf, 256, m_optionPath);
		m_BaudRate = cBuf;
	}
	else
	{
		return FALSE;
	}
	delete cBuf;
	return TRUE;
}

void CSmall_StudioDlg::OnBnClickedDebugdragon()
{
}

// dispNum���� �ش��ϴ� ī�޶� ��ü ���� ���� ��Ʈ�ѷ� ��� �۽ű�. Connect ���� ���� �����մϴ�.
// OnOff TRUE : Option���� ������ ���� ���� Channel ���� �������� �۽�
// OnOff FALSE : ��� 0���� ���� Channel ���� �������� �۽�. 20201105 ���Ѱ�
BOOL CSmall_StudioDlg::LightSend(int dispNum, BOOL OnOff)
{
	char stx = 0x02;
	char etx = 0x03;
	CString sendASCII;
	CString middle;
	INT32 chan = _ttoi(m_CamLightCh[dispNum]);
	if (m_pLightCtrl != NULL)
	{
		if (OnOff)
		{
			middle.Format(_T("CH%sS%s"), m_CamLightCh[dispNum], m_OptionBright[chan-1]);
			sendASCII = (CString)(stx + middle + etx);
			if (m_pLightCtrl->Send(sendASCII))
			{
				return TRUE;
			}
			return FALSE;
		}
		else
		{
			middle.Format(_T("CH%sS000"), m_CamLightCh[dispNum]);
			sendASCII = (CString)(stx + middle + etx);
			if (m_pLightCtrl->Send(sendASCII))
			{
				return TRUE;
			}
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
}


void CSmall_StudioDlg::Cam1OpenProc()
{
	int dispNum = 0;
	GetDlgItem(IDC_CAM1OPEN)->EnableWindow(FALSE);
	// Option.ini ������ �о� Open�� �ʿ��� ������ �н��ϴ�.
	GetOptionValue(OPT_READ_CAM, dispNum);
	if (!m_IsOpen[dispNum])
	{
		if (camOpenSeq(dispNum))
		{
			GetDlgItem(IDC_CAM1OPEN)->SetWindowTextW(_T("Camera 1 Open"));
			GetDlgItem(IDC_CAM1PLAY)->EnableWindow(TRUE);
		}
		else
		{
			m_strErr.Format(_T("ī�޶� Open ����!"));
			AfxMessageBox(m_strErr);
		}
	}
	else
	{
		if (m_hPlayThread[dispNum] != NULL)
		{
			// Thread Suspend
			SetEvent(m_hPlayTerminate[dispNum]);
			WaitForSingleObject(m_hPlayThread[dispNum], INFINITE);
			CloseHandle(m_hPlayThread[dispNum]);
			m_hPlayThread[dispNum] = NULL;
		}
		m_IsPlay[dispNum] = FALSE;
		delete m_pCamCtrl[dispNum];
		m_pCamCtrl[dispNum] = NULL;
		m_IsOpen[dispNum] = FALSE;
		LightSend(dispNum, FALSE);
		GetDlgItem(IDC_CAM1OPEN)->SetWindowTextW(_T("Camera 1 Closed"));
		GetDlgItem(IDC_CAM1PLAY)->SetWindowTextW(_T("Play"));
		GetDlgItem(IDC_CAM1PLAY)->EnableWindow(FALSE);
	}
	GetDlgItem(IDC_CAM1OPEN)->EnableWindow(TRUE);
}

void CSmall_StudioDlg::Cam2OpenProc()
{
	int dispNum = 1;
	GetDlgItem(IDC_CAM2OPEN)->EnableWindow(FALSE);
	GetOptionValue(OPT_READ_CAM, dispNum);
	if (!m_IsOpen[dispNum])
	{
		if (camOpenSeq(dispNum))
		{
			GetDlgItem(IDC_CAM2OPEN)->SetWindowTextW(_T("Camera 2 Open"));
			GetDlgItem(IDC_CAM2PLAY)->SetWindowTextW(_T("Play"));
			GetDlgItem(IDC_CAM2PLAY)->EnableWindow(TRUE);
		}
		else
		{
			m_strErr.Format(_T("ī�޶� Open ����!"));
			AfxMessageBox(m_strErr);
		}
	}
	else
	{
		if (m_hPlayThread[dispNum] != NULL)
		{
			// Thread Suspend
			SetEvent(m_hPlayTerminate[dispNum]);
			WaitForSingleObject(m_hPlayThread[dispNum], INFINITE);
			CloseHandle(m_hPlayThread[dispNum]);
			m_hPlayThread[dispNum] = NULL;
		}
		m_IsPlay[dispNum] = FALSE;
		delete m_pCamCtrl[dispNum];
		m_pCamCtrl[dispNum] = NULL;
		m_IsOpen[dispNum] = FALSE;
		LightSend(dispNum, FALSE);
		GetDlgItem(IDC_CAM2OPEN)->SetWindowTextW(_T("Camera 2 Closed"));
		GetDlgItem(IDC_CAM2PLAY)->EnableWindow(FALSE);
	}
	GetDlgItem(IDC_CAM2OPEN)->EnableWindow(TRUE);
}

// Screen1ThreadProc���� ����Ǵ� ������ ���μ��� 20201110 ���Ѱ�
void CSmall_StudioDlg::thread1proc()
{
	int dispNum = 0;
	if (m_CamTrig[dispNum] == CAMERA_TRIG_SW)
	{
		GrabImageSWTrigger(dispNum);
		InvalidateRect(m_rcDisp[dispNum], NULL);
		GetDlgItem(IDC_CAM1PLAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_CAM1PLAY)->SetWindowTextW(_T("Play"));
	}
	else if (m_CamTrig[dispNum] == CAMERA_TRIG_CONTINUOUS)
	{
		LightSend(dispNum, TRUE);
		while (WaitForSingleObject(m_hPlayTerminate[dispNum], 0) != WAIT_OBJECT_0)
		{
			Sleep(5);
			InvalidateRect(m_rcDisp[dispNum], NULL);
		}
	}
	else
	{
		return;
	}
}

// Screen2ThreadProc���� ����Ǵ� ������ ���μ��� 20201110 ���Ѱ�
void CSmall_StudioDlg::thread2proc()
{
	int dispNum = 1;
	if (m_CamTrig[dispNum] == CAMERA_TRIG_SW)
	{
		GrabImageSWTrigger(dispNum);
		InvalidateRect(m_rcDisp[dispNum], NULL);
		GetDlgItem(IDC_CAM2PLAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_CAM2PLAY)->SetWindowTextW(_T("Play"));
	}
	else if (m_CamTrig[dispNum] == CAMERA_TRIG_CONTINUOUS)
	{
		LightSend(dispNum, TRUE);
		while (WaitForSingleObject(m_hPlayTerminate[dispNum], 0) != WAIT_OBJECT_0)
		{
			Sleep(5);
			InvalidateRect(m_rcDisp[dispNum], NULL);
		}
	}
	else
	{
		return;
	}
}

LRESULT	CSmall_StudioDlg::OnMyMsg(WPARAM length, LPARAM lpara)
{
	CString str;
	char data[10000];
	if (m_pLightCtrl && m_optionmodal)
	{ 
		Sleep(50);
		m_pLightCtrl->Receive(data, (int)length);
		data[length] = '\0';
		str = data;
		AfxMessageBox(str);
		GetDlgItem(IDC_LTESTRCV)->SetWindowTextW(str);
		str = "";
	}
	return 0;
}
void CSmall_StudioDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CString debug;
	for (int i = 0; i < MAXCAM; i++)
	{
		if (m_rcDisp[i].PtInRect(point))
		{
			if (m_CamTrig[i] == CAMERA_TRIG_SW)
			{
				if (m_pImageView[i]->m_IsMove)
				{
					m_pImageView[i]->largerTopLeftMover(point);
				}

				m_CurSor.x = point.x;
				m_CurSor.y = point.y;
				InvalidateRect(m_rcDisp[i], NULL);
			}
		}
		else
		{
			m_pImageView[i]->m_IsMove = FALSE;
		}
	}
	CDialogEx::OnMouseMove(nFlags, point);
}

// ���� Ŭ���� �����ϴ� �κ� 20201118 ���Ѱ�
void CSmall_StudioDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	for (int i = 0; i < MAXCAM; i++)
	{
		// ���콺 ������ ��ġ�� ���÷��� ���ο� �������� ����
		if (m_rcDisp[i].PtInRect(point))
		{
			if (m_CamTrig[i] == CAMERA_TRIG_SW)
			{
				if (m_pImageView[i]->m_Islarger)
				{
					m_pImageView[i]->m_Islarger = FALSE;
					InvalidateRect(m_rcDisp[i], NULL);
				}
				else
				{
					m_pImageView[i]->m_Islarger = TRUE;
					InvalidateRect(m_rcDisp[i], NULL);
				}
			}
		}
	}
	CDialogEx::OnLButtonDblClk(nFlags, point);
}


void CSmall_StudioDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	
	CDialogEx::OnRButtonDown(nFlags, point);
}



void CSmall_StudioDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	for (int i = 0; i < MAXCAM; i++)
	{
		if (m_rcDisp[i].PtInRect(point))
		{
			if (m_pImageView[i]->m_Islarger)
			{
				m_pImageView[i]->m_prevMouse = point;
				m_pImageView[i]->m_IsMove = TRUE;
			}
		}
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CSmall_StudioDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	for (int i = 0; i < MAXCAM; i++)
	{
		m_pImageView[i]->m_prevMouse = CPoint(0,0);
		m_pImageView[i]->m_IsMove = FALSE;
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}

void CSmall_StudioDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CPoint cvtPoint = point;
	ScreenToClient(&cvtPoint);
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	for (int i = 0; i < MAXCAM; i++)
	{
		if (m_pImageView[i]->m_DrawMat.empty())
		{
			return;
		}

		if (m_rcDisp[i].PtInRect(cvtPoint))
		{
			CMenu menu;
			menu.CreatePopupMenu();
			CString options[3];

			if (m_pImageView[i]->m_IsOverlay)
			{
				options[0] = _T("�������� ǥ�� ON");
			}
			else
			{
				options[0] = _T("�������� ǥ�� OFF");
			}

			if (m_pImageView[i]->m_IsCursorLarger)
			{
				options[1] = _T("Ŀ���κ� Ȯ�� ON");
			}
			else
			{
				options[1] = _T("Ŀ���κ� Ȯ�� OFF");
			}

			if (m_pImageView[i]->m_IsCursorBin)
			{
				options[2] = _T("Ȯ��κ� ����ȭ ON");
			}
			else
			{
				options[2] = _T("Ȯ��κ� ����ȭ OFF");
			}

			// �޴��� �߰��մϴ�.
			if (i == 0)
			{
				menu.AppendMenu(MF_STRING, IDB_CONTEXTOVRLY1, options[0]);
				menu.AppendMenu(MF_STRING, IDB_CONTEXTLARG1, options[1]);
				menu.AppendMenu(MF_STRING, IDB_CONTEXTBIN1, options[2]);
			}
			else if (i == 1)
			{
				menu.AppendMenu(MF_STRING, IDB_CONTEXTOVRLY2, options[0]);
				menu.AppendMenu(MF_STRING, IDB_CONTEXTLARG2, options[1]);
				menu.AppendMenu(MF_STRING, IDB_CONTEXTBIN2, options[2]);
			}

			// ���ؽ�Ʈ �޴��� x,y ��ǥ�� ����մϴ�. 
			menu.TrackPopupMenu(TPM_LEFTALIGN,
				point.x,
				point.y,
				AfxGetMainWnd());
		}
	}
}

// Context menu ��ư Ŭ���� ���� 20201117 ���Ѱ�
// ���� �ش� �ɼ� TRUE / FALSE �����⸸ ����
void CSmall_StudioDlg::OnCtxtClickedOvrly1()
{
	int dispNum = 0;
	if (m_pImageView[dispNum]->m_IsOverlay)
	{
		m_pImageView[dispNum]->m_IsOverlay = FALSE;
	}
	else
	{
		m_pImageView[dispNum]->m_IsOverlay = TRUE;
	}
}
void CSmall_StudioDlg::OnCtxtClickedLarg1()
{
	int dispNum = 0;
	if (m_pImageView[dispNum]->m_IsCursorLarger)
	{
		m_pImageView[dispNum]->m_IsCursorLarger = FALSE;
	}
	else
	{
		m_pImageView[dispNum]->m_IsCursorLarger = TRUE;
	}
}
void CSmall_StudioDlg::OnCtxtClickedBin1()
{
	int dispNum = 0;
	if (m_pImageView[dispNum]->m_IsCursorBin)
	{
		m_pImageView[dispNum]->m_IsCursorBin = FALSE;
	}
	else
	{
		m_pImageView[dispNum]->m_IsCursorBin = TRUE;
	}
}
void CSmall_StudioDlg::OnCtxtClickedOvrly2()
{
	int dispNum = 1;
	if (m_pImageView[dispNum]->m_IsOverlay)
	{
		m_pImageView[dispNum]->m_IsOverlay = FALSE;
	}
	else
	{
		m_pImageView[dispNum]->m_IsOverlay = TRUE;
	}
}
void CSmall_StudioDlg::OnCtxtClickedLarg2()
{
	int dispNum = 1;
	if (m_pImageView[dispNum]->m_IsCursorLarger)
	{
		m_pImageView[dispNum]->m_IsCursorLarger = FALSE;
	}
	else
	{
		m_pImageView[dispNum]->m_IsCursorLarger = TRUE;
	}
}
void CSmall_StudioDlg::OnCtxtClickedBin2()
{
	int dispNum = 1;
	if (m_pImageView[dispNum]->m_IsCursorBin)
	{
		m_pImageView[dispNum]->m_IsCursorBin = FALSE;
	}
	else
	{
		m_pImageView[dispNum]->m_IsCursorBin = TRUE;
	}
}



// 1�� vision �˰��� ��� ��ư
void CSmall_StudioDlg::OnBnClickedAlgomod()
{
	int dispNum = 0;


	if (m_IsAlgoMod[dispNum])
	{
		// �˰��� ��� OFF
		m_IsAlgoMod[dispNum] = FALSE;

		GetDlgItem(IDC_ALGOMOD)->SetWindowTextW(_T("Algorithm Mode OFF"));

		GetDlgItem(IDC_ALGOADD1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ALGODEL1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ALGOEDIT1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CLEARRCD1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SAVERCD1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LOADRCD1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RUN1)->ShowWindow(SW_HIDE);


		GetDlgItem(IDC_ALGOLIST1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_OUTPUTLIST1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TACTIMELIST1)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_RECIPESTATIC1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_OUTPUTSTATIC1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TACTIMESTATIC1)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_PIC2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_ALGOMOD2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CAM2OPEN)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CAM2PLAY)->ShowWindow(SW_SHOW);
	}
	else
	{
		// �˰��� ��� ON
		// Recipe ���� ���� ��ư �� ����Ʈ�� ����
		m_IsAlgoMod[dispNum] = TRUE;
		GetDlgItem(IDC_ALGOMOD)->SetWindowTextW(_T("Algorithm Mode ON"));

		GetDlgItem(IDC_ALGOADD1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_ALGODEL1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_ALGOEDIT1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CLEARRCD1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SAVERCD1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_LOADRCD1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RUN1)->ShowWindow(SW_SHOW);
		

		GetDlgItem(IDC_ALGOLIST1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_OUTPUTLIST1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_TACTIMELIST1)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_RECIPESTATIC1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_OUTPUTSTATIC1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_TACTIMESTATIC1)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_PIC2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ALGOMOD2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CAM2OPEN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CAM2PLAY)->ShowWindow(SW_HIDE);
		
	}

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

// 2�� vision �˰��� ��� ��ư
void CSmall_StudioDlg::OnBnClickedAlgomod2()
{
	int dispNum = 1;


	if (m_IsAlgoMod[dispNum])
	{
		// �˰��� ��� OFF
		m_IsAlgoMod[dispNum] = FALSE;

		GetDlgItem(IDC_ALGOMOD2)->SetWindowTextW(_T("Algorithm Mode OFF"));

		GetDlgItem(IDC_ALGOADD2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ALGODEL2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ALGOEDIT2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CLEARRCD2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SAVERCD2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LOADRCD2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RUN2)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_ALGOLIST2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_OUTPUTLIST2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TACTIMELIST2)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_RECIPESTATIC2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_OUTPUTSTATIC2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TACTIMESTATIC2)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_PIC1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_ALGOMOD)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CAM1OPEN)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CAM1PLAY)->ShowWindow(SW_SHOW);
	}
	else
	{
		// �˰��� ��� ON
		// Recipe ���� ���� ��ư �� ����Ʈ�� ����
		m_IsAlgoMod[dispNum] = TRUE;
		GetDlgItem(IDC_ALGOMOD2)->SetWindowTextW(_T("Algorithm Mode ON"));

		GetDlgItem(IDC_ALGOADD2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_ALGODEL2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_ALGOEDIT2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CLEARRCD2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SAVERCD2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RUN2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_LOADRCD2)->ShowWindow(SW_SHOW);


		GetDlgItem(IDC_ALGOLIST2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_OUTPUTLIST2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_TACTIMELIST2)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_RECIPESTATIC2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_OUTPUTSTATIC2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_TACTIMESTATIC2)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_PIC1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ALGOMOD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CAM1OPEN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CAM1PLAY)->ShowWindow(SW_HIDE);
	}
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CSmall_StudioDlg::OnBnClickedAlgoadd1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

}


void CSmall_StudioDlg::OnBnClickedAlgoedit1()
{

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CSmall_StudioDlg::OnBnClickedAlgodel1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CSmall_StudioDlg::OnBnClickedAlgoadd2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CSmall_StudioDlg::OnBnClickedAlgoedit2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CSmall_StudioDlg::OnBnClickedAlgodel2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CSmall_StudioDlg::OnBnClickedRun2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CSmall_StudioDlg::OnBnClickedSavercd1()
{
	int dispNum = 0;
	recipeSaveSeq(dispNum);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CSmall_StudioDlg::OnBnClickedSavercd2()
{
	int dispNum = 1;
	recipeSaveSeq(dispNum);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CSmall_StudioDlg::OnBnClickedClearrcd1()
{
	if (AfxMessageBox(_T("�����Ǹ� ��ü Ŭ���� �Ͻðڽ��ϱ�?"), MB_YESNO) == IDYES)
	{
		m_AlgoList1.ResetContent();
		m_pHVSWrapper[0]->ClearRecipeStep();
	}
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CSmall_StudioDlg::OnBnClickedClearrcd2()
{
	if (AfxMessageBox(_T("�����Ǹ� ��ü Ŭ���� �Ͻðڽ��ϱ�?"), MB_YESNO) == IDYES)
	{
		m_AlgoList1.ResetContent();
		m_pHVSWrapper[0]->ClearRecipeStep();
	}
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CSmall_StudioDlg::OnBnClickedLoadrcd1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int dispNum = 0;
	recipeLoadSeq(dispNum);

}


void CSmall_StudioDlg::OnBnClickedLoadrcd2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int dispNum = 1;
	recipeLoadSeq(dispNum);
}

void CSmall_StudioDlg::OnBnClickedRecipeup1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CSmall_StudioDlg::OnBnClickedRecipedown1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CSmall_StudioDlg::OnBnClickedRecipeup2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CSmall_StudioDlg::OnBnClickedRecipedown2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}



CString CSmall_StudioDlg::findRCDPath()
{
	CString extension = _T("rcd files(*.rcd)|*.rcd|"); // rcd ���� ǥ��
	CFileDialog filedlg(TRUE, _T(".rcd"), NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, extension, this);

	if (filedlg.DoModal() == IDOK)
	{
		CString ret = filedlg.GetPathName();
		return ret;
	}
	return _T("");
}

BOOL CSmall_StudioDlg::recipeSaveSeq(int dispNum)
{
	CString extension = _T("rcd files(*.rcd)|*.rcd|"); // rcd ���� ǥ��
	CFileDialog filedlg(FALSE, _T(".rcd"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, extension, this);

	if (filedlg.DoModal() == IDOK)
	{
		CString path = filedlg.GetPathName();
		if (!m_pHVSWrapper[dispNum]->SaveRecipe(CT2A(path)) == RET_FAIL)
			return TRUE;
		else
			return FALSE;
	}
	else
	{
		return TRUE;
	}
	return FALSE;

}

BOOL CSmall_StudioDlg::recipeLoadSeq(int dispNum)
{
	USES_CONVERSION;
	CString fpath = findRCDPath();
	char buf[4096];
	DWORD dwRead;

	::ZeroMemory(buf, 4096);

	if (fpath != _T(""))
	{
		if (m_pHVSWrapper[dispNum]->LoadRecipe(CT2A(fpath)) == RET_OK)
		{
			int recipeCount = m_pHVSWrapper[dispNum]->GetRecipeCount();
			
			HANDLE hFile = CreateFile(fpath, GENERIC_READ, 0, NULL, OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL); // �����ڵ� ���

			ReadFile(hFile, buf, sizeof(buf), &dwRead, NULL); // buf�� ����ũ�⸸ŭ ����
			CloseHandle(hFile); // ���� �ʿ������ �ڵ��� �ݴ´�
			CString result = A2T(buf); // char to 2byte Char
			CString tok;
			
			if (dispNum == 0)
			{
				m_AlgoList1.ResetContent();
			}

			if (dispNum == 1)
			{
				m_AlgoLlist2.ResetContent();
			}

			for (int i = 0; i < recipeCount; i++)
			{
				AfxExtractSubString(tok, result, i, '\n');
				if (dispNum == 0)
				{
					m_AlgoList1.InsertString(-1, tok);
				}

				if (dispNum == 1)
				{
					m_AlgoLlist2.InsertString(-1, tok);
				}
			}
		}
		else
		{
			AfxMessageBox(_T("Recipe Load ����!"));
		}
	}
	return TRUE;
}

BOOL CSmall_StudioDlg::recipeRunSeq(int dispNum)
{
	m_pHVSWrapper[dispNum]->Run(
		m_pCamCtrl[dispNum]->m_pImage,
		m_pCamCtrl[dispNum]->m_camWidth,
		m_pCamCtrl[dispNum]->m_camHeight,
		8,
		RUN_TYPE_STEPSAVE_ON);

	for (int i = 0; i < RET_VARIABLE_MAX; i++)
	{
		CString str;
		CString varStr;

		if (m_pHVSWrapper[dispNum]->RetVariableGetText(i) != NULL)
		{
			varStr.Format(_T("%s"), m_pHVSWrapper[dispNum]->RetVariableGetText(i));
		}
		else
		{
			varStr = _T(" ");
		}
		str.Format(_T("Var %d %s : %.3lf"), i, varStr, m_pHVSWrapper[dispNum]->RetVariableGetValue(i));
		if (dispNum == 0)
		{
			m_OutputList1.InsertString(-1, str);
		}

		if (dispNum == 1)
		{
			m_OutputList2.InsertString(-1, str);
		}
	}
}