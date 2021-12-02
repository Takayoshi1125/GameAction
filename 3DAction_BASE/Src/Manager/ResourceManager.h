#pragma once
#include <map>
#include <string>
#include "Resource.h"
class ResourceManager
{
public:

	ResourceManager(void);
	~ResourceManager(void);

	enum class SRC
	{
		TITLE,
		PUSH_SPACE,
		SPEECH_BALLOON,
		PLAYER,
		SKY_DOME,
		MAIN_PLANET,
		FALL_PLANET,
		FLAT_PLANET_01,
		FLAT_PLANET_02,
		LAST_PLANET,
		SPECIAL_PLANET,
		FOOT_SMOKE,
		WARP_STAR,
		WARP_STAR_ROT_EFF,
		WARP_ORBIT,
		BLACK_HOLE,
		GOAL_STAR,
		CLEAR,
	};

	void Init(void);
	void Release(void);

	Resource Load(SRC src);
	int LoadModelDuplicate(SRC src);

private:

	std::map<SRC, Resource> mResourcesMap;
	std::map<SRC, Resource*> mLoadedMap;

	// ì‡ïîÉçÅ[Éh
	Resource* _Load(SRC src);

};

