#include "stdafx.h"
#include "HealingObject.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "Texture.h"
#include "Observer.h"

#include "Export_Function.h"

CHealingObject::CHealingObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CStaticObject(pGraphicDev)
{
}

CHealingObject::~CHealingObject()
{
}

HRESULT CHealingObject::Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName)
{
	FAILED_CHECK_RETURN(CStaticObject::Ready_GameObject(wContainerIdx, wstrBufferTag, wstrName), E_FAIL);

	m_pPlayerObserver = Engine::CObserver::Create(L"Player");
	NULL_CHECK_RETURN(m_pPlayerObserver, E_FAIL);

	m_pTransformCom->Set_Scale(0.01f);

	m_fSpeed = 10.f;
	m_fAcceleration = -10.f;


	m_pColliderCom->Deactivate();

	return S_OK;
}

HRESULT CHealingObject::Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags /*= 0*/)
{
	FAILED_CHECK_RETURN(CStaticObject::Add_Component(wContainerIdx, wstrBufferTag, dwFlags), E_FAIL);

	Engine::CComponent* pCom = nullptr;

	_matrix matCol;
	D3DXMatrixIdentity(&matCol);
	wstring wstrColName = L"Col_" + m_wstrName;
	pCom = m_pColliderCom = CSphereCollider::Create(m_pGraphicDev, Engine::CCollider::PT_OBJECT, this, &matCol, 0.2f, wstrColName);

	//pCom = pCol = CBoxCollider::Create(m_pGraphicDev, Engine::CCollider::PT_OBJECT, this, &matCol, &_vec3(2.f, 2.f, 2.f), wstrColName);
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Collider", pCom);
	//m_pColliderCom->Activate();

	return S_OK;
}

HRESULT CHealingObject::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	Engine::SetUp_OnShader(pEffect, L"Target_LightDepth", "g_LightDepthTexture");

	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	const D3DLIGHT9*		pLight = Engine::Get_LightInfo(0);
	NULL_CHECK_RETURN(pLight, S_OK);


	pEffect->SetVector("g_vLightDir", &_vec4(pLight->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLight->Diffuse);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLight->Specular);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLight->Ambient);


	D3DMATERIAL9		tMtrlInfo;
	ZeroMemory(&tMtrlInfo, sizeof(D3DMATERIAL9));

	tMtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	tMtrlInfo.Power = 100.f;

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&tMtrlInfo.Diffuse);
	pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&tMtrlInfo.Specular);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&tMtrlInfo.Ambient);

	_vec4		vCamPos;
	D3DXMatrixInverse(&matView, NULL, &matView);
	memcpy(&vCamPos, &matView.m[3][0], sizeof(_vec4));

	pEffect->SetVector("g_vCamPos", &vCamPos);
	pEffect->SetFloat("g_fPower", tMtrlInfo.Power);

	CCamera* pLightCam = Get_Camera(CAM_STATIC, L"Cam_Light");
	if (!pLightCam)
		return S_OK;
	NULL_CHECK_RETURN(pLightCam, E_FAIL);

	_matrix matLightView, matLightProj;
	pLightCam->Get_View(&matLightView);
	pLightCam->Get_Projection(&matLightProj);

	pEffect->SetMatrix("g_matLightView", &matLightView);
	pEffect->SetMatrix("g_matLightProj", &matLightProj);

	pEffect->SetFloat("g_fRim", 1.f);

	return S_OK;
}

_int CHealingObject::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bIsDead)
		return 0;

	m_pTransformCom->Rotation(Engine::ROT_Y, fTimeDelta);

	_vec3 vPlayerPos, vMyPos, vToPlayer;

	m_pPlayerObserver->Get_Data(SC_INT(EPlayerData::POSITION), &vPlayerPos);

	m_pTransformCom->Get_Info(Engine::INFO_POS, &vMyPos);

	vToPlayer = vPlayerPos - vMyPos;

	switch (m_eState)
	{
	case CHealingObject::EState::Spread:
	{
		m_fSpeed += m_fAcceleration * fTimeDelta;
		if (m_fSpeed <= 0.f)
		{
			m_fSpeed = 0.f;
			m_fAcceleration = 10.f;
			m_eState = EState::Stop;
		}
		m_pTransformCom->Move_Pos(&(m_vSpreadDir * m_fSpeed * fTimeDelta));
	}
		break;
	case CHealingObject::EState::Stop:
	{
		_float	fDist = D3DXVec3Length(&vToPlayer);
		if (fDist < 5.f)
		{
			m_eState = CHealingObject::EState::Move;
			m_pColliderCom->Activate();
		}
	}
		break;
	case CHealingObject::EState::Move:
	{
		D3DXVec3Normalize(&vToPlayer, &vToPlayer);

		m_fSpeed += m_fAcceleration * fTimeDelta;
		m_pTransformCom->Move_Pos(&(vToPlayer * m_fSpeed * fTimeDelta));
	}
		break;
	}

	_int iExit = CStaticObject::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	Engine::Add_GameObject_To_CollisionList(L"HealingObject", this);

	return 0;
}

_int CHealingObject::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_int iExit = CStaticObject::LateUpdate_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return 0;
}

void CHealingObject::Render_GameObject(const _float& fTimeDelta)
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

	//m_pMeshCom->Render_Meshes(pEffect);

	//pEffect->SetFloat()
	//m_fDissolveTime += fTimeDelta;
	//pEffect->SetFloat("g_fTime", m_fDissolveTime);
	//m_pDissolveTextureCom->Render_Texture("g_DissolveTexture", pEffect);

	//m_pMeshCom->Render_Meshes_WithPass(pEffect, 2);

	

	m_pMeshCom->Render_Meshes_WithPass(pEffect, 3);

	pEffect->End();

	::Safe_Release(pEffect);
}

void CHealingObject::Collision_Enter(CGameObject * pOpponent)
{
	m_bIsDead = true;

	Engine::Play_Sound(L"prop_pickup_ammo_01.ogg", Engine::CSoundMgr::EFFECT);
}

void CHealingObject::Collision_Stay(CGameObject * pOpponent)
{
}

void CHealingObject::Collision_Leave(CGameObject * pOpponent)
{
}

void CHealingObject::Set_Variables(const _float & fInitSpeed, const _float & fInitAccel, const _vec3 * pSpreadDir)
{
	m_fSpeed = fInitSpeed;
	m_fAcceleration = fInitAccel;
	memcpy(&m_vSpreadDir, pSpreadDir, sizeof(_vec3));
}

CHealingObject * CHealingObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const wstring & wstrBufferTag, const wstring & wstrNameTag)
{
	CHealingObject* pInst = new CHealingObject(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject(wContainerIdx, wstrBufferTag, wstrNameTag)))
		::Safe_Release(pInst);
 
	return pInst;
}

void CHealingObject::Free()
{
	::Safe_Release(m_pPlayerObserver);
	CStaticObject::Free();
}
