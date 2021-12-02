#pragma once
#include "../Common//Quaternion.h"
#include "Stage.h"
class SceneManager;
class GravityManager;
class WarpStar;

class Planet
{

public:

	// 重力の強さ
	static constexpr float DEFAULT_GRAVITY_POW = 25.0f;

	// 重力がかかる範囲
	static constexpr float DEFAULT_GRAVITY_RADIUS = 5000.0f;

	// ゲームオーバー範囲
	static constexpr float DEFAULT_DEAD_LENGTH = 1000.0f;

	enum class TYPE
	{
		GROUND,
		SPHERE,
		TRANS_ROT,
		TRANS_CAMERA_FIXED,
		ROAD,
	};

	Planet(SceneManager* manager, Stage::NAME name, TYPE type, Transform transform);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);
	
	// 惑星種別
	TYPE GetType(void);

	// 惑星名
	Stage::NAME GetName(void);

	// 位置の設定
	void SetPosition(VECTOR pos);

	// 回転の設定
	void SetRotation(Quaternion rot);

	// 重力の強さの取得・設定
	float GetGravityPow(void);
	void SetGravityPow(float pow);
	
	// 重力範囲の取得・設定
	float GetGravityRadius(void);
	void SetGravityRadius(float radius);

	Transform* GetTransform(void);

	// 重力範囲内
	bool InRangeGravity(VECTOR pos);

	// ゲームオーバー範囲内
	bool InRangeDead(VECTOR pos);

	// ゲームオーバー範囲
	void SetDeadLength(float len);

private:

	SceneManager* mSceneManager;
	GravityManager* mGravityManager;

	Transform mTransform;

	// 惑星種別
	TYPE mType;

	// 惑星名
	Stage::NAME mName;

	// 重力の強さ
	float mGravityPow;

	// 重力範囲
	float mGravityRadius;

	// ゲームーオーバー距離
	float mDeadLength;

};
