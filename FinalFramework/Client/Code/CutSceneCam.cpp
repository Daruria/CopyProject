#include "stdafx.h"
#include "CutSceneCam.h"

#include "Export_Function.h"

CCutSceneCam::CCutSceneCam(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CCamera(pGraphicDev)
{
}

CCutSceneCam::~CCutSceneCam()
{
}

HRESULT CCutSceneCam::Ready_Camera()
{
	FAILED_CHECK_RETURN(CCamera::Ready_Camera(), E_FAIL);

	m_fAspect = static_cast<_float>(WINCX) / WINCY;

	m_vEye = { 0.f, 5.f, 0.f };

	return S_OK;
}

_int CCutSceneCam::Update_Camera(const _float& fTimeDelta)
{
	Update_EyeAtUp(fTimeDelta);

	_int iExit = CCamera::Update_Camera(fTimeDelta);

	return iExit;
}

HRESULT CCutSceneCam::Reset_Camera()
{
	return S_OK;
}

_vec2 CCutSceneCam::Get_MousePos()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	return _vec2(static_cast<_float>(pt.x), static_cast<_float>(pt.y));
}



_int CCutSceneCam::Update_EyeAtUp(const _float & fTimeDelta)
{
	_matrix matRotX, matRotY, matRotZ, matRotAll;

	D3DXMatrixRotationX(&matRotX, m_vAngle.x);
	D3DXMatrixRotationY(&matRotY, m_vAngle.y);
	D3DXMatrixRotationZ(&matRotZ, m_vAngle.z);

	matRotAll = matRotX * matRotY * matRotZ;

	D3DXVec3TransformNormal(&m_vEye, &(AXIS_Z * (-m_fDistance)), &matRotAll);
	m_vEye += m_vAt;

	D3DXVec3TransformNormal(&m_vUp, &AXIS_Y, &matRotAll);

	return 0;
}

CCutSceneCam * CCutSceneCam::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCutSceneCam* pInst = new CCutSceneCam(pGraphicDev);

	if (FAILED(pInst->Ready_Camera()))
		Engine::Safe_Release(pInst);

	return pInst;
}

void CCutSceneCam::Free(void)
{
	CCamera::Free();
}

