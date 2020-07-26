#ifndef Mesh_h__
#define Mesh_h__

#include "Resources.h"

BEGIN(Engine)

class ENGINE_DLL CMesh : public CResources
{
protected:
	explicit CMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMesh(const CMesh& rhs);
	virtual ~CMesh(void);

public:
	virtual HRESULT		Ready_Meshes(const wstring& wstrFilePath, const wstring& wstrFileName) PURE;
	virtual void		Render_Meshes(void) PURE;
	virtual void		Render_Meshes(LPD3DXEFFECT& pEffect) PURE;
	virtual void		Render_Meshes(LPD3DXEFFECT& pEffect, const _uint& iTexture) {};
	virtual void		Render_Meshes_WithPass(LPD3DXEFFECT& pEffect, const _uint& iPass) {};
	virtual void		Render_Meshes(LPD3DXEFFECT& pEffect, const _uint& iTexture, const _uint& iPass) {};

protected:
	virtual _bool		Find_Alpha(const char* pFileName);

protected:
	_bool				m_bClone;

public:
	virtual CResources*		Clone(void)PURE;
	virtual void			Free(void);
};

END
#endif // Mesh_h__
