#pragma once
#include <vector>
#include "Common/Transform.h"
class SceneManager;
class ResourceManager;
class Player;

class WarpStar
{

public:

	// 準備時間
	static constexpr float TIME_WARP_RESERVE = 2.0f;

	// 半径
	static constexpr float RADIUS = 80.0f;

	// エフェクトの位置
	static constexpr VECTOR EFFECT_RELATIVE_POS = { 0.0f, 70.0f, 0.0f };
	
	// ワープ前の準備移動相対座標
	static constexpr VECTOR WARP_RELATIVE_POS = { 0.0f, -80.0f, 10.0f };

	// 回転速度
	static constexpr float SPEED_ROT_IDLE = 3.0f;
	static constexpr float SPEED_ROT_RESERVE = 15.0f;

	enum class STATE
	{
		NONE,
		IDLE,
		RESERVE,
		MOVE
	};

	WarpStar(SceneManager* manager, Player* player, Transform transform);
	void Init(void);
	void Update(void);
	void UpdateIdle(void);
	void UpdateReserve(void);
	void UpdateMove(void);
	void Draw(void);
	void Release(void);

	Transform* GetTransform(void);

private:

	SceneManager* mSceneManager;
	ResourceManager* mResourceManager;
	Player* mPlayer;

	Transform mTransform;

	STATE mState;

	void ChangeState(STATE state);

};

