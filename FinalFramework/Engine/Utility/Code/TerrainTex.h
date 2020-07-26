#ifndef TerrainTex_h__
#define TerrainTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrainTex : public CVIBuffer
{
private:
	explicit CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTerrainTex(const CTerrainTex& rhs);
	virtual ~CTerrainTex(void);

public:
	const _vec3*		Get_VtxPos(void) const { return m_pPos; }
	const _ulong		Get_VtxCntX(void) const { return m_dwCntX; }
	const _ulong		Get_VtxCntZ(void) const { return m_dwCntZ; }
	const _ulong		Get_Itv(void) const { return m_dwItv; }
public:
	HRESULT Ready_Buffer(const _ulong& dwCntX, 
						const _ulong& dwCntZ,	
						const _ulong dwVtxItv = 1) ;

private:
	HANDLE				m_hFile;
	_ulong				m_dwCntX;
	_ulong				m_dwCntZ;
	_ulong				m_dwItv;

	_vec3*				m_pPos;
	_bool				m_bClone;


public:
	static CTerrainTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev, 
							const _ulong& dwCntX, 
							const _ulong& dwCntZ, 
							const _ulong& dwVtxItv);

	virtual CResources* Clone(void) override;

private:
	virtual void Free(void);
};

END
#endif // TerrainTex_h__
