#pragma once

class CTexture;
class CTextureMgr
{
	DECLARE_SINGLETON(CTextureMgr)

private:
	CTextureMgr();
	~CTextureMgr();

public:
	const TEX_INFO* GetTexInfo(const wstring& wstrObjectKey, const wstring& wstrStateKey = L"", const WORD wIndex = 0)const;
	CTexture* GetTexture(const wstring& wstrObjectKey);
	const int GetTextureMaxFrame(const wstring& wstrObjectKey, const wstring& wstrStateKey);
	const float GetLoadingRate() const { return m_fLoadingRate; }
	const bool	GetLoadinSuccess() const { return m_bLoadingSuccess; }
	
public:
	void SetLoadingRate(float fLoadingRate) { m_fLoadingRate = fLoadingRate; }
	void SetLoadingSuccess(bool bSuccess) { m_bLoadingSuccess = bSuccess; }

public:
	HRESULT LoadTextureFromPathInfo(LPDIRECT3DDEVICE9 pGrahicDev, const wstring& wstrFilePath);
	HRESULT LoadTexture(LPDIRECT3DDEVICE9 pGraphicDev, TEXTURE_TYPE eTextureType, const wstring& wstrFilePath, const wstring& wstrObjectKey,
		const wstring& wstrStateKey = L"", const WORD& wCnt = 0, const WORD& wStart = 0);

public:
	HRESULT ReleaseTexture(TEXTURE_TYPE eTextureType, const wstring& wstrObjectKey, const wstring& wstrStateKey = L"");

private:
	void Release();

private:
	unordered_map<wstring, CTexture*> m_umapTexture;

	float	m_fLoadingRate;
	bool	m_bLoadingSuccess;
};

