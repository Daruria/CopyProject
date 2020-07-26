#pragma once
#include "afxwin.h"


// CColliderTool dialog

using namespace Engine;

class CColliderTool : public CDialog
{
	DECLARE_DYNAMIC(CColliderTool)

public:
	CColliderTool(CWnd* pParent = NULL);   // standard constructor
	virtual ~CColliderTool();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COLLIDERTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	LPDIRECT3DDEVICE9		m_pGraphicDev = nullptr;
	CListBox				m_MeshList;
	CListBox				m_BoneList;
	CButton					m_SphereRadio;
	CButton					m_BoxRadio;

	map<wstring, pair<int, wstring> >	m_mapFileInfo;
	vector<D3DXFRAME_DERIVED*>			m_vecFrames;
	vector<CGameObject*>				m_vecGameObject;

public:
	MESHTYPE	Find_MeshType(const wstring& wstrFullPath, const wstring& wstrRelative);
	void		Refresh_BonesData(const wstring& wstrFileName);
	void		Render_ColliderTool();

	afx_msg void OnDropFiles(HDROP hDropInfo);
	

	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeMeshList();
	afx_msg void OnLbnSelchangeBoneList();
	afx_msg void OnBnClickedBoneLoad();
	afx_msg void OnBnClickedPutMeshOnScreen();
	afx_msg void OnBnClickedAddCollider();
	CButton m_WireFrame;
	afx_msg void OnBnClickedWireFrame();
};
