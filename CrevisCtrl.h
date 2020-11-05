#pragma once
#define BUFSIZE 100;

// Crevis Camera Control�� ���� Class�Դϴ�.
// ī�޶� 1���� 1���� ��ü�� �����ϴ�.
// 20201029 ���Ѱ�
class CCrevisCtrl
{
public:
	CCrevisCtrl(CString InputIP);
	~CCrevisCtrl();
	CAMERA_ERRCODE  OpenDevice();
	CAMERA_ERRCODE	CloseDevice();
	DOUBLE			GetDeviceExposure();
	BOOL			SetDeviceExposure(DOUBLE ExposeTime);
	BOOL			TriggerSet(INT32 Trigger);
	BOOL			TriggerOff();
	BOOL			GrabImageSW();
	BOOL			SetSWTrigger();
	BOOL			GrabImageContinuous();

private:
	CAMERA_ERRCODE	GetTriggerStatus();

public:
	CRITICAL_SECTION mSc;
	BYTE*			m_pImage;			// �̹��� ����
	BOOL			m_IsDeviceOpen;		// Device Open ���� üũ
	BOOL			m_IsAcq;			// Device Acquisition on üũ
	bool			m_IsInitSystem;		// SDK Init ���� üũ
	INT32			m_status;			// Device ��� �۽� �� ����
	int				m_hDevice;			// Device �ڵ�
	CString			m_deviceIPCStr;		// Device ������� ���� IP �ּ�
	INT32			m_camWidth;			// Image ��
	INT32			m_camHeight;		// Image ����
	UINT32			m_camNum;			// Device ��ȣ
	INT32			m_bufferSize;		// ���� ���� ������ (width * height)
										// 20201030 ���Ѱ�
};

