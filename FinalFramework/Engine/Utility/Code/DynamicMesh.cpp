#include "DynamicMesh.h"
#include "Texture.h"

#include "Export_Function.h"
USING(Engine)

Engine::CDynamicMesh::CDynamicMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMesh(pGraphicDev)
	, m_pRootFrame(nullptr)
	, m_pLoader(nullptr)
	, m_pAniCtrl(nullptr)
{

}

Engine::CDynamicMesh::CDynamicMesh(const CDynamicMesh& rhs)
	: CMesh(rhs)
	, m_pRootFrame(rhs.m_pRootFrame)
	, m_pLoader(rhs.m_pLoader)
	, m_MeshContainerList(rhs.m_MeshContainerList)
{
	m_pAniCtrl = CAniCtrl::Create(*rhs.m_pAniCtrl);

	
}

Engine::CDynamicMesh::~CDynamicMesh(void)
{

}

HRESULT Engine::CDynamicMesh::Ready_Meshes(const wstring& wstrFilePath, const wstring& wstrFileName)
{
	_tchar		szFullPath[256] = L"";

	lstrcpy(szFullPath, wstrFilePath.c_str());
	lstrcat(szFullPath, wstrFileName.c_str());

	m_pLoader = CHierarchyLoader::Create(m_pGraphicDev, wstrFilePath);
	NULL_CHECK_RETURN(m_pLoader, E_FAIL);

	LPD3DXANIMATIONCONTROLLER		pAniCtrl = nullptr;

	FAILED_CHECK_RETURN(D3DXLoadMeshHierarchyFromX(szFullPath,
													D3DXMESH_MANAGED,
													m_pGraphicDev,
													m_pLoader,	// HierarchyLoader
													NULL,
													&m_pRootFrame,
													&pAniCtrl),	// AniCtrl
													E_FAIL);

	m_pAniCtrl = CAniCtrl::Create(pAniCtrl);
	NULL_CHECK_RETURN(m_pAniCtrl, E_FAIL);

	Safe_Release(pAniCtrl);

	_matrix		matTemp;
	Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, D3DXMatrixRotationY(&matTemp, D3DXToRadian(180.f)));
	
	//D3DXFRAME_DERIVED* p1stChild = (D3DXFRAME_DERIVED*)(m_pRootFrame->pFrameFirstChild);
	//if (p1stChild)
	//	memcpy(&m_v1stChildPrePos, p1stChild->CombinedTransformationMatrix.m[3], sizeof(_vec3));

	SetUp_FrameMatrixPointer((D3DXFRAME_DERIVED*)m_pRootFrame);

	return S_OK;
}

void Engine::CDynamicMesh::Render_Meshes()
{
	for (auto& iter : m_MeshContainerList)
	{
		D3DXMESHCONTAINER_DERIVED*			pMeshContainer = iter;

		for (_ulong i = 0; i < pMeshContainer->dwNumBones; ++i)
		{
			pMeshContainer->pRenderingMatrix[i] = pMeshContainer->pFrameOffsetMatrix[i] * (*pMeshContainer->ppFrameCombinedMatrix[i]);
		}

		void*		pSrcVtx = nullptr;
		void*		pDestVtx = nullptr;


		pMeshContainer->pOriMesh->LockVertexBuffer(0, &pSrcVtx);
		pMeshContainer->MeshData.pMesh->LockVertexBuffer(0, &pDestVtx);

		// 소프트웨어 스키닝을 수행하는 함수(스키닝 뿐 아니라 애니메이션 변경 시, 뼈대들과 정점 정보들의 변경을 동시에 수행하기도 함)
		pMeshContainer->pSkinInfo->UpdateSkinnedMesh(pMeshContainer->pRenderingMatrix,	// 최종 뼈의 변환상태 행렬
														nullptr,						// 원래 상태로 되돌리기 위한 상태 행렬(원래는 위 행렬의 역행렬을 구해서 넣어줘야 하지만 안넣어줘도 상관 없음)
														pSrcVtx,						// 변하지 않는 원본 메쉬의 정점 정보
														pDestVtx);						// 변환된 정보를 담기 위한 메쉬의 정점 정보


		for (_ulong i = 0; i < pMeshContainer->NumMaterials; ++i)
		{
			//m_pGraphicDev->SetTexture(0, pMeshContainer->pMeshTexture[i].pTexture[MTT_DIFFUSE]);
			pMeshContainer->pMeshTexture[i].pTexture[MTT_DIFFUSE]->Render_Texture();
			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}

		pMeshContainer->pOriMesh->UnlockVertexBuffer();
		pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();
	}
}

void CDynamicMesh::Render_Meshes(LPD3DXEFFECT & pEffect)
{
	for (auto& iter : m_MeshContainerList)
	{
		D3DXMESHCONTAINER_DERIVED*			pMeshContainer = iter;

		for (_ulong i = 0; i < pMeshContainer->dwNumBones; ++i)
		{
			pMeshContainer->pRenderingMatrix[i] = pMeshContainer->pFrameOffsetMatrix[i] * (*pMeshContainer->ppFrameCombinedMatrix[i]);
		}

		void*		pSrcVtx = nullptr;
		void*		pDestVtx = nullptr;

		pMeshContainer->pOriMesh->LockVertexBuffer(0, &pSrcVtx);
		pMeshContainer->MeshData.pMesh->LockVertexBuffer(0, &pDestVtx);

		pMeshContainer->pSkinInfo->UpdateSkinnedMesh(pMeshContainer->pRenderingMatrix, nullptr,	pSrcVtx, pDestVtx);						

		for (_ulong i = 0; i < pMeshContainer->NumMaterials; ++i)
		{

			for (_ulong j = 0; j < MTT_END; ++j)
			{
				string strShaderTexture = "";
				switch (j)
				{
				case MTT_DIFFUSE:
					//pEffect->SetTexture("g_BaseTexture", pMeshContainer->pMeshTexture[i].pTexture[j]);
					//pMeshContainer->pMeshTexture[i].pTexture[j]->Render_Texture("g_BaseTexture", pEffect);
					strShaderTexture = "g_BaseTexture";
					break;
				case MTT_NORMAL:
					//pEffect->SetTexture("g_NormalTexture", pMeshContainer->pMeshTexture[i].pTexture[j]);
					//pMeshContainer->pMeshTexture[i].pTexture[j]->Render_Texture("g_NormalTexture", pEffect);
					strShaderTexture = "g_NormalTexture";
					break;

				case MTT_MRE:
					strShaderTexture = "g_MreTexture";
					break;
				}

				if (pMeshContainer->pMeshTexture[i].pTexture[j])
				{
					pMeshContainer->pMeshTexture[i].pTexture[j]->Render_Texture(strShaderTexture.c_str(), pEffect);
					//pEffect->SetTexture(strShaderTexture.c_str(), pMeshContainer->pMeshTexture[i].pTexture[j]);

				}
			}

			pEffect->CommitChanges();
			pMeshContainer->MeshData.pMesh->DrawSubset(i);

			//pEffect->SetTexture("g_BaseTexture", pMeshContainer->pMeshTexture[i].pTexture[MTT_DIFFUSE]);

			//pEffect->CommitChanges();
			//pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}

		pMeshContainer->pOriMesh->UnlockVertexBuffer();
		pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();
	}
}


void CDynamicMesh::Render_Meshes(LPD3DXEFFECT & pEffect, const _uint& iIndex)
{
	for (auto& iter : m_MeshContainerList)
	{
		D3DXMESHCONTAINER_DERIVED*			pMeshContainer = iter;

		for (_ulong i = 0; i < pMeshContainer->dwNumBones; ++i)
		{
			pMeshContainer->pRenderingMatrix[i] = pMeshContainer->pFrameOffsetMatrix[i] * (*pMeshContainer->ppFrameCombinedMatrix[i]);
		}

		void*		pSrcVtx = nullptr;
		void*		pDestVtx = nullptr;

		pMeshContainer->pOriMesh->LockVertexBuffer(0, &pSrcVtx);
		pMeshContainer->MeshData.pMesh->LockVertexBuffer(0, &pDestVtx);

		pMeshContainer->pSkinInfo->UpdateSkinnedMesh(pMeshContainer->pRenderingMatrix, nullptr, pSrcVtx, pDestVtx);

		for (_ulong i = 0; i < pMeshContainer->NumMaterials; ++i)
		{
			if (iIndex >= MTT_END)
			{
				pEffect->CommitChanges();

				pMeshContainer->MeshData.pMesh->DrawSubset(i);

				continue;
			}



			string strShaderTexture = "";
			switch (iIndex)
			{
			case MTT_DIFFUSE:
				//pEffect->SetTexture("g_BaseTexture", pMeshContainer->pMeshTexture[i].pTexture[j]);
				//pMeshContainer->pMeshTexture[i].pTexture[j]->Render_Texture("g_BaseTexture", pEffect);
				strShaderTexture = "g_BaseTexture";
				break;
			case MTT_NORMAL:
				//pEffect->SetTexture("g_NormalTexture", pMeshContainer->pMeshTexture[i].pTexture[j]);
				//pMeshContainer->pMeshTexture[i].pTexture[j]->Render_Texture("g_NormalTexture", pEffect);
				strShaderTexture = "g_NormalTexture";
				break;
			}

			if (pMeshContainer->pMeshTexture[i].pTexture[iIndex])
			{
				pMeshContainer->pMeshTexture[i].pTexture[iIndex]->Render_Texture(strShaderTexture.c_str(), pEffect);
				//pEffect->SetTexture(strShaderTexture.c_str(), pMeshContainer->pMeshTexture[i].pTexture[j]);

			}


			pEffect->CommitChanges();
			pMeshContainer->MeshData.pMesh->DrawSubset(i);

			//pEffect->SetTexture("g_BaseTexture", pMeshContainer->pMeshTexture[i].pTexture[MTT_DIFFUSE]);

			//pEffect->CommitChanges();
			//pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}

		pMeshContainer->pOriMesh->UnlockVertexBuffer();
		pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();
	}
}


const Engine::D3DXFRAME_DERIVED* Engine::CDynamicMesh::Get_FrameByName(const char* pFrameName)
{
	return (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pFrameName);
}

_bool Engine::CDynamicMesh::Is_AnimationSetEnd(void)
{
	return m_pAniCtrl->Is_AnimationSetEnd();
}

void CDynamicMesh::Set_AnimationSet(const _uint & iIndex, const _double& dItplTime)
{
	m_pAniCtrl->Set_AnimationSet(iIndex, dItplTime);
}

void CDynamicMesh::Play_Animation(const _float & fTimeDelta)
{
	m_pAniCtrl->Play_Animation(fTimeDelta);


	_matrix		matTemp;
	Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, D3DXMatrixRotationY(&matTemp, D3DXToRadian(180.f)));
}

void CDynamicMesh::Play_Animation(const _float & fTimeDelta, _vec3 * pChangedPos)
{
	m_pAniCtrl->Play_Animation(fTimeDelta);

	_matrix		matTemp;
	Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, D3DXMatrixRotationY(&matTemp, D3DXToRadian(180.f)), pChangedPos);
}

void CDynamicMesh::Set_Animation(const _uint & iIndex, const _double & dItplTime, const _double& dPosition)
{
	m_pAniCtrl->Set_AnimationSet(iIndex, dItplTime);
	m_pAniCtrl->Play_Animation(0.001f);
	m_pAniCtrl->Set_AnimationPosition(dPosition);
	m_pAniCtrl->Play_Animation(0.0001f);

	_matrix		matTemp;
	_vec3 vTemp;
	Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, D3DXMatrixRotationY(&matTemp, D3DXToRadian(180.f)), &vTemp);
}

void CDynamicMesh::Set_SWSkinning()
{
	for (auto& iter : m_MeshContainerList)
	{
		D3DXMESHCONTAINER_DERIVED*			pMeshContainer = iter;

		for (_ulong i = 0; i < pMeshContainer->dwNumBones; ++i)
		{
			pMeshContainer->pRenderingMatrix[i] = pMeshContainer->pFrameOffsetMatrix[i] * (*pMeshContainer->ppFrameCombinedMatrix[i]);
		}

		void*		pSrcVtx = nullptr;
		void*		pDestVtx = nullptr;


		pMeshContainer->pOriMesh->LockVertexBuffer(0, &pSrcVtx);
		pMeshContainer->MeshData.pMesh->LockVertexBuffer(0, &pDestVtx);

		pMeshContainer->pSkinInfo->UpdateSkinnedMesh(pMeshContainer->pRenderingMatrix, nullptr, pSrcVtx, pDestVtx);						

		pMeshContainer->pOriMesh->UnlockVertexBuffer();
		pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();
	}
}

void CDynamicMesh::Render_Meshes_NoSkinning(LPD3DXEFFECT & pEffect)
{
	for (auto& iter : m_MeshContainerList)
	{
		D3DXMESHCONTAINER_DERIVED*			pMeshContainer = iter;

		for (_ulong i = 0; i < pMeshContainer->NumMaterials; ++i)
		{
			for (_ulong j = 0; j < MTT_END; ++j)
			{
				string strShaderTexture = "";
				switch (j)
				{
				case MTT_DIFFUSE:	strShaderTexture = "g_BaseTexture";		break;
				case MTT_NORMAL:	strShaderTexture = "g_NormalTexture";	break;
				case MTT_MRE:		strShaderTexture = "g_MreTexture";		break;
				}

				if (pMeshContainer->pMeshTexture[i].pTexture[j])
					pMeshContainer->pMeshTexture[i].pTexture[j]->Render_Texture(strShaderTexture.c_str(), pEffect);
			}

			pEffect->CommitChanges();
			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}
	}
}

void Engine::CDynamicMesh::Update_FrameMatrices(D3DXFRAME_DERIVED* pFrame, const _matrix* pParentMatrix)
{
	if (nullptr == pFrame)
		return;

	pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix * (*pParentMatrix);

	if (nullptr != pFrame->pFrameSibling)
		Update_FrameMatrices((D3DXFRAME_DERIVED*)pFrame->pFrameSibling, pParentMatrix);

	if(nullptr != pFrame->pFrameFirstChild)
		Update_FrameMatrices((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);

}

void CDynamicMesh::Update_FrameMatrices(D3DXFRAME_DERIVED * pFrame, const _matrix * pParentMatrix, _vec3 * pChangedPos)
{
	if (nullptr == pFrame)
		return;

	pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix * (*pParentMatrix);

	if (nullptr != pFrame->pFrameSibling)
		Update_FrameMatrices((D3DXFRAME_DERIVED*)pFrame->pFrameSibling, pParentMatrix);

	D3DXFRAME_DERIVED* pFirstChild = (D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild;

	if (nullptr == pFirstChild)
		return;

	_vec3  vAfterPos;

	pFirstChild->CombinedTransformationMatrix = pFirstChild->TransformationMatrix * pFrame->CombinedTransformationMatrix;
	memcpy(&vAfterPos, pFirstChild->CombinedTransformationMatrix.m[3], sizeof(_vec3));

	*pChangedPos = vAfterPos;

	//m_v1stChildPrePos = vAfterPos;
	//if (pChangedPos->x || pChangedPos->y || pChangedPos->z)
		ZeroMemory(pFirstChild->CombinedTransformationMatrix.m[3], sizeof(_vec3));

	if (nullptr != pFirstChild->pFrameFirstChild)
		Update_FrameMatrices((D3DXFRAME_DERIVED*)pFirstChild->pFrameFirstChild, &pFirstChild->CombinedTransformationMatrix);
}

Engine::CDynamicMesh* Engine::CDynamicMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrFilePath, const wstring& wstrFileName)
{
	CDynamicMesh*	pInstance = new CDynamicMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Meshes(wstrFilePath, wstrFileName)))
		Safe_Release(pInstance);

	return pInstance;
}

Engine::CResources* Engine::CDynamicMesh::Clone(void)
{
	return new CDynamicMesh(*this);
}

void Engine::CDynamicMesh::Free(void)
{
	CMesh::Free();

	Safe_Release(m_pAniCtrl);

	if (false == m_bClone)
	{
		m_pLoader->DestroyFrame(m_pRootFrame);
		Safe_Release(m_pLoader);
	}
	m_MeshContainerList.clear();
}

void Engine::CDynamicMesh::SetUp_FrameMatrixPointer(D3DXFRAME_DERIVED* pFrame)
{
	if (nullptr != pFrame->pMeshContainer)
	{
		D3DXMESHCONTAINER_DERIVED*		pDerivedMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;

		for (_ulong i = 0; i < pDerivedMeshContainer->dwNumBones; ++i)
		{
			const char*		pBoneName = pDerivedMeshContainer->pSkinInfo->GetBoneName(i);
			D3DXFRAME_DERIVED*	pBone = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pBoneName);

			pDerivedMeshContainer->ppFrameCombinedMatrix[i] = &pBone->CombinedTransformationMatrix;
		}

		m_MeshContainerList.push_back(pDerivedMeshContainer);
	}

	if (nullptr != pFrame->pFrameSibling)
		SetUp_FrameMatrixPointer((D3DXFRAME_DERIVED*)pFrame->pFrameSibling);

	if (nullptr != pFrame->pFrameFirstChild)
		SetUp_FrameMatrixPointer((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild);

}

