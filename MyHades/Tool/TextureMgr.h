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
	int GetTextureMaxFrame(const wstring& wstrObjectKey, const wstring& wstrStateKey) const;
	
public:
	HRESULT LoadTexture(LPDIRECT3DDEVICE9 pGraphicDev, TEXTURE_TYPE eTextureType, const wstring& wstrFilePath, const wstring& wstrObjectKey,
		const wstring& wstrStateKey = L"", const WORD& wCnt = 0, const WORD& wStart = 0);

public:
	HRESULT ReleaseTexture(TEXTURE_TYPE eTextureType, const wstring& wstrObjectKey, const wstring& wstrStateKey = L"");

private:
	void Release();

private:
	unordered_map<wstring, CTexture*> m_umapTexture;

};

