#pragma once
#include "afxwin.h"


// CMapTool dialog
class CAnimatedObject;
class CNonAnimatedObject;
class CImageObject;
class CToolView;
class CMapTool : public CDialog
{
	DECLARE_DYNAMIC(CMapTool)

public:
	CMapTool(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMapTool();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();

public: // Message Functions
	afx_msg void OnLbnSelchangeObjectKey();
	afx_msg void OnLbnSelchangeStateKey();
	afx_msg void OnLbnSelchangeFileName();
	afx_msg void OnBnClickedAnimated();
	afx_msg void OnBnClickedNonAnimated();
	afx_msg void OnBnClickedAddObject();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnBnClickedOption();
	afx_msg void OnBnClickedImgObjRadio();
	afx_msg void OnBnClickedApply();

public: // User Functions
	void SettingHorizontalScrollSize();
	BOOL RefreshMap();
	int	 GetCurImageObjectType();
	int	 GetCurOptionImageRadio();
	int  GetCurOptionType();
	int  GetCurPushList();
	CAnimatedObject* CreateAnimatedObject(CToolView* pToolView);
	CNonAnimatedObject* CreateNonAnimatedObject(CToolView* pToolView);


public: // Contirl Variables
	CButton m_Radio[4];
	CButton m_TileOptionRadio[14];
	CButton m_RenderTypeRadio[3];
	CListBox m_ObjectList;
	CListBox m_StateList;
	CListBox m_FileList;
	float m_ObjectScaleX;
	float m_ObjectScaleY;

	CStatic m_Picture;

	CButton m_ShowCheck[3];


public: // User Variables
	int m_iOptionID;

	wstring m_wstrCurSelObject;
	wstring m_wstrCurSelState;
	wstring m_wstrCurSelFile;

	CDeviceMgr*		m_pDeviceMgr;
	CTextureMgr*	m_pTextureMgr;

	unordered_map<wstring, FILE_INFO*> m_umapFileInfo;
	unordered_map<wstring, PATH_INFO*> m_umapAnimPathInfo;

	float m_fSort;
	float m_fPositionX;
	float m_fPositionY;
	float m_RotationDegree;
	afx_msg void OnBnClickedCheckTerrain();
	afx_msg void OnBnClickedCheckObject();
	afx_msg void OnBnClickedCheckFrontObject();
};
