#ifndef ResourcesMgr_h__
#define ResourcesMgr_h__

#include "Resources.h"

#include "TriCol.h"
#include "RcCol.h"
#include "RcTex.h"
#include "RcAlphaTex.h"
#include "TerrainTex.h"
#include "Texture.h"
#include "CubeTex.h"
#include "LineCol.h"
#include "Screen.h"
#include "TrailBuffer.h"
#include "CircleTex.h"
#include "CircularTrail.h"
#include "ParticleTex.h"
#include "CylinderTex.h"
#include "StaticMesh.h"
#include "DynamicMesh.h"
#include "NavigationMesh.h"

BEGIN(Engine)

class ENGINE_DLL CResourcesMgr : public CBase
{
	DECLARE_SINGLETON(CResourcesMgr)

private:
	explicit CResourcesMgr();
	virtual ~CResourcesMgr();

public:
	HRESULT		Reserve_ContainerSize(const _ushort& wSize);
	HRESULT		Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrBufferTag, BUFFERID eID, 
							const D3DXCOLOR& d3dColor, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv);
	HRESULT		Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrBufferTag, BUFFERID eID,
							const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY);
	HRESULT		Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrBufferTag, BUFFERID eID,
							CRcTex::RC_TEX_POS ePos);
	HRESULT		Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrBufferTag, BUFFERID eID,
							const _ulong& dwVtxMax);
	HRESULT		Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrBufferTag, BUFFERID eID,
							const _ulong& dwVtxMax, const _float& fInnerRadiuse, const _float& fOuterRadius);

	HRESULT		Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrBufferTag, BUFFERID eID,
							const _ulong& dwSlices, const _ulong& dwStacks, const _float& fUpperRadius, const _float& fLowerRadius, const _float& fHeight);




	HRESULT		Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrTextureTag, 
								TEXTURETYPE eType, const wstring& wstrPath, const _uint& iCnt = 1);
	HRESULT		Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrTextureTag, 
								TEXTURETYPE eType, LPDIRECT3DTEXTURE9* ppTexture, const _uint& iCnt = 1);

	HRESULT		Ready_Meshes(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrMeshTag, MESHTYPE eType, 
							const wstring& wstrFilePath, const wstring& wstrFileName);


	void		Render_Buffer(const _ushort& wContainerIdx, const wstring& wstrBufferTag);

	HRESULT		Clear_Resource(const _ushort& wContainerIdx);
	HRESULT		Remove_Resource(const _ushort& wContainerIdx, const wstring& wstrResourceTag);

	CComponent* Clone(const _ushort& wContainerIdx, const wstring& wstrResourceTag);

	CResources* Get_Resources(const _ushort& wContainerIdx, const wstring& wstrResourcesTag);

private:
	CResources* Find_Resources(const _ushort& wContainerIdx, const wstring& wstrResourcesTag);

private:
	map<wstring, CResources*>*	m_pMapResources = nullptr;
	_ushort								m_wSize = 0;

public:
	virtual void Free();

};


END

#endif // ResourcesMgr_h__
