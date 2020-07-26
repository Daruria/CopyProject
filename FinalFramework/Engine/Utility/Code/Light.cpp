#include "Light.h"
#include "Screen.h"
#include "Camera.h"

#include "Export_Function.h"
USING(Engine)

Engine::CLight::CLight(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_iIndex(0)
	, m_pScreenBuffer(nullptr)
{
	ZeroMemory(&m_tLightInfo, sizeof(D3DLIGHT9));
	m_pGraphicDev->AddRef();
}

Engine::CLight::~CLight(void)
{

}

void CLight::Set_LightProj(const _float& fFovy, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	D3DXMatrixPerspectiveFovLH(&m_matLightProj, fFovy, fAspect, fNear, fFovy);
}

void CLight::Set_LightView(const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp)
{
	D3DXMatrixLookAtLH(&m_matLightView, pEye, pAt, pUp);

}

HRESULT Engine::CLight::Ready_Light(const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
	memcpy(&m_tLightInfo, pLightInfo, sizeof(D3DLIGHT9));

	m_iIndex = iIndex;

	m_pGraphicDev->SetLight(iIndex, pLightInfo);
	m_pGraphicDev->LightEnable(iIndex, TRUE);

	D3DVIEWPORT9		ViewPort;
	m_pGraphicDev->GetViewport(&ViewPort);
	m_pScreenBuffer = CScreen::Create(m_pGraphicDev, 0.f, 0.f, static_cast<_float>(ViewPort.Width), static_cast<_float>(ViewPort.Height));
	NULL_CHECK_RETURN(m_pScreenBuffer, E_FAIL);

	return S_OK;
}

void CLight::Render_Light(LPD3DXEFFECT & pEffect)
{
	pEffect->SetVector("g_vLightDir", &_vec4(m_tLightInfo.Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&m_tLightInfo.Diffuse);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&m_tLightInfo.Ambient);


	_matrix			matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);

	pEffect->SetMatrix("g_matViewInv", &matView);
	pEffect->SetMatrix("g_matProjInv", &matProj);

	pEffect->SetVector("g_vCamPos", (_vec4*)&matView.m[3][0]);

	pEffect->SetFloat("g_fPower", 20.f);

	//pEffect->SetVector("g_vLightPos", &_vec4(m_tLightInfo.Position, 1.f));
	pEffect->SetMatrix("g_matLightView", &m_matLightView);
	pEffect->SetMatrix("g_matLightProj", &m_matLightProj);



	//CCamera* pConCam = Get_Camera(CAM_STATIC, L"Cam_Control");
	//NULL_CHECK_RETURN(pConCam, );

	//_matrix matConView, matConProj, matConViewIn;
	//_vec3 vLightPos;
	//pConCam->Get_View(&matConView);
	//pConCam->Get_Projection(&matConProj);

	////D3DXMatrixInverse(&matConViewIn, NULL, &matConView);
	////memcpy(&vLightPos, matConViewIn.m[3], sizeof(_vec3));

	//pConCam->Get_Eye(&vLightPos);
	//pEffect->SetVector("g_vLightPos", &_vec4(vLightPos, 1.f));
	//pEffect->SetMatrix("g_matLightView", &matConView);
	//pEffect->SetMatrix("g_matLightProj", &matConProj);

	pEffect->CommitChanges();

	pEffect->BeginPass(0);

	m_pScreenBuffer->Render_Buffer();

	pEffect->EndPass();
}

Engine::CLight* Engine::CLight::Create(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
	CLight*	pInstance = new CLight(pGraphicDev);

	if (FAILED(pInstance->Ready_Light(pLightInfo, iIndex)))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CLight::Free(void)
{
	m_pGraphicDev->LightEnable(m_iIndex, FALSE);
	Safe_Release(m_pScreenBuffer);
	Safe_Release(m_pGraphicDev);
}

