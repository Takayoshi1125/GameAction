#include <DxLib.h>
#include <map>
#include <string>
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager::ResourceManager(void)
{
}

ResourceManager::~ResourceManager(void)
{
	mResourcesMap.clear();
}

void ResourceManager::Init(void)
{

	Resource res;

	// タイトル画像
	res = Resource(Resource::TYPE::IMG, "Data/Image/Title.png");
	mResourcesMap.emplace(SRC::TITLE, res);

	// PushSpace
	res = Resource(Resource::TYPE::IMG, "Data/Image/PushSpace.png");
	mResourcesMap.emplace(SRC::PUSH_SPACE, res);

	// 吹き出し
	res = Resource(Resource::TYPE::IMG, "Data/Image/SpeechBalloon.png");
	mResourcesMap.emplace(SRC::SPEECH_BALLOON, res);
	
	// プレイヤー
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Player/Player.mv1");
	mResourcesMap.emplace(SRC::PLAYER, res);

	// スカイドーム
	res = Resource(Resource::TYPE::MODEL, "Data/Model/SkyDome/SkyDome.mv1");
	mResourcesMap.emplace(SRC::SKY_DOME, res);

	// 最初の惑星
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Planet/MainPlanet.mv1");
	mResourcesMap.emplace(SRC::MAIN_PLANET, res);

	// 落とし穴の惑星
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Planet/FallPlanet.mv1");
	mResourcesMap.emplace(SRC::FALL_PLANET, res);

	// 平坦な惑星01
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Planet/FlatPlanet01.mv1");
	mResourcesMap.emplace(SRC::FLAT_PLANET_01, res);

	// 平坦な惑星02
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Planet/FlatPlanet02.mv1");
	mResourcesMap.emplace(SRC::FLAT_PLANET_02, res);

	// 最後の惑星
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Planet/LastPlanet.mv1");
	mResourcesMap.emplace(SRC::LAST_PLANET, res);

	// 特別な惑星
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Planet/RoadPlanet.mv1");
	mResourcesMap.emplace(SRC::SPECIAL_PLANET, res);

	// 足煙
	res = Resource(Resource::TYPE::EFFEKSEER, "Data/Effect/Smoke/Smoke.efkefc");
	mResourcesMap.emplace(SRC::FOOT_SMOKE, res);

	// ワープスターモデル
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Star/star.mv1");
	mResourcesMap.emplace(SRC::WARP_STAR, res);

	// ワープスター用回転エフェクト
	res = Resource(Resource::TYPE::EFFEKSEER, "Data/Effect/StarDust/StarDust.efkefc");
	mResourcesMap.emplace(SRC::WARP_STAR_ROT_EFF, res);

	// ワープの軌跡線
	res = Resource(Resource::TYPE::EFFEKSEER, "Data/Effect/Warp/WarpOrbit.efkefc");
	mResourcesMap.emplace(SRC::WARP_ORBIT, res);

	// ブラックホール
	res = Resource(Resource::TYPE::EFFEKSEER, "Data/Effect/BlackHole/BlackHole.efkefc");
	mResourcesMap.emplace(SRC::BLACK_HOLE, res);

	// ゴール
	res = Resource(Resource::TYPE::MODEL, "Data/Model/GoalStar/GoalStar.mv1");
	mResourcesMap.emplace(SRC::GOAL_STAR, res);

	// Clear
	res = Resource(Resource::TYPE::IMG, "Data/Image/Congratulations.png");
	mResourcesMap.emplace(SRC::CLEAR, res);

}

void ResourceManager::Release(void)
{

	for (auto p : mLoadedMap)
	{
		p.second->Release();
		delete p.second;
	}

	mLoadedMap.clear();

}

Resource ResourceManager::Load(SRC src)
{

	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return Resource();
	}
	Resource ret = *res;
	return *res;

}

int ResourceManager::LoadModelDuplicate(SRC src)
{

	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res->mHandleId);
	res->mDuplicateModelIds.push_back(duId);

	return duId;

}

Resource* ResourceManager::_Load(SRC src)
{

	auto lPair = mLoadedMap.find(src);
	if (lPair != mLoadedMap.end())
	{
		return lPair->second;
	}

	auto rPair = mResourcesMap.find(src);
	if (rPair == mResourcesMap.end())
	{
		// 登録されていない
		return nullptr;
	}

	rPair->second.Load();
	
	// 念のためコピーコンストラクタ
	Resource* ret = new Resource(rPair->second);
	mLoadedMap.emplace(src, ret);

	return ret;

}
