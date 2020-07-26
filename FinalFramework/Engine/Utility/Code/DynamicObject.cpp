#include "DynamicObject.h"

#include "Transform.h"
#include "Renderer.h"
#include "DynamicMesh.h"

#include "Export_Function.h"

USING(Engine)
CDynamicObject::CDynamicObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CDynamicObject::~CDynamicObject()
{
}

HRESULT Engine::CDynamicObject::Ready_GameObject()
{
	return S_OK;
}

HRESULT CDynamicObject::Ready_GameObject(const _ushort & wContainerIdx, const wstring & wstrBufferTag, const wstring & wstrName)
{
	FAILED_CHECK_RETURN(Add_Component(wContainerIdx, wstrBufferTag), E_FAIL);

	m_wstrName = wstrName;

	return S_OK;
}

_int Engine::CDynamicObject::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	m_pRenderer->Add_RenderGroup(RENDER_NONALPHA, this);

	return iExit;
}

_int Engine::CDynamicObject::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::LateUpdate_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return iExit;
}

void Engine::CDynamicObject::Render_GameObject(const _float& fTimeDelta)
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorldMatrix());
	//m_pMeshCom->Render_Meshes();

	LPD3DXEFFECT	pEffect = m_vecShaderCom[0]->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_uint	iPassMax = 0;

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	pEffect->Begin(&iPassMax, 0);

	if (m_bDissolve)
	{
		m_fDissolveTime += fTimeDelta;
		pEffect->SetFloat("g_fTime", m_fDissolveTime);
		m_pDissolveTextureCom->Render_Texture("g_DissolveTexture", pEffect);

		pEffect->BeginPass(2);
	}
	else
		pEffect->BeginPass(0);

	m_pMeshCom->Render_Meshes(pEffect);

	pEffect->EndPass();

	pEffect->End();

	Safe_Release(pEffect);
}

void CDynamicObject::Render_GameObject(const _float & fTimeDelta, const _uint & iIndex)
{
	LPD3DXEFFECT	pEffect = m_vecShaderCom[iIndex]->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_uint	iPassMax = 0;

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	pEffect->Begin(&iPassMax, 0);

	pEffect->BeginPass(0);

	m_pMeshCom->Render_Meshes(pEffect, MTT_END);

	pEffect->EndPass();

	pEffect->End();

	Safe_Release(pEffect);

}

void CDynamicObject::Move_Pos(const _vec3 * pPos)
{
	m_pTransformCom->Move_Pos(pPos);
}

void CDynamicObject::Set_Pos(const _vec3 * pPos)
{
	m_pTransformCom->Set_Pos(pPos);
}

void CDynamicObject::Set_Pos(const _float & fX, const _float & fY, const _float & fZ)
{
	m_pTransformCom->Set_Pos(fX, fY, fZ);
}

void CDynamicObject::Set_PosX(const _float & fX)
{
	m_pTransformCom->Set_PosX(fX);
}

void CDynamicObject::Set_PosY(const _float & fY)
{
	m_pTransformCom->Set_PosY(fY);
}

void CDynamicObject::Set_PosZ(const _float & fZ)
{
	m_pTransformCom->Set_PosZ(fZ);
}

HRESULT CDynamicObject::Add_Collider(const wstring & wstrTag, CCollider * pColl)
{
	NULL_CHECK_RETURN(pColl, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(wstrTag, pColl);
	

	return S_OK;
}

HRESULT CDynamicObject::Add_Component(const _ushort & wContainerIdx, const wstring & wstrBufferTag, const _ulong& dwFlags)
{
	CComponent* pCom = nullptr;

	pCom = m_pTransformCom = CTransform::Create(dwFlags);
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pCom);

	pCom = m_pRenderer = Get_Renderer();
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_pRenderer->AddRef();
	m_mapComponent[ID_STATIC].emplace(L"Com_Renderer", pCom);

	pCom = m_pMeshCom = dynamic_cast<CDynamicMesh*>(Clone(wContainerIdx, wstrBufferTag));
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pCom);

	CShader* pShader = nullptr;

	pCom = pShader = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Mesh"));
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pCom);
	m_vecShaderCom.push_back(pShader);

	pCom = pShader = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Shadow"));
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader_Shadow", pCom);
	m_vecShaderCom.push_back(pShader);

	pCom = m_pDissolveTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STAGE, L"Texture_Dissolve_Bits"));
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_DissolveTexture", pCom);

	return S_OK;
}

HRESULT CDynamicObject::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	Engine::SetUp_OnShader(pEffect, L"Target_LightDepth", "g_LightDepthTexture");

	_matrix			matRenderOffset, matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	//m_pTransformCom->Get_RenderMatrix(&matWorld);
	m_pTransformCom->Get_RenderOffset(&matRenderOffset);
	matWorld = matRenderOffset * matWorld;
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

	CCamera* pConCam = Get_Camera(CAM_STATIC, L"Cam_Light");
	NULL_CHECK_RETURN(pConCam, E_FAIL);

	_matrix matConView, matConProj, matConViewIn;
	pConCam->Get_View(&matConView);
	pConCam->Get_Projection(&matConProj);

	pEffect->SetMatrix("g_matLightView", &matConView);
	pEffect->SetMatrix("g_matLightProj", &matConProj);

	pEffect->SetMatrix("g_matPreWorld", m_pTransformCom->Get_PreWorldMatrix());
	pEffect->SetMatrix("g_matPreView", Engine::Get_MainCamera()->Get_PreView());

	pEffect->SetFloat("g_fRim", m_fRim);
	pEffect->SetFloat("g_fAlpha", 1.f);

	//const Engine::CLight* pLightClass = Engine::Get_Light(0);

	//_matrix matLightView, matLightProj;

	//pLightClass->Get_LightView(&matLightView);
	//pLightClass->Get_LightProj(&matLightProj);

	//pEffect->SetMatrix("g_matLightView", &matLightView);
	//pEffect->SetMatrix("g_matLightProj", &matLightProj);


	return S_OK;
}

HRESULT CDynamicObject::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _uint & iIndex)
{

	_matrix			matRenderOffset, matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	//m_pTransformCom->Get_RenderMatrix(&matWorld);
	m_pTransformCom->Get_RenderOffset(&matRenderOffset);
	matWorld = matRenderOffset * matWorld;
	pEffect->SetMatrix("g_matWorld", &matWorld);

	//const D3DLIGHT9*		pLight = Engine::Get_LightInfo(0);
	//NULL_CHECK_RETURN(pLight, S_OK);

	CCamera* pConCam = Get_Camera(CAM_STATIC, L"Cam_Light");
	NULL_CHECK_RETURN(pConCam, E_FAIL);

	_matrix matConView, matConProj, matConViewIn;
	pConCam->Get_View(&matConView);
	pConCam->Get_Projection(&matConProj);

	pEffect->SetMatrix("g_matLightView", &matConView);
	pEffect->SetMatrix("g_matLightProj", &matConProj);

	//const Engine::CLight* pLightClass = Engine::Get_Light(0);

	//_matrix matLightView, matLightProj;

	//pLightClass->Get_LightView(&matLightView);
	//pLightClass->Get_LightProj(&matLightProj);

	//pEffect->SetMatrix("g_matLightView", &matLightView);
	//pEffect->SetMatrix("g_matLightProj", &matLightProj);

	return S_OK;
}

CDynamicObject * CDynamicObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const wstring & wstrBufferTag, const wstring & wstrNameTag)
{
	CDynamicObject* pInst = new CDynamicObject(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject(wContainerIdx, wstrBufferTag, wstrNameTag)))
		Safe_Release(pInst);

	return pInst;
}

void CDynamicObject::Free()
{
	CGameObject::Free();
}

