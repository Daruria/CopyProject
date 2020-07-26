#ifndef EffectMesh_h__
#define EffectMesh_h__

#include "Mesh.h"

BEGIN(Engine)
class CTexture;
class ENGINE_DLL CEffectMesh : public CMesh
{
protected:
	explicit	CEffectMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit	CEffectMesh(const CEffectMesh& rhs);
	virtual		~CEffectMesh(void);

public:
	_ulong			Get_NumVtx(void) const { return m_dwNumVtx; }
	_ulong			Get_Stride(void) const { return m_dwStride; }

public:
	HRESULT		Ready_Meshes(const wstring& wstrFilePath, const wstring& wstrFileName);
	void		Render_Meshes(void);
	void		Render_Meshes(LPD3DXEFFECT& pEffect);
	void		Change_UV(const _vec2* pUV, const _float& fTimeDelta);

private:
	LPD3DXMESH				m_pMesh;
	LPD3DXBUFFER			m_pAdjacency;
	LPD3DXBUFFER			m_pSubsetBuffer;
	D3DXMATERIAL*			m_pMtrl;
	_ulong					m_dwSubsetCnt;
	MESH_TEXTURES*			m_pMeshTexture;

	LPD3DXMESH				m_pOriMesh;
	_ulong					m_dwNumVtx;
	_ulong					m_dwStride;
	_ushort					m_sOffset;
	_vec2					m_vUV;

public:
	static CEffectMesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
									const wstring& wstrFilePath,
									const wstring& wstrFileName);
	
	virtual CResources*		Clone(void);
	virtual void			Free(void);

};

END
#endif // EffectMesh_h__
