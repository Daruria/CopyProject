#include "stdafx.h"
#include "TimeMgr.h"

IMPLEMENT_SINGLETON(CTimeMgr)

CTimeMgr::CTimeMgr():
	m_fDeltaTime(0.f)
{
	QueryPerformanceCounter(&m_OldTimeOne);
	QueryPerformanceCounter(&m_OldTimeTwo);
	QueryPerformanceCounter(&m_CurTime);

	QueryPerformanceFrequency(&m_CPUTick);
}


CTimeMgr::~CTimeMgr()
{
}

void CTimeMgr::Update()
{
	QueryPerformanceCounter(&m_CurTime);

	if (m_CurTime.QuadPart - m_OldTimeTwo.QuadPart > m_CPUTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CPUTick);
		m_OldTimeTwo = m_CurTime;
	}

	m_fDeltaTime = float(m_CurTime.QuadPart - m_OldTimeOne.QuadPart) / m_CPUTick.QuadPart;
	m_OldTimeOne = m_CurTime;
}
