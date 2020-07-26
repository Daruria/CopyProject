#include "RenderTarget.h"
#include "Screen.h"

USING(Engine)

Engine::CRenderTarget::CRenderTarget(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_pTargetTexture(nullptr)
	, m_pTargetSurface(nullptr)
	, m_pOldTargetSurface(nullptr)
	, m_pDebugBuffer(nullptr)
{
	m_pGraphicDev->AddRef();
}

Engine::CRenderTarget::~CRenderTarget(void)
{

}

HRESULT Engine::CRenderTarget::Ready_RenderTarget(const _uint& iWidth, 
													const _uint& iHeight,
													D3DFORMAT Format, 
													D3DXCOLOR Color)
{
	FAILED_CHECK_RETURN(D3DXCreateTexture(m_pGraphicDev, 
											iWidth, 
											iHeight, 
											1, 
											D3DUSAGE_RENDERTARGET, 
											Format, 
											D3DPOOL_DEFAULT, // ���� Ÿ�� ���� �� �ݵ�� default�� ����� �Ѵ�.
											&m_pTargetTexture), 
											E_FAIL);

	// �ؽ��ķκ��� ����Ÿ�� ������ �����Ͽ� 2���ڿ��� �Ѱ��ش�.
	m_pTargetTexture->GetSurfaceLevel(0, &m_pTargetSurface); // 1���� : miplevel�� ���� �� ������� ���, �� ��°�� ����� ���ΰ�. 
	Safe_Release(m_pTargetTexture);

	m_ClearColor = Color;

	return S_OK;
}

void Engine::CRenderTarget::SetUp_RenderTarget(const _uint& iIndex)
{
	m_pGraphicDev->GetRenderTarget(iIndex, &m_pOldTargetSurface);

	// ��ġ�� ��ũ�Ǿ� �ִ� ���� ����Ÿ�� ������ ������ ���ο� ����Ÿ�� ������ ��ũ��Ű�� ��Ȳ
	m_pGraphicDev->SetRenderTarget(iIndex, m_pTargetSurface);
}

void CRenderTarget::Release_OnGraphicDev(const _uint & iIndex)		// ������ �����ϴ� ���� Ÿ�� ���۵��� m_pTargetSurface�� ������ ����Ǿ� �ְ� m_pOldTargetSurface���� ����۰� �����Ǿ� �ִ�. 
{
	m_pGraphicDev->SetRenderTarget(iIndex, m_pOldTargetSurface);
	Safe_Release(m_pOldTargetSurface);
}



void CRenderTarget::Clear_RenderTarget(void)	// ���� ���ϴ� Ÿ�� ���۸� ����� ���� �����ϴ� �Լ�
{
	m_pGraphicDev->GetRenderTarget(0, &m_pOldTargetSurface);
	m_pGraphicDev->SetRenderTarget(0, m_pTargetSurface);

	m_pGraphicDev->Clear(0, NULL, D3DCLEAR_TARGET, m_ClearColor, 1.f, 0);

	m_pGraphicDev->SetRenderTarget(0, m_pOldTargetSurface);
	Safe_Release(m_pOldTargetSurface);
}


Engine::CRenderTarget* Engine::CRenderTarget::Create(LPDIRECT3DDEVICE9 pGraphicDev,
													const _uint& iWidth, 
													const _uint& iHeight,
													D3DFORMAT Format,
													D3DXCOLOR Color)
{
	CRenderTarget*	pInstance = new CRenderTarget(pGraphicDev);

	if (FAILED(pInstance->Ready_RenderTarget(iWidth, iHeight, Format, Color)))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CRenderTarget::Free(void)
{
	Safe_Release(m_pDebugBuffer);

	Safe_Release(m_pTargetTexture);
	Safe_Release(m_pOldTargetSurface);
	Safe_Release(m_pGraphicDev);
}

HRESULT CRenderTarget::Ready_DebugBuffer(const _float & fX, const _float & fY, const _float & fSizeX, const _float & fSizeY)
{
	m_pDebugBuffer = CScreen::Create(m_pGraphicDev, fX, fY, fSizeX, fSizeY);
	NULL_CHECK_RETURN(m_pDebugBuffer, E_FAIL);
	
	return S_OK;
}

void CRenderTarget::Render_DebugBuffer(void)
{
	m_pGraphicDev->SetTexture(0, m_pTargetTexture);

	m_pDebugBuffer->Render_Buffer();
}

void CRenderTarget::SetUp_OnShader(LPD3DXEFFECT & pEffect, const char * pConstantName)
{
	pEffect->SetTexture(pConstantName, m_pTargetTexture);
}
