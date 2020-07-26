#ifndef AxisLines_h__
#define AxisLines_h__

#include "Base.h"

using namespace Engine;

class CAxisLines : public CBase
{
public:
	enum ACT_AXIS { ACT_AXIS_X, ACT_AXIS_Y, ACT_AXIS_Z, ACT_AXIS_XZ, ACT_AXIS_END };
	enum AXIS_POINT { AP_X, AP_Y, AP_Z, AP_ORIGIN, AP_END };

public:
	explicit CAxisLines(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CAxisLines(const CAxisLines& rhs);
	virtual ~CAxisLines();

private:
	HRESULT		Ready_AxisLines();

public:
	void		Change_ActvateAxis(ACT_AXIS eActivateAxis);
	void		Render_AxisLines(const _float& fCamDist = 1.f, const _matrix* pmatWorld = nullptr);

public:
	ACT_AXIS	Get_ActivatedAxis() const { return m_eActvateAxis; }

private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;

	ACT_AXIS			m_eActvateAxis = ACT_AXIS_END;
	ACT_AXIS			m_ePreActAxis = ACT_AXIS_END;

	LPD3DXLINE	m_pD3DXLine;
	_vec3		m_vPoint[AP_END];
	
public:
	static CAxisLines* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	CAxisLines*	Clone();

private:
	virtual void Free();

};


#endif // AxisLines_h__
