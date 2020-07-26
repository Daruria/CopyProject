#include "stdafx.h"
#include "PlaneObject.h"

#include "Export_Function.h"

CPlaneObject::CPlaneObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CPlaneObject::~CPlaneObject()
{
}

HRESULT CPlaneObject::Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags)
{
	Engine::CComponent* pCom = nullptr;

	pCom = m_pTransformCom = Engine::CTransform::Create(dwFlags);
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", m_pTransformCom);

	pCom = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_LOGO, L"MainLogo"));
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", m_pTextureCom);

	pCom = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pCom);

	pCom = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pCom);
	m_pRendererCom->AddRef();
	
	return S_OK;
}

HRESULT CPlaneObject::Add_Component(const _ushort & wBufferConIdx, const wstring & wstrBufferTag, const _ushort & wTextureConIdx, const wstring & wstrTextureTag)
{
	Engine::CComponent* pCom = nullptr;

	pCom = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", m_pTransformCom);

	pCom = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(wBufferConIdx, wstrBufferTag));
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pCom);

	pCom = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(wTextureConIdx, wstrTextureTag));
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", m_pTextureCom);

	pCom = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pCom);
	m_pRendererCom->AddRef();


	return S_OK;
}

HRESULT CPlaneObject::Ready_GameObject()
{
	

	return S_OK;
}

HRESULT CPlaneObject::Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName)
{
	FAILED_CHECK_RETURN(Add_Component(wContainerIdx, wstrBufferTag), E_FAIL);

	m_wstrName = wstrName;

	return S_OK;
}

HRESULT CPlaneObject::Ready_GameObject(const _vec3* pPos, const _vec3* pAngle, const _vec3* pScale,
	const _ushort & wBufferConIdx, const wstring & wstrBufferTag, 
	const _ushort & wTextureConIdx, const wstring & wstrTextureTag, const wstring & wstrName)
{
	FAILED_CHECK_RETURN(Add_Component(wBufferConIdx, wstrBufferTag, wTextureConIdx, wstrTextureTag), E_FAIL);

	m_wstrName = wstrName;
	m_pTransformCom->Set_Pos(pPos);
	m_pTransformCom->Set_Angle(pAngle);
	m_pTransformCom->Set_Scale(pScale);

	return S_OK;
}

_int CPlaneObject::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);

	return iExit;
}

_int CPlaneObject::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return iExit;
}

void CPlaneObject::Render_GameObject(const _float& fTimeDelta)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->Render_Texture();

	m_pBufferCom->Set_LocalPos(Engine::CRcTex::RTS_CENTER_CENTER);
	m_pBufferCom->Set_UV(Engine::CRcTex::RTS_CENTER_BOTTOM, 1.f, 1.f);

	m_pBufferCom->Render_Buffer();

}

CPlaneObject * CPlaneObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * pPos, const _vec3 * pAngle, const _vec3 * pScale, 
	const _ushort & wBufferConIdx, const wstring & wstrBufferTag, 
	const _ushort & wTextureConIdx, const wstring & wstrTextureTag, const wstring & wstrName)
{
	CPlaneObject* pInst = new CPlaneObject(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject(pPos, pAngle, pScale,
		wBufferConIdx, wstrBufferTag, wTextureConIdx, wstrTextureTag, wstrName)))
		Engine::Safe_Release(pInst);

	return pInst;
}

void CPlaneObject::Free()
{
	CGameObject::Free();
}


