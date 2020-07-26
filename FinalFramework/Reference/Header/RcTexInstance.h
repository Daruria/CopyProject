#ifndef RcTexInstance_h__
#define RcTexInstance_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcTexInstance : public CVIBuffer
{
private:
	explicit CRcTexInstance(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcTexInstance(const CRcTexInstance& rhs);
	virtual	~CRcTexInstance();

public:
	HRESULT Ready_Buffer(const _uint& uiCaseNum, const _uint& uiInstNum, const _ulong& dwUCnt, const _ulong& dwVCnt);
	void	Render_Buffer(const _float& fTimeDelta, const _float& fSpeed, const _float& fGravity);

	void	Reset_Instance();

private:
	_uint							m_uiCaseNum;
	LPDIRECT3DVERTEXBUFFER9			m_pVBTransform;
	LPDIRECT3DVERTEXDECLARATION9	m_pDeclaration;
	_uint							m_uiInstNum;

	_float							m_fGravity = 0.f;
	vector<_vec3>					m_vecDir;

public:
	static CRcTexInstance* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& uiCaseNum, const _uint& uiInstNum, const _ulong& dwUCnt, const _ulong& dwVCnt);
	virtual CResources*	Clone() override;

private:
	virtual void Free();

};

END
#endif // RcTexInstance_h__
