
// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.
#define MAXCAM 2
#define LIGHTCH 4



// ī�޶� ���� �����ڵ� 20201030 ���Ѱ�
enum CAMERA_ERRCODE 
{ 
	CAMERA_OPEN_SUCCESS		= 0, 
	CAMSYS_NOT_INIT			= -1000, 
	CAMERA_OPEN_ERROR		= -1001, 
	NO_CAMERA_DETECTED		= -1002, 
	CAMERA_CLOSE_SUCCESS	= 0,
	CAMERA_CLOSE_FAIL		= -1003,
	CAMERA_CLOSE_NOTOPEN	= -1004,
	CAMERA_IP_NOTFOUND		= -1,
	CAMERA_MODCH_FAIL		= -1005,
	CAMERA_INFOGET_FAIL		= -1006,
	CAMERA_ACQ_FAIL			= -1007,
	CAMERA_TRIG_SW			= 0,
	CAMERA_TRIG_CONTINUOUS	= 1
};

enum OPTION_READ_MODE
{
	OPT_READ_ALL			= 0,
	OPT_READ_CAM			= 1,
	OPT_READ_PLAY			= 2,
	OPT_READ_LIGHT			= 3
};
#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // �Ϻ� CString �����ڴ� ��������� ����˴ϴ�.

// MFC�� ���� �κа� ���� ������ ��� �޽����� ���� ����⸦ �����մϴ�.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC �ٽ� �� ǥ�� ���� ����Դϴ�.
#include <afxext.h>         // MFC Ȯ���Դϴ�.


#include <afxdisp.h>        // MFC �ڵ�ȭ Ŭ�����Դϴ�.



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC�� ���� �� ��Ʈ�� ���� ����


// GDI+
#include <gdiplus.h>
#pragma comment(lib, "gdiplus")
using namespace Gdiplus;
//

#include "VirtualFG40.h" // Crevis Camera�� SDK include

#pragma warning (disable:4819)
#pragma warning (disable:4996)

#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>


#include "HVisionErrCode.h"
#include "HVisionLibWrapper.h"
#include "HVisionParameter.h"


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' nae='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

