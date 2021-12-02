#pragma once
#include <map>
#include "Common/Transform.h"
class SceneManager;
class ResourceManager;
class WarpStar;
class Planet;
class Player;

class Stage
{

public:

	// ステージの切り替え間隔
	static constexpr float TIME_STAGE_CHANGE = 1.0f;

	enum class NAME
	{
		MAIN_PLANET,
		FALL_PLANET,
		FLAT_PLANET_BASE,
		FLAT_PLANET_ROT01,
		FLAT_PLANET_ROT02,
		FLAT_PLANET_ROT03,
		FLAT_PLANET_ROT04,
		FLAT_PLANET_FIXED01,
		FLAT_PLANET_FIXED02,
		PLANET10,
		LAST_STAGE,
		SPECIAL_STAGE
	};

	Stage(SceneManager* manager, Player* player);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	// ステージ変更
	void ChangeStage(NAME type);

private:

	SceneManager* mSceneManager;
	ResourceManager* mResourceManager;

	Player* mPlayer;

	// アクティブなステージの情報
	NAME mActiveName;
	Planet* mActivePlanet;

	// 惑星
	std::map<NAME, Planet*> mPlanets;

	// ワープスター
	std::vector<WarpStar*> mWarpStars;

	float mStep;

	// 最初の惑星
	void MakeMainStage(void);

	// 落とし穴惑星
	void MakeFallPlanet(void);

	// 平坦な惑星
	void MakeFlatPlanet(void);

	// 最後の惑星
	void MakeLastPlanet(void);

	// 番外編の惑星
	void MakeSpecialPlanet(void);

	// ワープスター
	void MakeWarpStar(void);

	// ゴールスター
	void MakeGoalStar(void);

	// 対象ステージを取得
	Planet* GetPlanet(NAME type);

};