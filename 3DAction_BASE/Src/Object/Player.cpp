#include <string>
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/GravityManager.h"
#include "../Manager/Camera.h"
#include "Common/AnimationController.h"
#include "Common/Capsule.h"
#include "Common/Collider.h"
#include "Common/SpeechBalloon.h"
#include "Planet.h"
#include "Player.h"

Player::Player(SceneManager* manager)
{
	mSceneManager = manager;
	mResourceManager = manager->GetResourceManager();
	mGravityManager = manager->GetGravityManager();

	mAnimationController = nullptr;
	mState = STATE::NONE;
}

void Player::Init(void)
{

	// モデルの基本設定
	mTransform.SetModel(mResourceManager->LoadModelDuplicate(
		ResourceManager::SRC::PLAYER));
	mTransform.scl = AsoUtility::VECTOR_ONE;
	mTransform.pos = { 0.0f, -30.0f, 0.0f };
	mTransform.quaRot = Quaternion();
	mTransform.quaRotLocal = 
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	mTransform.Update();

	// アニメーションの設定
	InitAnimation();

	ChangeState(Player::STATE::PLAY);
}

void Player::InitAnimation(void)
{

	std::string path = "Data/Model/Player/";
	mAnimationController = new AnimationController(mSceneManager, mTransform.modelId);
	mAnimationController->Add((int)ANIM_TYPE::IDLE, path + "Idle.mv1", 20.0f);
	mAnimationController->Add((int)ANIM_TYPE::RUN, path + "Run.mv1", 20.0f);
	mAnimationController->Add((int)ANIM_TYPE::FAST_RUN, path + "FastRun.mv1", 20.0f);
	mAnimationController->Add((int)ANIM_TYPE::JUMP, path + "Jump.mv1", 60.0f);
	mAnimationController->Add((int)ANIM_TYPE::WARP_PAUSE, path + "WarpPose.mv1", 60.0f);
	mAnimationController->Add((int)ANIM_TYPE::FLY, path + "Flying.mv1", 60.0f);
	mAnimationController->Add((int)ANIM_TYPE::FALLING, path + "Falling.mv1", 80.0f);
	mAnimationController->Add((int)ANIM_TYPE::VICTORY, path + "Victory.mv1", 60.0f);

	mAnimationController->Play((int)ANIM_TYPE::IDLE);

}

void Player::Update(void)
{

	switch (mState)
	{
	case Player::STATE::NONE:
		break;
	case Player::STATE::PLAY:
		UpdatePlay();
		break;
	case Player::STATE::WARP_RESERVE:
		break;
	case Player::STATE::WARP_MOVE:
		break;
	case Player::STATE::DEAD:
		break;
	case Player::STATE::VICTORY:
		break;
	case Player::STATE::END:
		break;
	}

	mTransform.Update();
	mAnimationController->Update();
	
}

void Player::UpdatePlay(void)
{
	
	ProcessMove();

	//
	mMovedPos = VAdd(mTransform.pos, mMovePow);

	//移動
	mTransform.pos = mMovedPos;

	//回転をTransformに反映
	mTransform.quaRot = mGravityManager->GetTransform()->quaRot; 
	mTransform.quaRot = mTransform.quaRot.Mult(mPlayerRotY);
}

void Player::Draw(void)
{

	// モデルの描画
	MV1DrawModel(mTransform.modelId);

	// デバッグ用描画
	DrawDebug();

}

void Player::DrawDebug(void)
{

	int white = 0xffffff;
	int black = 0x000000;
	int red = 0xff0000;
	int green = 0x00ff00;
	int blue = 0x0000ff;
	int yellow = 0xffff00;
	int purpl = 0x800080;

	VECTOR v;

	// キャラ基本情報
	//-------------------------------------------------------
	// キャラ座標
	v = mTransform.pos;
	DrawFormatString(20, 60, black, "キャラ座標 ： (%0.2f, %0.2f, %0.2f)",
		v.x, v.y, v.z
	);
	//-------------------------------------------------------
	DrawLine3D(mGravHitUp, mGravHitDown, 0x000000);
}

void Player::Release(void)
{
}

void Player::ProcessMove(void)
{
	mMovePow = AsoUtility::VECTOR_ZERO;

	mSpeed = SPEED_MOVE;
	if (CheckHitKey(KEY_INPUT_RSHIFT))
	{
		mSpeed = SPEED_RUN;
	}

	//mMoveDir = { 0.0f,0.0f,0.0f };

	//x軸を除いたカメラ角度
	Quaternion cameraRot=mSceneManager->GetCamera()->GetQuaRotOutX();

	VECTOR dir = AsoUtility::VECTOR_ZERO;

	double rotRad = 0.0;

	//プレイヤー移動
	if (CheckHitKey(KEY_INPUT_W)) 
	{ dir = VAdd(dir, cameraRot.GetForward());
	rotRad=AsoUtility::Deg2RadD(0.0);
	}
	if (CheckHitKey(KEY_INPUT_S)) 
	{ dir = VAdd(dir, cameraRot.GetBack());
	rotRad=AsoUtility::Deg2RadD(180.0);
	}
	if (CheckHitKey(KEY_INPUT_A))
	{ dir = VAdd(dir, cameraRot.GetLeft()); 
	rotRad = AsoUtility::Deg2RadD(270.0);
	}
	if (CheckHitKey(KEY_INPUT_D)) 
	{ dir = VAdd(dir, cameraRot.GetRight()); 
	rotRad = AsoUtility::Deg2RadD(90.0);
	}

	if (!AsoUtility::EqualsVZero(dir))
	{
		//mMoveDir = VAdd(mMoveDir, dir);

		if (mSpeed == SPEED_MOVE)
		{
			mAnimationController->Play((int)ANIM_TYPE::RUN);
		}
		else
		{
			mAnimationController->Play((int)ANIM_TYPE::FAST_RUN);
		}
		
		mMovePow = VScale(dir, mSpeed);

		//mGorlQuaRot
		SetGoalRotate(rotRad);

		

	}
	else
	{
		mAnimationController->Play((int)ANIM_TYPE::IDLE);
	}

	//移動方向に応じた回転
	Rotate();

	//mMovePow = VScale(dir, mSpeed);
}

void Player::SetGoalRotate(double rad)
{
	Quaternion cameraRot = mSceneManager->GetCamera()->GetAngles();
	mGoalQuaRotY = Quaternion::AngleAxis(cameraRot.y + rad, AsoUtility::AXIS_Y);

	Quaternion axis= Quaternion::AngleAxis
	((double)cameraRot.y + rad, AsoUtility::AXIS_Y);

	//現在のゴール回転と新たなゴール回転の角度差を求める
	double angleDiff = Quaternion::Angle(axis, mGoalQuaRotY);

	//しきい値でリセットするか判断
	if(mStepRotTime>=0.)
//開店時間リセット
	mStepRotTime = TIME_ROT;
}

void Player::Rotate(void)
{
	mStepRotTime -= mSceneManager->GetDeltaTime(); 
	mPlayerRotY = Quaternion::Slerp(mPlayerRotY, mGoalQuaRotY, 
		(TIME_ROT - mStepRotTime) / TIME_ROT);

}

void Player::GalcGravityPow(void)
{
	//

	//mJumpPow

	VECTOR dirGravity = mGravityManager->GetDirUpGravity();
	//
	float gravityPow = mGravityManager->GetPower();

	VECTOR gravity = VScale(dirGravity, gravityPow);
	mJumpPow = VAdd(mJumpPow, gravity);

}

Transform* Player::GetTransform(void)
{
	return &mTransform;
}

void Player::AddCollider(Collider* collider)
{
	mColliders.push_back(collider);
}

void Player::ClearCollider(void)
{
	mColliders.clear();
}

void Player::ChangeState(STATE state)
{

	mState = state;
	switch (mState)
	{
	case Player::STATE::NONE:
		break;
	case Player::STATE::PLAY:
		break;
	case Player::STATE::WARP_RESERVE:
		break;
	case Player::STATE::WARP_MOVE:
		break;
	case Player::STATE::DEAD:
		break;
	case Player::STATE::VICTORY:
		break;
	case Player::STATE::END:
		break;
	}

}

void Player::Collision(void)
{
	mMovedPos = VAdd(mTransform.pos, mMovePow);


	CollisionGravity();

	mTransform.pos = mMovedPos;

}

void Player::CollisionGravity(void)
{
	//ジャンプ量を移動座標に加算
	mMovedPos = VAdd(mMovedPos, mJumpPow);

	auto hit = MV1CollCheck_Line(mTransform.modelId, -1, mGravHitUp, mGravHitDown); 
	if (hit.HitFlag > 0) {// 衝突地点から、少し上に移動
		mMovedPos = VAdd(hit.HitPosition, VScale(mGravityManager->GetDirUpGravity(), 2.0f));
						  // ジャンプリセット
		mJumpPow = AsoUtility::VECTOR_ZERO;}


	mTransform.pos = mMovedPos;
}
