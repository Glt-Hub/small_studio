#include "stdafx.h"
#include "CrevisCtrl.h"

// ���� �� ������ ī�޶��� IP �ּҸ� �Է��ؾ� �մϴ�. 20201029 ���Ѱ�
CCrevisCtrl::CCrevisCtrl(CString InputIP)
{
	m_IsDeviceOpen = FALSE;
	m_IsInitSystem = false;
	m_status = 0;
	m_hDevice = 0;
	m_deviceIPCStr = InputIP;
	m_IsAcq = FALSE;
}

// �Ҹ��� ���� �� ī�޶� Close�� ������ �ʾҴٸ� �ڵ����� Close�� �����մϴ�. 20201029 ���Ѱ�
CCrevisCtrl::~CCrevisCtrl()
{
	bool isopen = false;
	ST_IsOpenDevice(m_hDevice, &isopen);
	if (isopen)
	{
		CloseDevice();
	}
}

// �����ڿ��� ������ �� IP �ּҷ� ī�޶� Open�մϴ�. 20201029 ���Ѱ�
INT32 CCrevisCtrl::OpenDevice()
{
	// System Init üũ�Դϴ� 20201029 ���Ѱ�
	m_status = ST_IsInitSystem(&m_IsInitSystem);
	if (m_status != MCAM_ERR_SUCCESS)
	{
		return CAMSYS_NOT_INIT;
	}

	if (!m_IsInitSystem)
	{
		return CAMSYS_NOT_INIT;
	}

	m_status = ST_UpdateDevice();
	if (m_status != MCAM_ERR_SUCCESS)
	{
		m_deviceIPCStr = _T("");
		return CAMERA_OPEN_ERROR;
	}
	UINT AvailCams;
	// ���� ������ �� �ִ� ī�޶� �����ŭ IP �ּ� �˻��� ����
	// ��ġ�ϴ� ī�޶� �����մϴ�. 20201029 ���Ѱ�
	m_status = ST_GetAvailableCameraNum(&AvailCams);
	if (m_status != MCAM_ERR_SUCCESS)
	{
		return CAMERA_INFOGET_FAIL;
	}
	if (AvailCams == 0)
	{
		return NO_CAMERA_DETECTED;
	}

	unsigned int bufsize = 100;
	char * deviceIp = new char[100];
	CString deviceIPCstr = _T("");

	// ������ �� �ִ� ī�޶� ��ȸ, �Է��� IP �ּҿ� �ش��ϴ� ī�޶� ã���ϴ�. 20201029 ���Ѱ�
	for (int i = 0; i < AvailCams; i++)
	{
		m_status = ST_GetEnumDeviceInfo(i, MCAM_DEVICEINFO_IP_ADDRESS, deviceIp, &bufsize);
		if (m_status != MCAM_ERR_SUCCESS)
		{
			delete deviceIp;
			return CAMERA_INFOGET_FAIL;
		}
		else
		{
			deviceIPCstr = deviceIp;
		}
		// ��ġ�ϴ� ī�޶� ã���� ������ �����մϴ�. 20201029 ���Ѱ�
		if (deviceIPCstr == m_deviceIPCStr)
		{
			delete deviceIp;
			m_status = ST_OpenDevice(i, &m_hDevice);
			if (m_status != MCAM_ERR_SUCCESS)
			{
				return CAMERA_OPEN_ERROR;
			}
			m_status = ST_GetIntReg(m_hDevice, MCAM_WIDTH, &m_camWidth);
			if (m_status != MCAM_ERR_SUCCESS)
			{
				return CAMERA_INFOGET_FAIL;
			}

			m_status = ST_GetIntReg(m_hDevice, MCAM_HEIGHT, &m_camHeight);
			if (m_status != MCAM_ERR_SUCCESS)
			{
				return CAMERA_INFOGET_FAIL;
			}

			m_status = ST_AcqStart(m_hDevice);
			if (m_status != MCAM_ERR_SUCCESS)
			{
				return CAMERA_ACQ_FAIL;
			}
			else
			{
				m_IsAcq = TRUE;
			}
			m_IsDeviceOpen = TRUE;
			return CAMERA_OPEN_SUCCESS;
		}
	}
	delete deviceIp;
	return CAMERA_IP_NOTFOUND;
}

// ī�޶� Close�մϴ�. 20201029 ���Ѱ�
INT32 CCrevisCtrl::CloseDevice()
{
	if (m_IsDeviceOpen)
	{
		if (m_IsAcq)
		{
			ST_AcqStop(m_hDevice);
			m_IsAcq = FALSE;
		}

		m_status = ST_CloseDevice(m_hDevice);
		m_IsDeviceOpen = FALSE;
		if (m_status != MCAM_ERR_SUCCESS)
		{
			return CAMERA_CLOSE_FAIL;
		}
		return CAMERA_CLOSE_SUCCESS;
	}
	return CAMERA_CLOSE_NOTOPEN;
}



