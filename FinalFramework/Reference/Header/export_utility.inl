#include "Export_Utility.h"
//	Management Instance
//	Get
const CComponent* Get_Component(const wstring& wstrLayerTag, const wstring& wstrObjTag, const wstring& wstrComponentTag, COMPONENTID eID)
{
	return CManagement::GetInstance()->Get_Component(wstrLayerTag, wstrObjTag, wstrComponentTag, eID);
}
const CLayer*	 Get_Layer(const wstring& wstrLayerTag) 
{
	return CManagement::GetInstance()->Get_Layer(wstrLayerTag);
}
//	Set
//	General
HRESULT Create_Management(LPDIRECT3DDEVICE9& pGraphicDev, CManagement** ppManagemet)
{
	(*ppManagemet) = CManagement::GetInstance();

	if (nullptr == (*ppManagemet))
		return E_FAIL;

	FAILED_CHECK_RETURN((*ppManagemet)->Ready_Management(pGraphicDev), E_FAIL);

	return S_OK;
}

HRESULT SetUp_Scene(CScene* pScene)
{
	return CManagement::GetInstance()->SetUp_Scene(pScene);
}

HRESULT	Add_GameObject(const wstring& wstrLayerTag, const wstring& wstrObjTag, CGameObject* pObj)
{
	return CManagement::GetInstance()->Add_GameObject(wstrLayerTag, wstrObjTag, pObj);
}

HRESULT	Add_LoadingLayer(const wstring& wstrLayerTag, CLayer* pLayer)
{
	return CManagement::GetInstance()->Add_LoadingLayer(wstrLayerTag, pLayer);
}

HRESULT	Loading_Layer(map<wstring, CLayer*>* pmapLoadingLayer)
{
	return CManagement::GetInstance()->Loading_Layer(pmapLoadingLayer);
}

HRESULT	Clear_LoadingLayer()
{
	return CManagement::GetInstance()->Clear_LoadingLayer();
}
void Die_GameObject(const wstring& wstrLayerTag, const wstring& wstrObjTag)
{
	CManagement::GetInstance()->Die_GameObject(wstrLayerTag, wstrObjTag);
}

//	Renderer
HRESULT Ready_Renderer(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return CRenderer::GetInstance()->Ready_Renderer(pGraphicDev);
}
CRenderer* Get_Renderer()
{
	return CRenderer::GetInstance();
}


//	Camera Manager
//	Get
CCamera* Get_Camera(const _ulong& dwContainerIdx, const wstring& wstrCameraTag)
{
	return CCameraMgr::GetInstance()->Get_Camera(dwContainerIdx, wstrCameraTag);
}
CCamera* Get_MainCamera()
{
	return CCameraMgr::GetInstance()->Get_MainCamera();
}
void Get_MainCameraLook(_vec3* pLook)
{
	CCameraMgr::GetInstance()->Get_MainCameraLook(pLook);
}
void Get_MainCameraRight(_vec3* pRight)
{
	CCameraMgr::GetInstance()->Get_MainCameraRight(pRight);
}
void Get_MainCameraUp(_vec3* pUp)
{
	CCameraMgr::GetInstance()->Get_MainCameraUp(pUp);
}

const _vec3* Get_MainCameraAngle(_vec3* pAngle)
{
	return CCameraMgr::GetInstance()->Get_MainCameraAngle(pAngle);
}
CCameraMgr::MAIN_CAM Get_MainCamType()
{
	return CCameraMgr::GetInstance()->Get_MainCamType();
}
//	Set
//	General
HRESULT	Ready_CameraMgr(LPDIRECT3DDEVICE9 pGraphicDev, _ulong dwWinCX, _ulong dwWinCY)
{
	return CCameraMgr::GetInstance()->Ready_CameraMgr(pGraphicDev, dwWinCX, dwWinCY);
}
_int	Update_MainCamera(const _float& fTimeDelta)
{
	return CCameraMgr::GetInstance()->Update_MainCamera(fTimeDelta);
}

HRESULT	SetUp_MainCamera(const _ulong& dwContainerIdx, const wstring& wstrCameraTag)
{
	return CCameraMgr::GetInstance()->SetUp_MainCamera(dwContainerIdx, wstrCameraTag);
}

HRESULT	Change_MainCamera(const _ubyte& byOrder)
{
	return CCameraMgr::GetInstance()->Change_MainCamera(byOrder);
}

HRESULT	Add_Camera(const _ulong& dwContainerIdx, const wstring& wstrCameraTag, CCamera* pCamera)
{
	return CCameraMgr::GetInstance()->Add_Camera(dwContainerIdx, wstrCameraTag, pCamera);
}
HRESULT	Add_BasicCamera(const _ubyte& byOrder, const wstring& wstrCameraTag, CCamera* pCamera)
{
	return CCameraMgr::GetInstance()->Add_BasicCamera(byOrder, wstrCameraTag, pCamera);
}
HRESULT	Remove_Camera(const _ulong& dwContainerIdx, const wstring& wstrCameraTag)
{
	return CCameraMgr::GetInstance()->Remove_Camera(dwContainerIdx, wstrCameraTag);
}

HRESULT	Clear_Camera()
{
	return CCameraMgr::GetInstance()->Clear_Camera();
}

//	ResourcesMgr
//	Get
CResources* Get_Resources(const _ushort& wContainerIdx, const wstring& wstrResourcesTag)
{
	return CResourcesMgr::GetInstance()->Get_Resources(wContainerIdx, wstrResourcesTag);
}
//	Set
//	General
HRESULT	Reserve_ContainerSize(const _ushort& wSize)
{
	return CResourcesMgr::GetInstance()->Reserve_ContainerSize(wSize);
}
HRESULT	Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrBufferTag, BUFFERID eID, 
					const D3DXCOLOR d3dColor, const _ulong dwCntX, const _ulong dwCntZ, const _ulong dwVtxItv)
{
	return CResourcesMgr::GetInstance()->Ready_Buffer(pGraphicDev, wContainerIdx, wstrBufferTag, eID, d3dColor, dwCntX, dwCntZ, dwVtxItv);
}
HRESULT	Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrBufferTag, BUFFERID eID,
					const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY)
{
	return CResourcesMgr::GetInstance()->Ready_Buffer(pGraphicDev, wContainerIdx, wstrBufferTag, eID, fX, fY, fSizeX, fSizeY);
}
HRESULT	Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrBufferTag, BUFFERID eID,
					CRcTex::RC_TEX_POS ePos)
{
	return CResourcesMgr::GetInstance()->Ready_Buffer(pGraphicDev, wContainerIdx, wstrBufferTag, eID, ePos);
}
HRESULT	Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrBufferTag, BUFFERID eID,
					const _ulong& dwVtxMax)
{
	return CResourcesMgr::GetInstance()->Ready_Buffer(pGraphicDev, wContainerIdx, wstrBufferTag, eID, dwVtxMax);
}

HRESULT	Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrBufferTag, BUFFERID eID,
	const _ulong& dwVtxMax, const _float& fInnerRadiuse, const _float& fOuterRadius)
{
	return CResourcesMgr::GetInstance()->Ready_Buffer(pGraphicDev, wContainerIdx, wstrBufferTag, eID, dwVtxMax, fInnerRadiuse, fOuterRadius);
}

HRESULT	Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrTextureTag, TEXTURETYPE eType, const wstring& wstrPath, const _uint& iCnt/* = 1*/)
{
	return CResourcesMgr::GetInstance()->Ready_Texture(pGraphicDev, wContainerIdx, wstrTextureTag, eType, wstrPath, iCnt);
}
HRESULT	Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrTextureTag, TEXTURETYPE eType, LPDIRECT3DTEXTURE9* ppTexture, const _uint& iCnt/* = 1*/)
{
	return CResourcesMgr::GetInstance()->Ready_Texture(pGraphicDev, wContainerIdx, wstrTextureTag, eType, ppTexture, iCnt);
}
HRESULT	Ready_Meshes(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrMeshTag, MESHTYPE eType, const wstring& wstrFilePath, const wstring& wstrFileName)
{
	return CResourcesMgr::GetInstance()->Ready_Meshes(pGraphicDev, wContainerIdx, wstrMeshTag, eType, wstrFilePath, wstrFileName);
}
HRESULT	Clear_Resource(const _ushort& wContainerIdx)
{
	return CResourcesMgr::GetInstance()->Clear_Resource(wContainerIdx);
}
void Render_Buffer(const _ushort& wContainerIdx, const wstring& wstrBufferTag)
{
	CResourcesMgr::GetInstance()->Render_Buffer(wContainerIdx, wstrBufferTag);
}

CComponent* Clone(const _ushort& wContainerIdx, const wstring& wstrResourceTag)
{
	return CResourcesMgr::GetInstance()->Clone(wContainerIdx, wstrResourceTag);
}

//	CollisionMgr
//	Get
list<CGameObject*>*	Get_CollisionObjectList(const wstring& wstrCollisionListTag)
{
	return CCollisionMgr::GetInstance()->Get_CollisionObjectList(wstrCollisionListTag);
}
//	Set
//	General
_int Collision(const _float fTimeDelta)
{
	return CCollisionMgr::GetInstance()->Collision(fTimeDelta);
}
void Add_GameObject_To_CollisionList(const wstring& wstrCollisionListTag, CGameObject* pObj)
{
	CCollisionMgr::GetInstance()->Add_GameObject(wstrCollisionListTag, pObj);
}

void Add_CollisionFunction(const wstring& wstrSrcTag, const wstring& wstrDestTag, void(pFunc)(const wstring&, const wstring&))
{
	CCollisionMgr::GetInstance()->Add_CollisionFunction(wstrSrcTag, wstrDestTag, pFunc);
}
_bool Check_ByList(CGameObject* pSrc, const wstring& wstrDestTag)
{
	return CCollisionMgr::GetInstance()->Check_ByList(pSrc, wstrDestTag);
}
_bool Check_ByList(const _vec3* pPos, const _float& fRadius, const wstring& wstrDestTag)
{
	return CCollisionMgr::GetInstance()->Check_ByList(pPos, fRadius, wstrDestTag);
}

//	SubjectMgr
//	General
HRESULT	Add_Subject(const wstring& wstrSubjectKey, CSubject* pSubject)
{
	return CSubjectMgr::GetInstance()->Add_Subject(wstrSubjectKey, pSubject);
}
HRESULT Remove_Subject(const wstring& wstrSubjectKey)
{
	return CSubjectMgr::GetInstance()->Remove_Subject(wstrSubjectKey);
}

HRESULT	Clear_Subject()
{
	return CSubjectMgr::GetInstance()->Clear_Subject();
}

CSubject* Get_Subject(const wstring& wstrSubjectKey)
{
	return CSubjectMgr::GetInstance()->Get_Subject(wstrSubjectKey);
}
void Update_Subject(const wstring& wstrSubjectKey, int iMsg, void* pData)
{
	CSubjectMgr::GetInstance()->Update_Subject(wstrSubjectKey, iMsg, pData);
}


// LightMgr
// Get
const D3DLIGHT9* Get_LightInfo(const _uint& iIndex)
{
	return CLightMgr::GetInstance()->Get_LightInfo(iIndex);
}

const CLight*		Get_Light(const _uint& iIndex)
{
	return CLightMgr::GetInstance()->Get_Light(iIndex);
}
// Set
// General
HRESULT	Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
	return CLightMgr::GetInstance()->Ready_Light(pGraphicDev, pLightInfo, iIndex);
}
inline HRESULT Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9 * pLightInfo, const _uint & iIndex, const _float & fFovy, const _float & fAspect, 
	const _float & fNear, const _float & fFar)
{
	return CLightMgr::GetInstance()->Ready_Light(pGraphicDev, pLightInfo, iIndex, fFovy, fAspect, fNear, fFar);
}
void Render_Light(LPD3DXEFFECT& pEffect)
{
	CLightMgr::GetInstance()->Render_Light(pEffect);
}

void Render_Light(LPD3DXEFFECT& pEffect, const _uint& iIndex)
{
	CLightMgr::GetInstance()->Render_Light(pEffect, iIndex);
}

// PrototypeMgr
// Get
// Set
// General
HRESULT	Ready_Prototype(const wstring& wstrProtoTag, CComponent* pInstance)
{
	return CPrototypeMgr::GetInstance()->Ready_Prototype(wstrProtoTag, pInstance);
}
CComponent* Clone_Prototype(const wstring& wstrProtoTag)
{
	return CPrototypeMgr::GetInstance()->Clone_Prototype(wstrProtoTag);
}
// RenderTargetMgr
// Get
// Set
// General
HRESULT			Ready_RenderTarget(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrTargetTag, const _uint& iWidth, const _uint& iHeight,	D3DFORMAT Format, D3DXCOLOR Color)
{
	return CRenderTargetMgr::GetInstance()->Ready_RenderTarget(pGraphicDev, wstrTargetTag, iWidth, iHeight, Format, Color);
}
HRESULT	Ready_MRT(const wstring& wstrMRTTag, const wstring& wstrTargetTag)
{
	return CRenderTargetMgr::GetInstance()->Ready_MRT(wstrMRTTag, wstrTargetTag);
}
HRESULT	Begin_MRT(const wstring& wstrMRTTag)
{
	return CRenderTargetMgr::GetInstance()->Begin_MRT(wstrMRTTag);
}
HRESULT	End_MRT(const wstring& wstrMRTTag)
{
	return CRenderTargetMgr::GetInstance()->End_MRT(wstrMRTTag);
}

HRESULT	Ready_DebugBuffer(const wstring& wstrTargetTag, const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY)
{
	return CRenderTargetMgr::GetInstance()->Ready_DebugBuffer(wstrTargetTag, fX, fY, fSizeX, fSizeY);
}

void Render_DebugBuffer(const wstring& wstrMRTTag)
{
	CRenderTargetMgr::GetInstance()->Render_DebugBuffer(wstrMRTTag);
}

void SetUp_OnShader(LPD3DXEFFECT& pEffect, const wstring& wstrTargetTag, const char* pContantName)
{
	CRenderTargetMgr::GetInstance()->SetUp_OnShader(pEffect, wstrTargetTag, pContantName);
}

//	Utility Release
void Release_Utility(void)
{
	CRenderTargetMgr::GetInstance()->DestroyInstance();
	CPrototypeMgr::GetInstance()->DestroyInstance();
	CLightMgr::GetInstance()->DestroyInstance();
	CSubjectMgr::GetInstance()->DestroyInstance();
	CCollisionMgr::GetInstance()->DestroyInstance();
	CResourcesMgr::GetInstance()->DestroyInstance();
	CCameraMgr::GetInstance()->DestroyInstance();
	CRenderer::GetInstance()->DestroyInstance();
	CManagement::GetInstance()->DestroyInstance();
}