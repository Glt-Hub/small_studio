// OptionDlg.cpp : ���� �����Դϴ�.
//
#include "stdafx.h"
#include "OptionDlg.h"

#include "afxdialogex.h"


// COptionDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(COptionDlg, CDialogEx)

COptionDlg::COptionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COptionDlg::IDD, pParent)
{

}

COptionDlg::~COptionDlg()
{
}

void COptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COptionDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &COptionDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &COptionDlg::OnBnClickedCancel)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// COptionDlg �޽��� ó�����Դϴ�.

BOOL COptionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	GetDlgItem(IDC_CAM1IPCONTROL)->SetWindowTextW(m_CamIP[0]);
	GetDlgItem(IDC_CAM2IPCONTROL)->SetWindowTextW(m_CamIP[1]);
	GetDlgItem(IDC_CAM1EXPEDIT)->SetWindowTextW(m_CamExposure[0]);
	GetDlgItem(IDC_CAM2EXPEDIT)->SetWindowTextW(m_CamExposure[1]);
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void COptionDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	GetDlgItemText(IDC_CAM1IPCONTROL, m_CamIP[0]);
	GetDlgItemText(IDC_CAM2IPCONTROL, m_CamIP[1]);
	GetDlgItemText(IDC_CAM1EXPEDIT, m_CamExposure[0]);
	GetDlgItemText(IDC_CAM2EXPEDIT, m_CamExposure[1]);

	for (int i = 0; i < MAXCAM; i++)
	{
		CString camnum;
		camnum.Format(_T("CAMERA%d"), i + 1);
		WritePrivateProfileString(camnum, _T("IP"), m_CamIP[i], m_optionPath);
		WritePrivateProfileString(camnum, _T("Exposure"), m_CamExposure[i], m_optionPath);
	}

	CDialogEx::OnOK();
}

void COptionDlg::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CDialogEx::OnCancel();
}

void COptionDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}