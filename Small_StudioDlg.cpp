
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

void ContinuousThreadProc(CSmall_StudioDlg* pPrivate)
{
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
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	for (int i = 0; i < MAXCAM; i++)
	{
		m_pBit[i] = NULL;
		m_pCOriImage[i] = NULL;
		m_pCamCtrl[i] = NULL;
		m_CamIP[i] = _T("");
		m_CamExposure[i] = 0.0;
		m_IsOpen[i] = FALSE;
		m_IsPlay[i] = FALSE;
		m_DrawCommand[i] = FALSE;
		m_pBitmap[i] = NULL;

	}

	for (int i = 0; i < LIGHTCH; i++)
	{
		m_OptionBright[i] = _T("");
	}
}

void CSmall_StudioDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
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
	ON_MESSAGE(WM_MYRECEIVE, OnReceive)
	ON_BN_CLICKED(IDC_DEBUGDRAGON, &CSmall_StudioDlg::OnBnClickedDebugdragon)
END_MESSAGE_MAP()


// CSmall_StudioDlg �޽��� ó����

BOOL CSmall_StudioDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	::InitializeCriticalSection(&mSc);

	for (int i = 0; i < MAXCAM; i++)
	{
		m_pCOriImage[i] = new CImage();

		if (i == 0)
		{
			static CClientDC dispDC(GetDlgItem(IDC_PIC1));
			m_hDC[i] = dispDC.GetSafeHdc();
			GetDlgItem(IDC_PIC1)->GetWindowRect(m_rcDisp[i]);
			m_pGraphics[i] = Graphics::FromHDC(m_hDC[i]);
		}
		if (i == 1)
		{
			static CClientDC dispDC(GetDlgItem(IDC_PIC2));
			m_hDC[i] = dispDC.GetSafeHdc();
			GetDlgItem(IDC_PIC2)->GetWindowRect(m_rcDisp[i]);
			m_pGraphics[i] = Graphics::FromHDC(m_hDC[i]);
		}
	}

	m_optionPath.Format(_T("%s\\option.ini"), GetExePath());
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
		delete m_pLightCtrl;
	}

	for (int i = 0; i < MAXCAM; i++)
	{
		if (m_IsOpen[i])
		{
			delete m_pCamCtrl[i];
		}

		if (m_pCOriImage[i] != NULL)
		{
			delete m_pCOriImage[i];
		}
		if (m_pBit[i] != NULL)
		{
			free(m_pBit[i]);
			m_pBit[i] = NULL;
		}

		if (m_pBitmap[i] != NULL)
		{
			delete m_pBitmap[i];
			m_pBitmap[i] = NULL;
		}

	}

	if (m_IsSystemInit)
	{
		ST_FreeSystem();
	}
	::DeleteCriticalSection(&mSc);
}

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
	m_pCamCtrl[dispNum]->TriggerSet(m_CamTrig[dispNum]);

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
		delete m_pCamCtrl[dispNum];
		m_pCamCtrl[dispNum] = NULL;
		m_IsOpen[dispNum] = FALSE;
		GetDlgItem(IDC_CAM1OPEN)->SetWindowTextW(_T("Camera 1 Closed"));
		GetDlgItem(IDC_CAM1PLAY)->EnableWindow(FALSE);
		free(m_pBit[dispNum]);
		m_pBit[dispNum] = NULL;
	}
	GetDlgItem(IDC_CAM1OPEN)->EnableWindow(TRUE);
}


void CSmall_StudioDlg::OnBnClickedCam2open()
{
	int dispNum = 1;
	GetDlgItem(IDC_CAM2OPEN)->EnableWindow(FALSE);
	GetOptionValue(OPT_READ_CAM, dispNum);
	if (!m_IsOpen[dispNum])
	{
		if (camOpenSeq(dispNum))
		{
			GetDlgItem(IDC_CAM2OPEN)->SetWindowTextW(_T("Camera 2 Open"));
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
		delete m_pCamCtrl[dispNum];
		m_pCamCtrl[dispNum] = NULL;
		m_IsOpen[dispNum] = FALSE;
		GetDlgItem(IDC_CAM2OPEN)->SetWindowTextW(_T("Camera 2 Closed"));
		GetDlgItem(IDC_CAM2PLAY)->EnableWindow(FALSE);
		free(m_pBit[dispNum]);
		m_pBit[dispNum] = NULL;
	}
	GetDlgItem(IDC_CAM2OPEN)->EnableWindow(TRUE);
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

			if (m_pCamCtrl[i] != NULL)
			{
				// Exposure time�� ��� �ٷ� �ݿ��˴ϴ�. 20201103 ���Ѱ�
				if (m_IsOpen[i])
				{
					if (!m_IsPlay[i])
					{
						m_pCamCtrl[i]->TriggerSet(m_CamTrig[i]);
					}
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
		else
		{
			
		}
		m_IsSerialOpen = FALSE;
	}
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CSmall_StudioDlg::OnBnClickedCam1play()
{
	if (m_IsPlay[0])
	{
		GetDlgItem(IDC_CAM1PLAY)->SetWindowTextW(_T("Play"));
		m_IsPlay[0] = FALSE;
	}

	else
	{
		m_IsPlay[0] = TRUE;
		GetDlgItem(IDC_CAM1PLAY)->SetWindowTextW(_T("Stop"));
		DrawImageSeq(0);
	} // SW Trigger �� ���� ����� �����

	
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CSmall_StudioDlg::OnBnClickedCam2play()
{
	if (m_IsPlay[1])
	{
		GetDlgItem(IDC_CAM2PLAY)->SetWindowTextW(_T("Play"));
		m_IsPlay[1] = FALSE;
	}
	else
	{
		GetDlgItem(IDC_CAM2PLAY)->SetWindowTextW(_T("Stop"));
		m_IsPlay[1] = TRUE;
		DrawImageSeq(1);
	}
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

// Grab -> CImage -> ȭ�鿡 Draw �ϴ� ���� - ����
// Grab -> GDIPlus bitmap -> ȭ�鿡 Draw�ϴ� ����
// 20201105 ���Ѱ�
BOOL CSmall_StudioDlg::DrawImageSeq(int dispNum)
{
	if (m_CamTrig[dispNum] == CAMERA_TRIG_SW)
	{
		::EnterCriticalSection(&mSc);
		// ���� On
		LightSend(dispNum, TRUE);
		// Image Grab
		m_pCamCtrl[dispNum]->GrabImageSW();
		// ���� ó�� (MemDC Draw ��) �Ϸ�� Image ������ ���� alloc
		if (m_pBit[dispNum] == NULL)
		{
			m_pBit[dispNum] = (BYTE*)malloc(m_pCamCtrl[dispNum]->m_bufferSize);
		}
		// ó�� �Ϸ�� �̹��� ����
		memcpy(m_pBit[dispNum], m_pCamCtrl[dispNum]->m_pImage, m_pCamCtrl[dispNum]->m_bufferSize);
		
		Sleep(5);
		// ���� Off
		LightSend(dispNum, FALSE);
		// DIBMake(dispNum);
		// hbitmap2CImage(dispNum);
		// RawToGDIPBmp(dispNum);
		if (dispNum == 0)
		{
			GetDlgItem(IDC_CAM1PLAY)->SetWindowTextW(_T("Play"));
		}
		else if (dispNum == 1)
		{
			GetDlgItem(IDC_CAM2PLAY)->SetWindowTextW(_T("Play"));
		}
		else
		{
			return FALSE;
		}
		::LeaveCriticalSection(&mSc);
	}

	// continuous mode ������ 20201106 ���Ѱ�
	else if (m_CamTrig[dispNum] == CAMERA_TRIG_CONTINUOUS)
	{
		::EnterCriticalSection(&mSc);
		m_pCamCtrl[dispNum]->GrabImageContinuous();
		// DIBMake(dispNum);
		// hbitmap2CImage(dispNum);
		::LeaveCriticalSection(&mSc);
	}

	return TRUE;
}

BOOL CSmall_StudioDlg::RawToGDIPBmp(int dispNum, int width, int height, BYTE* buffer)
{
	BitmapData bitmapdata;
	Rect rc;
	ColorPalette* pPalette;
	int	size = 0;

	if (m_pBitmap[dispNum] != NULL)
	{
		delete m_pBitmap[dispNum];
		m_pBitmap[dispNum] = NULL;
	}

	m_pBitmap[dispNum] = new Bitmap(width, height, PixelFormat8bppIndexed);
	rc = Rect(0, 0, width, height);
	m_pBitmap[dispNum]->LockBits(&rc, 0, PixelFormat8bppIndexed, &bitmapdata);
	memcpy(bitmapdata.Scan0, buffer, width*height);
	m_pBitmap[dispNum]->UnlockBits(&bitmapdata);

	size = m_pBitmap[dispNum]->GetPaletteSize();
	pPalette = new ColorPalette[size];
	m_pBitmap[dispNum]->GetPalette(pPalette, size);
	for (unsigned int i = 0; i < pPalette->Count; i++)
	{
		pPalette->Entries[i] = Color::MakeARGB(255, i, i, i);
	}
	m_pBitmap[dispNum]->SetPalette(pPalette);
	delete[] pPalette;
	InvalidateRect(m_rcDisp[dispNum], NULL);
	return TRUE;
}

// �Ƹ�.. viewer�� �����ϸ� ����� �ϰ� �� ��? �ƴϸ� MemDC�� �����ϴ� �κ� ���� �������
// ���� ���Ұ� 20201106 ���Ѱ�
BOOL CSmall_StudioDlg::DIBMake(int dispNum)
{
	CString debug;
	OutputDebugString(_T("\n\n"));

	HDC hDC = ::GetDC(NULL);

	OutputDebugString(_T("after getDC\n"));
	for (int i = 0; i < 10; i++)
	{
		debug.Format(_T("%d"), m_pBit[dispNum][i]);
		OutputDebugString(debug);
	}
	OutputDebugString(_T("\n\n"));

	BITMAPINFO bmi;
	memset(&bmi, 0, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = m_pCamCtrl[dispNum]->m_camWidth;	// Cam���� ���� Width
	bmi.bmiHeader.biHeight = m_pCamCtrl[dispNum]->m_camHeight;	// Cam���� ���� Height
	bmi.bmiHeader.biPlanes = 1;									// ?
	bmi.bmiHeader.biBitCount = 8;								// 8bpp
	bmi.bmiHeader.biCompression = BI_RGB;						// RGB Raw data�̹Ƿ�
	// CreateDIBSection���� HBITMAP ����
	m_hBmp[dispNum] = CreateDIBSection(hDC, &bmi, DIB_RGB_COLORS, (void**)&(m_pBit[dispNum]), NULL, 0);
	OutputDebugString(_T("after createDIB\n"));
	for (int i = 0; i < 10; i++)
	{
		debug.Format(_T("%d"), m_pBit[dispNum][i]);
		OutputDebugString(debug);
	}
	OutputDebugString(_T("\n\n"));
	::ReleaseDC(NULL, hDC);

	return TRUE;
}
// ���� ���Ұ� 20201106 ���Ѱ�
BOOL CSmall_StudioDlg::hbitmap2CImage(int dispNum)
{
	if (!m_pCOriImage[dispNum]->IsNull())
	{
		m_pCOriImage[dispNum]->Destroy();
	}	
	
	CImage dest_image;
	BITMAP dest_bmp_info;
	GetObject(m_hBmp[dispNum], sizeof(BITMAP), &dest_bmp_info);
	/*
	dest_bmp_info.bmBits = m_hBmp[dispNum] + sizeof(BITMAPINFO);
	dest_bmp_info.bmBitsPixel = 8;
	dest_bmp_info.bmHeight = m_pCamCtrl[dispNum]->m_camHeight;
	dest_bmp_info.bmWidth = m_pCamCtrl[dispNum]->m_camWidth;
	dest_bmp_info.bmPlanes = 1;
	*/

	dest_image.Create(dest_bmp_info.bmWidth, dest_bmp_info.bmHeight, dest_bmp_info.bmBitsPixel);
	GetObject((HBITMAP)dest_image, sizeof(BITMAP), &dest_bmp_info);
	GetBitmapBits(m_hBmp[dispNum], dest_bmp_info.bmHeight*dest_bmp_info.bmWidth*dest_bmp_info.bmBitsPixel / 8, dest_bmp_info.bmBits);
	
	// m_pCOriImage[dispNum]->Attach(m_hBmp[dispNum]);
	COLORREF ref;
	CString debug;
	OutputDebugString(_T("cimage attach\n"));
	for (int i = 0; i < 10; i++)
	{
		debug.Format(_T("%d"), m_pBit[dispNum][i]);
		OutputDebugString(debug);
	}
	OutputDebugString(_T("\n\n"));
	OutputDebugString(_T("cimage getpixel\n"));
	for (int i = 0; i < 10; i++)
	{
		ref = dest_image.GetPixel(i, 0);
		debug.Format(_T("%d"), ref);
		OutputDebugString(debug);
	}
	OutputDebugString(_T("\n\n"));

	return TRUE;
}

void CSmall_StudioDlg::OnBnClickedDebugdragon()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

// dispNum���� �ش��ϴ� ī�޶� ��ü ���� ���� ��Ʈ�ѷ� ��� �۽ű�
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
			middle.Format(_T("CH%sS%s"), m_CamLightCh[dispNum], m_OptionBright[chan]);
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


LRESULT	CSmall_StudioDlg::OnReceive(WPARAM length, LPARAM lpara)
{
	CString str;
	char data[10000];
	if (m_pLightCtrl && m_optionmodal)
	{
		m_pLightCtrl->Receive(data, (int)length);
		data[length] = '\0';
		str = data;
		AfxMessageBox(str);
		GetDlgItem(IDC_LTESTRCV)->SetWindowTextW(str);
		str = "";
	}

	return 0;
}
