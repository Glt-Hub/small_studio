
// Small_StudioDlg.cpp : ���� ����
//

#include "stdafx.h"

#include "Small_Studio.h"
#include "Small_StudioDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

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
	, m_pCamCtrl1(NULL)
	, m_pCamCtrl2(NULL)
	, m_statusCode(-99999)
	, m_strErr(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
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
END_MESSAGE_MAP()


// CSmall_StudioDlg �޽��� ó����

BOOL CSmall_StudioDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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

	if (m_IsSystemInit)
	{
		ST_FreeSystem();
	}
}


void CSmall_StudioDlg::OnBnClickedCam1open()
{
	if (!m_IsSystemInit)
	{
		AfxMessageBox(_T("System Init�� ���� �ʾҽ��ϴ�. ī�޶� ����� �� �����ϴ�."));
		return;
	}
	
	m_pCamCtrl1 = new CCrevisCtrl(0);

	m_statusCode = m_pCamCtrl1->OpenDevice();
	
	if (m_statusCode == CAMERA_OPEN_ERROR)
	{
		m_strErr.Format(_T("ī�޶� Open ����!"));
		AfxMessageBox(m_strErr);
		delete m_pCamCtrl1;
		return;
	}
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CSmall_StudioDlg::OnBnClickedCam2open()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CSmall_StudioDlg::OnBnClickedOptionbtn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CSmall_StudioDlg::OnBnClickedLightopen()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CSmall_StudioDlg::OnBnClickedCam1play()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CSmall_StudioDlg::OnBnClickedCam2play()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

