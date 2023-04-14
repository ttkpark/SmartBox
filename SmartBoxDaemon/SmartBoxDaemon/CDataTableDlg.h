#pragma once

class CSmartBoxDaemonDlg;

struct SensorDataList;
// CDataTableDlg dialog

class CDataTableDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDataTableDlg)

public:
	CDataTableDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDataTableDlg();
	CSmartBoxDaemonDlg* p_parent;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DATATABLE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_MoveCursor;
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	int DataList_Update();
	CListCtrl m_List;
	int SensorDataAddtoList(int Col, SensorDataList& sdt);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};