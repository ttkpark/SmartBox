#pragma once
#include "SmartBox MasterKey Application.h"
#include "SmartBoxMasterKeyAppDlg.h"
#include "ColoredListCtrl.h"

// CTab1 dialog

class CTab1 : public CDialogEx
{
	DECLARE_DYNAMIC(CTab1)

public:
	CTab1(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CTab1();
	CSmartBoxMasterKeyAppDlg* p_parent;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAB1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	COLORREF* RowTxtColors;
	COLORREF* RowBkgColors;
	int m_nSortColumn;//전에 정렬했던 Column 번호.
	BOOL m_bSortAscending;// 정렬 방식.
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CStatic m_T1Caption;
	afx_msg void OnBnClickedBtntray();
	virtual BOOL OnInitDialog();
	int DataList_Update();
	int SensorDataAddtoList(int Col, SensorDataList& sdt);
	CColoredListCtrl m_List;
	afx_msg void OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnColumnclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedSerach();
	CEdit m_SerachEdit;
	CMap< CString, CString, SensorDataList, SensorDataList&> ListData;
	void ChangeCaption();
	CString GetCaption(int id);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
