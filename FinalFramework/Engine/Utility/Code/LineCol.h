#ifndef LineCol_h__
#define LineCol_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CLineCol : public CVIBuffer
{
private:
	explicit CLineCol(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CLineCol(const CLineCol& rhs);
	virtual ~CLineCol();

public:
	virtual HRESULT Ready_Buffer() override;
	HRESULT			Ready_Buffer(const _vec3* pStartPos, const _vec3* pEndPos);
	virtual void	Render_Buffer();
	void	Set_Color(D3DXCOLOR d3dColor) { m_d3dColor = d3dColor; };

private:
	D3DXCOLOR	m_d3dColor;

public:
	static CLineCol*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CLineCol*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pStartPos, const _vec3* pEndPos);
	virtual CResources*		Clone() override;

private:
	virtual void Free();

};

END

#endif // LineCol_h__
