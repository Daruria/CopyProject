#include "RenderTargetMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CRenderTargetMgr)

Engine::CRenderTargetMgr::CRenderTargetMgr(void)
{

}

Engine::CRenderTargetMgr::~CRenderTargetMgr(void)
{
	Free();
}

void CRenderTargetMgr::SetUp_OnShader(LPD3DXEFFECT & pEffect, const wstring& wstrTargetTag, const char * pContantName)
{
	CRenderTarget*	pRenderTarget = Find_RenderTarget(wstrTargetTag);

	if (nullptr == pRenderTarget)
		return;

	pRenderTarget->SetUp_OnShader(pEffect, pContantName);
}

HRESULT Engine::CRenderTargetMgr::Ready_RenderTarget(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrTargetTag, const _uint& iWidth, const _uint& iHeight,
													D3DFORMAT Format, D3DXCOLOR Color)
{
	CRenderTarget*	pRenderTarget = Find_RenderTarget(wstrTargetTag);

	if (nullptr != pRenderTarget)
		return E_FAIL;

	pRenderTarget = CRenderTarget::Create(pGraphicDev, iWidth, iHeight, Format, Color);
	NULL_CHECK_RETURN(pRenderTarget, E_FAIL);

	m_mapRenderTarget.emplace(wstrTargetTag, pRenderTarget);

	return S_OK;
}

HRESULT CRenderTargetMgr::Ready_MRT(const wstring& wstrMRTTag, const wstring& wstrTargetTag)
{
	CRenderTarget*	pRenderTarget = Find_RenderTarget(wstrTargetTag);

	if (nullptr == pRenderTarget)
		return E_FAIL;

	list<CRenderTarget*>*		pMRTList = Find_MRT(wstrMRTTag);
	if (nullptr == pMRTList)
	{
		list<CRenderTarget*>		MRTList;
		MRTList.push_back(pRenderTarget);
		m_mapMRT.emplace(wstrMRTTag, MRTList);
	}
	else
		pMRTList->push_back(pRenderTarget);

	return S_OK;
}

HRESULT CRenderTargetMgr::Begin_MRT(const wstring& wstrMRTTag)
{
	list<CRenderTarget*>*	pMRTList = Find_MRT(wstrMRTTag);
	NULL_CHECK_RETURN(pMRTList, E_FAIL);

	for (auto& iter : *pMRTList)
		iter->Clear_RenderTarget();

	_uint	iIndex = 0;

	for (auto& iter : *pMRTList)
		iter->SetUp_RenderTarget(iIndex++);	

	return S_OK;
}

HRESULT CRenderTargetMgr::End_MRT(const wstring& wstrMRTTag)
{
	list<CRenderTarget*>*	pMRTList = Find_MRT(wstrMRTTag);
	NULL_CHECK_RETURN(pMRTList, E_FAIL);

	_uint	iIndex = 0;

	for (auto& iter : *pMRTList)
		iter->Release_OnGraphicDev(iIndex++);

	return S_OK;
}

HRESULT CRenderTargetMgr::Ready_DebugBuffer(const wstring& wstrTargetTag, const _float & fX, const _float & fY, const _float & fSizeX, const _float & fSizeY)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(wstrTargetTag);
	NULL_CHECK_RETURN(pRenderTarget, E_FAIL);

	FAILED_CHECK_RETURN(pRenderTarget->Ready_DebugBuffer(fX, fY, fSizeX, fSizeY), E_FAIL);
	
	return S_OK;
}

void CRenderTargetMgr::Render_DebugBuffer(const wstring& wstrMRTTag)
{
	list<CRenderTarget*>* pMRTList = Find_MRT(wstrMRTTag);
	NULL_CHECK(pMRTList);

	for (auto& iter : *pMRTList)
		iter->Render_DebugBuffer();	
}

Engine::CRenderTarget* Engine::CRenderTargetMgr::Find_RenderTarget(const wstring& wstrTargetTag)
{
	auto	iter = m_mapRenderTarget.find(wstrTargetTag);

	if (iter == m_mapRenderTarget.end())
		return nullptr;

	return iter->second;
}

list<CRenderTarget*>* CRenderTargetMgr::Find_MRT(const wstring& wstrMRTTag)
{
	auto	iter = m_mapMRT.find(wstrMRTTag);

	if (iter == m_mapMRT.end())
		return nullptr;

	return &iter->second;
}

void Engine::CRenderTargetMgr::Free(void)
{
	for_each(m_mapRenderTarget.begin(), m_mapRenderTarget.end(), CDeleteMap());
	m_mapRenderTarget.clear();

	m_mapMRT.clear();
}

