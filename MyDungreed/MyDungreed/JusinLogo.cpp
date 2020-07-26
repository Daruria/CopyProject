#include "stdafx.h"
#include "JusinLogo.h"


CJusinLogo::CJusinLogo():
	m_bIntroSound(false)
{
}


CJusinLogo::~CJusinLogo()
{
}

void CJusinLogo::Initialize()
{
	INSERT_BITMAP("JusinLogo", L"Image/Logo/Logo.bmp");
}

int CJusinLogo::Update()
{	
	if (!m_bIntroSound)
	{
		GET_SINGLE(CSoundMgr)->PlaySound(L"IntroSound.wav", CSoundMgr::EFFECT);
		GET_SINGLE(CSoundMgr)->SetVolume(CSoundMgr::EFFECT, 0.6f);
		m_bIntroSound = true;
	}
	if (KEYUP(VK_RETURN))
		return SC_LOGO;

	return SC_NONE;
}

void CJusinLogo::LateUpdate()
{
}

void CJusinLogo::Render(HDC hDC)
{
	HDC hMemDC = FIND_BITMAP_DC("JusinLogo");

	SIZE tSize = GET_BITMAP_SIZE("JusinLogo");

	GdiTransparentBlt(hDC, 
		static_cast<int>(WINCX / 2 - tSize.cx * 0.5f), 
		static_cast<int>(WINCY / 2 - tSize.cy * 0.5f), 
		tSize.cx, tSize.cy, hMemDC, 0, 0, tSize.cx, tSize.cy, RGB(255, 0, 255));
}

void CJusinLogo::Release()
{
}
