#include "LightMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CLightMgr)

Engine::CLightMgr::CLightMgr(void)
{

}

Engine::CLightMgr::~CLightMgr(void)
{
	Free();
}


void Engine::CLightMgr::Free(void)
{
	for_each(m_vecLight.begin(), m_vecLight.end(), CDeleteObj());
	m_vecLight.clear();
}

HRESULT Engine::CLightMgr::Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, 
										const D3DLIGHT9* pLightInfo, 
										const _uint& iIndex)
{
	CLight*		pLight = CLight::Create(pGraphicDev, pLightInfo, iIndex);
	NULL_CHECK_RETURN(pLight, E_FAIL);

	m_vecLight.push_back(pLight);

	return S_OK;
}

HRESULT CLightMgr::Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9 * pLightInfo, const _uint & iIndex, const _float & fFovy, 
	const _float & fAspect, const _float & fNear, const _float & fFar)
{
	CLight*		pLight = CLight::Create(pGraphicDev, pLightInfo, iIndex);
	NULL_CHECK_RETURN(pLight, E_FAIL);

	pLight->Set_LightProj(fFovy, fAspect, fNear, fFar);

	_vec3 vEye = pLightInfo->Position;
	_vec3 vAt = vEye + (_vec3)pLightInfo->Direction;
	//_vec3 vAt = { 0.f, 0.f, 0.f };
	_vec3 vUp = { 0.f, 1.f, 0.f };

	pLight->Set_LightView(&vEye, &vAt, &vUp);

	m_vecLight.push_back(pLight);

	return S_OK;
}

void CLightMgr::Render_Light(LPD3DXEFFECT & pEffect)
{
	for (auto& iter : m_vecLight)
		iter->Render_Light(pEffect);
}

void CLightMgr::Render_Light(LPD3DXEFFECT & pEffect, const _uint& iIndex)
{
	if (!m_vecLight.empty())
		m_vecLight[iIndex]->Render_Light(pEffect);
}

const D3DLIGHT9* Engine::CLightMgr::Get_LightInfo(const _uint& iIndex /*= 0*/)
{
	if (m_vecLight.empty())
		return nullptr;

	return m_vecLight[iIndex]->Get_LightInfo();
}

const CLight * CLightMgr::Get_Light(const _uint & iIndex) const 
{
	if (m_vecLight.empty())
		return nullptr;

	return m_vecLight[iIndex];
}

