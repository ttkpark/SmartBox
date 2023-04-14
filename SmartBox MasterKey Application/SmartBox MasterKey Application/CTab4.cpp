// CTab4.cpp : implementation file
//

#include "pch.h"
#include "SmartBox MasterKey Application.h"
#include "CTab4.h"
#include "afxdialogex.h"


// CTab4 dialog

IMPLEMENT_DYNAMIC(CTab4, CDialogEx)

CTab4::CTab4(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TAB4, pParent)
{
	
}

CTab4::~CTab4()
{
}

void CTab4::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO40, m_Combo40);
	DDX_Control(pDX, IDC_COMBO41, m_Combo41);
	DDX_Control(pDX, IDC_BUTTON40, m_Button40);
	DDX_Control(pDX, IDC_BUTTON41, m_Button41);
}


BEGIN_MESSAGE_MAP(CTab4, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON40, &CTab4::OnBnClickedButton40)
	ON_BN_CLICKED(IDC_BUTTON41, &CTab4::OnBnClickedButton41)
	ON_CBN_SELCHANGE(IDC_COMBO41, &CTab4::OnCbnSelchangeCombo41)
	ON_CBN_EDITCHANGE(IDC_COMBO40, &CTab4::OnCbnEditchangeCombo40)
	ON_CBN_SELCHANGE(IDC_COMBO40, &CTab4::OnCbnSelchangeCombo40)
	ON_BN_CLICKED(IDC_APIRETRY, &CTab4::OnBnClickedApiretry)
END_MESSAGE_MAP()


// CTab4 message handlers


BOOL CTab4::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_RETURN)
		{
			//CString str;
			//GetDlgItem(IDC_LINE)->GetWindowText(str);
			//p_parent->KeyProcess(str);
			//GetDlgItem(IDC_LINE)->SetWindowText(_T(""));
			return true;
		}
		if (pMsg->wParam == VK_ESCAPE)
			return true;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


// //BUTTON41 : ����/���� ��ư
void CTab4::UpdateButton41(int m_connected)
{
	//CONSTVALUE
	CString Data;
	if (m_connected)
		p_parent->GetCaption(5, Data);//_T("����")
	else
		p_parent->GetCaption(4, Data);//_T("���� ����")
		
	m_Button41.SetWindowText(Data);
}

///�ڵ��˻�
void CTab4::OnBnClickedButton40()
{
	p_parent->COMAutoDetect();
}

//����
void CTab4::OnBnClickedButton41()
{
	if (!p_parent->m_COMConnected)
	{
		p_parent->COMConnect();
	}
	else p_parent->COMDisconnect();
}


BOOL CTab4::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetBackgroundColor(RGB(255, 255, 255));

	m_Combo40.ResetContent();
	m_Combo40.SetCurSel(0);

	//language�� ������ combo�� ����Ѵ�.
	m_Combo41.ResetContent();
	CString s_StrN, nowlang;
	int langlen = p_parent->LangTable.GetCount();
	for (int i = 0; i < langlen; i++)
	{
		if(!p_parent->LangTable.Lookup(i, s_StrN))s_StrN = _T("Null Name");
		m_Combo41.AddString(s_StrN);
		if (p_parent->nowLang == i)
			nowlang = s_StrN;
	}

	//Combo���� ���� language�� ��ġ�� focus�� �����.
	m_Combo41.SetCurSel(0);
	for (int i = 0; i < langlen; i++)
	{
		m_Combo41.GetLBText(i, s_StrN);
		if(s_StrN == nowlang)
			m_Combo41.SetCurSel(i);
	}

	updatecaption();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CTab4::OnCbnSelchangeCombo41()
{
	CString s_StrN,combostr;
	int k = m_Combo41.GetCurSel();
	m_Combo41.GetLBText(k, combostr);

	int langlen = p_parent->LangTable.GetCount();
	for (int i = 0; i < langlen; i++)
	{
		if (!p_parent->LangTable.Lookup(i, s_StrN))s_StrN = _T("Null Name");
		if (combostr == s_StrN)
		{
			p_parent->nowLang = i;
			break;
		}
	}

	p_parent->SetLang(p_parent->nowLang);
	p_parent->SystemConst_Store();

	p_parent->ChangeCaption();
}


void CTab4::OnCbnEditchangeCombo40()
{
	m_Combo40.GetWindowText(p_parent->m_COMPort);
}


void CTab4::OnCbnSelchangeCombo40()
{
	int i = m_Combo40.GetCurSel();
	m_Combo40.GetLBText(i, p_parent->m_COMPort);
}


void CTab4::OnBnClickedApiretry()
{
	p_parent->APIInitalUpdate();
	ChangeCaption();
}


int CTab4::updatecaption()
{
	CString str;//���� %d�� ������
	str.Format(GetCaption(0), p_parent->m_ListNumSensor);
	GetDlgItem(IDC_NUMSENSOR)->SetWindowText(str);
	return 0;
}


void CTab4::ChangeCaption()
{
	CString data;

	updatecaption();//���� %d�� ������

	p_parent->GetCaption(1, data);//������Ű ����
	GetDlgItem(IDC_STATIC40)->SetWindowText(data);

	p_parent->GetCaption(2, data);//COM ��Ʈ
	GetDlgItem(IDC_STATIC41)->SetWindowText(data);

	p_parent->GetCaption(3, data);//�ڵ�����
	GetDlgItem(IDC_BUTTON40)->SetWindowText(data);

	p_parent->GetCaption(6, data);//��õ�
	GetDlgItem(IDC_APIRETRY)->SetWindowText(data);

	UpdateButton41(p_parent->m_COMConnected);
}


CString CTab4::GetCaption(int id)
{
	return p_parent->GetCaption(id);
}
//ID0 ���� %d�� ������
//ID1 ������Ű ����
//ID2 COM ��Ʈ
//ID3 �ڵ�����
//ID4 ���� ����
//ID5 ����
//ID6 ��õ�