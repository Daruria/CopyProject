#include "StaticObject.h"

#include "Renderer.h"
#include "Transform.h"
#include "Mesh.h"

#include "Export_Function.h"

CStaticObject::CStaticObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CGameObject(pGraphicDev)
{
}

CStaticObject::~CStaticObject()
{
}

HRESULT CStaticObject::Ready_GameObject()
{


	return S_OK;
}

HRESULT CStaticObject::Ready_GameObject(const _ushort & wContainerIdx, const wstring & wstrBufferTag, const wstring& wstrName)
{
	FAILED_CHECK_RETURN(Add_Component(wContainerIdx, wstrBufferTag), E_FAIL);

	m_wstrName = wstrName;

	CStaticMesh*	pStaticMesh = dynamic_cast<CStaticMesh*>(m_pMeshCom);
	m_fRadius = pStaticMesh->Get_Radius();
	pStaticMesh->Get_Center(&m_vCenter);


	return S_OK;
}

_int CStaticObject::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	m_pRenderer->Add_RenderGroup(RENDER_NONALPHA, this);

	return iExit;
}

_int CStaticObject::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::LateUpdate_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return iExit;
}

void CStaticObject::Render_GameObject(const _float& fTimeDelta)
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

	m_pMeshCom->Render_Meshes(pEffect);

	pEffect->End();

	Safe_Release(pEffect);
}

void CStaticObject::Render_GameObject(const _float & fTimeDelta, const _uint & iIndex)
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

	CCamera* pLightCam = Get_Camera(CAM_STATIC, L"Cam_Light");
	NULL_CHECK_RETURN(pLightCam, );

	if (!pLightCam->Is_InFrustum(&vCenter, fRadius))
		return;

	LPD3DXEFFECT	pEffect = m_vecShader[iIndex]->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_uint	iPassMax = 0;

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	pEffect->Begin(&iPassMax, 0);

	m_pMeshCom->Render_Meshes(pEffect, MTT_END);

	pEffect->End();

	Safe_Release(pEffect);

}

void CStaticObject::Move_Pos(const _vec3 * pPos)
{
	m_pTransformCom->Move_Pos(pPos);
}

void CStaticObject::Set_Pos(const _vec3 * pPos)
{
	m_pTransformCom->Set_Pos(pPos); 
}

void CStaticObject::Set_Pos(const _float & fX, const _float & fY, const _float & fZ)
{
	m_pTransformCom->Set_Pos(fX, fY, fZ);
}

void CStaticObject::Set_PosX(const _float & fX)
{ 
	m_pTransformCom->Set_PosX(fX);
}

void CStaticObject::Set_PosY(const _float & fY)
{ 
	m_pTransformCom->Set_PosY(fY);
}

void CStaticObject::Set_PosZ(const _float & fZ)
{
	m_pTransformCom->Set_PosZ(fZ);
}

void CStaticObject::Set_Scale(const _float & fScale)
{
	m_pTransformCom->Set_Scale(fScale);
}

HRESULT CStaticObject::Add_Collider(const wstring & wstrTag, CCollider * pColl)
{
	NULL_CHECK_RETURN(pColl, false);
	m_mapComponent[ID_DYNAMIC].emplace(wstrTag, pColl);

	return S_OK;
}

HRESULT CStaticObject::Add_Component(const _ushort& wContainerIdx, const wstring & wstrBufferTag, const _ulong& dwFlags)
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
	pCom = pShader = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Mesh"));
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pCom);
	m_vecShader.push_back(pShader);

	pCom = pShader = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Shadow"));
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader_Shadow", pCom);
	m_vecShader.push_back(pShader);

	return S_OK;
}

HRESULT CStaticObject::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
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

	tMtrlInfo.Diffuse	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Specular	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Ambient	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Emissive	= D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
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

	pEffect->SetMatrix("g_matPreWorld", m_pTransformCom->Get_PreWorldMatrix());
	pEffect->SetMatrix("g_matPreView", Engine::Get_MainCamera()->Get_PreView());

	pEffect->SetFloat("g_fAlpha", 1.f);
	pEffect->SetFloat("g_fRim", 0.f);
	pEffect->SetFloat("g_fCapture", 0.f);

	//pEffect->SetBool("g_bMotionBlur", false);

	return S_OK;
}

HRESULT CStaticObject::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _uint & iIndex)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);

	pEffect->SetMatrix("g_matWorld", &matWorld);


	CCamera* pLightCam = Get_Camera(CAM_STATIC, L"Cam_Light");
	NULL_CHECK_RETURN(pLightCam, E_FAIL);

	_matrix matLightView, matLightProj;
	pLightCam->Get_View(&matLightView);
	pLightCam->Get_Projection(&matLightProj);

	pEffect->SetMatrix("g_matLightView", &matLightView);
	pEffect->SetMatrix("g_matLightProj", &matLightProj);

	//	Light View, Pos, Proj Setting
	//const Engine::CLight* pLightClass = Engine::Get_Light(0);

	//_matrix matLightView, matLightProj, matLightViewIn;

	//pLightClass->Get_LightView(&matLightView);
	//pLightClass->Get_LightProj(&matLightProj);

	//D3DXMatrixInverse(&matLightViewIn, NULL, &matLightView);

	//pEffect->SetMatrix("g_matLightView", &matLightView);
	//pEffect->SetMatrix("g_matLightProj", &matLightProj);


	return S_OK;
}

CStaticObject * CStaticObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const wstring & wstrBufferTag, const wstring& wstrNameTag)
{
	CStaticObject*	pInst = new CStaticObject(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject(wContainerIdx, wstrBufferTag, wstrNameTag)))
		Safe_Release(pInst);

	return pInst;
}

void CStaticObject::Free()
{
	CGameObject::Free();
}

