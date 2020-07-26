#ifndef Line_h__
#define Line_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CLine : public CBase
{
public:
	enum POINT		{ POINT_START, POINT_FINISH, POINT_END };
	enum COMPARE	{ COMPARE_LEFT, COMPARE_RIGHT };

private:
	explicit CLine(void);
	virtual ~CLine(void);

public:
	HRESULT			Ready_Line(const _vec2* pPointA, const _vec2* pPointB);
	COMPARE			Compare(const _vec2* pEndPos);
	const _vec2*	Get_Normal(_vec2* pOut = nullptr)	const { if (pOut) { memcpy(pOut, &m_vNormal, sizeof(_vec2)); } return &m_vNormal; }
	void			Get_Normal(_vec3* pOut)				const {	if (pOut) { *pOut = { m_vNormal.x, 0.f, m_vNormal.y }; } };
	const _vec2*	Get_Direction(_vec2* pOut = nullptr)	const { if (pOut) { memcpy(pOut, &m_vDirection, sizeof(_vec2)); } return &m_vDirection; }
	void			Get_Direction(_vec3* pOut)				const { if (pOut) { *pOut = { m_vDirection.x, 0.f, m_vDirection.y }; } };

private:
	_vec2			m_vPoint[POINT_END];
	_vec2			m_vDirection;
	_vec2			m_vNormal;

public:
	static CLine*		Create(const _vec2* pPointA, const _vec2* pPointB);
	virtual void		Free(void);
};

END
#endif // Line_h__
