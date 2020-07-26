#pragma once
class CMouse
{
	DECLARE_SINGLETON(CMouse)

private:
	CMouse();
	~CMouse();

public:
	void Render();

public:
	D3DXVECTOR3 GetMousePos();
	D3DXVECTOR3 GetWorldMousePos();

public:
	HRESULT Initalize();
	void Release();

public:
	void SetMouseKey(const wstring& wstrMouseKey) { m_wstrMouseKey = wstrMouseKey; }

private:
	wstring m_wstrMouseKey;

	CDeviceMgr* m_pDeviceMgr;
	CTextureMgr* m_pTextureMgr;
};