#include "stdafx.h"
#include "QuaterCam.h"
#include "Transform.h"
#include "Observer.h"

#include "Export_Function.h"

CQuaterCam::CQuaterCam(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CCamera(pGraphicDev)
{
}

CQuaterCam::~CQuaterCam()
{
}

HRESULT CQuaterCam::Ready_Camera()
{
	FAILED_CHECK_RETURN(CCamera::Ready_Camera(), E_FAIL);

	m_fAspect = static_cast<_float>(WINCX) / WINCY;

	m_pObserver = Engine::CObserver::Create(L"Player");
	NULL_CHECK_RETURN(m_pObserver, E_FAIL);
	//Engine::Subscribe(L"Player", m_pObserver);

	return S_OK;
}

_int CQuaterCam::Update_Camera(const _float& fTimeDelta)
{
	Smoothing_Rotation(fTimeDelta);
	Smoothing_Distancing(fTimeDelta);
	KeyInput(fTimeDelta);
	Update_EyeAtUp(fTimeDelta);

	_int iExit = CCamera::Update_Camera(fTimeDelta);

	return iExit;
}

HRESULT CQuaterCam::Reset_Camera()
{
	m_fShakingDir = 0.2f;

	return S_OK;
}

void CQuaterCam::CameraShake()
{
	m_bShaking = true;
	Engine::Get_Renderer()->On_Blur();
	m_fShakingDuringTime = 0.3f;
}

void CQuaterCam::CameraShake(const _float & fDuringTime)
{
	m_bShaking = true;
	Engine::Get_Renderer()->On_Blur();
	m_fShakingDuringTime = fDuringTime;
}

_vec2 CQuaterCam::Get_MousePos()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	return _vec2(static_cast<_float>(pt.x), static_cast<_float>(pt.y));
}


void CQuaterCam::Set_Rotation(const _vec3 * pAngle)
{
	m_bSmoothRotation = true;
	m_vTargetAngle = *pAngle;
}

void CQuaterCam::Change_Distance(const _float & fDist)
{
	m_bDistanceChange = true;
	m_fTargetDistance = fDist;
}

_int CQuaterCam::Smoothing_Rotation(const _float & fTimeDelta)
{
	if (!m_bSmoothRotation)
		return 0;

	_vec3 fDelta = m_vTargetAngle - m_vAngle;
	m_vAngle +=  fDelta * fTimeDelta;

	if (D3DXVec3Length(&fDelta) < 0.01f)
	{
		m_vAngle = m_vTargetAngle;
		m_bSmoothRotation = false;
	}

	return 0;
}

_int CQuaterCam::Smoothing_Distancing(const _float & fTimeDelta)
{
	if (!m_bDistanceChange)
		return 0;

	_float fDelta = m_fTargetDistance - m_fDistance;
	m_fDistance += fDelta * fTimeDelta;

	if (fDelta < 0.01f)
	{
		m_fDistance = m_fTargetDistance;
		m_bDistanceChange = false;
	}

	return 0;
}

_int CQuaterCam::Update_EyeAtUp(const _float & fTimeDelta)
{
	m_pObserver->Get_Data(SC_INT(EPlayerData::POSITION), &m_vAt);

	if (m_bShaking)
	{
		m_fShakingTime += fTimeDelta;
		m_vAt.y += sinf(m_fShakingTime * 80.f) * 0.1f;
		
		if (m_fShakingTime >= m_fShakingDuringTime)
		{
			m_bShaking = false;
			m_fShakingTime = 0.f;
			Engine::Get_Renderer()->Off_Blur();
		}
	}

	_matrix matRotX, matRotY, matRotZ, matRotAll;
	D3DXMatrixRotationX(&matRotX, m_vAngle.x);
	D3DXMatrixRotationY(&matRotY, m_vAngle.y);
	D3DXMatrixRotationZ(&matRotZ, m_vAngle.z);

	matRotAll = matRotX * matRotY * matRotZ;

	m_vEye = _vec3(0.f, 0.f, -m_fDistance);
	D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matRotAll);
	m_vEye += m_vAt;
	D3DXVec3TransformNormal(&m_vUp, &AXIS_Y, &matRotAll);

	return 0;
}

void CQuaterCam::KeyInput(const _float & fTimeDelta)
{
	_long	dwMouseMove = 0;

	//if (dwMouseMove = Engine::Get_DIMouseMove(Engine::DIMS_Z))	//	Zoom In & Out
	//	m_fDistance -= fTimeDelta * m_fSpeed * 0.01f * dwMouseMove * m_fDistance;
	if (m_fDistance < 1.f)
		m_fDistance = 1.f;
}

CQuaterCam * CQuaterCam::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CQuaterCam* pInst = new CQuaterCam(pGraphicDev);

	if (FAILED(pInst->Ready_Camera()))
		Engine::Safe_Release(pInst);

	return pInst;
}

void CQuaterCam::Free(void)
{
	Engine::Safe_Release(m_pObserver);
	CCamera::Free();
}

