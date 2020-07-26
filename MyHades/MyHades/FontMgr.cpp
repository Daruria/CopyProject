#include "stdafx.h"
#include "FontMgr.h"

IMPLEMENT_SINGLETON(CFontMgr)

CFontMgr::CFontMgr():
	m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance())
{
}


CFontMgr::~CFontMgr()
{
	m_umapFontKeys.clear();
}

HRESULT CFontMgr::Initialize()
{
	wchar_t tch = L'0';
	wstring wstrOri = L"BigFont";

	for (int i = 0; i < 10; ++i)
	{
		m_umapFontKeys[tch] = wstrOri + tch;
		tch++;
	}

	wchar_t wc[11] = L"!#$%&()@{}";
	for (int i = 0; i < 10; ++i)
		m_umapFontKeys[wc[i]] = wstrOri + wc[i];


	wchar_t wcAlpha[27] = L"abcdefghijklmnopqrstuvwxyz";
	for (int i = 0; i < 26; ++i)
		m_umapFontKeys[wcAlpha[i]] = wstrOri + L"Small" + wcAlpha[i];

	wchar_t wcAlphaL[27] = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	for (int i = 0; i < 26; ++i)
		m_umapFontKeys[wcAlphaL[i]] = wstrOri + L"Big" + wcAlphaL[i];

	m_umapFontKeys[L'?'] = wstrOri + L"Question";
	m_umapFontKeys[L'/'] = wstrOri + L"Slash";
	m_umapFontKeys[L'*'] = wstrOri + L"Star";

	return S_OK;
}

void CFontMgr::Render(const D3DXVECTOR3 & vPos, const D3DXVECTOR3 & vSize, const wstring & wstrText, D3DCOLOR d3dColor, CFontMgr::SortX eSortX /*= CFontMgr::LEFT*/, CFontMgr::SortY eSortY /*= CFontMgr::TOP*/)
{
	int iRenderStartX = 0;
	int iRenderStartY = 0;

	D3DXVECTOR3 vRenderPos = vPos;

	for (auto wc : wstrText)
		vRenderPos.x += RenderLetter(vRenderPos, vSize, wc, d3dColor);
}

void CFontMgr::Render(const D3DXMATRIX * pMatrix, const D3DXVECTOR3& vSize, const wstring & wstrText, D3DCOLOR d3dColor, CFontMgr::SortX eSortX, CFontMgr::SortY eSortY)
{
	D3DXMATRIX mat = *pMatrix;

	for (auto wc : wstrText)
		mat = RenderLetterMatrix(&mat, vSize, wc, d3dColor);
}

UINT CFontMgr::RenderLetter(const D3DXVECTOR3 & vPos, const D3DXVECTOR3 & vSize, wchar_t wcLetter, D3DCOLOR d3dColor)
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_umapFontKeys[wcLetter]);
	NULL_CHECK_RETURN(pTexInfo, 0);

	D3DXMATRIX matScale, matTrans;
	D3DXMatrixScaling(&matScale, vSize.x, vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, 0.f);

	//D3DXVECTOR3 vCenter = { pTexInfo->tImgInfo.Width*0.5f, pTexInfo->tImgInfo.Height*0.5f, 0.f };

	m_pDeviceMgr->GetSprite()->SetTransform(&(matScale * matTrans));
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, nullptr, nullptr, d3dColor);

	return static_cast<UINT>(pTexInfo->tImgInfo.Width * vSize.x);
}

D3DXMATRIX CFontMgr::RenderLetterMatrix(D3DXMATRIX * pMatrix, const D3DXVECTOR3& vSize, wchar_t wcLetter, D3DCOLOR d3dColor)
{
	D3DXMATRIX matResult = *pMatrix;

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_umapFontKeys[wcLetter]);
	NULL_CHECK_RETURN(pTexInfo, matResult);

	m_pDeviceMgr->GetSprite()->SetTransform(pMatrix);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, nullptr, nullptr, d3dColor);

	D3DXMATRIX matMove;
	D3DXMatrixTranslation(&matMove, static_cast<UINT>(pTexInfo->tImgInfo.Width * vSize.x), 0.f, 0.f);

	return matResult * matMove;
}

UINT CFontMgr::GetTextSizeX(const D3DXVECTOR3 & vSize, const wstring & wstrText)
{
	const TEX_INFO* pTexInfo = nullptr;
	UINT uiRet = 0;

	for (auto wc : wstrText)
	{
		pTexInfo = m_pTextureMgr->GetTexInfo(m_umapFontKeys[wc]);
		NULL_CHECK_RETURN(pTexInfo, 0);
		uiRet += pTexInfo->tImgInfo.Width;
	}

	return static_cast<UINT>(uiRet * vSize.x);
}

UINT CFontMgr::GetTextSizeY(const D3DXVECTOR3 & vSize, const wstring & wstrText)
{


	return 0;
}
