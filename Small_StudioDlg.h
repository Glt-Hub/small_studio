
// Small_StudioDlg.h : ��� ����
//

#pragma once
#include "CrevisCtrl.h"


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

public:
	bool			m_IsSystemInit;
	CCrevisCtrl*	m_pCamCtrl1;
	CCrevisCtrl*	m_pCamCtrl2;
	INT32			m_statusCode;
	CString			m_strErr;
};
