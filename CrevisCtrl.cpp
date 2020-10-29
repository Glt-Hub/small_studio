#include "stdafx.h"
#include "CrevisCtrl.h"

CCrevisCtrl::CCrevisCtrl(int camnum)
{
	m_IsDeviceOpen = FALSE;
	m_IsInitSystem = false;
	m_CamNum = camnum;
	m_AvailCams = 0;
}


CCrevisCtrl::~CCrevisCtrl()
{
	bool isopen = false;
	ST_IsOpenDevice(m_hDevice, &isopen);
	if (isopen)
	{
		CloseDevice();
	}
}

// �����ڿ��� ������ �� camnum �� ° ī�޶� Open�մϴ�. 20201029 ���Ѱ�
INT32 CCrevisCtrl::OpenDevice()
{
	m_status = ST_IsInitSystem(&m_IsInitSystem);
	if (m_status != MCAM_ERR_SUCCESS)
	{
		
	}

	if (!m_IsInitSystem)
	{
		return SYSTEM_NOT_INIT;
	}
	m_status = ST_GetAvailableCameraNum(&m_AvailCams);	

	if (m_AvailCams < 0)
	{
		return NO_CAMERA_DETECTED;
	}
	
	m_status = ST_OpenDevice(m_CamNum, &m_hDevice);

	if (m_status != MCAM_ERR_SUCCESS)
	{
		return CAMERA_OPEN_ERROR;
	}
	m_IsDeviceOpen = TRUE;
	return CAMERA_OPEN_SUCCESS;
}

// ��ü�� ������ �� Open�� ī�޶� Close�մϴ�. 20201029 ���Ѱ�
INT32 CCrevisCtrl::CloseDevice()
{
	m_status = ST_CloseDevice(m_hDevice);
	m_IsDeviceOpen = FALSE;
	if (m_status != MCAM_ERR_SUCCESS)
	{
		return CAMERA_CLOSE_FAIL;
	}
	return CAMERA_CLOSE_SUCCESS;
}

