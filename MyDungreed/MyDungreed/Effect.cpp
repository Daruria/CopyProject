#include "stdafx.h"
#include "Effect.h"
#include "ScrollManager.h"

CEffect::CEffect()
	: m_bLife(true),
	m_fRatio(1.f)
{
}


CEffect::~CEffect()
{
}

void CEffect::Initialize()
{
}

int CEffect::Update()
{
	return 0;
}

void CEffect::LateUpdate()
{
}

void CEffect::Render(HDC hDC)
{
}

void CEffect::Release()
{
}
