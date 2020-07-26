#include "stdafx.h"
#include "ControlCam.h"

#include "Export_Function.h"

CControlCam::CControlCam(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CCamera(pGraphicDev)
{
}

CControlCam::~CControlCam()
{
}

HRESULT CControlCam::Ready_Camera()
{
	FAILED_CHECK_RETURN(CCamera::Ready_Camera(), E_FAIL);

	m_fAspect = static_cast<_float>(WINCX) / WINCY;

	m_vEye = { 0.f, 5.f, 0.f };
	
	::ShowCursor(FALSE);

	return S_OK;
}

_int CControlCam::Update_Camera(const _float& fTimeDelta)
{
	Update_EyeAtUp(fTimeDelta);

	_int iExit = CCamera::Update_Camera(fTimeDelta);

	return iExit;
}

HRESULT CControlCam::Reset_Camera()
{
	return S_OK;
}

_vec2 CControlCam::Get_MousePos()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	return _vec2(static_cast<_float>(pt.x), static_cast<_float>(pt.y));
}

_int CControlCam::Update_EyeAtUp(const _float & fTimeDelta)
{
	Key_Input(fTimeDelta);
	Mouse_Move(fTimeDelta);

	_matrix matRotX, matRotY, matRotZ, matRotAll;

	D3DXMatrixRotationX(&matRotX, m_vAngle.x);
	D3DXMatrixRotationY(&matRotY, m_vAngle.y);
	D3DXMatrixRotationZ(&matRotZ, m_vAngle.z);

	matRotAll = matRotX * matRotY * matRotZ;

	D3DXVec3TransformNormal(&m_vAt, &AXIS_Z, &matRotAll);
	m_vAt += m_vEye;
	D3DXVec3TransformNormal(&m_vUp, &AXIS_Y, &matRotAll);

	return 0;
}

void CControlCam::Mouse_Move(const _float & fTimeDelta)
{
	if (false == m_bFixCursor)
		return;

	_long	dwMouseMove = 0;

	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::DIMS_Y))	//	X 축 회전
		m_vAngle.x += D3DXToRadian(m_fSpeed * fTimeDelta * dwMouseMove);

	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::DIMS_X))	//	Y 축 회전
		m_vAngle.y += D3DXToRadian(m_fSpeed * fTimeDelta * dwMouseMove);

	//	마우스 중심 고정
	POINT pt = { WINCX / 2, WINCY / 2 };
	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
}

void CControlCam::Key_Input(const _float& fTimeDelta)
{
	if (Engine::KeyDown(DIK_TAB))
	{
		m_bFixCursor = !m_bFixCursor;
		if (m_bFixCursor)
		{
			//CMouse::GetInstance()->CursorRenderOff();
			//CMouse::GetInstance()->AnimingPointOn();
		}
		else
		{
			//CMouse::GetInstance()->CursorRenderOn();
			//CMouse::GetInstance()->AnimingPointOff();
		}
	}

	_vec3 vLook = m_vAt - m_vEye;
	_vec3 vRight;
	D3DXVec3Cross(&vRight, &m_vUp, &vLook);

	if (Engine::KeyPress(DIK_U))
	{
		m_vEye += vLook * m_fSpeed * fTimeDelta;
	}
	if (Engine::KeyPress(DIK_J))
	{
		m_vEye -= vLook * m_fSpeed * fTimeDelta;
	}
	if (Engine::KeyPress(DIK_H))
	{
		m_vEye -= vRight * m_fSpeed * fTimeDelta;
	}
	if (Engine::KeyPress(DIK_K))
	{
		m_vEye += vRight * m_fSpeed * fTimeDelta;
	}
	if (Engine::KeyPress(DIK_O))
	{
		m_vEye += m_vUp * m_fSpeed * fTimeDelta;
	}
	if (Engine::KeyPress(DIK_L))
	{
		m_vEye -= m_vUp * m_fSpeed * fTimeDelta;
	}
}

CControlCam * CControlCam::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CControlCam* pInst = new CControlCam(pGraphicDev);

	if (FAILED(pInst->Ready_Camera()))
		Engine::Safe_Release(pInst);

	return pInst;
}

void CControlCam::Free(void)
{
	CCamera::Free();
	::ShowCursor(TRUE);
}

