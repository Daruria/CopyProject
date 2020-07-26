#pragma once
#include "afxwin.h"


// CObjectTool dialog

class CObjectTool : public CDialog
{
	DECLARE_DYNAMIC(CObjectTool)

public:
	CObjectTool(CWnd* pParent = NULL);   // standard constructor
	virtual ~CObjectTool();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OBJECTTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	CListBox m_ObjList;

	float m_PosX;
	float m_PosY;
	float m_PosZ;
	float m_RotX;
	float m_RotY;
	float m_RotZ;
	float m_ScaleX;
	float m_ScaleY;
	float m_ScaleZ;

	CButton m_PositionRadio;
	CButton m_RotationRadio;
	CButton m_ScaleRadio;

public:
	LPDIRECT3DDEVICE9		m_pGraphicDev = nullptr;
	map<wstring, wstring>	m_mapFilePath;
	CButton m_WireFrame;
	//list<pair<wstring, wstring> > m_FilePathAndName;

private:
	void SaveData(HANDLE hFile);
	void LoadData(HANDLE hFile);

public://	Message Functions
	afx_msg void OnBnClickedAddObject();
	afx_msg void OnBnClickedApply();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnBnClickedWireFrameOnOff();
};
