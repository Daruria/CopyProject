#include "stdafx.h"
#include "Weapon.h"
#include "ChildTransform.h"

#include "Export_Function.h"

CWeapon::CWeapon(LPDIRECT3DDEVICE9 pGraphicDev)
	: CStaticObject(pGraphicDev)
{
}

CWeapon::~CWeapon()
{
}

HRESULT CWeapon::Ready_GameObject()
{
	return S_OK;
}

void CWeapon::Set_BoneMatrix(const _matrix * pmatParent)
{
	m_pTransformCom->Set_BoneMatrix(pmatParent);
}

void CWeapon::Set_ParentTransform(CTransform* pTransform)
{
	m_pTransformCom->Set_ParentTransform(pTransform);
}

void CWeapon::Set_TransformFlags(const _ulong & dwFlags)
{
	m_pTransformCom->Set_Flag(dwFlags);
}

void CWeapon::Add_TransformFlags(const _ulong & dwFlags)
{
	m_pTransformCom->Add_Flag(dwFlags);
}

void CWeapon::Get_Pos(_vec3 * pOut)
{
	m_pTransformCom->Get_WorldPos(pOut);
}

void CWeapon::Get_WorldMatrix(_matrix * pOut)
{
	m_pTransformCom->Get_WorldMatrix(pOut);
}

void CWeapon::Get_RenderMatrix(_matrix * pOut)
{
	m_pTransformCom->Get_RenderMatrix(pOut);
}

void CWeapon::Set_Parent(const _matrix * pmatParent, CTransform * pParentTransform)
{
	m_pTransformCom->Set_BoneMatrix(pmatParent);
	m_pTransformCom->Set_ParentTransform(pParentTransform);
	m_pTransformCom->Add_Flag(TS_BONE | TS_PARENT);
}

void CWeapon::Multiply_WorldMatrix(const _matrix * pMatrix)
{
	m_pTransformCom->Multiply_WorldMatrix(pMatrix);
}

HRESULT CWeapon::Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags)
{
	FAILED_CHECK_RETURN(CStaticObject::Add_Component(wContainerIdx, wstrBufferTag, dwFlags), E_FAIL);

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pDissolveTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STAGE, L"Texture_Dissolve_Bits"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_DissolveTexture", pComponent);

	return S_OK;
}

HRESULT CWeapon::Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName)
{
	FAILED_CHECK_RETURN(Ready_GameObject(wContainerIdx, wstrBufferTag, wstrName), E_FAIL);

	return S_OK;
}

_int CWeapon::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CStaticObject::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return 0;
}

_int CWeapon::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_int iExit = CStaticObject::LateUpdate_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return 0;
}

void CWeapon::Render_GameObject(const _float& fTimeDelta)
{
	if (m_vecShader.empty())
		return;

	//_matrix matWorld;
	//m_pTransformCom->Get_RenderMatrix(&matWorld);
	//_vec3 vCenter, vScale;
	//D3DXVec3TransformCoord(&vCenter, &m_vCenter, &matWorld);

	//m_pTransformCom->Get_Scale(&vScale);

	//_float fScale = max(vScale.x, max(vScale.y, vScale.z));
	//_float fRadius = m_fRadius * fScale;

	//if (!Get_MainCamera()->Is_InFrustum(&vCenter, fRadius))
	//	return;

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
		pEffect->SetVector("g_vDissolveColor", &_vec4(1.f, 0.f, 0.f, 1.f));
		m_pDissolveTextureCom->Render_Texture("g_DissolveTexture", pEffect);

		m_pMeshCom->Render_Meshes_WithPass(pEffect, 2);
	}
	else
		m_pMeshCom->Render_Meshes(pEffect);

	pEffect->End();

	::Safe_Release(pEffect);
}

void CWeapon::Render_GameObject(const _float & fTimeDelta, const _uint & iIndex)
{
	if (m_vecShader.empty())
		return;

	//_matrix matWorld;
	//m_pTransformCom->Get_RenderMatrix(&matWorld);
	//_vec3 vCenter, vScale;
	//D3DXVec3TransformCoord(&vCenter, &m_vCenter, &matWorld);

	//m_pTransformCom->Get_Scale(&vScale);

	//_float fScale = max(vScale.x, max(vScale.y, vScale.z));
	//_float fRadius = m_fRadius * fScale;

	//CCamera* pLightCam = Get_Camera(CAM_STATIC, L"Cam_Light");
	//NULL_CHECK_RETURN(pLightCam, );

	//if (!pLightCam->Is_InFrustum(&vCenter, fRadius))
	//	return;

	LPD3DXEFFECT	pEffect = m_vecShader[iIndex]->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_uint	iPassMax = 0;

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	pEffect->Begin(&iPassMax, 0);

	m_pMeshCom->Render_Meshes(pEffect, MTT_END);

	pEffect->End();

	::Safe_Release(pEffect);
}

void CWeapon::Free()
{
	CStaticObject::Free();
}