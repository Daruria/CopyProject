#include "stdafx.h"
#include "GameObject.h"


CGameObject::CGameObject():
	m_eRenderType(RENDER_END),
	m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance())
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
}


CGameObject::~CGameObject()
{
}
