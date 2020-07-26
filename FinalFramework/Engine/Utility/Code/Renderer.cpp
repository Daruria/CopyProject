#include "Renderer.h"
#include "CameraMgr.h"
#include "Screen.h"

#include "Export_Function.h"

USING(Engine)
IMPLEMENT_SINGLETON(CRenderer)

Engine::CRenderer::CRenderer()
{

}

Engine::CRenderer::~CRenderer()
{
	Free();
}

HRESULT CRenderer::Ready_Renderer(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pGraphicDev = pGraphicDev;
	m_pGraphicDev->AddRef();

	D3DVIEWPORT9		ViewPort;
	pGraphicDev->GetViewport(&ViewPort);

	m_pBlendBuffer = CScreen::Create(m_pGraphicDev, 0.f, 0.f, static_cast<_float>(ViewPort.Width), static_cast<_float>(ViewPort.Height));
	NULL_CHECK_RETURN(m_pBlendBuffer, E_FAIL);

	m_pBlendShader = dynamic_cast<CShader*>(Clone_Prototype(L"Shader_Blend"));
	NULL_CHECK_RETURN(m_pBlendShader, E_FAIL);

	m_pLightShader = dynamic_cast<CShader*>(Clone_Prototype(L"Shader_Light"));
	NULL_CHECK_RETURN(m_pLightShader, E_FAIL);

	m_pBlurFilterBuffer = CScreen::Create(m_pGraphicDev, 0.f, 0.f, static_cast<_float>(ViewPort.Width), static_cast<_float>(ViewPort.Height));
	NULL_CHECK_RETURN(m_pBlurFilterBuffer, E_FAIL);

	m_pBlurFilterShader = dynamic_cast<CShader*>(Clone_Prototype(L"Shader_BlurFilter"));
	NULL_CHECK_RETURN(m_pBlurFilterShader, E_FAIL);

	m_pBlurBuffer = CScreen::Create(m_pGraphicDev, 0.f, 0.f, static_cast<_float>(ViewPort.Width) * 0.5f, static_cast<_float>(ViewPort.Height) * 0.5f);
	NULL_CHECK_RETURN(m_pBlurBuffer, E_FAIL);

	m_pBlurShader = dynamic_cast<CShader*>(Clone_Prototype(L"Shader_Blur"));
	NULL_CHECK_RETURN(m_pBlurShader, E_FAIL);

	m_pFinalBuffer = CScreen::Create(m_pGraphicDev, 0.f, 0.f, static_cast<_float>(ViewPort.Width), static_cast<_float>(ViewPort.Height));
	NULL_CHECK_RETURN(m_pFinalBuffer, E_FAIL);

	m_pFinalShader = dynamic_cast<CShader*>(Clone_Prototype(L"Shader_Final"));
	NULL_CHECK_RETURN(m_pFinalShader, E_FAIL);

	m_pPreDepthBuffer = CScreen::Create(m_pGraphicDev, 0.f, 0.f, static_cast<_float>(ViewPort.Width), static_cast<_float>(ViewPort.Height));
	NULL_CHECK_RETURN(m_pPreDepthBuffer, E_FAIL);

	m_pPreDepthShader = dynamic_cast<CShader*>(Clone_Prototype(L"Shader_PreDepth"));
	NULL_CHECK_RETURN(m_pPreDepthShader, E_FAIL);

	m_pBeforeBlendBuffer = CScreen::Create(m_pGraphicDev, 0.f, 0.f, static_cast<_float>(ViewPort.Width), static_cast<_float>(ViewPort.Height));
	NULL_CHECK_RETURN(m_pBeforeBlendBuffer, E_FAIL);

	m_pBeforeBlendShader = dynamic_cast<CShader*>(Clone_Prototype(L"Shader_BeforeBlend"));
	NULL_CHECK_RETURN(m_pBeforeBlendShader, E_FAIL);

#ifdef _DEBUG

	m_bColliderRender = false;
#endif // _DEBUG


	return S_OK;
}

void Engine::CRenderer::Add_RenderGroup(RENDERID eGroup, CGameObject* pGameObject)
{
	if (RENDER_END <= eGroup)
		return;

	m_RenderGroup[eGroup].push_back(pGameObject);

	pGameObject->AddRef();
}

void Engine::CRenderer::Render_GameObject(const _float& fTimeDelta)
{
	Render_Priority(fTimeDelta);

	Render_Deferred(fTimeDelta);

	Render_LightAcc(fTimeDelta);

	//Render_BeforeBlend(fTimeDelta);

	Render_Blend(fTimeDelta);

	Render_Final(fTimeDelta);

	Render_PreDepth(fTimeDelta);

	Render_Alpha(fTimeDelta);

	Render_UI(fTimeDelta);
#ifdef _DEBUG
	Render_Collider();
#endif

#ifdef _DEBUG
	Render_Collider();
#endif // _DEBUG

	if (KeyDown(DIK_8))
		m_bRenderTarget = !m_bRenderTarget;

	if (m_bRenderTarget)
	{
		//Engine::Render_DebugBuffer(L"MRT_Priority");
		//Engine::Render_DebugBuffer(L"MRT_Deferred");
		//Engine::Render_DebugBuffer(L"MRT_LightDepth");
		//Engine::Render_DebugBuffer(L"MRT_LightAcc");
		Engine::Render_DebugBuffer(L"MRT_BlurFilter");
		Engine::Render_DebugBuffer(L"MRT_HorizontalBlur");
		Engine::Render_DebugBuffer(L"MRT_VerticalBlur");
		//Engine::Render_DebugBuffer(L"MRT_PreData");
		//Engine::Render_DebugBuffer(L"MRT_Distortion");
		//Engine::Render_DebugBuffer(L"MRT_Blend");
		//Engine::Render_DebugBuffer(L"MRT_Effect");
		
		//Engine::Render_Font(L"Font_Second", L"Diffuse",				&_vec2(0.f, 0.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		//Engine::Render_Font(L"Font_Second", L"Normal",				&_vec2(0.f, 200.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		//Engine::Render_Font(L"Font_Second", L"Depth(Current)",		&_vec2(0.f, 400.f), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));

		//Engine::Render_Font(L"Font_Second", L"Shade",				&_vec2(200.f, 0.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		//Engine::Render_Font(L"Font_Second", L"Specular",			&_vec2(200.f, 200.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		//Engine::Render_Font(L"Font_Second", L"Shadow & BlurFilter", &_vec2(200.f, 400.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

		//Engine::Render_Font(L"Font_Second", L"Light Depth",			&_vec2(400.f, 0.f), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
		//Engine::Render_Font(L"Font_Second", L"Sky",					&_vec2(400.f, 200.f), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
		//Engine::Render_Font(L"Font_Second", L"Depth(Previous)",		&_vec2(400.f, 400.f), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));

		//Engine::Render_Font(L"Font_Second", L"Filtered",			&_vec2(600.f, 0.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		//Engine::Render_Font(L"Font_Second", L"Horizontal Blur",		&_vec2(600.f, 200.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		//Engine::Render_Font(L"Font_Second", L"Vertical Blur",		&_vec2(600.f, 400.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

		//Engine::Render_Font(L"Font_Second", L"Distortion",			&_vec2(800.f, 0.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		//Engine::Render_Font(L"Font_Second", L"Rim Light",			&_vec2(800.f, 200.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		//Engine::Render_Font(L"Font_Second", L"Glow Effect",			&_vec2(800.f, 400.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

		//Engine::Render_Font(L"Font_Second", L"Blend",				&_vec2(0.f, 600.f), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
		//Engine::Render_DebugBuffer(L"MRT_BeforeBlend");
	}

	//for (auto& iter : m_RenderGroup[RENDER_MOUSE])
	//	iter->Render_GameObject(fTimeDelta);

	Clear_RenderGroup();
}

void CRenderer::Render_Tool(const _float & fTimeDelta)
{
	Render_Priority(fTimeDelta);

	Render_NonAlpha(fTimeDelta);

	Render_Alpha(fTimeDelta);

	Render_UI(fTimeDelta);

	Clear_RenderGroup();
}

void Engine::CRenderer::Clear_RenderGroup()
{
	for (_uint i = 0; i < RENDER_END; ++i)
	{
		for_each(m_RenderGroup[i].begin(), m_RenderGroup[i].end(), CDeleteObj());
		m_RenderGroup[i].clear();
	}
}

void CRenderer::Render_Priority(const _float& fTimeDelta)
{
	Begin_MRT(L"MRT_Priority");

	for (auto& iter : m_RenderGroup[RENDER_PRIORITY])
		iter->Render_GameObject(fTimeDelta);

	End_MRT(L"MRT_Priority");
}

void CRenderer::Render_NonAlpha(const _float& fTimeDelta)
{
	for (auto& iter : m_RenderGroup[RENDER_NONALPHA])
		iter->Render_GameObject(fTimeDelta);
	
}

void CRenderer::Render_NonAlpha(const _float & fTimeDelta, const _uint & iIndex)
{
	for (auto& iter : m_RenderGroup[RENDER_NONALPHA])
		iter->Render_GameObject(fTimeDelta, iIndex);
}

void CRenderer::Render_Alpha(const _float& fTimeDelta)
{

	//m_RenderGroup[RENDER_ALPHA].sort([](CGameObject* pDest, CGameObject* pSour) 
	//{return pDest->Get_ViewZ() > pSour->Get_ViewZ();});
	
	for (auto& iter : m_RenderGroup[RENDER_ALPHA])
		iter->Render_GameObject(fTimeDelta);
}

void CRenderer::Render_UI(const _float& fTimeDelta)
{
	if (m_RenderGroup[RENDER_UI].empty())
		return;

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	CCameraMgr::GetInstance()->Update_OrthogonalCam();

	for (auto& iter : m_RenderGroup[RENDER_UI])
		iter->Render_GameObject(fTimeDelta);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void CRenderer::Render_Deferred(const _float & fTimeDelta)
{
	Begin_MRT(L"MRT_LightDepth");	//	빛 깊이 텍스쳐 생성.

	Render_NonAlpha(fTimeDelta, 1);

	End_MRT(L"MRT_LightDepth");

	m_pGraphicDev->Clear(0, nullptr,  D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DXCOLOR(0.f, 0.f, 0.f, 1.f), 1.f, 0);

	Begin_MRT(L"MRT_Deferred");	//	디퍼드 렌더링.

	Render_NonAlpha(fTimeDelta);
	
	End_MRT(L"MRT_Deferred");

	m_pGraphicDev->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DXCOLOR(0.f, 0.f, 0.f, 1.f), 1.f, 0);


	Begin_MRT(L"MRT_Effect");	//	Effect Rendering, GlowEffect만 일단 처리중.

	Render_Effect(fTimeDelta);

	End_MRT(L"MRT_Effect");


	Begin_MRT(L"MRT_BlurFilter");

	Render_BlurFilter(fTimeDelta);

	End_MRT(L"MRT_BlurFilter");

	//m_pGraphicDev->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DXCOLOR(0.f, 0.f, 0.f, 1.f), 1.f, 0);
	Begin_MRT(L"MRT_HorizontalBlur");

	Render_Blur(fTimeDelta, L"Target_BlurFilter", 0);

	End_MRT(L"MRT_HorizontalBlur");

	//m_pGraphicDev->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DXCOLOR(0.f, 0.f, 0.f, 1.f), 1.f, 0);
	Begin_MRT(L"MRT_VerticalBlur");

	Render_Blur(fTimeDelta, L"Target_HorizontalBlur", 1);

	End_MRT(L"MRT_VerticalBlur");

	m_pGraphicDev->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DXCOLOR(0.f, 0.f, 0.f, 1.f), 1.f, 0);

	Begin_MRT(L"MRT_Distortion");

	for (auto& iter : m_RenderGroup[RENDER_DISTORTION])
		iter->Render_GameObject(fTimeDelta);

	End_MRT(L"MRT_Distortion");

}

void CRenderer::Render_LightAcc(const _float & fTimeDelta)
{
	Begin_MRT(L"MRT_LightAcc");

	LPD3DXEFFECT pEffect = m_pLightShader->Get_EffectHandle();
	pEffect->AddRef();

	Engine::SetUp_OnShader(pEffect, L"Target_Normal",	"g_NormalTexture");
	Engine::SetUp_OnShader(pEffect, L"Target_Depth",	"g_DepthTexture");
	Engine::SetUp_OnShader(pEffect, L"Target_Shadow",	"g_ShadowTexture");

	pEffect->Begin(NULL, 0);

	Engine::Render_Light(pEffect, 0);

	pEffect->End();

	End_MRT(L"MRT_LightAcc");

	Safe_Release(pEffect);
}

void CRenderer::Render_BlurFilter(const _float & fTimeDelta)
{
	LPD3DXEFFECT pEffect = m_pBlurFilterShader->Get_EffectHandle();
	pEffect->AddRef();

	Engine::SetUp_OnShader(pEffect, L"Target_Albedo",		"g_AlbedoTexture");
	Engine::SetUp_OnShader(pEffect, L"Target_Shadow",		"g_ShadowTexture");
	Engine::SetUp_OnShader(pEffect, L"Target_Depth",		"g_DepthTexture");
	Engine::SetUp_OnShader(pEffect, L"Target_GlowEffect",	"g_GlowEffectTexture");

	pEffect->Begin(NULL, 0);

	pEffect->BeginPass(0);

	m_pBlurFilterBuffer->Render_Buffer();

	pEffect->EndPass();

	pEffect->End();

	Safe_Release(pEffect);
}

void CRenderer::Render_Blur(const _float & fTimeDelta, const wstring& wstrTarget, const _uint& iPass)
{
	LPD3DXEFFECT pEffect = m_pBlurShader->Get_EffectHandle();
	pEffect->AddRef();
	
	D3DVIEWPORT9		ViewPort;
	m_pGraphicDev->GetViewport(&ViewPort);
	pEffect->SetFloat("g_fScreeWidth",	static_cast<_float>(ViewPort.Width) * 0.5f);
	pEffect->SetFloat("g_fScreeHeight", static_cast<_float>(ViewPort.Height) * 0.5f);
	
	Engine::SetUp_OnShader(pEffect, wstrTarget, "g_FilteredTexture");

	pEffect->Begin(NULL, 0);

	pEffect->BeginPass(iPass);

	m_pBlurBuffer->Render_Buffer();

	pEffect->EndPass();

	pEffect->End();

	Safe_Release(pEffect);
}

void CRenderer::Render_BeforeBlend(const _float & fTimeDelta)
{
	Engine::Begin_MRT(L"MRT_BeforeBlend");

	LPD3DXEFFECT pEffect = m_pBeforeBlendShader->Get_EffectHandle();
	pEffect->AddRef();


	Engine::SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture");
	Engine::SetUp_OnShader(pEffect, L"Target_PreDepth", "g_PreDepthTexture");
	Engine::SetUp_OnShader(pEffect, L"Target_PreCapture", "g_CaptureTexture");

	D3DVIEWPORT9		ViewPort;
	m_pGraphicDev->GetViewport(&ViewPort);
	pEffect->SetFloat("g_fScreeWidth", static_cast<_float>(ViewPort.Width) * 0.5f);
	pEffect->SetFloat("g_fScreeHeight", static_cast<_float>(ViewPort.Height) * 0.5f);

	_matrix	matView, matProj, matPreView, matViewInv, matProjInv;

	CCamera* pMainCam = Engine::Get_MainCamera();
	if (!pMainCam)
	{
		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
		matPreView = matView;
	}
	else
	{
		Engine::Get_MainCamera()->Get_View(&matView);
		Engine::Get_MainCamera()->Get_Projection(&matProj);
		Engine::Get_MainCamera()->Get_PreView(&matPreView);
	}

	D3DXMatrixInverse(&matViewInv, NULL, &matView);
	D3DXMatrixInverse(&matProjInv, NULL, &matProj);
	D3DXMatrixInverse(&matPreView, NULL, &matPreView);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetMatrix("g_matViewInv", &matViewInv);
	pEffect->SetMatrix("g_matProjInv", &matProjInv);
	pEffect->SetMatrix("g_matPreViewInv", &matPreView);


	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	m_pBeforeBlendBuffer->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

	Engine::End_MRT(L"MRT_BeforeBlend");
}

void CRenderer::Render_Blend(const _float & fTimeDelta)
{
	Engine::Begin_MRT(L"MRT_Blend");

	LPD3DXEFFECT pEffect = m_pBlendShader->Get_EffectHandle();
	pEffect->AddRef();

	Engine::SetUp_OnShader(pEffect, L"Target_Albedo",			"g_AlbedoTexture");
	Engine::SetUp_OnShader(pEffect, L"Target_Light",			"g_LightTexture");
	Engine::SetUp_OnShader(pEffect, L"Target_Specular",			"g_SpecularTexture");
	Engine::SetUp_OnShader(pEffect, L"Target_Sky",				"g_PriorityTexture");
	Engine::SetUp_OnShader(pEffect, L"Target_HorizontalBlur",	"g_GlowTexture");
	Engine::SetUp_OnShader(pEffect, L"Target_RimLight",			"g_RimTexture");
	//Engine::SetUp_OnShader(pEffect, L"Target_Shadow",			"g_ShadowTexture");
	//Engine::SetUp_OnShader(pEffect, L"Target_PreCapture",		"g_CaptureTexture");

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	m_pBlendBuffer->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

	Engine::End_MRT(L"MRT_Blend");
}

void CRenderer::Render_PreDepth(const _float & fTimeDelta)
{
	Begin_MRT(L"MRT_PreData");

	LPD3DXEFFECT pEffect = m_pPreDepthShader->Get_EffectHandle();
	pEffect->AddRef();

	Engine::SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture");
	Engine::SetUp_OnShader(pEffect, L"Target_Capture", "g_CaptureTexture");

	pEffect->Begin(NULL, 0);

	pEffect->BeginPass(0);

	m_pPreDepthBuffer->Render_Buffer();

	pEffect->EndPass();

	pEffect->End();

	Safe_Release(pEffect);

	End_MRT(L"MRT_PreData");
}

void CRenderer::Render_Final(const _float & fTimeDelta)
{
	LPD3DXEFFECT pEffect = m_pFinalShader->Get_EffectHandle();
	pEffect->AddRef();

	Engine::SetUp_OnShader(pEffect, L"Target_Blend", "g_BlendTexture");
	Engine::SetUp_OnShader(pEffect, L"Target_Shadow", "g_ShadowTexture");
	Engine::SetUp_OnShader(pEffect, L"Target_PreDepth", "g_PreDepthTexture");
	Engine::SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture");
	Engine::SetUp_OnShader(pEffect, L"Target_Distortion", "g_DistortionTexture");
	Engine::SetUp_OnShader(pEffect, L"Target_DepthDifference", "g_DepthDifferenceTexture");

	D3DVIEWPORT9		ViewPort;
	m_pGraphicDev->GetViewport(&ViewPort);
	pEffect->SetFloat("g_fScreeWidth",	static_cast<_float>(ViewPort.Width) * 0.5f);
	pEffect->SetFloat("g_fScreeHeight", static_cast<_float>(ViewPort.Height) * 0.5f);
	if (m_iMotionBlur > 0)
		pEffect->SetBool("g_bBlur", true);
	else
		pEffect->SetBool("g_bBlur", false);

	_matrix	matView, matProj, matPreView, matViewInv, matProjInv;

	CCamera* pMainCam = Engine::Get_MainCamera();
	if (!pMainCam)
	{
		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
		matPreView = matView;
	}
	else
	{
		Engine::Get_MainCamera()->Get_View(&matView);
		Engine::Get_MainCamera()->Get_Projection(&matProj);
		Engine::Get_MainCamera()->Get_PreView(&matPreView);
	}

	D3DXMatrixInverse(&matViewInv, NULL, &matView);
	D3DXMatrixInverse(&matProjInv, NULL, &matProj);
	D3DXMatrixInverse(&matPreView, NULL, &matPreView);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetMatrix("g_matViewInv", &matViewInv);
	pEffect->SetMatrix("g_matProjInv", &matProjInv);
	pEffect->SetMatrix("g_matPreViewInv", &matPreView);

	

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	m_pFinalBuffer->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

void CRenderer::Render_Effect(const _float & fTimeDelta)
{
	for (auto& iter : m_RenderGroup[RENDER_EFFECT])
		iter->Render_GameObject(fTimeDelta);
}

void Engine::CRenderer::Free(void)
{
	Clear_RenderGroup();

	Safe_Release(m_pBeforeBlendBuffer);
	Safe_Release(m_pBeforeBlendShader);
	Safe_Release(m_pPreDepthBuffer);
	Safe_Release(m_pPreDepthShader);
	Safe_Release(m_pFinalBuffer);
	Safe_Release(m_pFinalShader);
	Safe_Release(m_pBlurFilterBuffer);
	Safe_Release(m_pBlurFilterShader);
	Safe_Release(m_pBlurShader);
	Safe_Release(m_pBlurBuffer);
	Safe_Release(m_pLightShader);
	Safe_Release(m_pBlendShader);
	Safe_Release(m_pBlendBuffer);
	Safe_Release(m_pGraphicDev);
}

#ifdef _DEBUG

void CRenderer::Add_RenderCollider(CCollider * pCollider)
{
	m_ColliderList.push_back(pCollider);

	pCollider->AddRef();
}

void CRenderer::Clear_RenderCollider()
{
	for_each(m_ColliderList.begin(), m_ColliderList.end(), CDeleteObj());
	m_ColliderList.clear();
}

void CRenderer::Render_Collider()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	if (KeyDown(DIK_9))
		m_bColliderRender = !m_bColliderRender;

	if (m_bColliderRender)
	{
		for (auto& pColl : m_ColliderList)
			pColl->Render_Collider();
	}
	Clear_RenderCollider();
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}
#endif // _DEBUG

