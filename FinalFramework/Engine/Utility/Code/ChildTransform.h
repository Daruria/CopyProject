#ifndef ChildTransform_h__
#define ChildTransform_h__

#include "Transform.h"

BEGIN(Engine)

class ENGINE_DLL CChildTransform : public CTransform
{
private:
	explicit CChildTransform();
	virtual ~CChildTransform();

public:
	virtual HRESULT Ready_Component() override;
	virtual _int Update_Component(const _float& fTimeDelta) override;

public:
	void Set_BoneMatrix(const _matrix* pmatParent)		{ m_pBoneParent = pmatParent; }
	void Set_ParentTransform(CTransform* pTransform)	{ m_pParentTransform = pTransform; }
	void Set_Revolution(const _vec3* pRevAngle)			{ memcpy(&m_vRevAngle, pRevAngle, sizeof(_vec3)); }
	void Set_RevolutionX(const _float& fx) { m_vRevAngle.x = fx; }
	void Set_RevolutionY(const _float& fy) { m_vRevAngle.y = fy; }
	void Set_RevolutionZ(const _float& fz) { m_vRevAngle.z = fz; }

public:
	void Get_Revolution(_vec3* pOut) { memcpy(pOut, &m_vRevAngle, sizeof(_vec3)); }

public:
	void Revolution(const _vec3* pRevAngle)				{ m_vRevAngle.x += pRevAngle->x, m_vRevAngle.y += pRevAngle->y, m_vRevAngle.z += pRevAngle->z; }
	void RevolutionX(const _float& fx)					{ m_vRevAngle.x += fx; }
	void RevolutionY(const _float& fy)					{ m_vRevAngle.y += fy; }
	void RevolutionZ(const _float& fz)					{ m_vRevAngle.z += fz; }

private:
	_vec3				m_vRevAngle;
	const _matrix*		m_pBoneParent;
	CTransform*			m_pParentTransform;

public:
	static CChildTransform* Create();

protected:
	virtual void Free() override;
};

END

#endif // ChildTransform_h__
