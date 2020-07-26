#pragma once
#include "Texture.h"
class CSingleTexture :
	public CTexture
{
private:
	CSingleTexture();

public:
	virtual ~CSingleTexture();

	
public:// Inherited via CTexture
	virtual const TEX_INFO * GetTexInfo(const wstring & wstrStateKey = L"", const WORD & wIndex = 0) const override;
	virtual HRESULT LoadTexture(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrFilePath, const wstring & wstrStateKey = L"", const WORD & wCnt = 0, const WORD& wStart = 0) override;
	virtual void Release() override;

public:
	static CSingleTexture* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrFilePath, const wstring& wstrStateKey = L"", const WORD& wCnt = 0);

private:
	TEX_INFO*	m_pTexInfo;
};

