#ifndef CutSceneCam_h__
#define CutSceneCam_h__
#define INITIAL_ANGLE _vec3(D3DXToRadian(45.f), 0.f, 0.f)

#include "Defines.h"
#include "Camera.h"

class CCutSceneCam : public Engine::CCamera
{
private:
	explicit CCutSceneCam(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCutSceneCam();

public:
	virtual HRESULT Ready_Camera() override;
	virtual _int	Update_Camera(const _float& fTimeDelta) override;

public:
	virtual HRESULT Reset_Camera() override;

public:
	virtual const _vec3*	Get_Angle(_vec3* pAngle) override { if (pAngle) memcpy(pAngle, &m_vAngle, sizeof(_vec3)); return &m_vAngle; }
	virtual _vec2			Get_MousePos() override;

public:
	void	Set_At(const _vec3* pAt)			{ memcpy(&m_vAt,	pAt,	sizeof(_vec3));	}
	void	Set_Distance(const _float& fDist)	{ m_fDistance = fDist; }

	virtual void Set_Angle(const _vec3* pAngle)										{ memcpy(&m_vAngle, pAngle, sizeof(_vec3)); }
	virtual void Set_Angle(const _float& fx, const _float& fy, const _float& fz)	{ m_vAngle = { fx, fy, fz }; }
	virtual void Set_AngleX(const _float& fx)										{ m_vAngle.x = fx; }
	virtual void Set_AngleY(const _float& fy)										{ m_vAngle.y = fy; }
	virtual void Set_AngleZ(const _float& fz)										{ m_vAngle.z = fz; }

private:
	_int	Update_EyeAtUp(const _float& fTimeDelta);

private:
	_float		m_fSpeed	= 10.f;
	_float		m_fDistance = 10.f;
	_vec3		m_vAngle = INITIAL_ANGLE;

public:
	static CCutSceneCam*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void Free(void) override;

};

#endif // CutSceneCam_h__
