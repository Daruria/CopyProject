#pragma once
class CFontMgr
{
	DECLARE_SINGLETON(CFontMgr)

public:
	enum SortX { LEFT, MIDDLE, RIGHT, SORT_X_END };
	enum SortY { TOP, CENTER, BOTTOM, SORT_Y_END };

private:
	CFontMgr();
	~CFontMgr();


public:
	HRESULT Initialize();
	void Render(const D3DXVECTOR3& vPos, const D3DXVECTOR3& vSize, const wstring& wstrText, D3DCOLOR d3dColor, CFontMgr::SortX eSortX = CFontMgr::LEFT, CFontMgr::SortY eSortY = CFontMgr::TOP);
	void Render(const D3DXMATRIX* pMatrix, const D3DXVECTOR3& vSize, const wstring& wstrText, D3DCOLOR d3dColor, CFontMgr::SortX eSortX = CFontMgr::LEFT, CFontMgr::SortY eSortY = CFontMgr::TOP);

private:
	UINT RenderLetter(const D3DXVECTOR3& vPos, const D3DXVECTOR3& vSize, wchar_t wcLetter, D3DCOLOR d3dColor);
	D3DXMATRIX RenderLetterMatrix(D3DXMATRIX* pMatrix, const D3DXVECTOR3& vSize, wchar_t wcLetter, D3DCOLOR d3dColor);

public:
	UINT GetTextSizeX(const D3DXVECTOR3& vSize, const wstring& wstrText);
	UINT GetTextSizeY(const D3DXVECTOR3& vSize, const wstring& wstrText);

private:
	unordered_map<wchar_t, wstring> m_umapFontKeys;

	CDeviceMgr*		m_pDeviceMgr;
	CTextureMgr*	m_pTextureMgr;
};

