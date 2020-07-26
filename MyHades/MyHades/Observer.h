#pragma once
class CObserver
{
protected:
	CObserver();
public:
	virtual ~CObserver();

public:
	virtual void Update(int iMsg, void* pData) PURE;
};

