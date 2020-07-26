#ifndef QuaterCam_h__
#define QuaterCam_h__
#define INITIAL_ANGLE _vec3(D3DXToRadian(45.f), 0.f, 0.f)

#include "Defines.h"
#include "Camera.h"

BEGIN(Engine)

class CTransform;
class CObserver;

END

//class CPlayerObserver;
class CQuaterCam : public Engine::CCamera
{
private:
	explicit CQuaterCam(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CQuaterCam();

public:
	virtual HRESULT Ready_Camera() override;
	virtual _int	Update_Camera(const _float& fTimeDelta) override;

public:
	virtual HRESULT Reset_Camera() override;
	virtual void	CameraShake() override;
	virtual void	CameraShake(const _float& fDuringTime);

public:
	virtual const _vec3*	Get_Angle(_vec3* pAngle) override { if (pAngle) memcpy(pAngle, &m_vAngle, sizeof(_vec3)); return &m_vAngle; }
	virtual _vec2			Get_MousePos() override;

	virtual void Set_Angle(const _float& fx, const _float& fy, const _float& fz) override { m_vAngle = { fx, fy, fz }; }

public:
	void	Set_Rotation(const _vec3* pAngle);
	void	Change_Distance(const _float& fDist);
	void	Set_Distance(const _float& fDist) { m_fDistance = fDist; }

private:
	_int	Smoothing_Rotation(const _float& fTimeDelta);
	_int	Smoothing_Distancing(const _float& fTimeDelta);
	_int	Update_EyeAtUp(const _float& fTimeDelta);
	void	KeyInput(const _float& fTimeDelta);

private:
	_float		m_fDistance = 15.f;
	_float		m_fSpeed = 10.f;
	_vec3		m_vAngle = INITIAL_ANGLE;

	_bool		m_bShaking = false;
	_float		m_fShakingTime = 0.f;
	_float		m_fShakingDuringTime = 0.3f;
	_float		m_fShakingDir = 0.1f;

	_bool		m_bSmoothRotation = false;
	_vec3		m_vTargetAngle = {};

	_bool		m_bDistanceChange = false;
	_float		m_fTargetDistance = 0.f;

	Engine::CObserver*	m_pObserver;

public:
	static CQuaterCam*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void Free(void) override;

};

#endif // QuaterCam_h__
