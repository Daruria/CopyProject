#pragma once
#include "Texture.h"
class CMultiTexture :
	public CTexture
{
private:
	CMultiTexture();

public:
	virtual ~CMultiTexture();

public:
	// Inherited via CTexture
	virtual const TEX_INFO * GetTexInfo(const wstring & wstrStateKey = L"", const WORD & wIndex = 0) const override;
	virtual HRESULT LoadTexture(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrFilePath, const wstring & wstrStateKey = L"", const WORD & wCnt = 0, const WORD& wStart = 0) override;
	virtual void Release() override;

public:
	static CMultiTexture* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrFilePath, const wstring& wstrStateKey, const WORD& wCnt, const WORD& wStart = 0);

public:
	HRESULT ReleaseStateTexture(const wstring& wstrStateKey);

public:
	size_t	GetUmapMultiSize() const { return m_umapMulti.size(); }
	size_t	GetStateSize(const wstring& wstrStateKey) const;
	vector<TEX_INFO*>* GetStateVector(const wstring& wstrStateKey);

private:
	unordered_map<wstring, vector<TEX_INFO*>> m_umapMulti;

};

