#include "stdafx.h"
#include "DamageEffect.h"

#include "CircularTrail.h"
#include "Shader.h"

#include "Export_Function.h"

#define	DMG_TEXTURE_X	795
#define	DMG_TEXTURE_Y	161

#define	DMG_TOP			31
#define	DMG_BOTTOM		137
#define DMG_SIZE_Y		106

int g_iDamageStart[10]	= { 52, 123, 200, 269, 331, 408, 470, 541, 608, 674 };
int g_iDamageSize[10]	= { 71, 57, 69, 62, 77, 62, 71, 67, 66, 69 };

CDamageEffect::CDamageEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CDamageEffect::~CDamageEffect()
{
}

HRESULT CDamageEffect::Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const D3DXCOLOR & dwColor, const _uint& iDamage)
{
	FAILED_CHECK_RETURN(Add_Component(wContainerIdx, wstrTextureTag, dwColor), E_FAIL);

	if (iDamage == 0)
		m_DigitList.push_front(iDamage);
	else
	{
		for (_uint i = iDamage; i > 0; i /= 10)
			m_DigitList.push_front(i % 10);
	}

	return S_OK;
}

_int CDamageEffect::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bIsDead)
		return 0;

	if (m_fDeltaAlpha != 0.f)
	{
		m_fAlpha += m_fDeltaAlpha * fTimeDelta;

		if (m_fAlpha > 1.f)
			m_fAlpha = 1.f;
		else if (m_fAlpha < 0.f)
		{
			m_fAlpha = 0.f;
			m_bIsDead = true;
			return 0;
		}
	}

	switch (m_eState)
	{
	case EDamageState::Scaling:
		m_fSpeed += m_fDeltaSpeed * fTimeDelta;
		m_fScale += fTimeDelta * m_fSpeed;

		if (m_fSpeed <= 0.f && m_fScale < m_fEndScale)
		{
			m_fScale = m_fEndScale;
			m_eState = EDamageState::ScaleStop;
		}
		break;
	case EDamageState::ScaleStop:

		break;
	}


	_vec3 vAngle;
	Engine::Get_MainCamera()->Get_Angle(&vAngle);

	m_pTransformCom->Set_Angle(&vAngle);

	_int iExit = CEffect::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	return 0;
}

_int CDamageEffect::LateUpdate_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CDamageEffect::Render_GameObject(const _float& fTimeDelta)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_uint	iPassMax = 0;

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	pEffect->Begin(&iPassMax, 0);


	_vec3 vAngle, vPos, vRight;

	m_pTransformCom->Get_Angle(&vAngle);
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
	m_pTransformCom->Get_Info(Engine::INFO_RIGHT, &vRight);
	D3DXVec3Normalize(&vRight, &vRight);
	
	_matrix matWorld, matTrans, matScale, matRot, matRotX, matRotY, matRotZ;

	D3DXMatrixRotationX(&matRotX, vAngle.x);
	D3DXMatrixRotationY(&matRotY, vAngle.y);
	D3DXMatrixRotationZ(&matRotZ, vAngle.z);
	
	matRot = matRotX * matRotY * matRotZ;

	_float fTotal = 0.f;
	for (auto& iDigit : m_DigitList)
		fTotal += g_iDamageSize[iDigit] * m_fScale;

	vPos -= vRight * fTotal * 0.5f;
	
	for (auto& iDigit : m_DigitList)
	{
		_float fSizeX = g_iDamageSize[iDigit] * m_fScale;
		D3DXMatrixScaling(&matScale, fSizeX, DMG_SIZE_Y * m_fScale, 1.f);
		matWorld = matScale * matRot;
		memcpy(matWorld.m[3], &vPos, sizeof(_vec3));

		Render_Digit(pEffect, &matWorld, iDigit);

		vPos += vRight * fSizeX;
	}


	pEffect->End();

	::Safe_Release(pEffect);
}

void CDamageEffect::SetUp_DigitScaling(const _float & fStartScale, const _float & fEndScale, const _float & fSpeed, const _float & fDeltaSpeed)
{
	m_fScale = fStartScale;
	m_fEndScale = fEndScale;
	m_fSpeed = fSpeed;
	m_fDeltaSpeed = fDeltaSpeed;
}

void CDamageEffect::Render_Digit(LPD3DXEFFECT pEffect, const _matrix * pmatWorld, const _uint & iDigit)
{
	pEffect->SetMatrix("g_matWorld", pmatWorld);

	pEffect->BeginPass(6);

	_vec2 vUVLeftTop = { g_iDamageStart[iDigit] / (_float)DMG_TEXTURE_X, DMG_TOP / (_float)DMG_TEXTURE_Y };
	_vec2 vUVSize = { g_iDamageSize[iDigit] / (_float)DMG_TEXTURE_X, DMG_SIZE_Y / (_float)DMG_TEXTURE_Y };
	m_pBufferCom->Set_UV(&vUVLeftTop, vUVSize.x, vUVSize.y);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
}

HRESULT CDamageEffect::Add_Component(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const D3DXCOLOR & dwColor)
{
	FAILED_CHECK_RETURN(CEffect::Add_Component(wContainerIdx, wstrTextureTag), E_FAIL);

	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	LPDIRECT3DTEXTURE9 pTexture = nullptr;

	m_pGraphicDev->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pTexture, NULL);

	D3DLOCKED_RECT	tLockRect;
	ZeroMemory(&tLockRect, sizeof(D3DLOCKED_RECT));

	pTexture->LockRect(0, &tLockRect, NULL, NULL);

	*((_ulong*)tLockRect.pBits) = dwColor;

	pTexture->UnlockRect(0);

	pComponent = m_pColorTextureCom = Engine::CTexture::Create(m_pGraphicDev, &pTexture, Engine::TEX_NORMAL);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Color", pComponent);
	::Safe_Release(pTexture);

	return S_OK;
}

HRESULT CDamageEffect::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	_matrix			matWorld, matView, matProj;

	//D3DXMatrixIdentity(&matWorld);
	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	pEffect->SetFloat("g_Alpha", m_fAlpha);

	m_pTextureCom->Set_Texture(pEffect, "g_MaskTexture");
	m_pColorTextureCom->Set_Texture(pEffect, "g_BaseTexture");
	Engine::SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture");

	return S_OK;
}


CDamageEffect * CDamageEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrTextureTag, const D3DXCOLOR & dwColor, const _uint& iDamage)
{
	CDamageEffect* pInst = new CDamageEffect(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject(wContainerIdx, wstrTextureTag, dwColor, iDamage)))
		::Safe_Release(pInst);

	return pInst;
}

void CDamageEffect::Free()
{
	CEffect::Free();
}