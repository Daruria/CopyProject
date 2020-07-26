#include "HierarchyLoader.h"
#include "MyMath.h"
#include "Texture.h"

#include "Export_Function.h"

USING(Engine)

Engine::CHierarchyLoader::CHierarchyLoader(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrPath)
	: m_pGraphicDev(pGraphicDev),
	m_wstrPath(wstrPath)
{
	m_pGraphicDev->AddRef();
}

Engine::CHierarchyLoader::~CHierarchyLoader(void)
{

}

STDMETHODIMP Engine::CHierarchyLoader::CreateFrame(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	D3DXFRAME_DERIVED*		pDerivedFrame = new D3DXFRAME_DERIVED;
	ZeroMemory(pDerivedFrame, sizeof(D3DXFRAME_DERIVED));

	Allocate_Name(&pDerivedFrame->Name, Name);

	pDerivedFrame->CombinedTransformationMatrix = *D3DXMatrixIdentity(&pDerivedFrame->TransformationMatrix);

	*ppNewFrame = pDerivedFrame;

	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::CreateMeshContainer(THIS_ LPCSTR Name, 
															CONST D3DXMESHDATA *pMeshData, 
															CONST D3DXMATERIAL *pMaterials, 
															CONST D3DXEFFECTINSTANCE *pEffectInstances, 
															DWORD NumMaterials, 
															CONST DWORD *pAdjacency, 
															LPD3DXSKININFO pSkinInfo, 
															LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	D3DXMESHCONTAINER_DERIVED*		pDerivedMeshContainer = new D3DXMESHCONTAINER_DERIVED;
	ZeroMemory(pDerivedMeshContainer, sizeof(D3DXMESHCONTAINER_DERIVED));

	Allocate_Name(&pDerivedMeshContainer->Name, Name);

	pDerivedMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	LPD3DXMESH		pMesh = pMeshData->pMesh;

	_ulong	dwNumFaces = pMesh->GetNumFaces();	// �޽��� ���� ������ ������ ��ȯ

	pDerivedMeshContainer->pAdjacency = new _ulong[dwNumFaces * 3];
	memcpy(pDerivedMeshContainer->pAdjacency, pAdjacency, sizeof(_ulong) * dwNumFaces * 3);

	//_ulong	dwFVF = pMesh->GetFVF();

	//if (!(dwFVF & D3DFVF_NORMAL))
	//{
	//	// fvf ������ ä���� ä �޽� ��ü�� ����
	//	pMesh->CloneMeshFVF(pMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pGraphicDev, &pDerivedMeshContainer->MeshData.pMesh);

	//	// �� ������ �Ǵ� ���� ������ ������ ����Ͽ� �������ִ� �Լ�
	//	D3DXComputeNormals(pDerivedMeshContainer->MeshData.pMesh, pDerivedMeshContainer->pAdjacency);
	//}
	//else
	//{
	//	pMesh->CloneMeshFVF(pMesh->GetOptions(), dwFVF, m_pGraphicDev, &pDerivedMeshContainer->MeshData.pMesh);
	//}

	D3DVERTEXELEMENT9		Decl[MAX_FVF_DECL_SIZE];
	ZeroMemory(Decl, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);

	pMesh->GetDeclaration(Decl);
	pMesh->CloneMesh(pMesh->GetOptions(), Decl, m_pGraphicDev, &pDerivedMeshContainer->MeshData.pMesh);

	pDerivedMeshContainer->NumMaterials = (NumMaterials == 0 ? 1 : NumMaterials);

	pDerivedMeshContainer->pMaterials = new D3DXMATERIAL[pDerivedMeshContainer->NumMaterials];
	ZeroMemory(pDerivedMeshContainer->pMaterials, sizeof(D3DXMATERIAL) * pDerivedMeshContainer->NumMaterials);

	//pDerivedMeshContainer->ppTexture = new LPDIRECT3DTEXTURE9[pDerivedMeshContainer->NumMaterials];
	//ZeroMemory(pDerivedMeshContainer->ppTexture, sizeof(LPDIRECT3DTEXTURE9) * pDerivedMeshContainer->NumMaterials);

	pDerivedMeshContainer->pMeshTexture = new MESH_TEXTURES[pDerivedMeshContainer->NumMaterials];
	ZeroMemory(pDerivedMeshContainer->pMeshTexture, sizeof(MESH_TEXTURES) * pDerivedMeshContainer->NumMaterials);

	if (0 != NumMaterials)
	{
		memcpy(pDerivedMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * pDerivedMeshContainer->NumMaterials);

		for (_ulong i = 0; i < pDerivedMeshContainer->NumMaterials; ++i)
		{
			TCHAR		szFullPath[256] = L"";
			TCHAR		szFileName[256] = L"";

			MultiByteToWideChar(CP_ACP, 
								0, 
								pDerivedMeshContainer->pMaterials[i].pTextureFilename, 
								(int)strlen(pDerivedMeshContainer->pMaterials[i].pTextureFilename),
								szFileName, 
								256);


			lstrcpy(szFullPath, m_wstrPath.c_str());
			lstrcat(szFullPath, szFileName);

			CTexture* pTexture = dynamic_cast<CTexture*>(Get_Resources(RESOURCE_STAGE, szFileName));
			if (!pTexture)
				Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, szFileName, TEX_NORMAL, szFullPath);

			pDerivedMeshContainer->pMeshTexture[i].pTexture[MTT_DIFFUSE] = dynamic_cast<CTexture*>(Engine::Clone(RESOURCE_STAGE, szFileName));

			//FAILED_CHECK_RETURN(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &pDerivedMeshContainer->pMeshTexture[i].pTexture[MTT_DIFFUSE]), E_FAIL);

			wstring wstrAddFileName;
			if (CMyMath::Search_NormalFile(m_wstrPath.c_str(), szFileName, &wstrAddFileName))
			{
				ZeroMemory(szFullPath, sizeof(_tchar) * 256);
				lstrcpy(szFullPath, m_wstrPath.c_str());
				lstrcat(szFullPath, wstrAddFileName.c_str());

				CTexture* pTexture = dynamic_cast<CTexture*>(Get_Resources(RESOURCE_STAGE, wstrAddFileName));
				if (!pTexture)
					Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, wstrAddFileName, TEX_NORMAL, szFullPath);

				pDerivedMeshContainer->pMeshTexture[i].pTexture[MTT_NORMAL] = dynamic_cast<CTexture*>(Engine::Clone(RESOURCE_STAGE, wstrAddFileName));
			}

			if (CMyMath::Search_MreFile(m_wstrPath.c_str(), szFileName, &wstrAddFileName))
			{
				ZeroMemory(szFullPath, sizeof(_tchar) * 256);
				lstrcpy(szFullPath, m_wstrPath.c_str());
				lstrcat(szFullPath, wstrAddFileName.c_str());

				CTexture* pTexture = dynamic_cast<CTexture*>(Get_Resources(RESOURCE_STAGE, wstrAddFileName));
				if (!pTexture)
					Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, wstrAddFileName, TEX_NORMAL, szFullPath);

				pDerivedMeshContainer->pMeshTexture[i].pTexture[MTT_MRE] = dynamic_cast<CTexture*>(Engine::Clone(RESOURCE_STAGE, wstrAddFileName));
			}

			//if (!CMyMath::Search_MreFile(m_wstrPath.c_str(), szFileName, &wstrAddFileName))
			//	continue;

			//ZeroMemory(szFullPath, sizeof(_tchar) * 256);
			//lstrcpy(szFullPath, m_wstrPath.c_str());
			//lstrcat(szFullPath, wstrAddFileName.c_str());
			//FAILED_CHECK_RETURN(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &pDerivedMeshContainer->pMeshTexture[i].pTexture[MTT_MRE]), E_FAIL);
		}
	}
	else
	{
		pDerivedMeshContainer->pMaterials[0].MatD3D.Diffuse = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedMeshContainer->pMaterials[0].MatD3D.Specular = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedMeshContainer->pMaterials[0].MatD3D.Ambient = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedMeshContainer->pMaterials[0].MatD3D.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedMeshContainer->pMaterials[0].MatD3D.Power = 0.f;

		//pDerivedMeshContainer->ppTexture[0] = nullptr;
		pDerivedMeshContainer->pMeshTexture[0].pTexture[MTT_DIFFUSE] = nullptr;
	}

	if (nullptr == pSkinInfo)
		return S_OK;

	pDerivedMeshContainer->pSkinInfo = pSkinInfo;
	pDerivedMeshContainer->pSkinInfo->AddRef();

	//pMesh->CloneMeshFVF(pDerivedMeshContainer->MeshData.pMesh->GetOptions(), pDerivedMeshContainer->MeshData.pMesh->GetFVF(), m_pGraphicDev, &pDerivedMeshContainer->pOriMesh);
	pMesh->CloneMesh(pDerivedMeshContainer->MeshData.pMesh->GetOptions(), Decl, m_pGraphicDev, &pDerivedMeshContainer->pOriMesh);

	// �޽� �����̳ʰ� ������ �޴� ���� ������ ��ȯ�ϴ� �Լ�
	pDerivedMeshContainer->dwNumBones = pDerivedMeshContainer->pSkinInfo->GetNumBones();

	pDerivedMeshContainer->pFrameOffsetMatrix = new _matrix[pDerivedMeshContainer->dwNumBones];
	ZeroMemory(pDerivedMeshContainer->pFrameOffsetMatrix, sizeof(_matrix) * pDerivedMeshContainer->dwNumBones);

	pDerivedMeshContainer->ppFrameCombinedMatrix = new _matrix*[pDerivedMeshContainer->dwNumBones];
	ZeroMemory(pDerivedMeshContainer->ppFrameCombinedMatrix, sizeof(_matrix*) * pDerivedMeshContainer->dwNumBones);

	pDerivedMeshContainer->pRenderingMatrix = new _matrix[pDerivedMeshContainer->dwNumBones];
	ZeroMemory(pDerivedMeshContainer->pRenderingMatrix, sizeof(_matrix) * pDerivedMeshContainer->dwNumBones);

	for (_ulong i = 0; i < pDerivedMeshContainer->dwNumBones; ++i)
		pDerivedMeshContainer->pFrameOffsetMatrix[i] = *pDerivedMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i);	// �޽��� �׸��� ���� ������� ������ �ε��� ���� ������ ����. �׷��� ��ȸ�ϸ鼭 ��� ������ ���ʴ�� ���� �� �ִ�.
	
	*ppNewMeshContainer = pDerivedMeshContainer;

	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree)
{
	Safe_Delete_Array(pFrameToFree->Name);

	if (nullptr != pFrameToFree->pMeshContainer)
		DestroyMeshContainer(pFrameToFree->pMeshContainer);

	if (nullptr != pFrameToFree->pFrameSibling)
		DestroyFrame(pFrameToFree->pFrameSibling);

	if (nullptr != pFrameToFree->pFrameFirstChild)
		DestroyFrame(pFrameToFree->pFrameFirstChild);

	Safe_Delete(pFrameToFree);

	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	D3DXMESHCONTAINER_DERIVED*		pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree;
	
	for (_ulong i = 0; i < pMeshContainer->NumMaterials; ++i)
	{
		for (_ulong j = 0; j < MTT_END; ++j)
			Safe_Release(pMeshContainer->pMeshTexture[i].pTexture[j]);
	}

	Safe_Delete_Array(pMeshContainer->pMeshTexture);
	Safe_Delete_Array(pMeshContainer->Name);
	Safe_Delete_Array(pMeshContainer->pAdjacency);
	Safe_Delete_Array(pMeshContainer->pFrameOffsetMatrix);
	Safe_Delete_Array(pMeshContainer->pMaterials);
	Safe_Delete_Array(pMeshContainer->ppFrameCombinedMatrix);
	Safe_Delete_Array(pMeshContainer->pRenderingMatrix);
	
	Safe_Release(pMeshContainer->MeshData.pMesh);
	Safe_Release(pMeshContainer->pOriMesh);
	Safe_Release(pMeshContainer->pSkinInfo);

	Safe_Delete(pMeshContainer);

	return S_OK;
}

void CHierarchyLoader::Allocate_Name(char ** ppName, const char * pFrameName)
{
	if (nullptr == pFrameName)
		return;

	size_t		iLength = strlen(pFrameName);

	*ppName = new char[iLength + 1];

	strcpy_s(*ppName, iLength + 1, pFrameName);

}

CHierarchyLoader * CHierarchyLoader::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrPath)
{
	return new CHierarchyLoader(pGraphicDev, wstrPath);
}

_ulong CHierarchyLoader::Release(void)
{
	Safe_Release(m_pGraphicDev);

	delete this;

	return 0;
}

