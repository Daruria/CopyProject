#include "stdafx.h"
#include "EffectImp.h"


CEffectImp::CEffectImp():
	m_pTimeMgr(CTimeMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pDeviceMgr(CDeviceMgr::GetInstance())
{
}


CEffectImp::~CEffectImp()
{
}
