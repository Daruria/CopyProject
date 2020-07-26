#include "stdafx.h"
#include "MainTitle.h"
#include "AnimatedObject.h"
#include "NonAnimatedObject.h"
#include "MyButton.h"

CMainTitle::CMainTitle():
	m_bLogoEnd(false),
	m_pMainBG(nullptr),
	m_pTitle(nullptr),
	m_pPlayButton(nullptr),
	m_pEndButton(nullptr)
{
}


CMainTitle::~CMainTitle()
{
	Release();
}

int CMainTitle::Update()
{
	m_pMainBG->Update();
	if (m_pMainBG->GetOnceFinish())
	{
		m_pMainBG->SetState(L"MainMenuLoopHalf", ANIM_REPEAT, 1/30.f);
		m_bLogoEnd = true;
	}

	if (m_pPlayButton->Update())
	{
		if (m_pKeyMgr->KeyDown(KEY_LBUTTON))
		{
			m_pSceneMgr->SceneChange(SCENE_MAIN_TITLE, SCENE_STAGE1);
			return	CHANGE_SCENE;
		}
		
	}
	if (m_pEndButton->Update())
	{
		if (m_pKeyMgr->KeyDown(KEY_LBUTTON))
		{
			m_pSceneMgr->SceneChange(SCENE_MAIN_TITLE, SCENE_END);
			return	CHANGE_SCENE;
		}
	}
		
	return NO_EVENT;
}

void CMainTitle::LateUpdate()
{
	m_pMainBG->LateUpdate();
	m_pTitle->LateUpdate();
	m_pPlayButton->LateUpdate();
	m_pEndButton->LateUpdate();
	
}

void CMainTitle::Render()
{
	m_pMainBG->Render();
	
	if (!m_bLogoEnd)
		return;
			
	m_pTitle->Render();
	m_pPlayButton->Render();
	m_pEndButton->Render();
}

HRESULT CMainTitle::Initialize()
{
	m_pMainBG = CAnimatedObject::Create(L"MainMenu", WINCX * 0.5f, WINCY* 0.5f, 1.f);
	m_pMainBG->SetState(L"MainMenuInHalf", ANIM_ONCE, 1/30.f);
	m_pMainBG->SetRenderType(RENDER_BACK_GROUND);

	m_pTitle = CNonAnimatedObject::Create(L"MainMenuLogo", L"", D3DXVECTOR3{ 190.f, 95.f, 0.f }, D3DXVECTOR3{0.5f, 0.5f, 0.f}, false);
	m_pTitle->SetRenderType(RENDER_OBJECT);

	m_pPlayButton = CMyButton::Create(CMyButton::IMAGE_ONLY, L"PlayButton", 100.f, WINCY * 0.5f);
	m_pPlayButton->SetRenderType(RENDER_UI);

	m_pEndButton = CMyButton::Create(CMyButton::IMAGE_ONLY, L"EndButton", 85.f, WINCY* 0.5f + 100);
	m_pEndButton->SetRenderType(RENDER_UI);

	CSoundMgr::GetInstance()->PlayBGM(L"01 No Escape 44khz 16bit HDK MSTR.wav");
	CSoundMgr::GetInstance()->SetVolume(CSoundMgr::BGM, 0.5f);

	CSoundMgr::GetInstance()->PlaySound(L"StabSplatter2.wav", CSoundMgr::EFFECT);
	CSoundMgr::GetInstance()->SetVolume(CSoundMgr::EFFECT, 4.f);

	return S_OK;
}

void CMainTitle::Release()
{
	CSoundMgr::GetInstance()->StopAll();

	SafeDelete(m_pEndButton);
	SafeDelete(m_pPlayButton);
	SafeDelete(m_pTitle);
	SafeDelete(m_pMainBG);
	m_pTextureMgr->ReleaseTexture(TEXTURE_MULTI, L"MainMenu");
}
