#include "stdafx.h"
#include "ChargeEffect.h"

#include "CircularTrail.h"
#include "Shader.h"

#include "Export_Function.h"
#include "TrailEffect.h"
#include "ChargeTrailRoute.h"
#include "MoveOnRoute.h"

CChargeEffect::CChargeEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CChargeEffect::~CChargeEffect()
{
}

HRESULT CChargeEffect::Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const D3DXCOLOR & dwColor)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_wContainerIdx = wContainerIdx;
	m_wstrTextureTag = wstrTextureTag;
	m_dwColor = dwColor;

	return S_OK;
}

_int CChargeEffect::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bIsDead)
		return 0;

	if (m_fLifeCount > m_fItvlTime + 0.3f)
	{
		m_fItvlTime = m_fLifeCount;

		CTrailEffect* pEffect = CTrailEffect::Create(m_pGraphicDev, m_wContainerIdx, m_wstrTextureTag, m_dwColor);
		NULL_CHECK_RETURN(pEffect, -1);
		FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_GameLogic", L"Fx_Trail", pEffect), -1);
		pEffect->Set_LifeTime(1.f);

		_vec3 vPos;
		m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
		_vec3 vEffectPos;
		_vec3 vAngle = { (rand() % 360) * 2.f * D3DX_PI / 360, (rand() % 360) * 2.f * D3DX_PI / 360, (rand() % 360) * 2.f * D3DX_PI / 360 };

		_matrix matRotAll, matRotX, matRotY, matRotZ;
		
		D3DXMatrixRotationX(&matRotX, vAngle.x);
		D3DXMatrixRotationY(&matRotY, vAngle.y);
		D3DXMatrixRotationZ(&matRotZ, vAngle.z);

		matRotAll = matRotX * matRotY * matRotZ;

		D3DXVec3TransformCoord(&vEffectPos, &(AXIS_Z * 4.f), &matRotAll);

		vEffectPos += vPos;

		pEffect->Set_Pos(&vEffectPos);
		pEffect->Set_InterpoleCnt(0);
		
		Engine::CTransform*	pEffectTrans = dynamic_cast<Engine::CTransform*>(pEffect->Get_Component(Engine::ID_DYNAMIC, L"Com_Transform"));
		CChargeTrailRoute* pRoute = CChargeTrailRoute::Create(pEffectTrans, &vEffectPos, &vPos, 8.f, 4.f);
		NULL_CHECK_RETURN(pRoute, -1);
		pEffect->Add_MoveOnRoute(pRoute);
	}
	

	_int iExit = CEffect::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return 0;
}

_int CChargeEffect::LateUpdate_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CChargeEffect::Render_GameObject(const _float& fTimeDelta)
{
}


HRESULT CChargeEffect::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	return S_OK;
}

CChargeEffect * CChargeEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrTextureTag, const D3DXCOLOR & dwColor)
{
	CChargeEffect* pInst = new CChargeEffect(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject(wContainerIdx, wstrTextureTag, dwColor)))
		::Safe_Release(pInst);

	return pInst;
}

void CChargeEffect::Free()
{
	CEffect::Free();
}