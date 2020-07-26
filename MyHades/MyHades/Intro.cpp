#include "stdafx.h"
#include "Intro.h"
CIntro::CIntro():
	m_fLoadingRatio(0.f)
{
}


CIntro::~CIntro()
{
	Release();
}

int CIntro::Update()
{

	m_fLoadingRatio = m_pTextureMgr->GetLoadingRate();

	if (m_fLoadingRatio > 1.f)
		m_fLoadingRatio = 1.f;

	if (m_pTextureMgr->GetLoadinSuccess())
	{
		//if (m_pKeyMgr->KeyDown(KEY_ENTER))
		{
			m_pSceneMgr->SceneChange(SCENE_INTRO, SCENE_MAIN_TITLE);
			return CHANGE_SCENE;
		}
	}

	return NO_EVENT;
}

void CIntro::LateUpdate()
{
}

void CIntro::Render()
{
	//m_pDeviceMgr->Render_Begin();

	
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);

	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"load_bg_blank");
	NULL_CHECK_MSG(pTexInfo, L"load_bg_blank TexInfo is null");

	D3DXVECTOR3 vCenter = {pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height* 0.5f, 0.f};
	D3DXVECTOR3 vPos = {WINCX * 0.5f, WINCY*0.5f, 0.f};
	
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));

	pTexInfo = m_pTextureMgr->GetTexInfo(L"load_bar");
	NULL_CHECK_MSG(pTexInfo, L"load_bar TexInfo is null");

	vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f };
	vPos = { WINCX * 0.5f, WINCY * 0.5f, 0.f };
	RECT rc = {0, 0, static_cast<LONG>(pTexInfo->tImgInfo.Width * m_fLoadingRatio), pTexInfo->tImgInfo.Height};

	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, &rc, &vCenter, &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));

	//m_pDeviceMgr->Render_End();
}

HRESULT CIntro::Initialize()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CDeviceMgr::GetInstance()->GetDevice();

	HRESULT hr = CTextureMgr::GetInstance()->LoadTexture(pGraphicDev, TEXTURE_SINGLE, L"../Texture/Launch/load_bg_blank/load_bg_blank.png", L"load_bg_blank");
	FAILED_CHECK_MSG_RETURN(hr, L"load_bg_blank LoadTexture Failed", E_FAIL);

	hr = CTextureMgr::GetInstance()->LoadTexture(pGraphicDev, TEXTURE_SINGLE, L"../Texture/Launch/load_bar/load_bar.png", L"load_bar");
	FAILED_CHECK_MSG_RETURN(hr, L"load_bar LoadTexture Failed", E_FAIL);

	m_hLoadingThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingThreadFunc, nullptr, 0, nullptr);
	NULL_CHECK_RETURN(m_hLoadingThread, E_FAIL);


	return S_OK;
}

void CIntro::Release()
{
	if (m_hLoadingThread)
		CloseHandle(m_hLoadingThread);
}

unsigned CIntro::LoadingThreadFunc(void* pParam)
{
	LPDIRECT3DDEVICE9 pGraphicDev = CDeviceMgr::GetInstance()->GetDevice();

	//HRESULT hr = CTextureMgr::GetInstance()->LoadTextureFromPathInfo(pGraphicDev, L"../Data/PathInfo_NoAlecto.txt");
	//FAILED_CHECK_MSG_RETURN(hr, L"Loading Failed", 1);

	HRESULT hr = CTextureMgr::GetInstance()->LoadTextureFromPathInfo(pGraphicDev, L"../Data/PathInfo.txt");
	FAILED_CHECK_MSG_RETURN(hr, L"Loading Failed", 1);

	return 0;
}