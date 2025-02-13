#ifndef DynamicMesh_h__
#define DynamicMesh_h__

#include "Mesh.h"
#include "HierarchyLoader.h"
#include "AniCtrl.h"

BEGIN(Engine)

class ENGINE_DLL	CDynamicMesh : public CMesh 
{
private:
	explicit CDynamicMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDynamicMesh(const CDynamicMesh& rhs);
	virtual ~CDynamicMesh(void);

public:
	HRESULT			Ready_Meshes(const wstring& wstrFilePath, const wstring& wstrFileName);
	void			Render_Meshes();
	void			Render_Meshes(LPD3DXEFFECT& pEffect);
	void			Render_Meshes(LPD3DXEFFECT& pEffect, const _uint& iIndex);

	const  D3DXFRAME_DERIVED*	Get_FrameByName(const char* pFrameName);
	D3DXFRAME*					Get_RootFrame() { return m_pRootFrame; }
	list<D3DXMESHCONTAINER_DERIVED*>* Get_MeshContainerList() { return &m_MeshContainerList; }

public:
	_bool						Is_AnimationSetEnd(void);
	_double						Get_AnimationProgress() { return m_pAniCtrl->Get_AnimationProgress(); }
	_bool						Is_InItpProgress() const { return m_pAniCtrl->Is_InItpProgress(); }
	_uint						Get_AnimationMax() const { return m_pAniCtrl->Get_AnimationMax(); }
	_double						Get_AnimationPosition() const { return m_pAniCtrl->Get_AnimationPosition(); }

public:
	void			Set_AnimationSet(const _uint& iIndex, const _double& dItplTime);
	void			Play_Animation(const _float& fTimeDelta);
	void			Play_Animation(const _float& fTimeDelta, _vec3* pChangedPos);

	void			Set_Animation(const _uint& iIndex, const _double& dItplTime, const _double& dPosition);
	
	void			Set_SWSkinning();
	void			Render_Meshes_NoSkinning(LPD3DXEFFECT& pEffect);

private:
	// 모든 뼈들을 순회하면서 뼈들이 갖고 있는 TransformationMatrix와 pParentMatrix를 결합하여 CombinedTransformationMatrix를 만들어주는 함수
	void			Update_FrameMatrices(D3DXFRAME_DERIVED* pFrame, const _matrix* pParentMatrix);
	void			Update_FrameMatrices(D3DXFRAME_DERIVED* pFrame, const _matrix* pParentMatrix, _vec3* pChangedPos);

	// CombinedTransformationMatrix의 주소값을 얻어와서 채워주는 함수
	void			SetUp_FrameMatrixPointer(D3DXFRAME_DERIVED* pFrame);

private:
	D3DXFRAME*				m_pRootFrame;
	CHierarchyLoader*		m_pLoader;
	CAniCtrl*				m_pAniCtrl;
	list<D3DXMESHCONTAINER_DERIVED*>			m_MeshContainerList;

public:
	static	CDynamicMesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrFilePath, const wstring& wstrFileName);
	virtual CResources*			Clone(void);
	virtual void				Free(void);
};

END
#endif // DynamicMesh_h__


//typedef struct _D3DXFRAME
//{
//	LPSTR                   Name;						// 뼈의 이름을 저장하기 위한 문자열(아스키코드)
//	D3DXMATRIX              TransformationMatrix;		// 뼈의 최초 로드 시점의 상태행렬
//
//	LPD3DXMESHCONTAINER     pMeshContainer;
//
//	struct _D3DXFRAME       *pFrameSibling;				// 형제 뼈의 주소를 저장하는 포인터
//	struct _D3DXFRAME       *pFrameFirstChild;			// 자식 뼈의 주소를 저장하는 포인터
//		
//} D3DXFRAME, *LPD3DXFRAME;
//
//
//typedef struct _D3DXMESHCONTAINER
//{
//	LPSTR                   Name;					// 메쉬 컨테이너의 이름을 저장하기 위한 문자열(아스키코드 : 근데 대부분 없거나 신경쓰지 않는다)
//	D3DXMESHDATA            MeshData;				// 메쉬의 종류를 결정하기 위한 열거체와, 컴객체를 소유하고 있는 구조체
//	LPD3DXMATERIAL          pMaterials;				// 재질 정보를 보관하는 포인터
//	LPD3DXEFFECTINSTANCE    pEffects;				// 메쉬가 지닌 특수한 이펙트(우리는 사용하지 못한다)
//	DWORD                   NumMaterials;			// 재질의 개수
//	DWORD                  *pAdjacency;				// 인접한 폴리곤의 첫 번째 주소값을 보관하는 포인터
//	LPD3DXSKININFO          pSkinInfo;				// 스키닝 애니메이션시 필요한 각종 함수를 제공하는 컴객체
//
//	struct _D3DXMESHCONTAINER *pNextMeshContainer;	// 다음 연결된 메쉬 컨테이너의 주소(사실상 사용하지 않음)
//} D3DXMESHCONTAINER, *LPD3DXMESHCONTAINER;