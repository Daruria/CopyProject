#ifndef StaticMesh_h__
#define StaticMesh_h__

#include "Mesh.h"

BEGIN(Engine)
class CTexture;
class ENGINE_DLL CStaticMesh : public CMesh
{
protected:
	explicit	CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit	CStaticMesh(const CStaticMesh& rhs);
	virtual		~CStaticMesh(void);

public:
	_ulong			Get_NumVtx(void) const { return m_dwNumVtx; }
	_ulong			Get_Stride(void) const { return m_dwStride; }
	const _vec3*	Get_VtxPos(void) const { return m_pVtxPos; }
	const _vec3*	Get_Center(_vec3* pOut = NULL) const { if (pOut) memcpy(pOut, &m_vCenter, sizeof(_vec3)); return &m_vCenter; }
	const _float&	Get_Radius(_float* pOut = NULL) const { if (pOut) *pOut = m_fRadius; return m_fRadius; }

public:
	virtual HRESULT		Ready_Meshes(const wstring& wstrFilePath, const wstring& wstrFileName) override;
	virtual void		Render_Meshes(void) override;
	virtual void		Render_Meshes(LPD3DXEFFECT& pEffect) override;
	virtual void		Render_Meshes_WithPass(LPD3DXEFFECT& pEffect, const _uint& iPass) override;
	virtual void		Render_Meshes(LPD3DXEFFECT& pEffect, const _uint& iTexture) override;
	virtual void		Render_Meshes(LPD3DXEFFECT& pEffect, const _uint& iTexture, const _uint& iPass) override;
	void				Render_Mesh(LPD3DXEFFECT& pEffect, const _uint& uiSubset);


private:
	LPD3DXMESH				m_pMesh;
	LPD3DXBUFFER			m_pAdjacency;
	LPD3DXBUFFER			m_pSubsetBuffer;
	D3DXMATERIAL*			m_pMtrl;
	_ulong					m_dwSubsetCnt;
	//LPDIRECT3DTEXTURE9*		m_ppTexture;
	//CTexture*				m_ppTexture[MTT_END];
	MESH_TEXTURES*			m_pMeshTexture;

	LPD3DXMESH				m_pOriMesh;
	_ulong					m_dwNumVtx;
	_ulong					m_dwStride;
	_vec3*					m_pVtxPos = nullptr;
	
	_vec3					m_vCenter = {};
	_float					m_fRadius = 0.f;

public:
	static CStaticMesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
									const wstring& wstrFilePath,
									const wstring& wstrFileName);
	
	virtual CResources*		Clone(void);
	virtual void			Free(void);

};

END
#endif // StaticMesh_h__
