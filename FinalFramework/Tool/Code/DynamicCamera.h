#ifndef DynamicCamera_h__
#define DynamicCamera_h__

//#include "Engine_Defines.h"
#include "Camera.h"

USING(Engine)

class CDynamicCamera : public Engine::CCamera
{
public:
	enum CAM_INPUT { CAM_INPUT_NONE, CAM_INPUT_ROT, CAM_INPUT_MOVE, CAM_INPUT_END };
private:
	explicit CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDynamicCamera();

public:
	virtual HRESULT Ready_Camera() override;
	virtual _int Update_Camera(const _float& fTimeDelta) override;

	virtual HRESULT Reset_Camera() override;

	virtual const _vec3* Get_Angle(_vec3* pAngle) override { if (pAngle) memcpy(pAngle, &m_vAngle, sizeof(_vec3)); return &m_vAngle; };

	virtual void Set_Angle(const _vec3* pAngle) override { if (pAngle) memcpy(&m_vAngle, pAngle, sizeof(_vec3)); };
	virtual void Set_AngleX(const _float& fx) override { m_vAngle.x = fx; };
	virtual void Set_AngleY(const _float& fy) override { m_vAngle.y = fy; };
	virtual void Set_AngleZ(const _float& fz) override { m_vAngle.z = fz; };

public:
	void MiddleClick(CAM_INPUT eState, const _vec2* pClickedPos);
	void MiddleClick_Update(const _vec2* pMousePos);
	void MiddleClickReset();

	void Move_LookForward()		{ m_vAt += m_vLook * m_fSpeed; };
	void Move_LookBackward()	{ m_vAt -= m_vLook * m_fSpeed; };
	void Move_RightForward()	{ m_vAt += m_vRight * m_fSpeed; };
	void Move_RightBackward()	{ m_vAt -= m_vRight * m_fSpeed; };
	void ZoomInOut(const _float& fDelta);

	_float	Get_Distance() const { return m_fDistance; }

private:
	void Update_EyeAtUp();
	void Update_RightLook();

private:
	_vec3		m_vAngle;
	_matrix		m_matRotX, m_matRotY, m_matRotZ, m_matRotAll;
	CAM_INPUT	m_eState	= CAM_INPUT_NONE;
	_vec2		m_vClickPos = { 0.f, 0.f };
	_vec3		m_vOldData	= { 0.f, 0.f, 0.f };
	_vec3		m_vLook		= { 0.f, 0.f, 0.f };
	_vec3		m_vRight	= { 0.f, 0.f, 0.f };
	_float		m_fSpeed	= 0.1f;
	_float		m_fDistance = 5.f;

public:
	static CDynamicCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void Free(void) override;

};


#endif // DynamicCamera_h__
