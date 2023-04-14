// CTab2.cpp : implementation file
//

#include "pch.h"
#include "SmartBox MasterKey Application.h"
#include "CTab2.h"
#include "afxdialogex.h"


// CTab2 dialog

IMPLEMENT_DYNAMIC(CTab2, CDialogEx)

CTab2::CTab2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TAB2, pParent)
{

}

CTab2::~CTab2()
{
}

void CTab2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT20, m_Edit20);
	DDX_Control(pDX, IDC_EDIT21, m_Edit21);
	DDX_Control(pDX, IDC_EDIT23, m_Edit23);
	DDX_Control(pDX, IDC_EDIT24, m_Edit24);
	DDX_Control(pDX, IDC_EDIT25, m_Edit25);
	DDX_Control(pDX, IDC_EDIT27, m_Edit27);
	DDX_Control(pDX, IDC_EDIT28, m_Edit28);
	DDX_Control(pDX, IDC_EDIT29, m_Edit29);
	DDX_Control(pDX, IDC_BUTTON2A, m_Button2A);
	DDX_Control(pDX, IDC_COMBO2, m_Combo2);
	DDX_Control(pDX, IDC_COMBO3, m_Combo3);
	DDX_Control(pDX, IDC_COMBO4, m_Combo4);
	DDX_Control(pDX, IDC_COMBO22, m_Combo22);
}


BEGIN_MESSAGE_MAP(CTab2, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CTab2::OnCbnSelchangeCombo2)
	ON_BN_CLICKED(IDC_BUTTON2A, &CTab2::OnBnClickedButton2a)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CTab2::OnCbnSelchangeCombo3)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CTab2::OnCbnSelchangeCombo4)
	ON_EN_CHANGE(IDC_EDIT23, &CTab2::OnEnChangeEdit23)
	ON_EN_CHANGE(IDC_EDIT24, &CTab2::OnEnChangeEdit24)
	ON_EN_CHANGE(IDC_EDIT25, &CTab2::OnEnChangeEdit25)
END_MESSAGE_MAP()


// CTab2 message handlers


BOOL CTab2::PreTranslateMessage(MSG* pMsg)
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

void CTab2::OnBnClickedButton2a()
{
	//TODO : ��Ų ����ó�� ó����ƾ �ڵ��ϱ�
	p_parent->isFinishedValue = true;
}

//ID27~
BOOL CTab2::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetBackgroundColor(RGB(255, 255, 255));

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
uint64_t CombotimetoMin(uint64_t i, uint64_t time)
{
	if (i == 0)		//Day -> min
		return time * (60 * 24);
	else if (i == 1)//Hour -> min
		return time * 60;
	else if (i == 2)//Hour -> min
		return time;
	return 0;
}
uint64_t MintoCombotime(uint64_t i, uint64_t Min)
{
	if (i == 0)		//Min -> day
		return Min / (60 * 24);
	else if (i == 1)//Min -> Hour
		return Min / 60;
	else if (i == 2)//Min -> min
		return Min;
	return 0;
}


int CTab2::SensorAccessed()
{
	p_parent->AccessData;
	CString str;

	m_Edit20.SetWindowText(p_parent->m_AccessedSerial);
	m_Edit21.SetWindowText(p_parent->AccessData.m_BoxNum);

	m_Combo22.ResetContent();// ��ǰ�� �ĺ��ڽ��� ����.
	//int len = p_parent->Goods.GetCount();//������ ��ǰ��(Goods)�� ������ ���
	//for (int i = 0; i < len; i++)// �� ������ŭ �ĺ��ڽ��� �ڸ��� �����.
	//	m_Combo22.AddString(_T(""));

	POSITION pos = p_parent->Goods.GetStartPosition();// ��ǰ�� ����Ʈ(Goods)���� �ϳ��� �о�´�.
	int i = 0;
	while (pos != NULL)
	{
		p_parent->Goods.GetNextAssoc(pos, i, str);// ��ġ(i)�� ����(str)�� �ĺ��ڽ��� ����Ѵ�.
		m_Combo22.AddString(str);
	}
	m_Combo22.SetCurSel(0);
	


	//m_Edit22.SetWindowText(p_parent->AccessData.m_StrProduct);
	
	str.Format(_T("%d"), MintoCombotime(m_Combo2.GetCurSel(), _wtoi(p_parent->AccessData.m_StrSavetotmin)));
	m_Edit23.SetWindowText(str);
	str.Format(_T("%d"), MintoCombotime(m_Combo3.GetCurSel(), _wtoi(p_parent->AccessData.m_StrWarningmin)));
	m_Edit24.SetWindowText(str);
	str.Format(_T("%d"), MintoCombotime(m_Combo4.GetCurSel(), _wtoi(p_parent->AccessData.m_StrDangermin)));
	m_Edit25.SetWindowText(str);

	//m_Edit26.SetWindowText(p_parent->AccessData.work_zone_num);

	m_Edit27.SetWindowText(p_parent->AccessData.m_StrRACLL);
	m_Edit28.SetWindowText(p_parent->AccessData.m_StrRACMM);
	m_Edit29.SetWindowText(p_parent->AccessData.m_StrRACSS);

	return 0;
}


int CTab2::AccessdatOut()
{

	CString str;

	m_Edit20.GetWindowText(p_parent->m_AccessedSerial);
	m_Edit21.GetWindowText(p_parent->AccessData.m_BoxNum);
	

	POSITION pos = p_parent->Goods.GetStartPosition();// ��ǰ�� ����Ʈ(Goods)���� �ϳ��� �о�´�.
	int i = 0;
	CString s_combo22;
	m_Combo22.GetLBText(m_Combo22.GetCurSel(), s_combo22);//�ĺ��ڽ��� ���ڿ��� �����´�.
	while (pos != NULL)
	{
		p_parent->Goods.GetNextAssoc(pos, i, str);// ��ġ(i)�� ����(str)�� ���Ѵ�.
		if (str == s_combo22)break;//��, s_combo�� �̿��ؼ� i(idxGoods)�� ���ڴ� ���̴�.
	}
	if (str != s_combo22)i = 0;
	p_parent->AccessData.m_StrProduct.Format(_T("%d"), i);

	m_Edit23.GetWindowText(str);
	p_parent->AccessData.m_StrSavetotmin.Format(_T("%d"), CombotimetoMin(m_Combo2.GetCurSel(), _wtoi(str)));
	m_Edit24.GetWindowText(str);
	p_parent->AccessData.m_StrWarningmin.Format(_T("%d"), CombotimetoMin(m_Combo3.GetCurSel(), _wtoi(str)));
	m_Edit25.GetWindowText(str);
	p_parent->AccessData.m_StrDangermin.Format(_T("%d"), CombotimetoMin(m_Combo4.GetCurSel(), _wtoi(str)));

	//m_Edit26.GetWindowText(p_parent->AccessData.work_zone_num);

	m_Edit27.GetWindowText(p_parent->AccessData.m_StrRACLL);
	m_Edit28.GetWindowText(p_parent->AccessData.m_StrRACMM);
	m_Edit29.GetWindowText(p_parent->AccessData.m_StrRACSS);

	return 0;
}


void CTab2::OnCbnSelchangeCombo2()
{
	CString str;
	str.Format(_T("%llu"), MintoCombotime(m_Combo2.GetCurSel(), _wtoi64(p_parent->AccessData.m_StrSavetotmin)));
	m_Edit23.SetWindowText(str);
}


void CTab2::OnCbnSelchangeCombo3()
{
	CString str;
	str.Format(_T("%llu"), MintoCombotime(m_Combo3.GetCurSel(), _wtoi64(p_parent->AccessData.m_StrWarningmin)));
	m_Edit24.SetWindowText(str);
}


void CTab2::OnCbnSelchangeCombo4()
{
	CString str;
	str.Format(_T("%llu"), MintoCombotime(m_Combo4.GetCurSel(), _wtoi64(p_parent->AccessData.m_StrDangermin)));
	m_Edit25.SetWindowText(str);
}


void CTab2::OnEnChangeEdit23()
{
	CString str;
	m_Edit23.GetWindowText(str);
	p_parent->AccessData.m_StrSavetotmin.Format(_T("%llu"), CombotimetoMin(m_Combo2.GetCurSel(), _wtoi64(str)));
}


void CTab2::OnEnChangeEdit24()
{
	CString str;
	m_Edit24.GetWindowText(str);
	p_parent->AccessData.m_StrWarningmin.Format(_T("%llu"), CombotimetoMin(m_Combo3.GetCurSel(), _wtoi64(str)));
}


void CTab2::OnEnChangeEdit25()
{
	CString str;
	m_Edit25.GetWindowText(str);
	p_parent->AccessData.m_StrDangermin.Format(_T("%llu"), CombotimetoMin(m_Combo4.GetCurSel(), _wtoi64(str)));
}


void CTab2::ChangeCaption()
{
	GetDlgItem(IDC_STATIC20)->SetWindowText(GetCaption(31));//ID31 ��ġ��ȣ
	GetDlgItem(IDC_STATIC21)->SetWindowText(GetCaption(32));//ID32 BOX��ȣ
	GetDlgItem(IDC_STATIC22)->SetWindowText(GetCaption(33));//ID33 ��ǰ��
	GetDlgItem(IDC_STATIC23)->SetWindowText(GetCaption(34));//ID34 �����ð�
	GetDlgItem(IDC_STATIC24)->SetWindowText(GetCaption(35));//ID35 ���ð�
	GetDlgItem(IDC_STATIC25)->SetWindowText(GetCaption(36));//ID36 ����ð�
	GetDlgItem(IDC_STATIC26)->SetWindowText(GetCaption(37));//ID37 RAC NO
	GetDlgItem(IDC_STATIC27)->SetWindowText(GetCaption(38));//ID38 (��)
	GetDlgItem(IDC_STATIC28)->SetWindowText(GetCaption(39));//ID39 (��)
	GetDlgItem(IDC_STATIC29)->SetWindowText(GetCaption(40));//ID40 (��)
	GetDlgItem(IDC_BUTTON2A)->SetWindowText(GetCaption(41));//ID41 ���

	CString str;
	POSITION pos = p_parent->Goods.GetStartPosition();// ��ǰ�� ����Ʈ(Goods)���� �ϳ��� �о�´�.
	int i = 0;
	while (pos != NULL)
	{
		p_parent->Goods.GetNextAssoc(pos, i, str);// ��ġ(i)�� ����(str)�� �ĺ��ڽ��� ����Ѵ�.
		m_Combo22.AddString(str);
	}

	CComboBox* p[3] = { &m_Combo2 ,&m_Combo3 ,&m_Combo4 };
	for (int i = 0; i < sizeof(p) / sizeof(*p); i++)
	{
		int a = p[i]->GetCurSel();
		if (a == -1)a = 0;
		p[i]->ResetContent();
		p[i]->AddString(GetCaption(28));//ID28 ��
		p[i]->AddString(GetCaption(29));//ID29 �ð�
		p[i]->AddString(GetCaption(30));//ID30 ��
		p[i]->SetCurSel(a);
	}
}


CString CTab2::GetCaption(int id)
{
	return p_parent->GetCaption(id);
}

//ID28 ��
//ID29 �ð�
//ID30 ��
//ID31 ��ġ��ȣ
//ID32 BOX��ȣ
//ID33 ��ǰ��
//ID34 �����ð�
//ID35 ���ð�
//ID36 ����ð�
//ID37 RAC NO
//ID38 (��)
//ID39 (��)
//ID40 (��)