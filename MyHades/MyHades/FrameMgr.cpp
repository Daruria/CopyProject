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

	// 일정 시간이 지날 때마다 CPU 진동수 갱신
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
	// FPS 출력
	++m_iFpsCount;

	m_fTimeCountForRender += m_pTimeMgr->GetDelta();

	if (1.f <= m_fTimeCountForRender) // 1초가 지났다.
	{
		swprintf_s(m_szFPS, L"FPS: %d", m_iFpsCount);

		m_fTimeCountForRender = 0.f;
		m_iFpsCount = 0;
	}

	// 다이렉트 폰트 출력
	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, 10.f, 10.f, 0.f);

	m_pDeviceMgr->GetSprite()->SetTransform(&matTrans);

	m_pDeviceMgr->GetFont()->DrawText(
		m_pDeviceMgr->GetSprite(), /* 스프라이트 COM 객체 */
		m_szFPS, /* 출력할 문자열 */
		lstrlen(m_szFPS), /* 문자열 길이 */
		nullptr, /* 사각형 영역 */
		0,
		D3DCOLOR_ARGB(255, 255, 0, 255));


	wstring wstrDelta = L"DeletaTime: " + to_wstring(m_pTimeMgr->GetDelta());

	D3DXMatrixTranslation(&matTrans, 250.f, 10.f, 0.f);

	m_pDeviceMgr->GetSprite()->SetTransform(&matTrans);
	m_pDeviceMgr->GetFont()->DrawText(
		m_pDeviceMgr->GetSprite(), /* 스프라이트 COM 객체 */
		wstrDelta.c_str(), /* 출력할 문자열 */
		wstrDelta.length(), /* 문자열 길이 */
		nullptr, /* 사각형 영역 */
		0,
		D3DCOLOR_ARGB(255, 255, 0, 255));



}
