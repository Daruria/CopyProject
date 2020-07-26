#include "Mesh.h"

USING(Engine)

Engine::CMesh::CMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CResources(pGraphicDev)
	, m_bClone(false)
{

}

Engine::CMesh::CMesh(const CMesh& rhs)
	: CResources(rhs)
	, m_bClone(true)
{

}

Engine::CMesh::~CMesh(void)
{

}

void Engine::CMesh::Free(void)
{
	CResources::Free();
}

_bool CMesh::Find_Alpha(const char * pFileName)
{
	_uint	iLength = (_uint)strlen(pFileName);

	for (_uint i = 0; i < iLength; ++i)
	{
		if (pFileName[i] == '.')
		{
			if (pFileName[i - 1] == 'A')
				return true;
		}
	}

	return false;
}