#include "Management.h"

#include "Export_Function.h"

USING(Engine)
IMPLEMENT_SINGLETON(CManagement)

Engine::CManagement::CManagement()
{

}

Engine::CManagement::~CManagement()
{
	Free();
}

const CComponent * CManagement::Get_Component(const wstring & wstrLayerTag, const wstring & wstrObjTag, const wstring & wstrComponentTag, COMPONENTID eID)
{
	return m_pScene->Get_Component(wstrLayerTag, wstrObjTag, wstrComponentTag, eID);
}

const CLayer * CManagement::Get_Layer(const wstring & wstrLayerTag) const
{
	return m_pScene->Get_Layer(wstrLayerTag);
}

HRESULT CManagement::Ready_Management(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	D3DVIEWPORT9		ViewPort;
	pGraphicDev->GetViewport(&ViewPort);

	//	첫번째 열
	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_Albedo", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_Albedo", 0.f, 0.f, 200.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_Normal", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_Normal", 0.f, 200.f, 200.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_Depth", ViewPort.Width, ViewPort.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(1.f, 1.f, 0.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_Depth", 0.f, 400.f, 200.f, 200.f), E_FAIL);


	//	두번째 열
	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_Light", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_Light", 200.f, 0.f, 200.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_Specular", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_Specular", 200.f, 200.f, 200.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_Shadow", ViewPort.Width, ViewPort.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_Shadow", 200.f, 400.f, 200.f, 200.f), E_FAIL);


	//	세번째 열
	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_LightDepth", ViewPort.Width, ViewPort.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(1.f, 1.f, 1.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_LightDepth", 400.f, 0.f, 200.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_Sky", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_Sky", 400.f, 200.f, 200.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_PreDepth", ViewPort.Width, ViewPort.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(1.f, 1.f, 0.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_PreDepth", 400.f, 400.f, 200.f, 200.f), E_FAIL);

	//	네번째 열
	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_BlurFilter", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_BlurFilter", 600.f, 0.f, 400.f, 400.f), E_FAIL);

	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_HorizontalBlur", ViewPort.Width / 2, ViewPort.Height / 2, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_HorizontalBlur", 600.f, 400.f, 400.f, 400.f), E_FAIL);

	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_VerticalBlur", ViewPort.Width / 2, ViewPort.Height / 2, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_VerticalBlur", 1000.f, 400.f, 400.f, 400.f), E_FAIL);



	//	왼쪽 아래 큰 블렌드
	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_Blend", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_Blend", 0.f, 600.f, 400.f, 400.f), E_FAIL);
	
	//	
	//FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_Capture", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_Capture", 400.f, 600.f, 200.f, 200.f), E_FAIL);
	//
	//FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_DepthDifference", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_DepthDifference", 400.f, 800.f, 200.f, 200.f), E_FAIL);


	//FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_PreCapture", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_PreCapture", 600.f, 600.f, 200.f, 200.f), E_FAIL);

	//	다섯번째 열
	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_Distortion", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_Distortion", 800.f, 0.f, 200.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_RimLight", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_RimLight", 800.f, 200.f, 200.f, 200.f), E_FAIL);

	//	
	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_GlowEffect", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_GlowEffect", 800.f, 400.f, 200.f, 200.f), E_FAIL);

	//FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_Glow", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_Glow", 600.f, 400.f, 600.f, 600.f), E_FAIL);

	//FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_VerticalBlur", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_VerticalBlur", 600.f, 200.f, 200.f, 200.f), E_FAIL);

	//FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_Blur", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_Blur", 600.f, 400.f, 200.f, 200.f), E_FAIL);


	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_Priority", L"Target_Sky"), E_FAIL);

	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_LightDepth", L"Target_LightDepth"), E_FAIL);

	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_Deferred", L"Target_Albedo"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_Deferred", L"Target_Normal"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_Deferred", L"Target_Shadow"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_Deferred", L"Target_Depth"), E_FAIL);

	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_LightAcc", L"Target_Light"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_LightAcc", L"Target_Specular"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_LightAcc", L"Target_RimLight"), E_FAIL);

	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_Effect", L"Target_GlowEffect"), E_FAIL);

	//FAILED_CHECK_RETURN(Ready_MRT(L"MRT_BeforeBlend", L"Target_DepthDifference"), E_FAIL);

	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_Blend", L"Target_Blend"), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_MRT(L"MRT_Blend", L"Target_Capture"), E_FAIL);

	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_PreData", L"Target_PreDepth"), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_MRT(L"MRT_PreData", L"Target_PreCapture"), E_FAIL);

	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_Distortion", L"Target_Distortion"), E_FAIL);

	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_BlurFilter", L"Target_BlurFilter"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_HorizontalBlur", L"Target_HorizontalBlur"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_VerticalBlur", L"Target_VerticalBlur"), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_MRT(L"MRT_Blur", L"Target_VerticalBlur"), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_MRT(L"MRT_Blur", L"Target_Blur"), E_FAIL);


	CShader*		pShader = nullptr;

	pShader = CShader::Create(pGraphicDev, L"../Bin/Shader/Shader_Sample.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Shader_Sample", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../Bin/Shader/Shader_Terrain.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Shader_Terrain", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../Bin/Shader/Shader_Mesh.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Shader_Mesh", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../Bin/Shader/Shader_Light.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Shader_Light", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../Bin/Shader/Shader_Blend.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Shader_Blend", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../Bin/Shader/Shader_Effect.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Shader_Effect", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../Bin/Shader/Shader_Portal.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Shader_Portal", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../Bin/Shader/Shader_Shadow.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Shader_Shadow", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../Bin/Shader/Shader_Sky.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Shader_Sky", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../Bin/Shader/Shader_Fire.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Shader_Fire", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../Bin/Shader/Shader_Blur.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Shader_Blur", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../Bin/Shader/Shader_BlurFilter.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Shader_BlurFilter", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../Bin/Shader/Shader_Glow.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Shader_Glow", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../Bin/Shader/Shader_Final.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Shader_Final", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../Bin/Shader/Shader_PreDepth.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Shader_PreDepth", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../Bin/Shader/Shader_BeforeBlend.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Shader_BeforeBlend", pShader), E_FAIL);

	//pShader = CShader::Create(pGraphicDev, L"../Bin/Shader/Shader_Particle.hpp");
	//NULL_CHECK_RETURN(pShader, E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Shader_Particle", pShader), E_FAIL);

	return S_OK;
}

HRESULT Engine::CManagement::SetUp_Scene(CScene* pScene)
{
	Safe_Release(m_pScene);
	
	Get_Renderer()->Clear_RenderGroup();

	m_pScene = pScene;

	return S_OK;
}

_int Engine::CManagement::Update_Scene(const _float& fTimeDelta)
{
	if (nullptr == m_pScene)
		return -1;

	_int iExit = m_pScene->Update_Scene(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return iExit;
}

_int CManagement::LateUpdate_Scene(const _float & fTimeDelta)
{
	if (nullptr == m_pScene)
		return -1;

	_int iExit = m_pScene->LateUpdate_Scene(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return iExit;
}

void Engine::CManagement::Render_Scene(const _float& fTimeDelta)
{
	Get_Renderer()->Render_GameObject(fTimeDelta);

	if (nullptr == m_pScene)
		return;

	m_pScene->Render_Scene(fTimeDelta);
}

HRESULT CManagement::Add_GameObject(const wstring& wstrLayerTag, const wstring& wstrObjTag, CGameObject * pObj)
{
	if (nullptr == m_pScene)
		E_FAIL;

	return m_pScene->Add_GameObject(wstrLayerTag, wstrObjTag, pObj);
}

HRESULT CManagement::Add_LoadingLayer(const wstring & wstrLayerTag, CLayer * pLayer)
{
	if (!pLayer)
		return E_FAIL;

	auto iter = m_mapLoadingLayer.find(wstrLayerTag);

	if (m_mapLoadingLayer.end() != iter)
		return E_FAIL;

	m_mapLoadingLayer.insert(make_pair(wstrLayerTag, pLayer));

	return S_OK;
}

HRESULT CManagement::Loading_Layer(map<wstring, CLayer*>* pmapLoadingLayer)
{
	for (auto& Pair : m_mapLoadingLayer)
	{
		pmapLoadingLayer->insert(make_pair(Pair.first, Pair.second));
		Pair.second->AddRef();
	}

	Clear_LoadingLayer();
	return S_OK;
}

HRESULT CManagement::Clear_LoadingLayer()
{
	for_each(m_mapLoadingLayer.begin(), m_mapLoadingLayer.end(), CDeleteMap());
	m_mapLoadingLayer.clear();

	return S_OK;
}

void CManagement::Die_GameObject(const wstring & wstrLayerTag, const wstring & wstrObjTag)
{
	m_pScene->Die_GameObject(wstrLayerTag, wstrObjTag);
}

void Engine::CManagement::Free(void)
{
	Clear_LoadingLayer();
	Safe_Release(m_pScene);
}