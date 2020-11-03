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
	DDX_Control(pDX, IDC_CAM1MODE, m_Cam1ModeCombo);
	DDX_Control(pDX, IDC_CAM2MODE, m_Cam2ModeCombo);
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

	CString cstr;
	
	cstr.Format(_T("Software Trigger"));
	m_Cam1ModeCombo.InsertString(CAMERA_TRIG_SW, cstr);
	m_Cam2ModeCombo.InsertString(CAMERA_TRIG_SW, cstr);

	cstr.Format(_T("Continuous"));
	m_Cam1ModeCombo.InsertString(CAMERA_TRIG_CONTINUOUS, cstr);
	m_Cam2ModeCombo.InsertString(CAMERA_TRIG_CONTINUOUS, cstr);

	m_Cam1ModeCombo.SetCurSel(_ttoi(m_CamTrig[0]));
	m_Cam2ModeCombo.SetCurSel(_ttoi(m_CamTrig[1]));

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
	
	CString grabmode;

	grabmode.Format(_T("%d"), m_Cam1ModeCombo.GetCurSel());
	m_CamTrig[0] = grabmode;
	
	grabmode.Format(_T("%d"), m_Cam2ModeCombo.GetCurSel());
	m_CamTrig[1] = grabmode;
	
	for (int i = 0; i < MAXCAM; i++)
	{
		if (_ttof(m_CamExposure[i]) > 65555.0f || _ttof(m_CamExposure[i]) < 0.0f)
		{
			AfxMessageBox(_T("Exposure Time�� 0~65535 ������ ���� �Է� �ٶ��ϴ�."));
			return;
		}

		CString camnum;
		camnum.Format(_T("CAMERA%d"), i + 1);
		WritePrivateProfileString(camnum, _T("IP"), m_CamIP[i], m_optionPath);
		WritePrivateProfileString(camnum, _T("Exposure"), m_CamExposure[i], m_optionPath);
		WritePrivateProfileString(camnum, _T("GRAB_MODE"), m_CamTrig[i], m_optionPath);
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