#pragma once

// Crevis Camera Control�� ���� Class�Դϴ�.
// ī�޶� 1���� 1���� ��ü�� �����ϴ�.
// 20201029 ���Ѱ�
class CCrevisCtrl
{
public:
	CCrevisCtrl(int camnum);
	~CCrevisCtrl();

	// Camera Open | Close�� �Լ��Դϴ� 20201029 ���Ѱ�
	INT32 OpenDevice();
	INT32 CloseDevice();
	

public:
	BOOL			m_IsDeviceOpen;
	bool			m_IsInitSystem;
	int				m_CamNum;
	UINT			m_AvailCams;
	INT32			m_status;
	INT32			m_hDevice;

};

