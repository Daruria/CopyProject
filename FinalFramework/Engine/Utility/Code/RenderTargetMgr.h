#ifndef RenderTargetMgr_h__
#define RenderTargetMgr_h__

#include "Engine_Defines.h"
#include "Base.h"
#include "RenderTarget.h"

BEGIN(Engine)

class ENGINE_DLL CRenderTargetMgr : public CBase
{
	DECLARE_SINGLETON(CRenderTargetMgr)

private:
	explicit CRenderTargetMgr(void);
	virtual ~CRenderTargetMgr(void);

public:
	void			SetUp_OnShader(LPD3DXEFFECT& pEffect, const wstring& wstrTargetTag, const char* pContantName);

public:
	HRESULT			Ready_RenderTarget(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrTargetTag, const _uint& iWidth, const _uint& iHeight, 
										D3DFORMAT Format, D3DXCOLOR Color);

	HRESULT			Ready_MRT(const wstring& wstrMRTTag, const wstring& wstrTargetTag);
	
	HRESULT			Begin_MRT(const wstring& wstrMRTTag);
	HRESULT			End_MRT(const wstring& wstrMRTTag);

	HRESULT			Ready_DebugBuffer(const wstring& wstrTargetTag, const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY);

	void			Render_DebugBuffer(const wstring& wstrMRTTag);

private:
	CRenderTarget*				Find_RenderTarget(const wstring& wstrTargetTag);
	list<CRenderTarget*>*		Find_MRT(const wstring& wstrMRTTag);



private:
	map<wstring, CRenderTarget*>		m_mapRenderTarget;

	map<wstring, list<CRenderTarget*>>	m_mapMRT;


private:
	virtual void		Free(void);
};

END
#endif // RenderTargetMgr_h__
