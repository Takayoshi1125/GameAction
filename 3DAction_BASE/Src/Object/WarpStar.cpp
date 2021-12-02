#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "Common/Transform.h"
#include "Player.h"
#include "WarpStar.h"

WarpStar::WarpStar(SceneManager* manager, Player* player, Transform transform)
{
	mSceneManager = manager;
	mResourceManager = manager->GetResourceManager();
	mPlayer = player;
	mTransform = transform;

	mState = STATE::NONE;
}

void WarpStar::Init(void)
{

	// ƒ‚ƒfƒ‹‚ÌŠî–{î•ñ
	mTransform.SetModel(
		mSceneManager->GetResourceManager()->LoadModelDuplicate(
			ResourceManager::SRC::WARP_STAR)
	);
	mTransform.Update();

	ChangeState(STATE::IDLE);

}

void WarpStar::Update(void)
{

	switch (mState)
	{
	case WarpStar::STATE::IDLE:
		UpdateIdle();
		break;
	case WarpStar::STATE::RESERVE:
		UpdateReserve();
		break;
	case WarpStar::STATE::MOVE:
		UpdateMove();
		break;
	}

}

void WarpStar::UpdateIdle(void)
{
}

void WarpStar::UpdateReserve(void)
{
}

void WarpStar::UpdateMove(void)
{
}

void WarpStar::Draw(void)
{
	MV1DrawModel(mTransform.modelId);
}

void WarpStar::Release(void)
{
}

Transform* WarpStar::GetTransform(void)
{
	return &mTransform;
}

void WarpStar::ChangeState(STATE state)
{

	mState = state;
	switch (mState)
	{
	case WarpStar::STATE::IDLE:
		break;
	case WarpStar::STATE::RESERVE:
		break;
	case WarpStar::STATE::MOVE:
		break;
	}

}
