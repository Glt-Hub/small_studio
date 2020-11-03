#pragma once
#include "stdafx.h"
#include "Small_StudioDlg.h"
#include "Small_Studio.h"
#include "afxwin.h"

// COptionDlg ��ȭ �����Դϴ�.

class COptionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COptionDlg)

public:
	COptionDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~COptionDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_OPTIONDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCam1ipsetbtn();
	afx_msg void OnBnClickedCam2ipsetbtn();
	afx_msg void OnBnClickedCam1expset();
	afx_msg void OnBnClickedCam2expset();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();

public:
	CString			m_CamIP[MAXCAM];
	CString			m_CamExposure[MAXCAM];
	CString			m_CamTrig[MAXCAM];
	CString			m_optionPath;
	afx_msg void OnBnClickedSerialsetbtn();
	// ī�޶� Grab Mode ������ �޺��ڽ� 20201102 ���Ѱ�
	CComboBox m_Cam1ModeCombo;
	CComboBox m_Cam2ModeCombo;
};
