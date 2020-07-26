#ifndef Macro_h__
#define Macro_h__

#define WINCX 1920
#define WINCY 1080

//#define WINCX 1280
//#define WINCY 720

//#define WINCX 800
//#define WINCY 600

#define	STR_MAX 256

#define GRAVITY 0.98f

#define SC_INT(Enum) static_cast<int>(Enum)
#define DC_ENG(TargetClass, SrcPointer) dynamic_cast<Engine::TargetClass*>(SrcPointer)

#define AXIS_X _vec3(1.f, 0.f, 0.f)
#define AXIS_Y _vec3(0.f, 1.f, 0.f)
#define AXIS_Z _vec3(0.f, 0.f, 1.f)

#define	VTXCNTX 129
#define VTXCNTZ 129
#define VTXITV	1

#ifndef MSG_BOX
#define	MSG_BOX(_message)			MessageBox(NULL, TEXT(_message), L"System Message", MB_OK)
#endif

#ifndef NULL_CHECK
#define NULL_CHECK( _ptr)	\
{if( _ptr == 0){  __debugbreak();/*__asm { int 3 };*/return;}}
#endif


#ifndef NULL_CHECK_RETURN
#define NULL_CHECK_RETURN( _ptr, _return)	\
{if( _ptr == 0){  __debugbreak();/*__asm { int 3 };*/return _return;}}
#endif


#ifndef NULL_CHECK_MSG
#define NULL_CHECK_MSG( _ptr, _message )		\
{if( _ptr == 0){MessageBox(NULL, _message, L"System Message",MB_OK);  __debugbreak();/*__asm { int 3 };*/}}
#endif

#ifndef NULL_CHECK_RETURN_MSG
#define NULL_CHECK_RETURN_MSG( _ptr, _return, _message )	\
{if( _ptr == 0){MessageBox(NULL, _message, L"System Message",MB_OK);  __debugbreak();/*__asm { int 3 };*/return _return;}}
#endif


#ifndef FAILED_CHECK
#define FAILED_CHECK(_hr)	if( ((HRESULT)(_hr)) < 0 )	\
{ MessageBoxW(NULL, L"Failed", L"System Error",MB_OK);  __debugbreak();/* __asm { int 3 }; */return E_FAIL;}
#endif

#ifndef FAILED_CHECK_RETURN
#define FAILED_CHECK_RETURN(_hr, _return)	if( ((HRESULT)(_hr)) < 0 )		\
{ MessageBoxW(NULL, L"Failed", L"System Error",MB_OK);  __debugbreak();/* __asm { int 3 };*/ return _return;}
#endif

#ifndef FAILED_CHECK_MSG
#define FAILED_CHECK_MSG( _hr, _message)	if( ((HRESULT)(_hr)) < 0 )	\
{ MessageBoxW(NULL, _message, L"System Message",MB_OK);  __debugbreak();/*__asm { int 3 };*/return E_FAIL;}
#endif

#ifndef FAILED_CHECK_RETURN_MSG
#define FAILED_CHECK_RETURN_MSG( _hr, _return, _message)	if( ((HRESULT)(_hr)) < 0 )	\
{ MessageBoxW(NULL, _message, L"System Message",MB_OK);  __debugbreak();/* __asm { int 3 };*/return _return;}
#endif



#define NO_COPY(CLASSNAME)							\
	private:										\
	CLASSNAME(const CLASSNAME&);					\
	CLASSNAME& operator = (const CLASSNAME&);		

#define SINGLETON(CLASSNAME)						\
	NO_COPY(CLASSNAME)								\
	private:										\
	static CLASSNAME*	m_pInstance;				\
	public:											\
	static CLASSNAME*	GetInstance( void ){		\
		if(NULL == m_pInstance) {					\
			m_pInstance = new CLASSNAME;			\
		}											\
		return m_pInstance;							\
	}												\
	static void DestroyInstance( void ){			\
			if(NULL != m_pInstance)	{				\
			delete m_pInstance;						\
			m_pInstance = NULL;						\
		}											\
	}			

#define IMP_SINGLETON(CLASSNAME)					\
	CLASSNAME*	CLASSNAME::m_pInstance = NULL;

#endif // Macro_h__
