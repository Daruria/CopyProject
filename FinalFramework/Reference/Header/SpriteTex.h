#ifndef RcCol_h__
#define RcCol_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcCol : public CVIBuffer
{
private:
	explicit CRcCol(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcCol(const CRcCol& rhs);
	virtual	~CRcCol();

public:
	virtual HRESULT Ready_Buffer() override;

public:
	static CRcCol* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CResources*	Clone() override;

private:
	virtual void Free();

};

END
#endif // RcCol_h__
