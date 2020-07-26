#ifndef ControlCam_h__
#define ControlCam_h__
#define INITIAL_ANGLE _vec3(D3DXToRadian(45.f), 0.f, 0.f)

#include "Defines.h"
#include "Camera.h"

class CControlCam : public Engine::CCamera
{
private:
	explicit CControlCam(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CControlCam();

public:
	virtual HRESULT Ready_Camera() override;
	virtual _int	Update_Camera(const _float& fTimeDelta) override;

public:
	virtual HRESULT Reset_Camera() override;

public:
	virtual const _vec3*	Get_Angle(_vec3* pAngle) override { if (pAngle) memcpy(pAngle, &m_vAngle, sizeof(_vec3)); return &m_vAngle; }
	virtual _vec2			Get_MousePos() override;

private:
	_int	Update_EyeAtUp(const _float& fTimeDelta);
	void	Mouse_Move(const _float& fTimeDelta);
	void	Key_Input(const _float& fTimeDelta);

private:
	_float		m_fSpeed = 40.f;
	_vec3		m_vAngle = INITIAL_ANGLE;
	_bool		m_bFixCursor = true;

public:
	static CControlCam*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void Free(void) override;

};

#endif // ControlCam_h__
