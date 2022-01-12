#include <DxLib.h>
#include<EffekseerForDXLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "Common/Transform.h"
#include"Common/Capsule.h"
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

	// モデルの基本情報
	mTransform.SetModel(
		mSceneManager->GetResourceManager()->LoadModelDuplicate(
			ResourceManager::SRC::WARP_STAR)
	);
	mTransform.Update();

	//Zgamukaitenn
	VECTOR angle = mTransform.quaRot.ToEuler();
	mWarpQua = Quaternion::Euler(angle.x, angle.y, 0.0f);

	//ワープ準備用の移動座標
	mWarpResarvePos = VAdd(mTransform.pos,mWarpQua.PosAxis(WARP_RELATIVE_POS));


	mEffectRotParticle = mResourceManager->Load(
		ResourceManager::SRC::WARP_STAR_ROT_EFF
	).mHandleId;

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
	RotateZ(SPEED_ROT_IDLE);

	PlayEffectRotParticle();


	//キャラクターの位置とワープスターの位置
	VECTOR vPos = mTransform.pos;
	VECTOR cPos = mPlayer->GetCapsule()->GetCenter();

	//キャラクターが範囲に入ったら準備Stateにする
	VECTOR diff = VSub(cPos, vPos);
	float dis = AsoUtility::SqrMagnitude(diff);
	if (dis < RADIUS * RADIUS)
	{
		ChangeState(WarpStar::STATE::RESERVE);
		return;
	}
}

void WarpStar::UpdateReserve(void)
{
	RotateZ(SPEED_ROT_RESERVE);

	PlayEffectRotParticle();

	if (mPlayer->IsWarpMove())
	{
		ChangeState(STATE::MOVE);
	}

}

void WarpStar::UpdateMove(void)
{
	if (mPlayer->IsPlay())
	{
		ChangeState(STATE::IDLE);
	}
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
		mPlayer->StartWarp(TIME_WARP_RESERVE,mWarpQua,mWarpResarvePos);
		break;
	case WarpStar::STATE::MOVE:
		break;
	}

}

void WarpStar::RotateZ(float speed)
{
	//Z回転

	auto axis = Quaternion::AngleAxis(
		AsoUtility::Deg2RadD(speed), AsoUtility::AXIS_Z);

	mTransform.quaRot = mTransform.quaRot.Mult(axis);

	mTransform.Update();
}

void WarpStar::PlayEffectRotParticle(void)
{
	if (mTime % 5 == 0)
	{
		int playeHandle = PlayEffekseer3DEffect(mEffectRotParticle);

		SetScalePlayingEffekseer3DEffect(playeHandle, 5.0f, 5.0f, 5.0f);

		//モデルの回転に合わせる
		Quaternion rot = mTransform.quaRot;
		VECTOR angle = rot.ToEuler();
		SetRotationPlayingEffekseer3DEffect(playeHandle,
			angle.x, angle.y, angle.z);

		VECTOR pos = VAdd(mTransform.pos, rot.PosAxis(EFFECT_RELATIVE_POS));
		SetPosPlayingEffekseer3DEffect(playeHandle, pos.x, pos.y, pos.z);
	}

	mTime++;
}
