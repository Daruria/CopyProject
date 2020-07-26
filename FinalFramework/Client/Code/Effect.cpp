#include "stdafx.h"
#include "Effect.h"

#include "Texture.h"
#include "TrailBuffer.h"
#include "Shader.h"

#include "Export_Function.h"

CEffect::CEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CEffect::~CEffect()
{
}

HRESULT CEffect::Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrTextureTag)
{
	FAILED_CHECK_RETURN(Add_Component(wContainerIdx, wstrTextureTag), E_FAIL);


	return S_OK;
}

_int CEffect::Update_GameObject(const _float& fTimeDelta)
{
	m_fLifeCount += fTimeDelta;
	if (m_fLifeCount > m_fLifeTime)
	{
		m_bIsDead = true;
		return 0;
	}

	_vec3 vScale;
	m_pTransformCom->Get_Scale(&vScale);
	if ((vScale.x > 0.f && vScale.y > 0.f && vScale.z > 0.f) && (m_vDeltaScale.x || m_vDeltaScale.y || m_vDeltaScale.z))
	{
		vScale += m_vDeltaScale * fTimeDelta;

		if (vScale.x < 0.f)
			vScale.x = 0.f;
		else if (vScale.x > m_vMaxScale.x)
			vScale.x = m_vMaxScale.x;

		if (vScale.y < 0.f)
			vScale.y = 0.f;
		else if (vScale.y > m_vMaxScale.y)
			vScale.y = m_vMaxScale.y;

		if (vScale.z < 0.f)
			vScale.z = 0.f;
		else if (vScale.z > m_vMaxScale.z)
			vScale.z = m_vMaxScale.z;

		m_pTransformCom->Set_Scale(&vScale);
	}

	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return 0;
}

_int CEffect::LateUpdate_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CEffect::Render_GameObject(const _float& fTimeDelta)
{

}

void CEffect::Set_Pos(const _vec3 * pPos)
{
	m_pTransformCom->Set_Pos(pPos);
}

void CEffect::Set_Pos(const _float & fX, const _float & fY, const _float & fZ)
{
	m_pTransformCom->Set_Pos(fX, fY, fZ);
}

void CEffect::Set_PosX(const _float & fX)
{
	m_pTransformCom->Set_PosX(fX);
}

void CEffect::Set_PosY(const _float & fY)
{
	m_pTransformCom->Set_PosY(fY);
}

void CEffect::Set_PosZ(const _float & fZ)
{
	m_pTransformCom->Set_PosZ(fZ);
}

void CEffect::Set_Scale(const _vec3 * pScale)
{
	m_pTransformCom->Set_Scale(pScale);
	m_vMaxScale = *pScale;
}

void CEffect::Set_Scale(const _float & fScale)
{
	m_pTransformCom->Set_Scale(fScale);
	m_vMaxScale = { fScale, fScale, fScale };
}

void CEffect::Set_Scale(const _float & fX, const _float & fY, const _float & fZ)
{
	m_pTransformCom->Set_Scale(fX, fY, fZ);
	m_vMaxScale = { fX, fY, fZ };
}

void CEffect::Set_ScaleX(const _float & fX)
{
	m_pTransformCom->Set_ScaleX(fX);
}

void CEffect::Set_ScaleY(const _float & fY)
{
	m_pTransformCom->Set_ScaleY(fY);
}

void CEffect::Set_ScaleZ(const _float & fZ)
{
	m_pTransformCom->Set_ScaleZ(fZ);
}

void CEffect::SetUp_ScaleChange(const _vec3 * pStartScale, const _vec3 * pMaxScale, const _float& fScalingSpeed)
{
	m_pTransformCom->Set_Scale(pStartScale);

	memcpy(&m_vMaxScale, pMaxScale, sizeof(_vec3));
	
	m_vDeltaScale = (*pMaxScale - *pStartScale) * fScalingSpeed;
}

void CEffect::Set_Rotation(const _vec3 * pRot)
{
	m_pTransformCom->Set_Angle(pRot);
}

void CEffect::Set_Rotation(const _float & fX, const _float & fY, const _float & fZ)
{
	m_pTransformCom->Set_Angle(fX, fY, fZ);
}

void CEffect::Set_RotationX(const _float & fX)
{
	m_pTransformCom->Set_AngleX(fX);
}

void CEffect::Set_RotationY(const _float & fY)
{
	m_pTransformCom->Set_AngleY(fY);
}

void CEffect::Set_RotationZ(const _float & fZ)
{
	m_pTransformCom->Set_AngleZ(fZ);
}

HRESULT CEffect::Add_Component(const _ushort& wContainerIdx, const wstring& wstrTextureTag)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(wContainerIdx, wstrTextureTag));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Effect"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);
	pComponent->AddRef();

	return S_OK;
}

HRESULT CEffect::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	pEffect->SetFloat("g_WINCY", (_float)WINCY);

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture");
	Engine::SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture");
	
	return S_OK;
}

CEffect * CEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrTextureTag)
{
	CEffect* pInst = new CEffect(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject(wContainerIdx, wstrTextureTag)))
		::Safe_Release(pInst);

	return pInst;
}

void CEffect::Free()
{
	CGameObject::Free();
}