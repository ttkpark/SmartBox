// CTab3.cpp : implementation file
//

#include "pch.h"
#include "SmartBox MasterKey Application.h"
#include "CTab3.h"
#include "afxdialogex.h"


// CTab3 dialog

IMPLEMENT_DYNAMIC(CTab3, CDialogEx)

CTab3::CTab3(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TAB3, pParent)
{

}

CTab3::~CTab3()
{
}

void CTab3::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT30, m_Edit30);
	DDX_Control(pDX, IDC_EDIT31, m_Edit31);
	DDX_Control(pDX, IDC_EDIT32, m_Edit32);
	DDX_Control(pDX, IDC_EDIT33, m_Edit33);
	DDX_Control(pDX, IDC_EDIT34, m_Edit34);
	DDX_Control(pDX, IDC_EDIT35, m_Edit35);
	DDX_Control(pDX, IDC_EDIT37, m_Edit37);
	DDX_Control(pDX, IDC_EDIT38, m_Edit38);
	DDX_Control(pDX, IDC_EDIT39, m_Edit39);
	DDX_Control(pDX, IDC_EDIT3A, m_Edit3A);
	DDX_Control(pDX, IDC_BUTTON3A, m_Button3A);
}


BEGIN_MESSAGE_MAP(CTab3, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON3A, &CTab3::OnBnClickedButton3a)
END_MESSAGE_MAP()


// CTab3 message handlers


BOOL CTab3::PreTranslateMessage(MSG* pMsg)
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


void CTab3::OnBnClickedButton3a()
{
	if (!p_parent->m_isSensorReplace)return;
	//it only works in REPLACE MODE!!

	CString str;
	m_Edit3A.GetWindowText(str); 
	if (str == _T(""))
	{
		AfxMessageBox(GetCaption(53));//ID53 새로운 장치 번호를 입력하세요.
		return;
	}
	
	p_parent->isFinishedValue = true;
	// TODO: Add your control notification handler code here
}


BOOL CTab3::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetBackgroundColor(RGB(255, 255, 255));

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


int CTab3::ValUpdate()
{
	m_Edit3A.GetWindowText(p_parent->m_toSerialNo);
	return 0;
}


int CTab3::ReplaceDBClicked()
{
	p_parent->replaceData;

	m_Edit30.SetWindowText(p_parent->m_frSerialNo);
	m_Edit31.SetWindowText(p_parent->replaceData.m_BoxNum);

	CString GoodsName;
	if (p_parent->Goods.Lookup(_wtoi(p_parent->replaceData.m_StrProduct), GoodsName))
		;
	else GoodsName = p_parent->replaceData.m_StrProduct;

	m_Edit32.SetWindowText(GoodsName);

	m_Edit33.SetWindowText(p_parent->replaceData.m_StrSavetotmin);
	m_Edit34.SetWindowText(p_parent->replaceData.m_StrWarningmin);
	m_Edit35.SetWindowText(p_parent->replaceData.m_StrDangermin);
	m_Edit37.SetWindowText(p_parent->replaceData.m_StrRACLL);
	m_Edit38.SetWindowText(p_parent->replaceData.m_StrRACMM);
	m_Edit39.SetWindowText(p_parent->replaceData.m_StrRACSS);

	return 0;
}


void CTab3::ChangeCaption()
{
	GetDlgItem(IDC_STATIC30)->SetWindowText(GetCaption(31));//ID31 장치번호
	GetDlgItem(IDC_STATIC31)->SetWindowText(GetCaption(32));//ID32 BOX번호
	GetDlgItem(IDC_STATIC32)->SetWindowText(GetCaption(33));//ID33 상품명
	GetDlgItem(IDC_STATIC33)->SetWindowText(GetCaption(34));//ID34 보관시간
	GetDlgItem(IDC_STATIC34)->SetWindowText(GetCaption(35));//ID35 경고시간
	GetDlgItem(IDC_STATIC35)->SetWindowText(GetCaption(36));//ID36 위험시간
	GetDlgItem(IDC_STATIC36)->SetWindowText(GetCaption(37));//ID37 RAC NO
	GetDlgItem(IDC_STATIC37)->SetWindowText(GetCaption(38));//ID38 (대)
	GetDlgItem(IDC_STATIC38)->SetWindowText(GetCaption(39));//ID39 (중)
	GetDlgItem(IDC_STATIC39)->SetWindowText(GetCaption(40));//ID40 (소)
	GetDlgItem(IDC_BUTTON3A)->SetWindowText(GetCaption(64));//ID64 교체
	GetDlgItem(IDC_STATIC3B)->SetWindowText(GetCaption(42));//ID42 덮어 쓸 센서 
}


CString CTab3::GetCaption(int id)
{
	return p_parent->GetCaption(id);
}

//ID53 새로운 장치 번호를 입력하세요.