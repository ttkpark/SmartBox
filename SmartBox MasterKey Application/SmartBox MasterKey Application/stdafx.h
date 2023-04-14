#pragma once

#define RES_OK  0
#define RES_ERR 1
#define NULLSTRING _T("")
#define Lengthis0 0

#define WM_TAB2_UPDATE_VAL	(WM_USER+1)
#define WM_STATBARTXT		(WM_USER+2)
#define WM_LOG				(WM_USER+3)

#define WM_TRAY_NOTIFICATION WM_APP + 1
#define SMARTBOXAPPLICATION_TRAY 84935

class DataList {
	void** data;
	int* Size;
	int contents;
	int focus;
public:
	void initContents(int);
	void initData(int, void*, int);
	void DeleteData(int);
	void* WhereisData(int, int&);
	int AlldataSize();
	void DeleteContents();
	int getfocusID();
	int setfocusID(int);
	int getendID();
	~DataList();
};


class COMPort {
public:
	int number;
	char Descreption[30];
	wchar_t* GetPortNameW();
	char* GetPortName();
};

struct SensorRange
{ 
	CString sensortype;//�µ�:TP ����:HU...
	float d01fr;//���� ���� 1
	float d01to;//
	float d02fr;//���� ���� 2
	float d02to;//
	float w01fr;//��� ���� 1
	float w01to;//
	float w02fr;//��� ���� 2
	float w02to;//
};


struct SensorInitData
{
	CString m_StrAPPKEY;     // APPKEY
	CString m_StrStartDate;  // ��������
	CString m_StrWarningmin; // ���Ⱓ
	CString m_StrDangermin;  // ����Ⱓ
	CString m_StrSavetotmin; // �����Ⱓ
	CString m_StrProduct;    // ��ǰ��
	CString m_StrRACLL;      // RAC����(LL)
	CString m_StrRACMM;      // RAC����(MM)
	CString m_StrRACSS;      // RAC����(SS)
	CString m_StrSendTime;   // �����ֱ�
	CString m_StrRevision;		 // ������
	CString m_isReplace;	 // ��ü �÷���

	CString m_BoxNum;		 // �ڽ� ��ȣ
	CString work_zone_num;	 // ��ũ�� ��ȣ
	CString work_zone_name;	 // ��ũ�� �̸�
	CString m_idxWE;		 // idxWE
	SensorRange range01;
};
struct SensorDataList
{
	CString serialno;     // ��ǰ��
	CString appkey;

	CString StartDate;
	CString WarningDate;
	CString DangerDate;
	CString LastDate;

	CString alertstatus;

	CString savetotmin;
	CString warningmin;
	CString dangermin;

	CString idxGoods;
	CString RACLL;
	CString RACMM;
	CString RACSS;
	CString SendTimeSec;
	CString Revision;

	CString BoxNum;

	CString work_zone_num;
	CString work_zone_name;	 // ��ũ�� �̸�
	CString idxWE;

	SensorRange range01;
};