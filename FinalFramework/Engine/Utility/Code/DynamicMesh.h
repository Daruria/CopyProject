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
	// ��� ������ ��ȸ�ϸ鼭 ������ ���� �ִ� TransformationMatrix�� pParentMatrix�� �����Ͽ� CombinedTransformationMatrix�� ������ִ� �Լ�
	void			Update_FrameMatrices(D3DXFRAME_DERIVED* pFrame, const _matrix* pParentMatrix);
	void			Update_FrameMatrices(D3DXFRAME_DERIVED* pFrame, const _matrix* pParentMatrix, _vec3* pChangedPos);

	// CombinedTransformationMatrix�� �ּҰ��� ���ͼ� ä���ִ� �Լ�
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
//	LPSTR                   Name;						// ���� �̸��� �����ϱ� ���� ���ڿ�(�ƽ�Ű�ڵ�)
//	D3DXMATRIX              TransformationMatrix;		// ���� ���� �ε� ������ �������
//
//	LPD3DXMESHCONTAINER     pMeshContainer;
//
//	struct _D3DXFRAME       *pFrameSibling;				// ���� ���� �ּҸ� �����ϴ� ������
//	struct _D3DXFRAME       *pFrameFirstChild;			// �ڽ� ���� �ּҸ� �����ϴ� ������
//		
//} D3DXFRAME, *LPD3DXFRAME;
//
//
//typedef struct _D3DXMESHCONTAINER
//{
//	LPSTR                   Name;					// �޽� �����̳��� �̸��� �����ϱ� ���� ���ڿ�(�ƽ�Ű�ڵ� : �ٵ� ��κ� ���ų� �Ű澲�� �ʴ´�)
//	D3DXMESHDATA            MeshData;				// �޽��� ������ �����ϱ� ���� ����ü��, �İ�ü�� �����ϰ� �ִ� ����ü
//	LPD3DXMATERIAL          pMaterials;				// ���� ������ �����ϴ� ������
//	LPD3DXEFFECTINSTANCE    pEffects;				// �޽��� ���� Ư���� ����Ʈ(�츮�� ������� ���Ѵ�)
//	DWORD                   NumMaterials;			// ������ ����
//	DWORD                  *pAdjacency;				// ������ �������� ù ��° �ּҰ��� �����ϴ� ������
//	LPD3DXSKININFO          pSkinInfo;				// ��Ű�� �ִϸ��̼ǽ� �ʿ��� ���� �Լ��� �����ϴ� �İ�ü
//
//	struct _D3DXMESHCONTAINER *pNextMeshContainer;	// ���� ����� �޽� �����̳��� �ּ�(��ǻ� ������� ����)
//} D3DXMESHCONTAINER, *LPD3DXMESHCONTAINER;