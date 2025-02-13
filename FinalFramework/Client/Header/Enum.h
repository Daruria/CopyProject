#ifndef Enum_h__
#define Enum_h__

enum RESOURCETYPE { RESOURCE_STATIC, RESOURCE_LOGO, RESOURCE_STAGE, RESOURCE_END };
enum LOADINGID  { LOADING_STAGE, LOADING_BOSS, LOADING_CLEAR, LOADING_END };
enum ANIMATION_TYPE { AT_REPEAT, AT_ONCE, AT_END };

enum class EPlayer
{
	IDLE, RUN, JUMP, FALL, DASH, TEST, IMPACT_FRONT, IMPACT_BACK, IMPACT_LEFT, IMPACT_RIGHT,
	FINISH
};

enum class EWar
{
	IDLE, RUN, JUMP, FALL, DASH, TEST, IMPACT_FRONT, IMPACT_BACK, IMPACT_LEFT, IMPACT_RIGHT,
	ATK_HEAVY_01, ATK_HEAVY_01b, ATK_HEAVY_02, ATK_HEAVY_03, ATK_CHARGE_START, ATK_CHARGE_LOOP, ATK_CHARGE_END, 
	ATK_LIGHT_01, ATK_LIGHT_02, ATK_LIGHT_02b, ATK_LIGHT_03, ATK_LIGHT_03_SINGLE, ATK_LIGHT_04, ATK_LIGHT_04_SINGLE,
	ATK_FLAME_BRAND_START, ATK_FLAME_BRAND_LOOP, ATK_FLAME_BRAND_END,
	FINISH
};

enum class EStrife
{
	IDLE, RUN, JUMP, FALL, DASH, TEST, IMPACT_FRONT, IMPACT_BACK, IMPACT_LEFT, IMPACT_RIGHT,
	FINISH
};

enum class EPlayerData	{ POSITION, SCALE, ANGLE, WORLD, TRANSFORM, TRANSFORM_POINTER, HP, HPMAX, NAVI_INDEX, EEND };
enum class EBossData	{ POSITION, SCALE, ANGLE, WORLD, TRANSFORM, TRANSFORM_POINTER, HP, HPMAX, NAVI_INDEX, EEND };

#endif // Enum_h__
