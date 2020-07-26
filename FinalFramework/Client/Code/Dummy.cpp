#include "stdafx.h"
#include "Dummy.h"

#include "Export_Function.h"
#include "Camera.h"

CDummy::CDummy(LPDIRECT3DDEVICE9 pGraphicDev)
	:	Engine::CGameObject(pGraphicDev)
{
}

CDummy::~CDummy()
{
}

HRESULT CDummy::Ready_GameObject(const wstring& wstrFilePath, const wstring& wstrFileName, const _ulong& dwFlags /*= 0*/)
{
	FAILED_CHECK_RETURN(Add_Component(wstrFilePath, wstrFileName, dwFlags), E_FAIL);

	return S_OK;
}

_int CDummy::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bIsDead)
		return 0;

	m_fLifeCount += fTimeDelta;
	if (m_fLifeCount > m_fLifeTime)
	{
		m_bIsDead = true;
		return 0;
	}

	if (m_fDeltaAlpha != 0.f)
	{
		m_fAlpha += m_fDeltaAlpha * fTimeDelta;

		if (m_fAlpha > 1.f)
		{
			m_fAlpha = 1.f;
			m_fDeltaAlpha = 0.f;
		}
		else if (m_fAlpha < 0.f)
		{
			m_fAlpha = 0.f;
			m_fDeltaAlpha = 0.f;
		}
	}

	if (m_fDeltaRim != 0.f)
	{
		m_fRim += m_fDeltaRim * fTimeDelta;

		if (m_fRim > 1.f)
		{
			m_fRim = 1.f;
			m_fDeltaRim = 0.f;
		}
		else if (m_fRim < 0.f)
		{
			m_fRim = 0.f;
			m_fDeltaRim = 0.f;
		}
	}


	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	m_pRenderer->Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return iExit;
}

_int CDummy::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::LateUpdate_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return iExit;

}

void CDummy::Render_GameObject(const _float& fTimeDelta)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_uint	iPassMax = 0;

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	pEffect->Begin(&iPassMax, 0);

	pEffect->BeginPass(0);

	m_pDynamicMeshCom->Render_Meshes_NoSkinning(pEffect);

	pEffect->EndPass();

	pEffect->End();

	::Safe_Release(pEffect);


}

void CDummy::SWSkinning(const _uint& iIndex, const _double& dItplTime, const _double& dPosition)
{
	m_pDynamicMeshCom->Set_Animation(iIndex, dItplTime, dPosition);

	m_pDynamicMeshCom->Set_SWSkinning();
}

void CDummy::Set_Transform(const _vec3 * pScale, const _vec3 * pAngle, const _vec3 * pPos)
{
	m_pTransformCom->Set_Scale(pScale);
	m_pTransformCom->Set_Angle(pAngle);
	m_pTransformCom->Set_Pos(pPos);
}


void CDummy::SetUp_Variables(const _float & fLifeTime, const _float & fAlpha, const _float& fRim, const _float & fDeltaAlpha, const _float& fDeltaRim)
{
	m_fLifeTime = fLifeTime;
	m_fAlpha = fAlpha;
	m_fRim = fRim;
	m_fDeltaAlpha = fDeltaAlpha;
	m_fDeltaRim = fDeltaRim;
}

void CDummy::Reset()
{
	m_bIsDead = false;

	m_fLifeCount = 0.f;
	m_fLifeTime = 0.f;
	m_fAlpha = 0.f;
	m_fRim = 0.f;
}

HRESULT CDummy::Add_Component(const wstring& wstrFilePath, const wstring& wstrFileName, const _ulong& dwFlags /*= 0*/)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = Engine::CTransform::Create(dwFlags);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	if (dwFlags | TS_RENDEROFFSET)
	{
		_matrix matRenderOffset;
		m_pTransformCom->Set_RenderOffset(D3DXMatrixRotationY(&matRenderOffset, D3DXToRadian(90.f)));
	}

	pComponent = m_pRenderer = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pRenderer->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);

	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Mesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pDynamicMeshCom = Engine::CDynamicMesh::Create(m_pGraphicDev, wstrFilePath, wstrFileName);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_DynamicMesh", pComponent);

	return S_OK;
}

HRESULT CDummy::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
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

	Engine::CCamera* pConCam = Engine::Get_Camera(Engine::CAM_STATIC, L"Cam_Light");
	NULL_CHECK_RETURN(pConCam, E_FAIL);

	_matrix matConView, matConProj, matConViewIn;
	pConCam->Get_View(&matConView);
	pConCam->Get_Projection(&matConProj);

	pEffect->SetMatrix("g_matLightView", &matConView);
	pEffect->SetMatrix("g_matLightProj", &matConProj);

	pEffect->SetMatrix("g_matPreWorld", m_pTransformCom->Get_PreWorldMatrix());
	pEffect->SetMatrix("g_matPreView", Engine::Get_MainCamera()->Get_PreView());

	pEffect->SetFloat("g_fRim", m_fRim);
	pEffect->SetFloat("g_fAlpha", m_fAlpha);

	return S_OK;
}

CDummy * CDummy::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrFilePath, const wstring& wstrFileName, const _ulong& dwFlags/* = 0*/)
{
	CDummy* pInstance = new CDummy(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(wstrFilePath, wstrFileName, dwFlags)))
		::Safe_Release(pInstance);

	return pInstance;
}

void CDummy::Free()
{
	Engine::CGameObject::Free();
}
