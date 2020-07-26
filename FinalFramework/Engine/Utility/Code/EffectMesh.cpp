#include "EffectMesh.h"
#include "MyMath.h"
#include "Texture.h"

#include "Export_Function.h"

USING(Engine)

Engine::CEffectMesh::CEffectMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMesh(pGraphicDev)
{

}

Engine::CEffectMesh::CEffectMesh(const CEffectMesh& rhs)
	: CMesh(rhs)
	, m_pMesh(rhs.m_pMesh)
	, m_pAdjacency(rhs.m_pAdjacency)
	, m_pSubsetBuffer(rhs.m_pSubsetBuffer)
	, m_pMtrl(rhs.m_pMtrl)
	, m_dwSubsetCnt(rhs.m_dwSubsetCnt)
	, m_dwStride(rhs.m_dwStride)
	, m_dwNumVtx(rhs.m_dwNumVtx)
	, m_pOriMesh(rhs.m_pOriMesh)
{
	m_pMeshTexture = new MESH_TEXTURES[rhs.m_dwSubsetCnt];

	for (_ulong i = 0; i < rhs.m_dwSubsetCnt; ++i)
	{
		for (_ulong j = 0; j < MTT_END; ++j)
		{
			
			m_pMeshTexture[i].pTexture[j] = rhs.m_pMeshTexture[i].pTexture[j];
			if (m_pMeshTexture[i].pTexture[j])
				m_pMeshTexture[i].pTexture[j]->AddRef();
		}
	}

	
	m_pMesh->AddRef();
	m_pOriMesh->AddRef();
	m_pAdjacency->AddRef();
	m_pSubsetBuffer->AddRef();
}

Engine::CEffectMesh::~CEffectMesh(void)
{

}

HRESULT Engine::CEffectMesh::Ready_Meshes(const wstring& wstrFilePath, const wstring& wstrFileName)
{
	_tchar		szFullPath[256] = L"";

	lstrcpy(szFullPath, wstrFilePath.c_str());
	lstrcat(szFullPath, wstrFileName.c_str());

	FAILED_CHECK_RETURN(D3DXLoadMeshFromX(szFullPath, 
											D3DXMESH_MANAGED, 
											m_pGraphicDev, 
											&m_pAdjacency, 
											&m_pSubsetBuffer, 
											NULL, 
											&m_dwSubsetCnt, 
											&m_pOriMesh),
											E_FAIL);

	D3DVERTEXELEMENT9		Decl[MAX_FVF_DECL_SIZE];
	ZeroMemory(Decl, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);

	m_pOriMesh->GetDeclaration(Decl);
	m_pOriMesh->CloneMesh(m_pOriMesh->GetOptions(), Decl, m_pGraphicDev, &m_pMesh);
	
	m_dwNumVtx = m_pMesh->GetNumVertices();	// 현재 메쉬가 지닌 정점의 개수를 반환하는 함수


	for (_ulong i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		if (Decl[i].Usage == D3DDECLUSAGE_TEXCOORD)
		{
			m_sOffset = Decl[i].Offset;
			break;
		}
	}

	m_dwStride = D3DXGetDeclVertexSize(Decl, 0);

	m_pMtrl = (D3DXMATERIAL*)m_pSubsetBuffer->GetBufferPointer();
	
	m_pMeshTexture = new MESH_TEXTURES[m_dwSubsetCnt];
	ZeroMemory(m_pMeshTexture, sizeof(MESH_TEXTURES) * m_dwSubsetCnt);
	
	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		_tchar	szFileName[256] = L"";
		lstrcpy(szFullPath, wstrFilePath.c_str());

		MultiByteToWideChar(CP_ACP, 0, m_pMtrl[i].pTextureFilename, (int)strlen(m_pMtrl[i].pTextureFilename), szFileName, 256);
		lstrcat(szFullPath, szFileName);
		
		CTexture* pTexture = dynamic_cast<CTexture*>(Get_Resources(RESOURCE_STAGE, szFileName));
		if (!pTexture)
			Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, szFileName, TEX_NORMAL, szFullPath);
		
		m_pMeshTexture[i].pTexture[MTT_DIFFUSE] = dynamic_cast<CTexture*>(Engine::Clone(RESOURCE_STAGE, szFileName));


		wstring wstrAddFileName;

		if (CMyMath::Search_NormalFile(wstrFilePath.c_str(), szFileName, &wstrAddFileName))
		{
			ZeroMemory(szFullPath, sizeof(_tchar) * 256);
			lstrcpy(szFullPath, wstrFilePath.c_str());
			lstrcat(szFullPath, wstrAddFileName.c_str());

			pTexture = dynamic_cast<CTexture*>(Get_Resources(RESOURCE_STAGE, wstrAddFileName));
			if (!pTexture)
				Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, wstrAddFileName.c_str(), TEX_NORMAL, szFullPath);

			m_pMeshTexture[i].pTexture[MTT_NORMAL] = dynamic_cast<CTexture*>(Engine::Clone(RESOURCE_STAGE, wstrAddFileName));

		}
	}

	return S_OK;
}

void Engine::CEffectMesh::Render_Meshes(void)
{
	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		m_pMeshTexture[i].pTexture[MTT_DIFFUSE]->Render_Texture();
		m_pMesh->DrawSubset(i);
	}
}

void CEffectMesh::Render_Meshes(LPD3DXEFFECT & pEffect)
{
	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		_bool	bAlpha = false;
		_uint	iPass = 0;

		if (bAlpha = Find_Alpha(m_pMtrl[i].pTextureFilename))
			iPass = 1;
		
		for (_ulong j = 0; j < MTT_END; ++j)
		{
			string strShaderTexture = "";


			switch (j)
			{
			case MTT_DIFFUSE:
				strShaderTexture = "g_BaseTexture";
				break;
			case MTT_NORMAL:
				strShaderTexture = "g_NormalTexture";
				break;
			}

			if (m_pMeshTexture[i].pTexture[j])
			{
				m_pMeshTexture[i].pTexture[j]->Render_Texture(strShaderTexture.c_str(), pEffect);

				pEffect->CommitChanges();

				pEffect->BeginPass(iPass);

				m_pMesh->DrawSubset(i);

				pEffect->EndPass();
			}
		}
	}
}

void CEffectMesh::Change_UV(const _vec2 * pUV, const _float & fTimeDelta)
{
	void* pVertex = nullptr;

	_vec2 vUV = *pUV * fTimeDelta;
	m_vUV += vUV;

	if (m_vUV.x > 2.f)
	{
		m_vUV.x -= 1.f;
		vUV.x -= 1.f;
	}

	if (m_vUV.y > 2.f)
	{
		m_vUV.y -= 1.f;
		vUV.y -= 1.f;
	}
	

	m_pMesh->LockVertexBuffer(0, &pVertex);

	for (_ulong i = 0; i < m_dwNumVtx; ++i)
	{
		*((_vec2*)(((_ubyte*)pVertex) + (m_dwStride * i + m_sOffset))) += vUV;
	}

	m_pMesh->UnlockVertexBuffer();
}

Engine::CEffectMesh* Engine::CEffectMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrFilePath, const wstring& wstrFileName)
{
	CEffectMesh*	pInstance = new CEffectMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Meshes(wstrFilePath, wstrFileName)))
		Safe_Release(pInstance);

	return pInstance;
}

Engine::CResources* Engine::CEffectMesh::Clone(void)
{
	return new CEffectMesh(*this);
}

void Engine::CEffectMesh::Free(void)
{
	CMesh::Free();

	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		for (_ulong j = 0; j < MTT_END; ++j)
			Safe_Release(m_pMeshTexture[i].pTexture[j]);
	}

	Safe_Delete_Array(m_pMeshTexture);

	Safe_Release(m_pAdjacency);
	Safe_Release(m_pSubsetBuffer);
	Safe_Release(m_pMesh);
	Safe_Release(m_pOriMesh);
}

