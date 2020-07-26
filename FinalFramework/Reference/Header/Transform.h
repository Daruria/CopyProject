#ifndef Transform_h__
#define Transform_h__

#include "Component.h"

#define		TS_NONE				0x00000000
#define		TS_RENDEROFFSET		0x00000001
#define		TS_BONE				0x00000002
#define		TS_PARENT			0x00000004
#define		TS_PARENT_NO_SCALE	0x00000008

BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent
{
protected:
	explicit	CTransform();
	virtual		~CTransform();

public:	//	Set Functions
	void			Set_Scale(const _float& fx, const _float& fy, const _float& fz) { m_vScale = { fx, fy, fz }; }
	void			Set_Scale(const _vec3& vScale)									{ m_vScale = vScale; }
	void			Set_Scale(const _vec3* pScale)									{ memcpy(&m_vScale, pScale, sizeof(_vec3)); }
	void			Set_Scale(const _float& fScale)									{ m_vScale.x = fScale, m_vScale.y = fScale, m_vScale.z = fScale; }

	void			Set_ScaleX(const _float& fx)									{ m_vScale.x = fx; }
	void			Set_ScaleY(const _float& fy)									{ m_vScale.y = fy; }
	void			Set_ScaleZ(const _float& fz)									{ m_vScale.z = fz; }

	void			Set_Pos(const _vec3& vPos)										{ m_vInfo[INFO_POS] = vPos; }
	void			Set_Pos(const _vec3* pPos)										{ memcpy(&m_vInfo[INFO_POS], pPos, sizeof(_vec3)); }
	void			Set_Pos(const _float& fx, const _float& fy, const _float& fz)	{ m_vInfo[INFO_POS] = _vec3(fx, fy, fz); }
	void			Set_PosX(const _float& fx)										{ m_vInfo[INFO_POS].x = fx; }
	void			Set_PosY(const _float& fy)										{ m_vInfo[INFO_POS].y = fy; }
	void			Set_PosZ(const _float& fz)										{ m_vInfo[INFO_POS].z = fz; }

	void			Set_Angle(const _vec3& vAngle)									{ m_vAngle = vAngle; }
	void			Set_Angle(const _vec3* pAngle)									{ memcpy(&m_vAngle, pAngle, sizeof(_vec3)); }
	void			Set_Angle(const _float& fx, const _float& fy, const _float& fz) { m_vAngle = { fx, fy, fz }; }
	void			Set_AngleX(const _float& fx)									{ m_vAngle.x = fx; }
	void			Set_AngleY(const _float& fy)									{ m_vAngle.y = fy; }
	void			Set_AngleZ(const _float& fz)									{ m_vAngle.z = fz; }

	void			Set_WorldMatrix(const _matrix* pMatrix = nullptr)				{ if (pMatrix) { memcpy(&m_matWorld, pMatrix, sizeof(_matrix)); } }
	void			Set_WorldMatrix(const _matrix& mat)								{ m_matWorld = mat; }
	void			Set_RenderOffset(const _matrix* pMatrix)						{ if (pMatrix) { memcpy(&m_matRenderOffset, pMatrix, sizeof(_matrix)); } }
	void			Set_BoneMatrix(const _matrix* pmatParent)						{ m_pBoneParent = pmatParent; }
	void			Set_ParentTransform(CTransform* pTransform);

	void			Set_Flag(const _ulong& dwFlag)									{ m_dwFlags = dwFlag; }
	void			Add_Flag(const _ulong& dwFlag)									{ m_dwFlags |= dwFlag; }

public:	//	Get Functions
	const _vec3*	Get_Info(INFO eType, _vec3* pInfo = nullptr)	const { if (pInfo)		memcpy(pInfo, &m_vInfo[eType], sizeof(_vec3));	return &m_vInfo[eType]; }
	void			Get_WorldRight(_vec3* pOut)						const { if (pOut)		memcpy(pOut, m_matWorld.m[0], sizeof(_vec3)); }
	void			Get_WorldUp(_vec3* pOut)						const { if (pOut)		memcpy(pOut, m_matWorld.m[1], sizeof(_vec3)); }
	void			Get_WorldLook(_vec3* pOut)						const { if (pOut)		memcpy(pOut, m_matWorld.m[2], sizeof(_vec3)); }
	void			Get_WorldPos(_vec3* pOut)						const { if (pOut)		memcpy(pOut, m_matWorld.m[3], sizeof(_vec3)); }
	void			Get_ScaleMatrix(_matrix* pMatrix = nullptr)		const { if (pMatrix)	D3DXMatrixScaling(pMatrix, m_vScale.x, m_vScale.y, m_vScale.z);  }
	const _matrix*	Get_WorldMatrix(_matrix* pMatrix = nullptr)		const { if (pMatrix)	memcpy(pMatrix, &m_matWorld, sizeof(_matrix));			return &m_matWorld; }
	const _matrix*	Get_RotationMatrix(_matrix* pMatrix = nullptr)	const { if (pMatrix)	memcpy(pMatrix, &m_matRot, sizeof(_matrix));			return &m_matRot; }
	const _matrix*	Get_NoScaleMatrix(_matrix* pMatrix = nullptr)	const { if (pMatrix)	memcpy(pMatrix, &m_matNoScale, sizeof(_matrix));		return &m_matNoScale; }
	const _matrix*	Get_RenderMatrix(_matrix* pMatrix = nullptr)	const { if (pMatrix)	memcpy(pMatrix, &m_matRender, sizeof(_matrix));			return &m_matRender; }
	const _matrix*	Get_RenderOffset(_matrix* pMatrix = nullptr)	const { if (pMatrix)	memcpy(pMatrix, &m_matRenderOffset, sizeof(_matrix));	return &m_matRenderOffset; }
	const _matrix*	Get_PreWorldMatrix(_matrix* pMatrix = nullptr)	const { if (pMatrix)	memcpy(pMatrix, &m_matPreWorld, sizeof(_matrix));		return &m_matPreWorld; }
	const _matrix*	Get_PreRenderMatrix(_matrix* pMatrix = nullptr)	const { if (pMatrix)	memcpy(pMatrix, &m_matPreRender, sizeof(_matrix));		return &m_matPreRender; }


	const _vec3*	Get_Angle(_vec3* pAngle = nullptr)				const {	if (pAngle)		memcpy(pAngle, &m_vAngle, sizeof(_vec3));				return &m_vAngle; }
	const _vec3*	Get_Scale(_vec3* pScale = nullptr)				const { if (pScale)		memcpy(pScale, &m_vScale, sizeof(_vec3));				return &m_vScale; }

	_float			Get_AngleX()									const { return m_vAngle.x; }
	_float			Get_AngleY()									const { return m_vAngle.y; }
	_float			Get_AngleZ()									const { return m_vAngle.z; }

	const _ulong&	Get_Flags()										const { return m_dwFlags; }

public:	//	return Reference
	const _vec3&	Get_InfoRef(INFO eType)							const { return m_vInfo[eType]; }
	const _vec3&	Get_ScaleRef()									const { return m_vScale; }
	const _vec3&	Get_AngleRef()									const { return m_vAngle; }
	const _matrix&	Get_WorldMatrixRef()							const { return m_matWorld; }
	const _matrix&	Get_RotationMatrixRef()							const { return m_matRot; }
	const _matrix&	Get_NoScaleMatrixRef()							const { return m_matNoScale; }
	const _matrix&	Get_RenderOffsetRef()							const { return m_matRenderOffset; }

public:	//	Move or Rotationd etc
	void			Move_Pos(const _vec3* const pDir)				{ m_vInfo[INFO_POS] += *pDir; }
	void			Move_Pos(const _vec3& vDir)						{ m_vInfo[INFO_POS] += vDir; }
	void			Move_Pos(INFO eType, const _float& fDist)		{ m_vInfo[INFO_POS] += m_vInfo[eType] * fDist; }
	void			Move_PosX(const _float& fx)						{ m_vInfo[INFO_POS].x += fx; }
	void			Move_PosY(const _float& fy)						{ m_vInfo[INFO_POS].y += fy; }
	void			Move_PosZ(const _float& fz)						{ m_vInfo[INFO_POS].z += fz; }
	void			Rotation(ROTATION eType, const _float& fAngle)	{ *(((_float*)&m_vAngle) + eType) += fAngle; }
	void			Multiply_Scale(const _vec3* const pScale)		{ m_vScale.x *= pScale->x, m_vScale.y *= pScale->y, m_vScale.z *= pScale->z; }
	void			Multiply_WorldMatrix(const _matrix* pMatrix)	{ m_matWorld *= *pMatrix; }
	_bool			Check_Flag(const _ulong& dwFlag)		const { if (m_dwFlags & dwFlag) return true; return false; }

public:
	virtual HRESULT			Ready_Component();
	HRESULT					Ready_Component(const _ulong& dwFlags);
	virtual _int			Update_Component(const _float& fTimeDelta);

protected:
	_vec3				m_vInfo[INFO_END];
	_vec3				m_vScale;
	_vec3				m_vAngle;

	_matrix				m_matWorld;
	_matrix				m_matRot;
	_matrix				m_matNoScale;
	_matrix				m_matRenderOffset;
	_matrix				m_matRender;

	_matrix				m_matPreWorld;
	_matrix				m_matPreRender;

	const _matrix*		m_pBoneParent = nullptr;
	CTransform*			m_pParentTransform = nullptr;

	_ulong				m_dwFlags = 0;

public:
	static CTransform* Create(const _ulong& dwFlags = TS_NONE);

protected:
	virtual void Free() override;

};

END

#endif // Transform_h__
