#pragma once
#include "afxwin.h"


// PathTool dialog

class CPathTool : public CDialog
{
	DECLARE_DYNAMIC(CPathTool)

public:
	CPathTool(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPathTool();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATHTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public: // Message Functions
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();

public: // User Functions
	void SettingHorizontalScrollSize();

public:
	CListBox m_ListBox;

public: // User Variables
	list<PATH_INFO*>	m_PathInfoList;
	afx_msg void OnBnClickedListClear();
};
