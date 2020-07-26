#ifndef __STRUCTS_H__

typedef struct tagInfo
{
	float fx;
	float fy;
	int icx;
	int icy;
}INFO, *PINFO;

typedef struct tagPosition
{
	float fx;
	float fy;
}POSITION, *PPOSITION;

typedef struct _tagCharacter
{
	int		iHP;
	int		iHPMax;
	float	fAttack;
	float	fArmor;
}CHARACTER_INFO, *PCHARACTER_INFO;

typedef struct _tagDash
{
	bool	bDash;
	float	fDashSpeed;
	float	fDashTime;
	float	fDashX;
	float	fDashY;
	float	fDashCoolDown;
	float	fDashCoolTime;
	int		iDashCount;
	int		iDashMax;
}DASH, *PDASH;

typedef struct _tagLine
{
	POSITION P1;
	POSITION P2;
}LINE, *PLINE;

typedef struct _tagSphere
{
	float fCenterX;
	float fCenterY;
	float fRadius;
}SPHERE, *PSPHERE;

typedef struct _tagStatus
{
	int		iWrath;
	int		iSwiftness;
	int		iPatience;
	int		iMystic;
	int		iGreed;
}STATUS, *PSTATUS;

#define __STRUCTS_H__
#endif