#include "StaticMesh.h"
#include "MyMath.h"
#include "Texture.h"

#include "Export_Function.h"

USING(Engine)

Engine::CStaticMesh::CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMesh(pGraphicDev)
{

}

Engine::CStaticMesh::CStaticMesh(const CStaticMesh& rhs)
	: CMesh(rhs)
	, m_pMesh(rhs.m_pMesh)
	, m_pAdjacency(rhs.m_pAdjacency)
	, m_pSubsetBuffer(rhs.m_pSubsetBuffer)
	, m_pMtrl(rhs.m_pMtrl)
	, m_dwSubsetCnt(rhs.m_dwSubsetCnt)
	, m_dwStride(rhs.m_dwStride)
	, m_dwNumVtx(rhs.m_dwNumVtx)
	, m_pVtxPos(rhs.m_pVtxPos)
	, m_pOriMesh(rhs.m_pOriMesh)
	, m_vCenter(rhs.m_vCenter)
	, m_fRadius(rhs.m_fRadius)
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

Engine::CStaticMesh::~CStaticMesh(void)
{

}

HRESULT Engine::CStaticMesh::Ready_Meshes(const wstring& wstrFilePath, const wstring& wstrFileName)
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

	// 만약에 메쉬에 노말 정보가 없는 경우
	//_ulong	dwFVF = m_pOriMesh->GetFVF();	// 메쉬의 정점 정보를 얻어오는 함수

	//if (!(dwFVF & D3DFVF_NORMAL))
	//{
	//	// fvf 정보만 채워준 채 메쉬 객체를 생성
	//	m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pGraphicDev, &m_pMesh);
	//	
	//	// 각 폴리곤 또는 정점 기준의 법선을 계산하여 삽입해주는 함수
	//	D3DXComputeNormals(m_pMesh, (_ulong*)m_pAdjacency->GetBufferPointer());
	//}
	//else
		//m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF, m_pGraphicDev, &m_pMesh);

	D3DVERTEXELEMENT9		Decl[MAX_FVF_DECL_SIZE];
	ZeroMemory(Decl, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);

	m_pOriMesh->GetDeclaration(Decl);
	m_pOriMesh->CloneMesh(m_pOriMesh->GetOptions(), Decl, m_pGraphicDev, &m_pMesh);

	void*		pVertex = nullptr;

	m_dwNumVtx = m_pMesh->GetNumVertices();	// 현재 메쉬가 지닌 정점의 개수를 반환하는 함수

	m_pVtxPos = new _vec3[m_dwNumVtx];

	m_pMesh->LockVertexBuffer(0, &pVertex);

	_ushort		sOffset = 0;

	for (_ulong i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		if (Decl[i].Usage == D3DDECLUSAGE_POSITION)
		{
			sOffset = Decl[i].Offset;
			break;
		}
	}

	//// fvf의 사이즈를 통해 메쉬가 지닌 정점의 크기를 얻어온다
	m_dwStride = D3DXGetDeclVertexSize(Decl, 0);

	for (_ulong i = 0; i < m_dwNumVtx; ++i)
	{
		m_pVtxPos[i] = *((_vec3*)(((_ubyte*)pVertex) + (m_dwStride * i + sOffset)));
	}

	m_pMesh->UnlockVertexBuffer();

	D3DXComputeBoundingSphere(m_pVtxPos, m_dwNumVtx, sizeof(_vec3), &m_vCenter, &m_fRadius);

	//DWORD dwBufferSize = m_pSubsetBuffer->GetBufferSize();

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


		if (CMyMath::Search_MreFile(wstrFilePath.c_str(), szFileName, &wstrAddFileName))
		{
			ZeroMemory(szFullPath, sizeof(_tchar) * 256);
			lstrcpy(szFullPath, wstrFilePath.c_str());
			lstrcat(szFullPath, wstrAddFileName.c_str());

			pTexture = dynamic_cast<CTexture*>(Get_Resources(RESOURCE_STAGE, wstrAddFileName));
			if (!pTexture)
				Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, wstrAddFileName.c_str(), TEX_NORMAL, szFullPath);

			m_pMeshTexture[i].pTexture[MTT_MRE] = dynamic_cast<CTexture*>(Engine::Clone(RESOURCE_STAGE, wstrAddFileName));
		}
	}

	return S_OK;
}

void Engine::CStaticMesh::Render_Meshes(void)
{
	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		//m_pGraphicDev->SetTexture(0, m_pMeshTexture[i].pTexture[MTT_DIFFUSE]);
		m_pMeshTexture[i].pTexture[MTT_DIFFUSE]->Render_Texture();
		m_pMesh->DrawSubset(i);
	}
}

void CStaticMesh::Render_Meshes(LPD3DXEFFECT & pEffect)
{
	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		_bool	bAlpha = false;
		_uint	iPass = 0;

		//if (bAlpha = Find_Alpha(m_pMtrl[i].pTextureFilename))
		//	iPass = 1;
		
		for (_ulong j = 0; j < MTT_END; ++j)
		{
			string strShaderTexture = "";

			switch (j)
			{
			case MTT_DIFFUSE:
				//pEffect->SetTexture("g_BaseTexture", m_pMeshTexture[i].pTexture[j]);
				strShaderTexture = "g_BaseTexture";
				break;
			case MTT_NORMAL:
				//pEffect->SetTexture("g_NormalTexture", m_pMeshTexture[i].pTexture[j]);
				//m_pMeshTexture[i].pTexture[j]->Render_Texture("g_NormalTexture", pEffect);
				strShaderTexture = "g_NormalTexture";
				break;

			case MTT_MRE:
				strShaderTexture = "g_MreTexture";
				break;
			}

			if (m_pMeshTexture[i].pTexture[j])
			{
				m_pMeshTexture[i].pTexture[j]->Render_Texture(strShaderTexture.c_str(), pEffect);

				//pEffect->SetTexture(strShaderTexture.c_str(), m_pMeshTexture[i].pTexture[j]);

			}

		}

		pEffect->CommitChanges();

		pEffect->BeginPass(iPass);

		m_pMesh->DrawSubset(i);

		pEffect->EndPass();
		//pEffect->SetTexture("g_BaseTexture", m_pMeshTexture[i].pTexture[MTT_DIFFUSE]);

		//pEffect->CommitChanges();

		//pEffect->BeginPass(iPass);

		//m_pMesh->DrawSubset(i);

		//pEffect->EndPass();
	}
}


void CStaticMesh::Render_Meshes_WithPass(LPD3DXEFFECT & pEffect, const _uint & iPass)
{
	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		_bool	bAlpha = false;

		for (_ulong j = 0; j < MTT_END; ++j)
		{
			string strShaderTexture = "";

			switch (j)
			{
			case MTT_DIFFUSE:	strShaderTexture = "g_BaseTexture";		break;
			case MTT_NORMAL:	strShaderTexture = "g_NormalTexture";	break;
			case MTT_MRE:		strShaderTexture = "g_MreTexture";		break;
			}

			if (m_pMeshTexture[i].pTexture[j])
				m_pMeshTexture[i].pTexture[j]->Render_Texture(strShaderTexture.c_str(), pEffect);
		}

		pEffect->CommitChanges();

		pEffect->BeginPass(iPass);

		m_pMesh->DrawSubset(i);

		pEffect->EndPass();
	}
}

void CStaticMesh::Render_Meshes(LPD3DXEFFECT & pEffect, const _uint& iTexture)
{
	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		_bool	bAlpha = false;
		_uint	iPass = 0;

		if (bAlpha = Find_Alpha(m_pMtrl[i].pTextureFilename))
			iPass = 1;

		if (iTexture >= MTT_END)
		{
			pEffect->CommitChanges();

			pEffect->BeginPass(iPass);

			m_pMesh->DrawSubset(i);

			pEffect->EndPass();

			continue;
		}
		string strShaderTexture = "";

		switch (iTexture)
		{
		case MTT_DIFFUSE:	strShaderTexture = "g_BaseTexture";		break;
		case MTT_NORMAL:	strShaderTexture = "g_NormalTexture";	break;
		case MTT_MRE:		strShaderTexture = "g_MreTexture";		break;
		}

		if (m_pMeshTexture[i].pTexture[iTexture])
			m_pMeshTexture[i].pTexture[iTexture]->Render_Texture(strShaderTexture.c_str(), pEffect);
		
		pEffect->CommitChanges();

		pEffect->BeginPass(iPass);

		m_pMesh->DrawSubset(i);

		pEffect->EndPass();


	}
}


void CStaticMesh::Render_Meshes(LPD3DXEFFECT & pEffect, const _uint & iTexture, const _uint & iPass)
{
	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		_bool	bAlpha = false;

		if (iTexture >= MTT_END)
		{
			pEffect->CommitChanges();

			pEffect->BeginPass(iPass);

			m_pMesh->DrawSubset(i);

			pEffect->EndPass();

			continue;
		}
		string strShaderTexture = "";

		switch (iTexture)
		{
		case MTT_DIFFUSE:	strShaderTexture = "g_BaseTexture";		break;
		case MTT_NORMAL:	strShaderTexture = "g_NormalTexture";	break;
		case MTT_MRE:		strShaderTexture = "g_MreTexture";		break;
		}

		if (m_pMeshTexture[i].pTexture[iTexture])
			m_pMeshTexture[i].pTexture[iTexture]->Render_Texture(strShaderTexture.c_str(), pEffect);

		pEffect->CommitChanges();

		pEffect->BeginPass(iPass);

		m_pMesh->DrawSubset(i);

		pEffect->EndPass();
	}
}

void CStaticMesh::Render_Mesh(LPD3DXEFFECT & pEffect, const _uint& uiSubset)
{
	m_pMesh->DrawSubset(uiSubset);
}

Engine::CStaticMesh* Engine::CStaticMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrFilePath, const wstring& wstrFileName)
{
	CStaticMesh*	pInstance = new CStaticMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Meshes(wstrFilePath, wstrFileName)))
		Safe_Release(pInstance);

	return pInstance;
}

Engine::CResources* Engine::CStaticMesh::Clone(void)
{
	return new CStaticMesh(*this);
}

void Engine::CStaticMesh::Free(void)
{
	CMesh::Free();

	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		for (_ulong j = 0; j < MTT_END; ++j)
			Safe_Release(m_pMeshTexture[i].pTexture[j]);
	}

	Safe_Delete_Array(m_pMeshTexture);

	if (false == m_bClone)
		Safe_Delete_Array(m_pVtxPos);

	Safe_Release(m_pAdjacency);
	Safe_Release(m_pSubsetBuffer);
	Safe_Release(m_pMesh);
	Safe_Release(m_pOriMesh);
}

