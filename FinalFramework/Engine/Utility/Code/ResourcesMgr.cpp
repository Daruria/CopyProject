#include "ResourcesMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CResourcesMgr)

Engine::CResourcesMgr::CResourcesMgr()
{

}

Engine::CResourcesMgr::~CResourcesMgr()
{
	Free();
}

HRESULT Engine::CResourcesMgr::Reserve_ContainerSize(const _ushort& wSize)
{
	if (nullptr != m_pMapResources)
		return S_OK;

	m_pMapResources = new map<wstring, CResources*>[wSize];

	m_wSize = wSize;

	return S_OK;
}

HRESULT Engine::CResourcesMgr::Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrBufferTag, BUFFERID eID, const D3DXCOLOR& d3dColor, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{
	NULL_CHECK_RETURN(m_pMapResources, E_FAIL);

	CResources* pResources = Find_Resources(wContainerIdx, wstrBufferTag);

	if (nullptr != pResources)
		return E_FAIL;

	switch (eID)
	{
	case Engine::BUFFER_TRICOL:
		pResources = CTriCol::Create(pGraphicDev);
		break;
	case Engine::BUFFER_RCCOL:
		pResources = CRcCol::Create(pGraphicDev);
		break;
	case Engine::BUFFER_RCTEX:
		pResources = CRcTex::Create(pGraphicDev);
		break;
	case Engine::BUFFER_RCCOLTEX:
		pResources = CRcAlphaTex::Create(pGraphicDev, d3dColor);
		break;
	case Engine::BUFFER_TERRAINTEX:
		pResources = CTerrainTex::Create(pGraphicDev, dwCntX, dwCntZ, dwVtxItv);
		break;
	case Engine::BUFFER_CUBETEX:
		pResources = CCubeTex::Create(pGraphicDev);
		break;
		case Engine::BUFFER_LINECOL:
		pResources = CLineCol::Create(pGraphicDev);
		break;
	}
	NULL_CHECK_RETURN(pResources, E_FAIL);

	m_pMapResources[wContainerIdx].emplace(wstrBufferTag, pResources);

	return S_OK;
}

HRESULT CResourcesMgr::Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const wstring & wstrBufferTag, BUFFERID eID, const _float & fX, const _float & fY, const _float & fSizeX, const _float & fSizeY)
{
	NULL_CHECK_RETURN(m_pMapResources, E_FAIL);

	CResources* pResources = Find_Resources(wContainerIdx, wstrBufferTag);

	if (nullptr != pResources)
		return E_FAIL;

	switch (eID)
	{
	case BUFFER_SCREEN:
		pResources = CScreen::Create(pGraphicDev, fX, fY, fSizeX, fSizeY);
		break;
	}

	NULL_CHECK_RETURN(pResources, E_FAIL);

	m_pMapResources[wContainerIdx].emplace(wstrBufferTag, pResources);

	return S_OK;
}

HRESULT CResourcesMgr::Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const wstring & wstrBufferTag, BUFFERID eID, CRcTex::RC_TEX_POS ePos)
{
	NULL_CHECK_RETURN(m_pMapResources, E_FAIL);

	CResources* pResources = Find_Resources(wContainerIdx, wstrBufferTag);

	if (nullptr != pResources)
		return E_FAIL;

	switch (eID)
	{
	case BUFFER_RCTEX:
		pResources = CRcTex::Create(pGraphicDev, ePos);
		break;
	}

	NULL_CHECK_RETURN(pResources, E_FAIL);

	m_pMapResources[wContainerIdx].emplace(wstrBufferTag, pResources);

	return S_OK;
}

HRESULT CResourcesMgr::Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const wstring & wstrBufferTag, BUFFERID eID, const _ulong & dwVtxMax)
{

	NULL_CHECK_RETURN(m_pMapResources, E_FAIL);

	CResources* pResources = Find_Resources(wContainerIdx, wstrBufferTag);

	if (nullptr != pResources)
		return E_FAIL;

	switch (eID)
	{
	case BUFFER_TRAIL:
		pResources = CTrailBuffer::Create(pGraphicDev, dwVtxMax);
		break;
	case BUFFER_CIRCLETEX:
		pResources = CCircleTex::Create(pGraphicDev, dwVtxMax);
		break;
	case BUFFER_CIRCULAR_TRAIL:
		pResources = CCircularTrail::Create(pGraphicDev, dwVtxMax);
		break;
	case BUFFER_PARTICLE_TEX:
		pResources = CParticleTex::Create(pGraphicDev, dwVtxMax);
		break;
	}

	NULL_CHECK_RETURN(pResources, E_FAIL);

	m_pMapResources[wContainerIdx].emplace(wstrBufferTag, pResources);

	return S_OK;
}

HRESULT CResourcesMgr::Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const wstring & wstrBufferTag, BUFFERID eID, const _ulong & dwVtxMax, const _float & fInnerRadiuse, const _float & fOuterRadius)
{
	NULL_CHECK_RETURN(m_pMapResources, E_FAIL);

	CResources* pResources = Find_Resources(wContainerIdx, wstrBufferTag);

	if (nullptr != pResources)
		return E_FAIL;

	switch (eID)
	{
	case BUFFER_CIRCULAR_TRAIL:
		pResources = CCircularTrail::Create(pGraphicDev, dwVtxMax, fInnerRadiuse, fOuterRadius);
		break;
	}

	NULL_CHECK_RETURN(pResources, E_FAIL);

	m_pMapResources[wContainerIdx].emplace(wstrBufferTag, pResources);

	return S_OK;
}

HRESULT CResourcesMgr::Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const wstring & wstrBufferTag, BUFFERID eID, 
	const _ulong & dwSlices, const _ulong & dwStacks, const _float& fUpperRadius, const _float& fLowerRadius, const _float& fHeight)
{
	NULL_CHECK_RETURN(m_pMapResources, E_FAIL);

	CResources* pResources = Find_Resources(wContainerIdx, wstrBufferTag);

	if (nullptr != pResources)
		return E_FAIL;

	switch (eID)
	{
	case BUFFER_CYLINDERTEX:
		CCylinderTex::Create(pGraphicDev, dwSlices, dwStacks, fUpperRadius, fLowerRadius, fHeight);
		break;
	}

	NULL_CHECK_RETURN(pResources, E_FAIL);

	m_pMapResources[wContainerIdx].emplace(wstrBufferTag, pResources);

	return S_OK;
}

HRESULT Engine::CResourcesMgr::Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrTextureTag, TEXTURETYPE eType, const wstring& wstrPath, const _uint& iCnt /*= 1*/)
{
	NULL_CHECK_RETURN(m_pMapResources, E_FAIL);

	CResources* pResources = Find_Resources(wContainerIdx, wstrTextureTag);

	if (nullptr != pResources)
		return E_FAIL;

	pResources = CTexture::Create(pGraphicDev, wstrPath, eType, iCnt);

	m_pMapResources[wContainerIdx].emplace(wstrTextureTag, pResources);

	return S_OK;
}

HRESULT CResourcesMgr::Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const wstring & wstrTextureTag, TEXTURETYPE eType, LPDIRECT3DTEXTURE9* ppTexture, const _uint & iCnt)
{
	NULL_CHECK_RETURN(m_pMapResources, E_FAIL);

	CResources* pResources = Find_Resources(wContainerIdx, wstrTextureTag);

	if (nullptr != pResources)
		return E_FAIL;

	pResources = CTexture::Create(pGraphicDev, ppTexture, eType, iCnt);

	m_pMapResources[wContainerIdx].emplace(wstrTextureTag, pResources);

	return S_OK;
}

HRESULT CResourcesMgr::Ready_Meshes(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const wstring & wstrMeshTag, MESHTYPE eType, const wstring & wstrFilePath, const wstring & wstrFileName)
{
	NULL_CHECK_RETURN(m_pMapResources, E_FAIL);

	CResources*	pResources = Find_Resources(wContainerIdx, wstrMeshTag);

	if (nullptr != pResources)
		return S_FALSE;

	switch (eType)
	{
	case TYPE_STATIC:
		pResources = CStaticMesh::Create(pGraphicDev, wstrFilePath, wstrFileName);
		break;

	case TYPE_DYNAMIC:
		pResources = CDynamicMesh::Create(pGraphicDev, wstrFilePath, wstrFileName);
		break;

	case TYPE_NAVI:
		pResources = CNavigationMesh::Create(pGraphicDev, wstrFilePath, wstrFileName);
		//pResources = CNavigationMesh::Create(pGraphicDev);
		break;
	}

	NULL_CHECK_RETURN(pResources, E_FAIL);

	m_pMapResources[wContainerIdx].insert(make_pair(wstrMeshTag, pResources));

	return S_OK;
}

void Engine::CResourcesMgr::Render_Buffer(const _ushort& wContainerIdx, const wstring& wstrBufferTag)
{
	CResources*	pResources = Find_Resources(wContainerIdx, wstrBufferTag);
	NULL_CHECK(pResources);

	dynamic_cast<CVIBuffer*>(pResources)->Render_Buffer();
}

HRESULT CResourcesMgr::Clear_Resource(const _ushort & wContainerIdx)
{
	for_each(m_pMapResources[wContainerIdx].begin(), m_pMapResources[wContainerIdx].end(), CDeleteMap());
	m_pMapResources[wContainerIdx].clear();

	return S_OK;
}

HRESULT CResourcesMgr::Remove_Resource(const _ushort & wContainerIdx, const wstring& wstrResourceTag)
{
	auto iter = m_pMapResources[wContainerIdx].find(wstrResourceTag);

	if (m_pMapResources[wContainerIdx].end() == iter)
	{
		_tchar szFailMsg[256];
		wsprintf(szFailMsg, L"Remove_Resource failed, There is no resource that tagged ", wstrResourceTag);
		MessageBox(NULL, szFailMsg, L"System Message", MB_OK);
		return E_FAIL;
	}

	Safe_Release(iter->second);
	m_pMapResources[wContainerIdx].erase(iter);

	return S_OK;
}

Engine::CComponent* Engine::CResourcesMgr::Clone(const _ushort& wContainerIdx, const wstring& wstrResourceTag)
{
	auto iter = m_pMapResources[wContainerIdx].find(wstrResourceTag);

	if (m_pMapResources[wContainerIdx].end() == iter)
		return nullptr;

	return iter->second->Clone();
}

CResources * CResourcesMgr::Get_Resources(const _ushort & wContainerIdx, const wstring & wstrResourcesTag)
{
	auto iter = m_pMapResources[wContainerIdx].find(wstrResourcesTag);

	if (m_pMapResources[wContainerIdx].end() == iter)
		return nullptr;

	return iter->second;
}

Engine::CResources* Engine::CResourcesMgr::Find_Resources(const _ushort& wContainerIdx, const wstring& wstrResourcesTag)
{
	auto iter = m_pMapResources[wContainerIdx].find(wstrResourcesTag);

	if (m_pMapResources[wContainerIdx].end() == iter)
		return nullptr;

	return iter->second;
}

void Engine::CResourcesMgr::Free()
{
	for (_uint i = 0; i < m_wSize; ++i)
	{
		for_each(m_pMapResources[i].begin(), m_pMapResources[i].end(), CDeleteMap());
		m_pMapResources[i].clear();
	}

	Safe_Delete_Array(m_pMapResources);
}

