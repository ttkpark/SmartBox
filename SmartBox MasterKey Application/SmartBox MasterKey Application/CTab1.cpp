// CTab1.cpp : implementation file
//

#include "pch.h"
#include "SmartBox MasterKey Application.h"
#include "CTab1.h"
#include "CTab3.h"
#include "afxdialogex.h"
#include "ColoredListCtrl.h"

CTab1 *pTab1;
void OnList1DrawColor(LPNMLVCUSTOMDRAW lplvcd);
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
	DDX_Control(pDX, IDC_T1CAP, m_T1Caption);
	DDX_Control(pDX, IDC_LIST1, (CListCtrl&)m_List);
	DDX_Control(pDX, IDC_FINDEDIT, m_SerachEdit);
}


BEGIN_MESSAGE_MAP(CTab1, CDialogEx)
	ON_BN_CLICKED(IDC_BTNTRAY, &CTab1::OnBnClickedBtntray)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CTab1::OnNMDblclkList1)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CTab1::OnLvnColumnclickList1)
	ON_BN_CLICKED(IDC_SERACH, &CTab1::OnBnClickedSerach)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CTab1 message handlers


BOOL CTab1::PreTranslateMessage(MSG* pMsg)
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


void CTab1::OnBnClickedBtntray()
{
	p_parent->TrayButtonEvent();
}



BOOL CTab1::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetBackgroundColor(RGB(255, 255, 255));
	
	pTab1 = this;
	m_List.OnRowDraw = OnList1DrawColor;
	ListData.InitHashTable(10007);

	p_parent->SensorList;
	RowTxtColors = 0;
	RowBkgColors = 0;

	//CONSTVALUE
	m_List.InsertColumn(0, _T(""), LVCFMT_LEFT, 125);//ID7 SerialNum
	m_List.InsertColumn(1, _T(""), LVCFMT_LEFT, 100);//8 상품명
	m_List.InsertColumn(2, _T(""), LVCFMT_LEFT, 80);//9 현재상태
	m_List.InsertColumn(3, _T(""), LVCFMT_LEFT, 155);//10 시작일자
	m_List.InsertColumn(4, _T(""), LVCFMT_LEFT, 80);//11 경고시간
	m_List.InsertColumn(5, _T(""), LVCFMT_LEFT, 80);//12 위험시간
	m_List.InsertColumn(6, _T(""), LVCFMT_LEFT, 80);//13 보관시간
	m_List.InsertColumn(7, _T(""), LVCFMT_LEFT, 63);//14 RACLL
	m_List.InsertColumn(8, _T(""), LVCFMT_LEFT, 70);//15 RACMM 
	m_List.InsertColumn(9, _T(""), LVCFMT_LEFT, 62);//16 RACSS 
	m_List.InsertColumn(10, _T(""), LVCFMT_LEFT, 91);//17 BoxCode
	m_List.InsertColumn(11, _T(""), LVCFMT_LEFT, 100);//18 Work-Zone

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

CString MintoHourCStr(CString Min)
{
	int i = _wtoi(Min);

	Min.Format(pTab1->GetCaption(20), i / 60);//20  %d시간

	return Min;
}

int CTab1::DataList_Update()
{
	if (!this)return 0;
	CString strKey;
	SensorDataList sdt;
	POSITION pos = p_parent->SensorList.GetStartPosition();
	int i = 0;
	m_List.DeleteAllItems();
	ListData.RemoveAll();
	while (pos != NULL)
	{
		p_parent->SensorList.GetNextAssoc(pos, strKey, sdt);
		if (p_parent->isSensorWorking(sdt))
		{

			CString statusText;
			if (sdt.alertstatus == _T("n"))statusText = GetCaption(21);//21 정상
			else if (sdt.alertstatus == _T("w"))statusText = GetCaption(22);//22 경고
			else if (sdt.alertstatus == _T("d"))statusText = GetCaption(23);//23 위험
			else if (sdt.alertstatus == _T("o"))statusText = GetCaption(24);//24 만기
			else								statusText = _T("");
			sdt.alertstatus = statusText;

			sdt.idxGoods;
			CString GoodsName;
			if (p_parent->Goods.Lookup(_wtoi(sdt.idxGoods), GoodsName))
				sdt.idxGoods = GoodsName;

			sdt.warningmin = MintoHourCStr(sdt.warningmin);
			sdt.dangermin  = MintoHourCStr(sdt.dangermin);
			sdt.savetotmin = MintoHourCStr(sdt.savetotmin);
			
			sdt.work_zone_name =sdt.work_zone_name + _T("(") + sdt.work_zone_num + _T(")");

			ListData.SetAt(strKey, sdt);
			SensorDataAddtoList(i++, sdt);
		}
	}

	return 0;
}

int CTab1::SensorDataAddtoList(int Col, SensorDataList& sdt)
{
	CString str;
	int index = m_List.InsertItem(Col, sdt.serialno, 0);
	//n:일반:검정  w:경고:노랑or주황 d:위험:빨강 o:만기:회색
	m_List.SetItemText(Col, 1, sdt.idxGoods);

	//m_List.SetItemState(Col, LVIS_SELECTED, LVIS_SELECTED);

	m_List.SetItemText(Col, 2, sdt.alertstatus);
	m_List.SetItemText(Col, 3, sdt.StartDate);

	m_List.SetItemText(Col, 4, sdt.warningmin);
	m_List.SetItemText(Col, 5, sdt.dangermin);
	m_List.SetItemText(Col, 6, sdt.savetotmin);

	m_List.SetItemText(Col, 7, sdt.RACLL);
	m_List.SetItemText(Col, 8, sdt.RACMM);
	m_List.SetItemText(Col, 9, sdt.RACSS);
	m_List.SetItemText(Col, 10, sdt.BoxNum);
	m_List.SetItemText(Col, 11, sdt.work_zone_name);
	
	return 0;
}


void CTab1::OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	if (pNMItemActivate->iItem != -1)
	{
		p_parent->m_frSerialNo = m_List.GetItemText(pNMItemActivate->iItem, 0);


		SensorDataList sdt;
		POSITION pos = p_parent->SensorList.GetStartPosition();
		int i = 0;
		if (pos == NULL)//Error!!
		{
			//CONSTVALUE
			AfxMessageBox(GetCaption(25));//ID25 Invaild Sensor.
			return;
		}
		while (pos != NULL)
		{
			p_parent->SensorList.GetNextAssoc(pos, sdt.serialno, sdt);
			if (p_parent->m_frSerialNo == sdt.serialno && p_parent->isSensorWorking(sdt))
			{
				p_parent->GetAPISDI(sdt.serialno, sdt);//교체할 센서에 대한 모든 정보 얻기
				p_parent->SensorDataList2SensorData(sdt, p_parent->replaceData);//replaceData의 부족한 정보를 업데이트한다.
				break;
			}
		}
		p_parent->pTab3->ReplaceDBClicked();

		p_parent->m_Tab.SetCurSel(2);//to TAB3(to go to the replace tab.)
		LRESULT k = 0;
		p_parent->OnTcnSelchangeTab1(0, &k);

	}
	*pResult = 0;
}

void OnList1DrawColor(LPNMLVCUSTOMDRAW lplvcd)
{
	int iRow = lplvcd->nmcd.dwItemSpec;
	CString text;
	//text.Format(_T("%d"), iRow);
	//AfxMessageBox(text);
	CString name;
	SensorDataList sdt;
	name = pTab1->m_List.GetItemText(iRow, 0);

	pTab1->p_parent->SensorList.Lookup(name, sdt);

	if (sdt.alertstatus == _T("n"))
	{
		lplvcd->clrText = RGB(0, 0, 0);
		lplvcd->clrTextBk = RGB(255, 255, 255);
	}
	else if (sdt.alertstatus == _T("w"))
	{
		lplvcd->clrText = RGB(0, 0, 0);
		lplvcd->clrTextBk = RGB(192, 192, 63);
	}
	else if (sdt.alertstatus == _T("d"))
	{
		lplvcd->clrText = RGB(0, 0, 0);
		lplvcd->clrTextBk = RGB(255, 0, 0);
	}
	else if (sdt.alertstatus == _T("o"))
	{
		lplvcd->clrText = RGB(255, 255, 255);
		lplvcd->clrTextBk = RGB(100, 100, 100);
	}
	else
	{
		lplvcd->clrText = RGB(0, 0, 0);
		lplvcd->clrTextBk = RGB(255, 255, 255);
	}


	//lplvcd->clrText = pTab1->RowTxtColors[iRow];
	//lplvcd->clrTextBk = pTab1->RowBkgColors[iRow];
}
static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParam)
{
	CString* pStr1 = (CString*)lParam1;// 비교할 두 문자열. CString*로 변환한다.
	CString* pStr2 = (CString*)lParam2;
	BOOL bSortAscending = (BOOL)lParam;//정렬 방식

	int result = StrCmp(*pStr1,*pStr2);
	if (bSortAscending == TRUE)// 오름차순이면 그냥 Return
		return result;
	else // 내림차순이면 반대로 리턴
		return -result;
}

void CTab1::OnLvnColumnclickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int iColumn = pNMLV->iSubItem; // Coulmn 번호.

	if (iColumn == m_nSortColumn)  // 두 번째 눌렸다면 정렬 상태 반전
		 m_bSortAscending = !m_bSortAscending;
	else m_bSortAscending = TRUE;  // 다른 Column이면 오름차순 정렬

	m_nSortColumn = iColumn;       // 정렬한 Column을 멤버변수로 저장

	int nItem = m_List.GetItemCount();
	CString** arStr = new CString * [nItem];
	for (int i = 0; i < nItem; i++)
	{							   //문자열에 각 Item의 내용을 넣는다.
		arStr[i] = new CString(m_List.GetItemText(i, iColumn));
		m_List.SetItemData(i, (LPARAM)arStr[i]);//ItemData에 넣는다.
	}

	// 첫번째 인수는 위에서 만든 함수
	// 두번째 인수는 정렬 방식
	m_List.SortItems(CompareFunc, (LPARAM)m_bSortAscending);//정렬

	for (int i = 0; i < nItem; i++)
		delete arStr[i];
	delete []arStr;
	
	*pResult = 0;
}

int isStrInSensorDataList(CString str,SensorDataList& sdt)
{
	if ((bool)StrStr(sdt.serialno, str) ||
		(bool)StrStr(sdt.alertstatus, str) ||
		(bool)StrStr(sdt.StartDate, str) ||
		(bool)StrStr(sdt.savetotmin, str) ||
		(bool)StrStr(sdt.warningmin, str) ||
		(bool)StrStr(sdt.dangermin, str) ||
		(bool)StrStr(sdt.idxGoods, str) ||
		(bool)StrStr(sdt.RACLL, str) ||
		(bool)StrStr(sdt.RACMM, str) ||
		(bool)StrStr(sdt.RACSS, str) ||
		(bool)StrStr(sdt.SendTimeSec, str) ||
		(bool)StrStr(sdt.BoxNum, str) ||
		(bool)StrStr(sdt.work_zone_name, str)
		)
		return 1;
	else return 0;
}

void CTab1::OnBnClickedSerach()
{
	CString strKey,Search;
	SensorDataList sdt;
	int i = 0, flag;
	m_List.DeleteAllItems();// 센서 출력 리스트 비우기
	
	POSITION pos = ListData.GetStartPosition();//센서 정보 딕셔너리 준비

	m_SerachEdit.GetWindowText(Search);// 검색조건에 해당하는 Serach 문자열 가져오기

	if (Search == _T(""))//검색조건이 없으면
		flag = TRUE;// 전부 출력 (조건문 | True = True)
	else
		flag = FALSE;// 입력한 문자열을 검색하여 출력
	
	while (pos != NULL)
	{
		ListData.GetNextAssoc(pos, strKey, sdt);// 딕셔너리 다음 데이터 불러오기

		if (flag | isStrInSensorDataList(Search,sdt))// 데이터속에 Search의 문자열이 포함되면 출력
			SensorDataAddtoList(i++, sdt);//출력!
	}
}


void CTab1::ChangeCaption()
{
	DataList_Update();
	
	LVCOLUMNW lvCol;
	::ZeroMemory((void*)&lvCol, sizeof(lvCol));

	CString caption;
	lvCol.mask = LVCF_TEXT;
	caption = GetCaption( 7); lvCol.pszText = (LPWSTR)caption.GetBuffer(); m_List.SetColumn( 0, &lvCol);//ID7 SerialNum
	caption = GetCaption( 8); lvCol.pszText = (LPWSTR)caption.GetBuffer(); m_List.SetColumn( 1, &lvCol);//ID8 상품명
	caption = GetCaption( 9); lvCol.pszText = (LPWSTR)caption.GetBuffer(); m_List.SetColumn( 2, &lvCol);//ID9 현재상태
	caption = GetCaption(10); lvCol.pszText = (LPWSTR)caption.GetBuffer(); m_List.SetColumn( 3, &lvCol);//ID10 시작일자
	caption = GetCaption(11); lvCol.pszText = (LPWSTR)caption.GetBuffer(); m_List.SetColumn( 4, &lvCol);//ID11 경고시간
	caption = GetCaption(12); lvCol.pszText = (LPWSTR)caption.GetBuffer(); m_List.SetColumn( 5, &lvCol);//ID12 위험시간
	caption = GetCaption(13); lvCol.pszText = (LPWSTR)caption.GetBuffer(); m_List.SetColumn( 6, &lvCol);//ID13 보관시간
	caption = GetCaption(14); lvCol.pszText = (LPWSTR)caption.GetBuffer(); m_List.SetColumn( 7, &lvCol);//ID14 RACLL
	caption = GetCaption(15); lvCol.pszText = (LPWSTR)caption.GetBuffer(); m_List.SetColumn( 8, &lvCol);//ID15 RACMM
	caption = GetCaption(16); lvCol.pszText = (LPWSTR)caption.GetBuffer(); m_List.SetColumn( 9, &lvCol);//ID16 RACSS
	caption = GetCaption(17); lvCol.pszText = (LPWSTR)caption.GetBuffer(); m_List.SetColumn(10, &lvCol);//ID17 BoxCode
	caption = GetCaption(18); lvCol.pszText = (LPWSTR)caption.GetBuffer(); m_List.SetColumn(11, &lvCol);//ID18 Work-Zone
	GetDlgItem(IDC_T1CAP)->SetWindowText(GetCaption(19));//19  ※교체할 센서를 더블클릭하세요.
	
	GetDlgItem(IDC_BTNTRAY)->SetWindowText(GetCaption(26));//ID26 Hide
	GetDlgItem(IDC_STATIC10)->SetWindowText(GetCaption(27));//ID27 조회조건
	GetDlgItem(IDC_SERACH)->SetWindowText(GetCaption(43));//43 검색

}


CString CTab1::GetCaption(int id)
{
	return p_parent->GetCaption(id);
}

HBRUSH CTab1::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID() == IDC_T1CAP)
	{
		pDC->SetTextColor(RGB(255, 0, 0));
	}
	
	return hbr;
}

//ID7 SerialNum
//ID8 상품명
//ID9 현재상태
//ID10 시작일자
//ID11 경고시간
//ID12 위험시간
//ID13 보관시간
//ID14 RACLL
//ID15 RACMM 
//ID16 RACSS 
//ID17 BoxCode
//ID18 Work-Zone
//ID19  ※교체할 센서를 더블클릭하세요.
//ID20  %d시간
//ID21 정상
//ID22 경고
//ID23 위험
//ID24 만기
//ID25 Invaild Sensor.
//ID26 Hide
//ID27 조회조건
