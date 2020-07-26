#include "stdafx.h"
#include "DamageFont.h"
#include "Observer.h"
#include "UI.h"

#include "Export_Function.h"

#define	DMG_FONT_TEXTURE_X	795
#define	DMG_FONT_TEXTURE_Y	161

#define	DMG_FONT_TOP		31
#define	DMG_FONT_BOTTOM		137
#define DMG_FONT_SIZE_Y		106

int g_iFontStart[10]	= {	52, 123, 200, 269, 331, 408, 470, 541, 608, 674 };
int g_iFontSize[10]		= {	71, 57, 69, 62, 77, 62, 71, 67, 66, 69 };

CDamageFont::CDamageFont(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
{
}

CDamageFont::~CDamageFont()
{
}

HRESULT CDamageFont::Ready_GameObject(const _uint& iNumber)
{
	FAILED_CHECK_RETURN(Add_Component(Engine::RESOURCE_STATIC, L"Buffer_RcColTex", L"Texture_DamageFont"), E_FAIL);

	m_uiRcPos = Engine::CRcAlphaTex::RTS_LEFT_TOP;

	if (iNumber == 0)
		m_DigitList.push_front(iNumber);
	else
	{
		for (_uint i = iNumber; i > 0; i /= 10)
			m_DigitList.push_front(i % 10);
	}

	m_iDamage = iNumber;

	m_pTransformCom->Set_Scale((_float)DMG_FONT_TEXTURE_X, (_float)DMG_FONT_TEXTURE_Y, 1.f);
	

	return S_OK;
}


_int CDamageFont::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bIsDead)
		return 0;

	m_fLifeCount += fTimeDelta;

	if (m_fLifeCount >= m_fLifeTime)
	{
		m_bIsDead = true;
		return 0;
	}



	switch (m_eState)
	{
	case CDamageFont::EFontState::Scaling:
		m_fSpeed += m_fDeltaSpeed * fTimeDelta;
		m_fScale += fTimeDelta * m_fSpeed;

		if (m_fSpeed <= 0.f && m_fScale < m_fEndScale)
		{
			m_fScale = m_fEndScale;
			m_eState = EFontState::ScaleStop;
		}
		break;
	case CDamageFont::EFontState::ScaleStop:

		break;
	}

	_int iExit = CUI::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);

	return 0;
}

_int CDamageFont::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_int iExit = CUI::LateUpdate_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return 0;
}

void CDamageFont::Render_GameObject(const _float& fTimeDelta)
{
	_matrix matWorld;
	_vec3 vScale;
	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pTransformCom->Get_Scale(&vScale);

	m_pTextureCom->Render_Texture();

	_float fTotal = 0.f;
	for (auto& iDigit : m_DigitList)
		fTotal += g_iFontSize[iDigit] * m_fScale;
	
	_float	fStartX = matWorld._41 - fTotal * 0.5f;

	matWorld._41 = fStartX;
	for (auto& iDigit : m_DigitList)
	{
		_matrix matScale;
		D3DXMatrixScaling(&matScale, g_iFontSize[iDigit] * m_fScale, DMG_FONT_TEXTURE_Y * m_fScale, 1.f);
		memcpy(matScale.m[3], matWorld.m[3], sizeof(_vec3));

		Render_Digit(&matScale, iDigit);
		matWorld._41 += g_iFontSize[iDigit] * m_fScale;
		//Render_Digit();
	}
}

void CDamageFont::SetUp_Scaling(const _float & fStartScale, const _float & fMaxScale, const _float & fEndScale, const _float& fSpeed)
{
	m_fScale = fStartScale;
	m_fMaxScale = fMaxScale;
	m_fEndScale = fEndScale;
	m_fSpeed = fSpeed;
}

void CDamageFont::SetUp_ScalingDelta(const _float & fStartScale, const _float& fEndScale, const _float & fSpeed, const _float & fDeltaSpeed)
{
	m_fScale = fStartScale;
	m_fEndScale = fEndScale;
	m_fSpeed = fSpeed;
	m_fDeltaSpeed = fDeltaSpeed;
}

void CDamageFont::Render_Digit(const _matrix * pWorldMatrix, const _uint& iDigit)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, pWorldMatrix);

	m_pBufferCom->Set_Color(m_dwColor);
	//m_pBufferCom->Set_LocalPos((Engine::CRcAlphaTex::RC_TEX_POS)m_uiRcPos);
	//m_pBufferCom->Set_UV((Engine::CRcAlphaTex::RC_TEX_POS)m_uiRcPos, &m_vUV);
	_vec2 vUVLeftTop	= { g_iFontStart[iDigit] / (_float)DMG_FONT_TEXTURE_X, DMG_FONT_TOP / (_float)DMG_FONT_TEXTURE_Y };
	_vec2 vUVSize		= { g_iFontSize[iDigit] / (_float)DMG_FONT_TEXTURE_X, DMG_FONT_SIZE_Y / (_float)DMG_FONT_TEXTURE_Y };
	m_pBufferCom->Set_UV(&vUVLeftTop, vUVSize.x, vUVSize.y);

	m_pBufferCom->Render_Buffer();
}

CDamageFont* CDamageFont::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iNumber)
{
	CDamageFont* pInst = new CDamageFont(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject(iNumber)))
		::Safe_Release(pInst);

	return pInst;
}

void CDamageFont::Free()
{

	CUI::Free();
}
