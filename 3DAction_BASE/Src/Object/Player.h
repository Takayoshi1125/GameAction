#pragma once
#include <DxLib.h>
#include "Common/Transform.h"
class SceneManager;
class ResourceManager;
class GravityManager;
class AnimationController;
class Capsule;
class Collider;

class Player
{

public:
	static constexpr float SPEED_MOVE = 5.0f;
	static constexpr float SPEED_RUN = 10.0f;

	//回転完了までの時間
	static constexpr float TIME_ROT = 1.0f;

	//ジャンプ力
	static constexpr float POW_JUMP = 35.0f;

	//ジャンプ受付時間
	static constexpr float TIME_JUMP_IN = 0.5f;

	// 状態
	enum class STATE
	{
		NONE,
		PLAY,
		WARP_RESERVE,
		WARP_MOVE,
		DEAD,
		VICTORY,
		END
	};

	// アニメーション種別
	enum class ANIM_TYPE
	{
		IDLE,
		RUN,
		FAST_RUN,
		JUMP,
		WARP_PAUSE,
		FLY,
		FALLING,
		VICTORY
	};

	Player(SceneManager* manager);
	void Init(void);
	void InitAnimation(void);
	void Update(void);
	void UpdatePlay(void);
	void Draw(void);
	void DrawShadow(void);
	void DrawDebug(void);
	void Release(void);

	//操作
	void ProcessMove(void);
	void ProcessJamp(void);

	//回転
	void SetGoalRotate(double rad);
	void Rotate(void);

	//重力の強さ計算
	void CalcGravityPow(void);

	Transform* GetTransform(void);

	//衝突判定に用いられるコライダ
	void AddCollider(Collider* collider);
	void ClearCollider(void);

private:

	SceneManager* mSceneManager;
	ResourceManager* mResourceManager;
	GravityManager* mGravityManager;

	Transform mTransform;

	// アニメーション
	AnimationController* mAnimationController;

	// 状態
	STATE mState;

	//移動スピード
	float mSpeed;
	//移動方向
	VECTOR mMoveDir;
	//移動量
	VECTOR mMovePow;
	//移動後の座標
	VECTOR mMovedPos;

	//回転
	Quaternion mPlayerRotY;
	Quaternion mGoalQuaRotY;
	float mStepRotTime;

	//ジャンプ量
	VECTOR mJumpPow;

	bool mIsJump;

	//ジャンプ受付時間
	float mStepJump;

	//衝突判定に用いられるコライダ
	Capsule* mCapsule;
	std::vector<Collider*>mColliders;

	VECTOR mGravHitDown;
	VECTOR mGravHitUp;


	//丸形画像
	int mImgShadow;

	//足煙
	int mEffectSmoke;
	int mStepFootSmoke;

	//フレームごとの移動量
	VECTOR mMoveDiff;

	// 状態遷移
	void ChangeState(STATE state);


	//衝突判定
	void Collision(void);
	void CollisionCapsule(void);

	void CollisionGravity(void);
	//着地モーション終了
	bool IsEndLanding(void);

	void EffectFootSmoke(void);

};

