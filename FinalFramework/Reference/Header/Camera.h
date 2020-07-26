#ifndef Camera_h__
#define Camera_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CCamera : public CBase
{
	typedef struct tagFrustum
	{
		_vec3		vPoint[8];
		D3DXPLANE	Plane[6];
	}FRUSTUM;

protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCamera();

public:
	virtual HRESULT Ready_Camera();
	virtual	_int	Update_Camera(const _float& fTimeDelta);
	virtual HRESULT Reset_Camera() { return S_OK; }
	virtual	void	Update_Frustum();

public:	//	Get Functions
	const _matrix*	Get_View(_matrix* pmatView = nullptr)		const { if (pmatView) memcpy(pmatView, &m_matView, sizeof(_matrix)); return &m_matView; }
	const _matrix*	Get_Projection(_matrix* pmatProj = nullptr) const { if (pmatProj) memcpy(pmatProj, &m_matProj, sizeof(_matrix)); return &m_matProj; }
	const _matrix*	Get_PreView(_matrix* pmatView = nullptr)	const { if (pmatView) memcpy(pmatView, &m_matPreView, sizeof(_matrix)); return &m_matPreView; }

	const _vec3*	Get_Eye(_vec3* pEye = nullptr)	const { if (pEye) memcpy(pEye, &m_vEye, sizeof(_vec3)); return &m_vEye; }
	const _vec3*	Get_At(_vec3* pAt = nullptr)	const { if (pAt) memcpy(pAt, &m_vAt, sizeof(_vec3)); return &m_vAt; }
	const _vec3*	Get_Up(_vec3* pUp = nullptr)	const { if (pUp) memcpy(pUp, &m_vUp, sizeof(_vec3)); return &m_vUp; }
	
	void			Get_Right(_vec3* pRight) const;
	void			Get_Look(_vec3* pLook) const;

	virtual	const _vec3*	Get_Angle(_vec3* pAngle);
	virtual void			CameraShake() {}
	virtual void			CameraShake(const _float& fDuringTime) {};
	virtual _vec2			Get_MousePos() { return _vec2(); }

	const _float Get_Fovy()		const { return m_fFovY; }
	const _float Get_Aspect()	const { return m_fAspect; }
	const _float Get_Near()		const { return m_fNear; }
	const _float Get_Far()		const { return m_fFar; }

public:	//	Set Functios
	void	Set_Aspect(const _float& fAspect) { m_fAspect = fAspect; }
	virtual void Set_Angle(const _vec3* pAngle);
	virtual void Set_Angle(const _float& fx, const _float& fy, const _float& fz);
	virtual void Set_AngleX(const _float& fx);
	virtual void Set_AngleY(const _float& fy);
	virtual void Set_AngleZ(const _float& fz);

public:	//	Frustum Culling
	_bool	Is_InFrustum(const _vec3* pPos);
	_bool	Is_InFrustum(const _vec3* pPos, const _float& fRadius);

protected:
	LPDIRECT3DDEVICE9	m_pGraphicDev = nullptr;
	_matrix				m_matView, m_matProj;
	_matrix				m_matPreView;
	_vec3				m_vEye, m_vAt, m_vUp;
	_float				m_fFovY		= D3DXToRadian(60.f);
	_float				m_fAspect	= 1.f;
	_float				m_fNear		= 0.1f;
	_float				m_fFar		= 1000.f;

	FRUSTUM				m_tFrustum;
	
protected:
	virtual void Free(void) override;
};

END

#endif // Camera_h__
