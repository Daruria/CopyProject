#include "stdafx.h"
#include "MeshEffect.h"
#include "SphereCollider.h"
#include "BoxCollider.h"

#include "Export_Function.h"

CMeshEffect::CMeshEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CStaticObject(pGraphicDev)
{
}

CMeshEffect::~CMeshEffect()
{
}


HRESULT CMeshEffect::Ready_GameObject()
{
	FAILED_CHECK_RETURN(CStaticObject::Ready_GameObject(), E_FAIL);

	return S_OK;
}

HRESULT CMeshEffect::Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName)
{
	FAILED_CHECK_RETURN(CStaticObject::Ready_GameObject(wContainerIdx, wstrBufferTag, wstrName), E_FAIL);

	m_pTransformCom->Set_Scale(0.01f);

	m_bDissolve = false;

	return S_OK;
}

void CMeshEffect::Get_Pos(_vec3 * pOut)
{
	if (pOut)
		m_pTransformCom->Get_Info(Engine::INFO_POS, pOut);
}

HRESULT CMeshEffect::Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags /*= 0*/)
{
	FAILED_CHECK_RETURN(CStaticObject::Add_Component(wContainerIdx, wstrBufferTag, dwFlags), E_FAIL);

	Engine::CComponent* pCom = nullptr;

	pCom = m_pDissolveTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STAGE, L"Texture_Dissolve_Bits"));
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_DissolveTexture", pCom);

	return S_OK;
}


_int CMeshEffect::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bIsDead)
		return 0;

	m_fLifeCnt += fTimeDelta;

	if (m_fLifeCnt > m_fLifeTime)
	{
		m_bIsDead = true;
		return 0;
	}

	if (m_fLifeCnt > m_fDissolveStartTime)
		m_bDissolve = true;

	if (m_bDissolve && m_fDissolveTime > 2.f)
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

	_int iExit = CStaticObject::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	m_pRenderer->Add_RenderGroup(Engine::RENDER_NONALPHA, this);	

	return 0;
}

_int CMeshEffect::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_int iExit = CStaticObject::LateUpdate_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return 0;
}

void CMeshEffect::Render_GameObject(const _float& fTimeDelta)
{
	if (m_vecShader.empty())
		return;

	_matrix matWorld;
	m_pTransformCom->Get_WorldMatrix(&matWorld);
	_vec3 vCenter, vScale;
	D3DXVec3TransformCoord(&vCenter, &m_vCenter, &matWorld);

	m_pTransformCom->Get_Scale(&vScale);

	_float fScale = max(vScale.x, max(vScale.y, vScale.z));
	_float fRadius = m_fRadius * fScale;

	if (!Get_MainCamera()->Is_InFrustum(&vCenter, fRadius))
		return;

	LPD3DXEFFECT	pEffect = m_vecShader[0]->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_uint	iPassMax = 0;

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	pEffect->Begin(&iPassMax, 0);

	if (m_bDissolve)
	{
		m_fDissolveTime += fTimeDelta;
		pEffect->SetFloat("g_fTime", m_fDissolveTime);
		pEffect->SetVector("g_vDissolveColor", &_vec4(1.f, 0.8f, 0.6f, 1.f));
		m_pDissolveTextureCom->Render_Texture("g_DissolveTexture", pEffect);

		m_pMeshCom->Render_Meshes_WithPass(pEffect, 2);
	}
	else
		m_pMeshCom->Render_Meshes(pEffect);

	pEffect->End();

	::Safe_Release(pEffect);
}

void CMeshEffect::Set_Pos(const _vec3 * pPos)
{
	m_pTransformCom->Set_Pos(pPos);
}

void CMeshEffect::Set_Pos(const _float & fX, const _float & fY, const _float & fZ)
{
	m_pTransformCom->Set_Pos(fX, fY, fZ);
}

void CMeshEffect::Set_PosX(const _float & fX)
{
	m_pTransformCom->Set_PosX(fX);
}

void CMeshEffect::Set_PosY(const _float & fY)
{
	m_pTransformCom->Set_PosY(fY);
}

void CMeshEffect::Set_PosZ(const _float & fZ)
{
	m_pTransformCom->Set_PosZ(fZ);
}

void CMeshEffect::Set_Scale(const _vec3 * pScale)
{
	m_pTransformCom->Set_Scale(pScale);
}

void CMeshEffect::Set_Scale(const _float & fScale)
{
	m_pTransformCom->Set_Scale(fScale);
}

void CMeshEffect::Set_Scale(const _float & fX, const _float & fY, const _float & fZ)
{
	m_pTransformCom->Set_Scale(fX, fY, fZ);
}

void CMeshEffect::Set_ScaleX(const _float & fX)
{
	m_pTransformCom->Set_ScaleX(fX);
}

void CMeshEffect::Set_ScaleY(const _float & fY)
{
	m_pTransformCom->Set_ScaleY(fY);
}

void CMeshEffect::Set_ScaleZ(const _float & fZ)
{
	m_pTransformCom->Set_ScaleZ(fZ);
}


CMeshEffect * CMeshEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const wstring & wstrBufferTag, const wstring & wstrNameTag)
{
	CMeshEffect* pInst = new CMeshEffect(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject(wContainerIdx, wstrBufferTag, wstrNameTag)))
		::Safe_Release(pInst);
 
	return pInst;
}

void CMeshEffect::Free()
{
	CStaticObject::Free();
}

void CMeshEffect::SetUp_ScaleChange(const _vec3 * pStartScale, const _vec3 * pMaxScale, const _float & fScalingSpeed)
{
	m_pTransformCom->Set_Scale(pStartScale);

	m_vMaxScale = *pMaxScale;
	m_vDeltaScale = (*pMaxScale - *pStartScale) * fScalingSpeed;
}

void CMeshEffect::Set_Rotation(const _vec3 * pRot)
{
	m_pTransformCom->Set_Angle(pRot);
}

void CMeshEffect::Set_Rotation(const _float & fX, const _float & fY, const _float & fZ)
{
	m_pTransformCom->Set_Angle(fX, fY, fZ);
}

void CMeshEffect::Set_RotationX(const _float & fX)
{
	m_pTransformCom->Set_AngleX(fX);
}

void CMeshEffect::Set_RotationY(const _float & fY)
{
	m_pTransformCom->Set_AngleY(fY);
}

void CMeshEffect::Set_RotationZ(const _float & fZ)
{
	m_pTransformCom->Set_AngleZ(fZ);
}
