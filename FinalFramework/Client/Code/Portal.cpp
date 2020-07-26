#include "stdafx.h"
#include "Portal.h"

#include "Texture.h"
#include "TrailBuffer.h"
#include "Shader.h"
#include "NavigationMesh.h"

#include "Export_Function.h"
#include "CircleTex.h"
#include "Monster.h"
#include "Goblin.h"
#include "FallenDog.h"
#include "HellHound.h"

CPortal::CPortal(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CPortal::~CPortal()
{
}

HRESULT CPortal::Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const wstring& wstrNoiseTextureTag, const wstring& wstrMaskTextureTag, _vec4 vColor)
{
	FAILED_CHECK_RETURN(Add_Component(wContainerIdx, wstrTextureTag, wstrNoiseTextureTag, wstrMaskTextureTag), E_FAIL);

	m_pTransformCom->Set_Scale(0.f);

	m_pTransformCom->Set_Pos(165.f, -7.f, 17.f);
	m_pTransformCom->Set_AngleY(D3DXToRadian(90.f));

	m_vColor = vColor;

	m_eCurState = EPortal::OPENING;

	Change_NaviIndex(Engine::CCell::NO_MOVE);

	return S_OK;
}

_int CPortal::Update_GameObject(const _float& fTimeDelta)
{
	m_fLifeCount += fTimeDelta;

	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	iExit = Update_State(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	//m_pBufferCom->Move_UV_Up(fTimeDelta, 1.f);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	return 0;
}

_int CPortal::LateUpdate_GameObject(const _float& fTimeDelta)
{


	return 0;
}

void CPortal::Render_GameObject(const _float& fTimeDelta)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_uint	iPassMax = 0;

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	pEffect->Begin(&iPassMax, 0);

	pEffect->BeginPass(1);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();

	pEffect->End();

	::Safe_Release(pEffect);
}


_int CPortal::Update_State(const _float & fTimeDelta)
{
	_int iExit = 0;
	switch (m_eCurState)
	{
	case CPortal::EPortal::OPENING:
		iExit = Update_Opening(fTimeDelta);
		break;
	case CPortal::EPortal::SPAWN:
		iExit = Update_Spawn(fTimeDelta);
		break;
	case CPortal::EPortal::CLOSING:
		iExit = Update_Closing(fTimeDelta);
		break;
	}

	return iExit;
}

_int CPortal::Update_Opening(const _float & fTimeDelta)
{
	m_fCurScale += fTimeDelta * m_fSizing_Speed;

	if (m_fCurScale >= m_fMaxScale)
	{
		m_fCurScale = m_fMaxScale;
		m_eCurState = EPortal::SPAWN;
	}

	m_pTransformCom->Set_Scale(m_fCurScale);

	return 0;
}

_int CPortal::Update_Spawn(const _float & fTimeDelta)
{
	if (m_iMonsterCount > 20)
	{
		m_eCurState = EPortal::CLOSING;

		Change_NaviIndex(Engine::CCell::NO_OPTION);

		return 0;
	}

	m_fIntervalTime += fTimeDelta;

	if (m_fIntervalTime < 0.5f)
	{
		return 0;
	}

	if (Engine::Get_Layer(L"Layer_Monster")->Get_ObjectMapSize() > 20)
		return 0;

	m_fIntervalTime -= 0.5f;
	CMonster* pMonster = nullptr;

	switch (rand() % 3)
	{
	case 0:
		pMonster = CGoblin::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_Goblin", L"Goblin", CGoblin::EGoblin::RUN);
		break;
	case 1:
		pMonster = CFallenDog::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_FallenDog", L"FallenDog", CFallenDog::EFallenDog::START_WALK);
		break;
	case 2:
		pMonster = CHellHound::Create(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Mesh_HellHound", L"HellHound");
		break;
	}

	NULL_CHECK_RETURN(pMonster, -1);
	FAILED_CHECK_RETURN(Engine::Add_GameObject(L"Layer_Monster", L"Obj_Monster", pMonster), E_FAIL);

	pMonster->Set_NaviIndex(90);
	_vec3 vPos;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
	vPos.z += rand() % 10 - 5.f;
	pMonster->Set_Pos(&vPos);
	pMonster->Find_CorrectIndex();
	pMonster->Set_AngleY(D3DXToRadian(-90.f));
	++m_iMonsterCount;

	return 0;
}

_int CPortal::Update_Closing(const _float & fTimeDelta)
{
	m_fCurScale -= fTimeDelta * m_fSizing_Speed;

	if (m_fCurScale <= 0.f)
	{
		m_fCurScale = 0.f;
		m_bIsDead = true;
	}

	m_pTransformCom->Set_Scale(m_fCurScale);

	return 0;
}

void CPortal::Change_NaviIndex(const _int& iOption)
{
	m_pNaviCom->Change_CellOption(523,	(Engine::CCell::CELL_OPTION)iOption);
	m_pNaviCom->Change_CellOption(81,	(Engine::CCell::CELL_OPTION)iOption);
	m_pNaviCom->Change_CellOption(82,	(Engine::CCell::CELL_OPTION)iOption);
	m_pNaviCom->Change_CellOption(84,	(Engine::CCell::CELL_OPTION)iOption);
	m_pNaviCom->Change_CellOption(90,	(Engine::CCell::CELL_OPTION)iOption);
	m_pNaviCom->Change_CellOption(91,	(Engine::CCell::CELL_OPTION)iOption);
	m_pNaviCom->Change_CellOption(94,	(Engine::CCell::CELL_OPTION)iOption);
	m_pNaviCom->Change_CellOption(95,	(Engine::CCell::CELL_OPTION)iOption);
	
}

void CPortal::Set_Pos(const _vec3 * pPos)
{
	m_pTransformCom->Set_Pos(pPos);
}

void CPortal::Set_Pos(const _vec3 & rPos)
{
	m_pTransformCom->Set_Pos(rPos);
}

void CPortal::Set_Pos(const _float & fX, const _float & fY, const _float & fZ)
{
	m_pTransformCom->Set_Pos(fX, fY, fZ);
}

void CPortal::Set_PosX(const _float & fX)
{
	m_pTransformCom->Set_PosX(fX);
}

void CPortal::Set_PosY(const _float & fY)
{
	m_pTransformCom->Set_PosY(fY);
}

void CPortal::Set_PosZ(const _float & fZ)
{
	m_pTransformCom->Set_PosZ(fZ);
}

HRESULT CPortal::Add_Component(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const wstring& wstrNoiseTextureTag, const wstring& wstrMaskTextureTag)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(wContainerIdx, wstrTextureTag));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);
	
	pComponent = m_pNoiseTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(wContainerIdx, wstrNoiseTextureTag));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_NoiseTexture", pComponent);

	pComponent = m_pMaskTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(wContainerIdx, wstrMaskTextureTag));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_MaskTexture", pComponent);

	pComponent = m_pBufferCom = dynamic_cast<Engine::CCircleTex*>(Engine::Clone(Engine::RESOURCE_STAGE, L"Buffer_CircleTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Portal"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);
	pComponent->AddRef();

	pComponent = m_pNaviCom = dynamic_cast<Engine::CNavigationMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, L"Mesh_Navi"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Navi", pComponent);
	

	return S_OK;
}

HRESULT CPortal::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	pEffect->SetFloat("g_Alpha", 1.f);
	pEffect->SetFloat("g_Glow", 1.f);
	pEffect->SetFloat("g_fTime", m_fLifeCount);

	_vec3 vScrollSpeed = { 1.f, 0.5f, 0.25f };
	_vec3 vScales = { 1.f, 3.f, 2.f };

	_vec2 vDistortion1 = { 1.f, 1.f };
	_vec2 vDistortion2 = { 2.f, 2.f };
	_vec2 vDistortion3 = { 0.7f, 0.7f };

	pEffect->SetFloatArray("g_vScrollSpeeds", &vScrollSpeed.x, 3);
	pEffect->SetFloatArray("g_vScales", &vScales.x, 3);

	pEffect->SetFloatArray("g_vDistortion1", &vDistortion1.x, 2);
	pEffect->SetFloatArray("g_vDistortion2", &vDistortion2.x, 2);
	pEffect->SetFloatArray("g_vDistortion3", &vDistortion3.x, 2);
	pEffect->SetFloat("fDistortionScale", 0.5f);
	pEffect->SetFloat("fDistortionBias", 0.4f);
	pEffect->SetVector("g_vColor", &m_vColor);

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture");
	m_pNoiseTextureCom->Set_Texture(pEffect, "g_NoiseTexture");
	m_pMaskTextureCom->Set_Texture(pEffect, "g_MaskTexture");
	Engine::SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture");

	return S_OK;
}

CPortal * CPortal::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, 
	const wstring& wstrTextureTag, const wstring& wstrNoiseTextureTag, const wstring& wstrMaskTextureTag, _vec4 vColor)
{
	CPortal* pInst = new CPortal(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject(wContainerIdx, wstrTextureTag, wstrNoiseTextureTag, wstrMaskTextureTag, vColor)))
		::Safe_Release(pInst);

	return pInst;
}

void CPortal::Free()
{
	CGameObject::Free();
}