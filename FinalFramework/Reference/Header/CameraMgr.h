#ifndef CameraMgr_h__
#define CameraMgr_h__

#include "Camera.h"

BEGIN(Engine)

class ENGINE_DLL CCameraMgr : public CBase
{
	DECLARE_SINGLETON(CCameraMgr)

public:
	enum MAIN_CAM { MAIN_CAM_3RD, MAIN_CAM_QUATER, MAIN_CAM_CONTROLL, MAIN_CAM_CUT_SCENE, MAIN_CAM_END };

private:
	explicit CCameraMgr();
	virtual ~CCameraMgr();

public:
	HRESULT		Ready_CameraMgr(LPDIRECT3DDEVICE9 pGraphicDev, _ulong dwWinCX, _ulong dwWinCY);
	_int		Update_MainCamera(const _float& fTimeDelta);

	HRESULT		SetUp_RenderUI();
	HRESULT		Finish_RenderUI();
	
public:
	_int		Update_NullCamera(const _float& fTimeDelta);
	_int		Update_OrthogonalCam();

public:
	HRESULT		SetUp_MainCamera(const _ulong& dwContainerIdx, const wstring& wstrCameraTag);
	HRESULT		Change_MainCamera(const _ubyte& byOrder);

	HRESULT		Add_Camera(const _ulong& dwContainerIdx, const wstring& wstrCameraTag, CCamera* pCamera);
	HRESULT		Add_BasicCamera(const _ubyte& byOrder, const wstring& wstrCameraTag, CCamera* pCamera);

	HRESULT		Remove_Camera(const _ulong& dwContainerIdx, const wstring& wstrCameraTag);
	HRESULT		Clear_Camera();

public:	//	Get Functions
	CCamera*		Get_Camera(const _ulong& dwContainerIdx, const wstring& wstrCameraTag);
	CCamera*		Get_MainCamera()					const { return m_pMainCamera; }
	void			Get_MainCameraLook(_vec3* pLook)	const { m_pMainCamera->Get_Look(pLook); }
	void			Get_MainCameraRight(_vec3* pRight)	const { m_pMainCamera->Get_Right(pRight); }
	void			Get_MainCameraUp(_vec3* pUp)		const { m_pMainCamera->Get_Up(pUp); }
	const _vec3*	Get_MainCameraAngle(_vec3* pAngle) const { return m_pMainCamera->Get_Angle(pAngle); }

	CCameraMgr::MAIN_CAM Get_MainCamType()	const { return m_eMainCam; }
	

private:
	void		Key_Input();

private:
	LPDIRECT3DDEVICE9				m_pGraphicDev = nullptr;
	map<wstring, CCamera*>			m_mapCamera[CAM_END];

	CCameraMgr::MAIN_CAM			m_eMainCam = MAIN_CAM_QUATER;
	wstring							m_wstrBasicCamTag[MAIN_CAM_END];
	CCamera*						m_pMainCamera = nullptr;
	_float							m_fWinCX = 1.f;
	_float							m_fWinCY = 1.f;
	_bool							m_bCanKeyInput = true;
	

private:
	virtual void Free();
};

END

#endif // CameraMgr_h__
