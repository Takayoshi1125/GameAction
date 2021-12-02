#pragma once
#include "../Object//Common/Transform.h"
#include "../Common/Quaternion.h"
class SceneManager;
class Planet;
class Player;

class GravityManager
{

public:

	// 重力方向が完全に変わるまでの時間
	static constexpr float TIME_CHANGE_DIR = 1.0f;

	// 回転力
	static constexpr float DEFAULT_POW_SLERP = 0.08f;

	enum class STATE
	{
		IDLE,
		STAGE_CHANGE
	};

	GravityManager(SceneManager* manager);

	void Init(void);
	void Update(void);
	void UpdateIdle(void);
	void UpdateChangeStage(void);

	void Calculate(void);

	Planet* GetActivePlanet(void);
	void ChangeActivePlanet(Planet* planet);

	void SetPlayer(Player* player);

	Transform* GetTransform(void);
	
	// 重力方向を取得
	VECTOR GetDirGravity(void);

	// 重力方向の反対方向を取得
	VECTOR GetDirUpGravity(void);

	// 重力の強さを取得
	float GetPower(void);

	// 惑星切り替え前の回転情報
	Quaternion GetPremQuaRot(void);

	// プレイヤーと足元衝突した法線
	void SetLastHitNormal(VECTOR normal);

	// プレイヤーと足元衝突した位置
	void SetLastHitPos(VECTOR pos);
	VECTOR GetLastHitPos(void);

protected:

	SceneManager* mSceneManager;

	Player* mPlayer;
	Planet* mActivePlanet;

	Transform mTransform;

	// 状態
	STATE mState;

	// 重力方向
	VECTOR mDirGravity;
	Quaternion mQuaGravity;
	VECTOR mPreDirGravity;

	// 重力方向の反対方向
	VECTOR mDirUpGravity;
	Quaternion mQuaUpGravity;

	// 惑星切り替え前の回転情報
	Quaternion mPreQuaRot;

	// プレイヤーと足元衝突した法線
	VECTOR mLastHitNormal;

	// プレイヤーと足元衝突した位置
	VECTOR mLastHitPos;

	float mStep;

	// 回転力
	double mSlerpPow;

	// 状態遷移
	void ChangeState(STATE state);

	// 重力方向を計算
	VECTOR CalcDirGravity(void);

};

