#pragma once
class CTexture
{
protected:
	CTexture();

public:
	virtual ~CTexture();

public:
	virtual const TEX_INFO* GetTexInfo(const wstring& wstrStateKey = L"", const WORD& wIndex = 0) const PURE;

public:
	virtual HRESULT LoadTexture(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrFilePath, const wstring& wstrStateKey = L"", const WORD& wCnt = 0, const WORD& wStart = 0) PURE;
	virtual void Release() PURE;
};

