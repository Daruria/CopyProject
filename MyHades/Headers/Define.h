#pragma once

#define WINCX 1280
#define WINCY 720

#define TILECX 193
#define TILECY 98

#define TILEX 50
#define TILEY 50

#define MIN_STR 64
#define MID_STR 128
#define MAX_STR 256

#define NO_EVENT		0
#define DEAD_OBJ		1
#define CHANGE_SCENE	2

#define PI 3.1415926535f

#define MatScaling(matScale, vScale) D3DXMatrixScaling((matScale), (vScale).x, (vScale).y, (vScale).z)
#define MatTranslation(matTrans, vPos) D3DXMatrixTranslation((matTrans), (vPos).x, (vPos).y, (vPos).z);

#define NULL_CHECK(ptr) if(nullptr == (ptr)) return;
#define NULL_CHECK_CONTINUE(ptr) if(nullptr == (ptr)) continue;
#define NULL_CHECK_MSG(ptr, msg) if(nullptr == (ptr)) { ::MessageBox(0, msg, L"Error", MB_OK); return; }
#define NULL_CHECK_RETURN(ptr, val) if(nullptr == (ptr)) return val;
#define NULL_CHECK_MSG_RETURN(ptr, msg, val) if(nullptr == (ptr)) { ::MessageBox(0, msg, L"Error", MB_OK); return val; }

#define FAILED_CHECK(hr) if(FAILED(hr)) return;
#define FAILED_CHECK_MSG(hr, msg) if(FAILED(hr)) { ::MessageBox(0, msg, L"Error", MB_OK); return; }
#define FAILED_CHECK_RETURN(hr, val) if(FAILED(hr)) return val;
#define FAILED_CHECK_MSG_RETURN(hr, msg, val) if(FAILED(hr)) { ::MessageBox(0, msg, L"Error", MB_OK); return val; }

#define NO_COPY(ClassName)						\
private:										\
	ClassName(const ClassName&);				\
	ClassName& operator=(const ClassName&);

#define DECLARE_SINGLETON(ClassName)			\
		NO_COPY(ClassName)						\
public:											\
	static ClassName* GetInstance();			\
	void DestroyInstance();						\
private:										\
	static ClassName* m_pInstance;

#define IMPLEMENT_SINGLETON(ClassName)			\
ClassName* ClassName::m_pInstance = nullptr;	\
ClassName* ClassName::GetInstance()				\
{												\
	if(nullptr == m_pInstance)					\
		m_pInstance = new ClassName;			\
	return m_pInstance;							\
}												\
void ClassName::DestroyInstance()				\
{												\
	if(m_pInstance)								\
	{											\
		delete m_pInstance;						\
		m_pInstance = nullptr;					\
	}											\
}

#define MACRO_CREATE(ClassName)					\
public:											\
	static ClassName* Create()					\
	{											\
		ClassName*	pInstance = new ClassName;	\
		if (FAILED(pInstance->Initialize()))	\
		{										\
			SafeDelete(pInstance);				\
			return nullptr;						\
		}										\
		return pInstance;						\
	}