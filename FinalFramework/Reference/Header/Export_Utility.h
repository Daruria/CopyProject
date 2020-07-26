#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "Management.h"
#include "Transform.h"
#include "Renderer.h"
#include "CameraMgr.h"
#include "MyMath.h"
#include "ResourcesMgr.h"
#include "CollisionMgr.h"
#include "SubjectMgr.h"
#include "LightMgr.h"
#include "PrototypeMgr.h"
#include "Shader.h"
#include "RenderTargetMgr.h"

//#include "Camera.h"

BEGIN(Engine)

//	Management Instance
//	Get
//inline const CGameObject* Get_GameObjectFromScene(const wstring& wstrLayerTag, const wstring& wstrObjTag);
inline const CComponent* Get_Component(const wstring& wstrLayerTag, const wstring& wstrObjTag, const wstring& wstrComponentTag, COMPONENTID eID);
inline const CLayer*	 Get_Layer(const wstring& wstrLayerTag);
//	Set
//	General
inline HRESULT	Create_Management(LPDIRECT3DDEVICE9& pGraphicDev, CManagement** ppManagemet);
inline HRESULT	SetUp_Scene(CScene* pScene);
inline HRESULT	Add_GameObject(const wstring& wstrLayerTag, const wstring& wstrObjTag, CGameObject* pObj);
inline HRESULT	Add_LoadingLayer(const wstring& wstrLayerTag, CLayer* pLayer);
inline HRESULT	Loading_Layer(map<wstring, CLayer*>* pmapLoadingLayer);
inline HRESULT	Clear_LoadingLayer();
inline void		Die_GameObject(const wstring& wstrLayerTag, const wstring& wstrObjTag);

//	Renderer
inline HRESULT		Ready_Renderer(LPDIRECT3DDEVICE9 pGraphicDev);
inline CRenderer*	Get_Renderer();

//	Camera Manager
//	Get
inline CCamera*				Get_Camera(const _ulong& dwContainerIdx, const wstring& wstrCameraTag);
inline CCamera*				Get_MainCamera();
inline void					Get_MainCameraLook(_vec3* pLook);
inline void					Get_MainCameraRight(_vec3* pRight);
inline void					Get_MainCameraUp(_vec3* pUp);
inline const _vec3*			Get_MainCameraAngle(_vec3* pAngle);
inline CCameraMgr::MAIN_CAM Get_MainCamType();

//	Set
//	General
inline HRESULT	Ready_CameraMgr(LPDIRECT3DDEVICE9 pGraphicDev, _ulong dwWinCX, _ulong dwWinCY);
inline _int		Update_MainCamera(const _float& fTimeDelta);

inline HRESULT	SetUp_MainCamera(const _ulong& dwContainerIdx, const wstring& wstrCameraTag);
inline HRESULT	Change_MainCamera(const _ubyte& byOrder);

inline HRESULT	Add_Camera(const _ulong& dwContainerIdx, const wstring& wstrCameraTag, CCamera* pCamera);
inline HRESULT	Add_BasicCamera(const _ubyte& byOrder, const wstring& wstrCameraTag, CCamera* pCamera);
inline HRESULT	Remove_Camera(const _ulong& dwContainerIdx, const wstring& wstrCameraTag);

inline HRESULT	Clear_Camera();



//	ResourcesMgr
//	Get
inline CResources* Get_Resources(const _ushort& wContainerIdx, const wstring& wstrResourcesTag);
//	Set
//	General
inline HRESULT	Reserve_ContainerSize(const _ushort& wSize);
inline HRESULT	Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrBufferTag, BUFFERID eID, 
							const D3DXCOLOR d3dColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f), const _ulong dwCntX = 129, const _ulong dwCntZ = 129, const _ulong dwVtxItv = 1);
inline HRESULT	Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrBufferTag, BUFFERID eID,
							const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY);
inline HRESULT	Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrBufferTag, BUFFERID eID,
							CRcTex::RC_TEX_POS ePos);
inline HRESULT	Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrBufferTag, BUFFERID eID,
							const _ulong& dwVtxMax);
inline HRESULT	Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrBufferTag, BUFFERID eID,
	const _ulong& dwVtxMax, const _float& fInnerRadiuse, const _float& fOuterRadius);

inline HRESULT	Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrTextureTag, TEXTURETYPE eType, const wstring& wstrPath, const _uint& iCnt = 1);
inline HRESULT	Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrTextureTag, TEXTURETYPE eType, LPDIRECT3DTEXTURE9* ppTexture, const _uint& iCnt = 1);
inline HRESULT	Ready_Meshes(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrMeshTag, MESHTYPE eType, const wstring& wstrFilePath, const wstring& wstrFileName);


inline HRESULT	Clear_Resource(const _ushort& wContainerIdx);

inline void Render_Buffer(const _ushort& wContainerIdx, const wstring& wstrBufferTag);

inline CComponent* Clone(const _ushort& wContainerIdx, const wstring& wstrResourceTag);

//	CollisionMgr
//	Get
inline list<CGameObject*>*	Get_CollisionObjectList(const wstring& wstrCollisionListTag);
//	Set
//	General
inline 	_int				Collision(const _float fTimeDelta);
inline void					Add_GameObject_To_CollisionList(const wstring& wstrCollisionListTag, CGameObject* pObj);
inline void					Add_CollisionFunction(const wstring& wstrSrcTag, const wstring& wstrDestTag, void(pFunc)(const wstring&, const wstring&));
inline _bool				Check_ByList(CGameObject* pSrc, const wstring& wstrDestTag);
inline _bool				Check_ByList(const _vec3* pPos, const _float& fRadius, const wstring& wstrDestTag);

//	SubjectMgr
//	General
inline HRESULT		Add_Subject(const wstring& wstrSubjectKey, CSubject* pSubject);
inline HRESULT		Remove_Subject(const wstring& wstrSubjectKey);
inline HRESULT		Clear_Subject();

inline CSubject*	Get_Subject(const wstring& wstrSubjectKey);
inline void			Update_Subject(const wstring& wstrSubjectKey, int iMsg, void* pData);


// LightMgr
// Get
inline const D3DLIGHT9*		Get_LightInfo(const _uint& iIndex = 0);
inline const CLight*		Get_Light(const _uint& iIndex = 0);
// Set
// General
inline HRESULT		Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex);
inline HRESULT		Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex,
								const _float& fFovy, const _float& fAspect, const _float& fNear, const _float& fFar);
inline void			Render_Light(LPD3DXEFFECT& pEffect);
inline void			Render_Light(LPD3DXEFFECT& pEffect, const _uint& iIndex);


// PrototypeMgr
// Get
// Set
// General
inline HRESULT			Ready_Prototype(const wstring& wstrProtoTag, CComponent* pInstance);
inline CComponent*		Clone_Prototype(const wstring& wstrProtoTag);

// RenderTargetMgr
// Get
// Set
// General
inline HRESULT			Ready_RenderTarget(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrTargetTag, const _uint& iWidth, const _uint& iHeight, D3DFORMAT Format, D3DXCOLOR Color);
inline HRESULT			Ready_MRT(const wstring& wstrMRTTag, const wstring& wstrTargetTag);
inline HRESULT			Begin_MRT(const wstring& wstrMRTTag);
inline HRESULT			End_MRT(const wstring& wstrMRTTag);
inline HRESULT			Ready_DebugBuffer(const wstring& wstrTargetTag,	const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY);

inline void				Render_DebugBuffer(const wstring& wstrMRTTag);

inline void				SetUp_OnShader(LPD3DXEFFECT& pEffect, const wstring& wstrTargetTag, const char* pContantName);




//	Utility Release
inline void Release_Utility(void);

#include "Export_Utility.inl"

END

#endif // Export_Utility_h__
