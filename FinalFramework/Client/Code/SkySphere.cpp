#include "stdafx.h"
#include "SkySphere.h"

#include "Shader.h"

#include "Export_Function.h"

CSkySphere::CSkySphere(LPDIRECT3DDEVICE9 pGraphicDev)
	:CStaticObject(pGraphicDev)
{
}

CSkySphere::~CSkySphere()
{
}

HRESULT CSkySphere::Ready_GameObject(const _ushort & wContainerIdx, const wstring & wstrBufferTag, const wstring & wstrName, const _ulong & dwFlags)
{
	FAILED_CHECK_RETURN(Add_Component(wContainerIdx, wstrBufferTag, dwFlags), E_FAIL);

	m_wstrName = wstrName;

	m_pTransformCom->Set_Scale(0.1f);

	return S_OK;
}

_int CSkySphere::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDissolve)
	{
		m_fDissolveTime += fTimeDelta * 0.5f;
		if (m_fDissolveTime > 2.f)
		{
			m_bDissolve = false;
			m_fDissolveTime = 0.f;
			m_vColor = m_vAfterColor;
		}
	}

	_vec3 vCamPos = *Engine::Get_MainCamera()->Get_Eye();

	m_pTransformCom->Set_Pos(vCamPos);

	_int iExit = CGameObject::Update_GameObject(fTimeDelta);
	//_int iExit = CStaticObject::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	m_pRenderer->Add_RenderGroup(Engine::RENDER_PRIORITY, this);

	return iExit;
}

_int CSkySphere::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_int iExit = CStaticObject::LateUpdate_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return iExit;
}

void CSkySphere::Render_GameObject(const _float& fTimeDelta)
{
	//CStaticObject::Render_GameObject(fTimeDelta);

	if (m_vecShader.empty())
		return;


	LPD3DXEFFECT	pEffect = m_vecShader[0]->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_uint	iPassMax = 0;

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	pEffect->Begin(&iPassMax, 0);

	if (m_bDissolve)
	{
		pEffect->SetFloat("g_fTime", m_fDissolveTime);
		pEffect->SetVector("g_vColor", &m_vColor);
		pEffect->SetVector("g_vDissolveColor", &m_vDissolveColor);
		pEffect->SetVector("g_vAfterColor", &m_vAfterColor);
		
		m_pDissolveTextureCom->Set_Texture(pEffect, "g_DissolveTexture");
		m_pMeshCom->Render_Meshes_WithPass(pEffect, 1);
	}
	else
	{
		pEffect->SetVector("g_vColor", &m_vColor);
		m_pMeshCom->Render_Meshes_WithPass(pEffect, 0);
	}

	pEffect->End();

	::Safe_Release(pEffect);

}

void CSkySphere::Start_ChangeColor(const _vec4 * pAfterColor)
{
	m_fDissolveTime = 0.f;
	m_bDissolve = true;
	m_vAfterColor = *pAfterColor;
}

HRESULT CSkySphere::Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags /*= 0*/)
{
	CComponent* pCom = nullptr;

	pCom = m_pTransformCom = CTransform::Create(dwFlags);
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pCom);

	pCom = m_pRenderer = Get_Renderer();
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_pRenderer->AddRef();
	m_mapComponent[ID_STATIC].emplace(L"Com_Renderer", pCom);

	pCom = m_pMeshCom = dynamic_cast<CMesh*>(Clone(wContainerIdx, wstrBufferTag));
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pCom);

	CShader* pShader = nullptr;
	pCom = pShader = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Sky"));
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pCom);
	m_vecShader.push_back(pShader);

	pCom = m_pDissolveTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STAGE, L"Texture_Dissolve"));
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_DissolveTexture", pCom);
	
	return S_OK;
}

HRESULT CSkySphere::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	//CStaticObject::SetUp_ConstantTable(pEffect);

	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	//D3DXMatrixIdentity(&matWorld);
	//matWorld._11 = matWorld._22 = matWorld._33 = 0.01f;

	Engine::Get_MainCamera()->Get_View(&matView);
	Engine::Get_MainCamera()->Get_Projection(&matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	return S_OK;
}

CSkySphere * CSkySphere::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const wstring & wstrBufferTag, const wstring& wstrNameTag, const _ulong & dwFlags)
{
	CSkySphere* pInst = new CSkySphere(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject(wContainerIdx, wstrBufferTag, wstrNameTag, dwFlags)))
		::Safe_Release(pInst);

	return pInst;
}

void CSkySphere::Free()
{
	CStaticObject::Free();
}

