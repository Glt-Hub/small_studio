
// Small_Studio.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CSmall_StudioApp:
// �� Ŭ������ ������ ���ؼ��� Small_Studio.cpp�� �����Ͻʽÿ�.
//

class CSmall_StudioApp : public CWinApp
{
public:
	CSmall_StudioApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CSmall_StudioApp theApp;