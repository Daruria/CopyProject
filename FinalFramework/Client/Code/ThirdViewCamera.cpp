#include "stdafx.h"
#include "ThirdViewCamera.h"
#include "Mouse.h"
#include "Observer.h"

#include "Export_Function.h"
#include "Transform.h"
#include "MyMath.h"

CThirdViewCamera::CThirdViewCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{

}

CThirdViewCamera::~CThirdViewCamera()
{

}

HRESULT CThirdViewCamera::Ready_Camera()
{
	FAILED_CHECK_RETURN(CCamera::Ready_Camera(), E_FAIL);


	m_pObserver = Engine::CObserver::Create(L"Player");
	NULL_CHECK_RETURN(m_pObserver, E_FAIL);

	m_fAspect = static_cast<_float>(WINCX) / WINCY;
	m_fNear = 0.1f;
	m_fDistance = 3.f;

	m_fShakingDir = 0.2f;

	return S_OK;
}

_int CThirdViewCamera::Update_Camera(const _float& fTimeDelta)
{
	//	Eye, At, Up 벡터 업데이트
	Update_EyeAtUp(fTimeDelta);
	Key_Input();

	//	matView, matProj 완성 후 적용
	_int	iExit = CCamera::Update_Camera(fTimeDelta);

	return iExit;
}

HRESULT CThirdViewCamera::Reset_Camera()
{
	m_bFixCursor = true;

	return S_OK;
}

void CThirdViewCamera::Update_EyeAtUp(const _float& fTimeDelta)
{
	Mouse_Move(fTimeDelta);

	m_pObserver->Get_Data(SC_INT(EPlayerData::POSITION), &m_vAt);
	m_vAt.y += 1.f;

	if (m_bShaking)
	{
		m_fShakingTime += fTimeDelta;
		m_vAt.y += m_fShakingDir;
		m_fShakingDir *= -1.f;
		if (m_fShakingTime >= 0.5f)
		{
			m_bShaking = false;
			m_fShakingTime = 0.f;
		}
	}

	_matrix matRotX, matRotY, matRotZ, matRotAll;

	D3DXMatrixRotationX(&matRotX, m_vAngle.x);
	D3DXMatrixRotationY(&matRotY, m_vAngle.y);
	D3DXMatrixRotationZ(&matRotZ, m_vAngle.z);

	matRotAll = matRotX * matRotY * matRotZ;

	D3DXVec3TransformNormal(&m_vUp, &AXIS_Y, &matRotAll);

	m_vEye = _vec3(0.f, 0.f, -m_fDistance);
	D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matRotAll);
	m_vEye += m_vAt;
}

void CThirdViewCamera::Mouse_Move(const _float& fTimeDelta)
{
	if (false == m_bFixCursor)
		return;

	_long	dwMouseMove = 0;

	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::DIMS_Y))	//	X 축 회전
		m_vAngle.x += D3DXToRadian(m_fSpeed * fTimeDelta * dwMouseMove);

	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::DIMS_X))	//	Y 축 회전
		m_vAngle.y += D3DXToRadian(m_fSpeed * fTimeDelta * dwMouseMove);
	
	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::DIMS_Z))	//	Zoom In & Out
		m_fDistance -= fTimeDelta * m_fSpeed * 0.1f * dwMouseMove;

	if (m_fDistance < 1.f)
		m_fDistance = 1.f;
	else if (m_fDistance > 20.f)
		m_fDistance = 20.f;

	if (m_vAngle.x > D3DXToRadian(90.f))
		m_vAngle.x = D3DXToRadian(90.f);
	else if (m_vAngle.x < D3DXToRadian(-90.f))
		m_vAngle.x = D3DXToRadian(-90.f);

	POINT pt = {WINCX / 2, WINCY / 2};
	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
}

void CThirdViewCamera::Key_Input()
{
	if (Engine::KeyDown(DIK_TAB))
	{
		m_bFixCursor = !m_bFixCursor;
	}

	//if (Engine::KeyDown(DIK_B))
	//{
	//	const Engine::CComponent* pComponent = Engine::Get_Component(L"Layer_GameLogic", L"Crate_A", L"Com_Transform", Engine::ID_DYNAMIC);
	//
	//	const Engine::CTransform* pTransformCom = dynamic_cast<const Engine::CTransform*>(pComponent);
	//	_vec3 vTargetPos;
	//	pTransformCom->Get_WorldPos(&vTargetPos);

	//	_vec3 vPtoT = vTargetPos - m_vAt;
	//	D3DXVec3Normalize(&vPtoT, &vPtoT);

	//	_vec3 vCamLook = m_vAt - m_vEye;
	//	D3DXVec3Normalize(&vCamLook, &vCamLook);

	//	_float fAngle = Engine::CMyMath::AngleBetween2Vectors(&_vec3(vCamLook.x, 0.f, vCamLook.z), &_vec3(vPtoT.x, 0.f, vPtoT.z), &AXIS_Y);

	//	m_vAngle.y += fAngle;



	//	//fAngle = Engine::CMyMath::AngleBetween2Vectors(&_vec3(0.f, vCamLook.y, vCamLook.z), &_vec3(0.f, vPtoT.y, vPtoT.z), &AXIS_X);

	//	//m_vAngle.x += fAngle;

	//}
}

void CThirdViewCamera::CameraShake()
{
	m_bShaking = true;
}

_vec2 CThirdViewCamera::Get_MousePos()
{
	if (m_bFixCursor)
		return _vec2(WINCX * 0.5f, WINCY * 0.5f);
	else
	{
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);
		return _vec2(static_cast<_float>(pt.x), static_cast<_float>(pt.y));
	}
}

CThirdViewCamera * CThirdViewCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CThirdViewCamera* pInstance = new CThirdViewCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Camera()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CThirdViewCamera::Free(void)
{
	::Safe_Release(m_pObserver);
	CCamera::Free();
}
