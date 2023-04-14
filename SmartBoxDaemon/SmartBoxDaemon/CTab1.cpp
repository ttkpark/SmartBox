// CTab1.cpp : implementation file
//

#include "pch.h"
#include "SmartBoxDaemon.h"
#include "CTab1.h"
#include "afxdialogex.h"

#include "SmartBoxDaemonDlg.h"


// CTab1 dialog

IMPLEMENT_DYNAMIC(CTab1, CDialogEx)

CTab1::CTab1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TAB1, pParent)
{

}

CTab1::~CTab1()
{
}

void CTab1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCPROPERTYGRID1, m_ManualData);
	DDX_Control(pDX, IDC_SENSORCNT, m_SensorCnt);
	DDX_Control(pDX, IDC_SNREPLACECHK, m_SensorReplaceChk);
	DDX_Control(pDX, IDC_TOSERIALEDIT, m_toSerialEdit);
	DDX_Control(pDX, IDC_FRSERIALEDIT, m_frSerialEdit);
	DDX_Control(pDX, IDC_REPBTN, m_ReplaceUpdateBtn);
	DDX_Control(pDX, IDC_SEND, m_SendButton);
	DDX_Control(pDX, IDC_TAB1ICON, m_Tab1ICON);
}


BEGIN_MESSAGE_MAP(CTab1, CDialogEx)
//	ON_WM_ERASEBKGND()
ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, &CTab1::OnAfxWmPropertyChanged)
ON_EN_CHANGE(IDC_SENSORCNT, &CTab1::OnEnChangeSensorcnt)
ON_BN_CLICKED(IDC_SNREPLACECHK, &CTab1::OnBnClickedSnreplacechk)
ON_BN_CLICKED(IDC_REPBTN, &CTab1::OnBnClickedRepbtn)
ON_EN_CHANGE(IDC_FRSERIALEDIT, &CTab1::OnEnChangeFrserialedit)
ON_EN_CHANGE(IDC_TOSERIALEDIT, &CTab1::OnEnChangeToserialedit)
ON_BN_CLICKED(IDC_SEND, &CTab1::OnBnClickedSend)
END_MESSAGE_MAP()


// CTab1 message handlers


void CTab1::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialogEx::PostNcDestroy();
}


//BOOL CTab1::OnEraseBkgnd(CDC* pDC)
//{
//	// TODO: Add your message handler code here and/or call default
//	CRect rect;
//	GetClientRect(&rect);
//	pDC->FillSolidRect(&rect, RGB(255, 255, 255));
//	Invalidate(0);
//
//	return CDialogEx::OnEraseBkgnd(pDC);
//}
LPCTSTR PropertyData[10][3] = {};
void UpdateVal(CTab1 *my)
{
	CMFCPropertyGridProperty** Propertyarr[10] = { &my->m_PropAPPKEY, &my->m_PropStartDate,&my->m_PropWarningmin,&my->m_PropDangermin,&my->m_PropSavetotmin,&my->m_PropProduct,&my->m_PropRACLL,&my->m_PropRACMM,&my->m_PropRACSS,&my->m_PropSendTime };
	LPCTSTR PropertyData[10][3] = {
								   { _T("APPKEY"), _T("sbkk00010003"), _T("��� ���� �������Դϴ�.")},
								   { _T("��������(��¥)"), _T("2020-02-29 17:02:28"), _T("���� �۵��� �����ϴ� �ð��Դϴ�.")},
								   { _T("���Ⱓ(��)"), _T("7"), _T("��ǰ�� �����ϴ� �Ⱓ�Դϴ�.")},
								   { _T("����Ⱓ(��)"), _T("5"), _T("�������κ��� '����'���°� �Ǳ������ �ð��Դϴ�.")},
								   { _T("�����Ⱓ(��)"), _T("5"), _T("��ü �����Ⱓ�Դϴ�.")},
								   { _T("��ǰ��"), _T("None"), _T("�ڽ� �ӿ� ���� ��ǰ���Դϴ�.")},
								   { _T("RACLL(��)"), _T(""), _T("RAC ��ġ ū L(��)������ ���Դϴ�.")},
								   { _T("RACMM(��)"), _T(""), _T("RAC ��ġ �߰� M(��)������ ���Դϴ�.")},
								   { _T("RACSS(��)"), _T(""), _T("RAC ��ġ ���� L(��)������ ���Դϴ�.")},
								   { _T("�����ֱ�(��)"), _T("60"), _T("�������� �����͸� �����ϴ� �ֱ��Դϴ�. 1�ð� : 60, 12�ð� : 720, �Ϸ� : 1440")}
								  };
	;
	for (int i = 0; i < 10; i++)
	{
		*Propertyarr[i] = new CMFCPropertyGridProperty(PropertyData[i][0], PropertyData[i][1], PropertyData[i][2]);
		my->m_ManualData.AddProperty(*Propertyarr[i]);
	}
}

BOOL CTab1::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetBackgroundColor(RGB(255, 255, 255));

	UpdateVal(this);
	SendVals();
	UpdateVals();

	m_frSerialEdit.EnableWindow(FALSE);
	m_toSerialEdit.EnableWindow(FALSE);
	m_ReplaceUpdateBtn.EnableWindow(FALSE);
	m_SendButton.EnableWindow(FALSE);
	m_frSerialEdit.SetWindowText(_T("sbkkb000000000"));
	m_toSerialEdit.SetWindowText(_T("sbkkb000000000"));
	m_SendButton.SetWindowText(_T("����"));
	UpdateReplacingData();

	m_SensorCnt.SetWindowText(_T("0"));

	m_SensorReplaceChk.SetCheck(FALSE);
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


afx_msg LRESULT CTab1::OnAfxWmPropertyChanged(WPARAM wParam, LPARAM lParam)
{
	SendVals();
	UpdateVals();
	UpdateVals();
	return 0;
}

// MainDlg�� ���� ���
void CTab1::UpdateVals()
{
	//SensorData *ManualData = &p_parent->MannualData;

	m_PropAPPKEY->SetValue(p_parent->MannualData.m_StrAPPKEY);
	m_PropStartDate->SetValue(p_parent->MannualData.m_StrStartDate);
	m_PropWarningmin->SetValue(p_parent->MannualData.m_StrWarningmin);
	m_PropDangermin->SetValue(p_parent->MannualData.m_StrDangermin);
	m_PropSavetotmin->SetValue(p_parent->MannualData.m_StrSavetotmin);
	m_PropProduct->SetValue(p_parent->MannualData.m_StrProduct);
	m_PropRACLL->SetValue(p_parent->MannualData.m_StrRACLL);
	m_PropRACMM->SetValue(p_parent->MannualData.m_StrRACMM);
	m_PropRACSS->SetValue(p_parent->MannualData.m_StrRACSS);
	m_PropSendTime->SetValue(p_parent->MannualData.m_StrSendTime);
	/*
	m_DataAPPKEY->SetValue(p_parent->MannualData.m_StrAPPKEY);
	m_DataDangerTime->SetValue(p_parent->MannualData.m_StrDangerTime);
	m_DataProduct->SetValue(p_parent->MannualData.m_StrProduct);
	m_DataRAC->SetValue(p_parent->MannualData.m_StrRAC);
	m_DataSavingTime->SetValue(p_parent->MannualData.m_StrSavingTime);
	m_DataSendTime->SetValue(p_parent->MannualData.m_StrSendTime);
	m_DataStartDate->SetValue(p_parent->MannualData.m_StrStartDate);
	*/


	//m_ManualData.GetProperty(0)->SetValue(p_parent->MannualData.m_StrProduct);
	//m_ManualData.GetProperty(1)->SetValue(p_parent->MannualData.m_StrStartDate);
	//m_ManualData.GetProperty(2)->SetValue(p_parent->MannualData.m_StrSavingTime);
	//m_ManualData.GetProperty(3)->SetValue(p_parent->MannualData.m_StrDangerTime);
	//m_ManualData.GetProperty(4)->SetValue(p_parent->MannualData.m_StrSendTime);
	//m_ManualData.GetProperty(5)->SetValue(p_parent->MannualData.m_StrRAC);
	//m_ManualData.GetProperty(6)->SetValue(p_parent->MannualData.m_StrAPPKEY);
	//m_ManualData.OnPropertyChanged(m_DataProduct);
}


// MainDlg�� ���尪�� ������Ʈ
void CTab1::SendVals()
{
	SensorData Data = {
		m_PropAPPKEY->GetValue(),
		m_PropStartDate->GetValue(),
		m_PropWarningmin->GetValue(),
		m_PropDangermin->GetValue(),
		m_PropSavetotmin->GetValue(),
		m_PropProduct->GetValue(),
		m_PropRACLL->GetValue(),
		m_PropRACMM->GetValue(),
		m_PropRACSS->GetValue(),
		m_PropSendTime->GetValue(),
		p_parent->MannualData.m_StrRevision,
		_T("1")
	};
	Data.m_isReplace = p_parent->MannualData.m_isReplace;
	p_parent->MannualData = Data;
	/*
	p_parent->MannualData.m_StrAPPKEY = m_DataAPPKEY->GetValue();
	p_parent->MannualData.m_StrDangerTime = m_DataDangerTime->GetValue();
	p_parent->MannualData.m_StrProduct = m_DataProduct->GetValue();
	p_parent->MannualData.m_StrRAC = m_DataRAC->GetValue();
	p_parent->MannualData.m_StrSavingTime = m_DataSavingTime->GetValue();
	p_parent->MannualData.m_StrSendTime = m_DataSendTime->GetValue();
	p_parent->MannualData.m_StrStartDate = m_DataStartDate->GetValue();
	*/
}


void CTab1::OnEnChangeSensorcnt()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString str;
	m_SensorCnt.GetWindowTextW(str);
	p_parent->m_iSensorCnt = _wtoi(str.GetString());
}


void CTab1::OnBnClickedSnreplacechk()
{
	p_parent->m_isSensorReplace = m_SensorReplaceChk.GetCheck();
	if (p_parent->m_isSensorReplace)
	{
		m_frSerialEdit.EnableWindow(TRUE);
		m_toSerialEdit.EnableWindow(TRUE);
		m_ReplaceUpdateBtn.EnableWindow(TRUE);
		UpdateReplacingData();
	}
	else 
	{
		m_frSerialEdit.EnableWindow(FALSE);
		m_toSerialEdit.EnableWindow(FALSE);
		m_ReplaceUpdateBtn.EnableWindow(FALSE);
	}
}


void CTab1::OnBnClickedRepbtn()
{
	UpdateReplacingData();
	SensorDataList sdt;
	if (p_parent->SensorList.Lookup(p_parent->m_frSerialNo, sdt))
	{
		p_parent->SensorDataList2SensorData(sdt, p_parent->MannualData);
	}

	UpdateVals();
}


void CTab1::UpdateReplacingData()
{
	m_frSerialEdit.GetWindowText(p_parent->m_frSerialNo);
	m_toSerialEdit.GetWindowText(p_parent->m_toSerialNo);
}


void CTab1::OnEnChangeFrserialedit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateReplacingData();
	SensorDataList sdt;
	if (p_parent->SensorList.Lookup(p_parent->m_frSerialNo, sdt))
	{
		p_parent->SensorDataList2SensorData(sdt, p_parent->MannualData);
		UpdateVals();
	}
}


void CTab1::OnEnChangeToserialedit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateReplacingData();
}


BOOL CTab1::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return true;
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CTab1::OnBnClickedSend()
{
	p_parent->isFinishedValue = 1;
}
