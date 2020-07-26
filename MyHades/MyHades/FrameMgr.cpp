#include "stdafx.h"
#include "FrameMgr.h"


IMPLEMENT_SINGLETON(CFrameMgr)

CFrameMgr::CFrameMgr():
	m_fTimeCount(0.f), m_fTimeCountForRender(0.f),
	m_iFpsCount(0), m_szFPS(L""),
	m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTimeMgr(CTimeMgr::GetInstance())
{
	QueryPerformanceCounter(&m_OldTimeOne);
	QueryPerformanceCounter(&m_OldTimeOne);
	QueryPerformanceCounter(&m_OldTimeOne);

	QueryPerformanceFrequency(&m_CPUTick);
}

CFrameMgr::~CFrameMgr()
{
}

bool CFrameMgr::LimitFrame(float fFps)
{
	QueryPerformanceCounter(&m_CurTime);

	// ���� �ð��� ���� ������ CPU ������ ����
	if (m_CurTime.QuadPart - m_OldTimeTwo.QuadPart > m_CPUTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CPUTick);
		m_OldTimeTwo = m_CurTime;
	}

	m_fTimeCount += float(m_CurTime.QuadPart - m_OldTimeOne.QuadPart) / m_CPUTick.QuadPart;
	m_OldTimeOne = m_CurTime;

	if (1.f / fFps <= m_fTimeCount)
	{
		m_fTimeCount = 0.f;
		return true;
	}

	return false;
}

void CFrameMgr::RenderFPS()
{
	// FPS ���
	++m_iFpsCount;

	m_fTimeCountForRender += m_pTimeMgr->GetDelta();

	if (1.f <= m_fTimeCountForRender) // 1�ʰ� ������.
	{
		swprintf_s(m_szFPS, L"FPS: %d", m_iFpsCount);

		m_fTimeCountForRender = 0.f;
		m_iFpsCount = 0;
	}

	// ���̷�Ʈ ��Ʈ ���
	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, 10.f, 10.f, 0.f);

	m_pDeviceMgr->GetSprite()->SetTransform(&matTrans);

	m_pDeviceMgr->GetFont()->DrawText(
		m_pDeviceMgr->GetSprite(), /* ��������Ʈ COM ��ü */
		m_szFPS, /* ����� ���ڿ� */
		lstrlen(m_szFPS), /* ���ڿ� ���� */
		nullptr, /* �簢�� ���� */
		0,
		D3DCOLOR_ARGB(255, 255, 0, 255));


	wstring wstrDelta = L"DeletaTime: " + to_wstring(m_pTimeMgr->GetDelta());

	D3DXMatrixTranslation(&matTrans, 250.f, 10.f, 0.f);

	m_pDeviceMgr->GetSprite()->SetTransform(&matTrans);
	m_pDeviceMgr->GetFont()->DrawText(
		m_pDeviceMgr->GetSprite(), /* ��������Ʈ COM ��ü */
		wstrDelta.c_str(), /* ����� ���ڿ� */
		wstrDelta.length(), /* ���ڿ� ���� */
		nullptr, /* �簢�� ���� */
		0,
		D3DCOLOR_ARGB(255, 255, 0, 255));



}
