#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Utility/AsoUtility.h"
#include "Common/Transform.h"
#include "SkyDome.h"

SkyDome::SkyDome(SceneManager* manager, Transform* syncTransform)
{
	mSceneManager = manager;
	mSyncTransform = syncTransform;

	mState = STATE::NONE;
}

void SkyDome::Init(void)
{

	// モデル制御の基本情報
	mTransform.SetModel(
		mSceneManager->GetResourceManager()->LoadModelDuplicate(
			ResourceManager::SRC::SKY_DOME));
	float scale = 100.0f;
	mTransform.scl = { scale, scale, scale };
	mTransform.pos = AsoUtility::VECTOR_ZERO;
	mTransform.quaRot = Quaternion::Euler(
		0.0f, 
		AsoUtility::Deg2RadF(180.0f),
		0.0f
	);
	mTransform.quaRotLocal = Quaternion();
	mTransform.Update();

	// Zバッファ無効(突き抜け対策)
	MV1SetUseZBuffer(mTransform.modelId, false);
	MV1SetWriteZBuffer(mTransform.modelId, false);

	// 状態遷移
	if (mSyncTransform == nullptr)
	{
		ChangeState(STATE::STAY);
	}
	else
	{
		ChangeState(STATE::FOLLOW);
	}

}

void SkyDome::Update(void)
{

	switch (mState)
	{
	case SkyDome::STATE::STAY:
		break;
	case SkyDome::STATE::FOLLOW:
		mTransform.pos = mSyncTransform->pos;
		break;
	default:
		break;
	}

	mTransform.Update();

}

void SkyDome::Draw(void)
{
	MV1DrawModel(mTransform.modelId);
}

void SkyDome::Release(void)
{
}

void SkyDome::ChangeState(STATE state)
{

	mState = state;
	switch (mState)
	{
	case SkyDome::STATE::STAY:
		mTransform.pos = AsoUtility::VECTOR_ZERO;
		break;
	case SkyDome::STATE::FOLLOW:
		break;
		mTransform.pos = mSyncTransform->pos;
	default:
		break;
	}

	mTransform.Update();

}
