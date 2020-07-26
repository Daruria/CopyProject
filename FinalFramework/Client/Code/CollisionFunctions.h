#ifndef CollisionFunctions_h__
#define CollisionFunctions_h__

#include "Defines.h"
#include "Engine_Defines.h"

class CCollisionFunctions
{
private:
	explicit CCollisionFunctions();
	~CCollisionFunctions();

public:
	static HRESULT	Ready_Functions();
	static void		CollisionExample(const wstring& wstrSrcTag, const wstring& wstrDestTag);
	static void		CollisionMapObject(const wstring& wstrSrcTag, const wstring& wstrDestTag);
};

#endif // CollisionFunctions_h__
