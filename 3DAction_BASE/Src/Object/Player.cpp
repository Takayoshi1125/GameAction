#include <string>
#include<EffekseerForDXLib.h>
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

	//〇影画像の読み神
	mImgShadow = LoadGraph("Data/Image/Shadow.png");

	//足煙エフェクト
	mEffectSmoke = mResourceManager->Load(
		ResourceManager::SRC::FOOT_SMOKE).mHandleId;

	mIsJump = false;
	mStepJump = 0.0f;
	mJumpPow = AsoUtility::VECTOR_ZERO;

	mCapsule = new Capsule(&mTransform);
	mCapsule->SetRelativePosTop({0.0f,110.0f,0.0f});
	mCapsule->SetRelativePosDown({ 0.0f,0.0f,0.0f});

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
	ProcessJamp();

	CalcGravityPow();
	
	Collision();

	
	//回転をTransformに反映
	mTransform.quaRot = mGravityManager->GetTransform()->quaRot; 
	mTransform.quaRot = mTransform.quaRot.Mult(mPlayerRotY);

	//歩きエフェクト
	EffectFootSmoke();

}

void Player::Draw(void)
{
	
	// モデルの描画
	MV1DrawModel(mTransform.modelId);

	//
	DrawShadow();

	// デバッグ用描画
	DrawDebug();

}

void Player::DrawShadow(void)
{
	float PLAYER_SHADOW_HEIGHT=300.0f;
	float PLAYER_SHADOW_SIZE=30.0f;

	int i;
	MV1_COLL_RESULT_POLY_DIM HitResDim;
	MV1_COLL_RESULT_POLY* HitRes;
	VERTEX3D Vertex[3];
	VECTOR SlideVec;
	int ModelHandle;

	// ライティングを無効にする
	SetUseLighting(FALSE);

	// Ｚバッファを有効にする
	SetUseZBuffer3D(TRUE);

	// テクスチャアドレスモードを CLAMP にする( テクスチャの端より先は端のドットが延々続く )
	SetTextureAddressMode(DX_TEXADDRESS_CLAMP);

	// 影を落とすモデルの数だけ繰り返し
	for (auto c:mColliders)
	{
		// チェックするモデルは、jが0の時はステージモデル、1以上の場合はコリジョンモデル
		ModelHandle = c->mModelId;
		
		// プレイヤーの直下に存在する地面のポリゴンを取得
		HitResDim = MV1CollCheck_Capsule(ModelHandle, -1,
			mTransform.pos, 
			VAdd(mTransform.pos, VGet(0.0f, -PLAYER_SHADOW_HEIGHT, 0.0f)),
			PLAYER_SHADOW_SIZE);

		// 頂点データで変化が無い部分をセット
		Vertex[0].dif = GetColorU8(255, 255, 255, 255);
		Vertex[0].spc = GetColorU8(0, 0, 0, 0);
		Vertex[0].su = 0.0f;
		Vertex[0].sv = 0.0f;
		Vertex[1] = Vertex[0];
		Vertex[2] = Vertex[0];

		// 球の直下に存在するポリゴンの数だけ繰り返し
		HitRes = HitResDim.Dim;
		for (i = 0; i < HitResDim.HitNum; i++, HitRes++)
		{
			// ポリゴンの座標は地面ポリゴンの座標
			Vertex[0].pos = HitRes->Position[0];
			Vertex[1].pos = HitRes->Position[1];
			Vertex[2].pos = HitRes->Position[2];

			// ちょっと持ち上げて重ならないようにする
			SlideVec = VScale(HitRes->Normal, 0.5f);
			Vertex[0].pos = VAdd(Vertex[0].pos, SlideVec);
			Vertex[1].pos = VAdd(Vertex[1].pos, SlideVec);
			Vertex[2].pos = VAdd(Vertex[2].pos, SlideVec);

			// ポリゴンの不透明度を設定する
			Vertex[0].dif.a = 0;
			Vertex[1].dif.a = 0;
			Vertex[2].dif.a = 0;
			if (HitRes->Position[0].y > mTransform.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[0].dif.a = 128 * (1.0f - fabs(HitRes->Position[0].y - mTransform.pos.y) / PLAYER_SHADOW_HEIGHT);

			if (HitRes->Position[1].y > mTransform.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[1].dif.a = 128 * (1.0f - fabs(HitRes->Position[1].y - mTransform.pos.y) / PLAYER_SHADOW_HEIGHT);

			if (HitRes->Position[2].y > mTransform.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[2].dif.a = 128 * (1.0f - fabs(HitRes->Position[2].y - mTransform.pos.y) / PLAYER_SHADOW_HEIGHT);

			// ＵＶ値は地面ポリゴンとプレイヤーの相対座標から割り出す
			Vertex[0].u = (HitRes->Position[0].x - mTransform.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[0].v = (HitRes->Position[0].z - mTransform.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[1].u = (HitRes->Position[1].x - mTransform.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[1].v = (HitRes->Position[1].z - mTransform.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[2].u = (HitRes->Position[2].x - mTransform.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[2].v = (HitRes->Position[2].z - mTransform.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;

			// 影ポリゴンを描画
			DrawPolygon3D(Vertex, 1, mImgShadow, TRUE);
		}

		// 検出した地面ポリゴン情報の後始末
		MV1CollResultPolyDimTerminate(HitResDim);
	}

	// ライティングを有効にする
	SetUseLighting(TRUE);

	// Ｚバッファを無効にする
	SetUseZBuffer3D(FALSE);
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

	//カプセルコライダ
	mCapsule->Draw();

}

void Player::Release(void)
{
	delete mCapsule;
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

	if (!AsoUtility::EqualsVZero(dir)&&(mIsJump||IsEndLanding()))
	{
		//mMoveDir = VAdd(mMoveDir, dir);

		if (!mIsJump&&IsEndLanding())
		{
			if (mSpeed == SPEED_MOVE)
			{
				mAnimationController->Play((int)ANIM_TYPE::RUN);
			}
			else
			{
				mAnimationController->Play((int)ANIM_TYPE::FAST_RUN);
			}
		}
		
		mMovePow = VScale(dir, mSpeed);

		//mGorlQuaRot
		SetGoalRotate(rotRad);

	}
	else
	{
		if (!mIsJump&&IsEndLanding())
		{
			mAnimationController->Play((int)ANIM_TYPE::IDLE);
		}
		
	}

	//移動方向に応じた回転
	Rotate();

	//mMovePow = VScale(dir, mSpeed);
}

void Player::ProcessJamp(void)
{

	//ジャンプボタン押されたらジャンプ
	bool isHitKey = CheckHitKey(KEY_INPUT_BACKSLASH);
	if (isHitKey&&(mIsJump||IsEndLanding()))
	{
		if (mIsJump)
		{
			//ジャンプアニメーション
	//mAnimationController->Play((int)ANIM_TYPE::JUMP);
	//mAnimationController->Play((int)ANIM_TYPE::JUMP,false);
	/*mAnimationController->Play(
		(int)ANIM_TYPE::JUMP,false,13.0f,25.0f);*/

		//切り取りつつ再生が終わったら
			mAnimationController->Play(
				(int)ANIM_TYPE::JUMP, true, 13.0f, 25.0f);
			mAnimationController->SetEndLoop(23.0f, 25.0f, 5.0f);
		}

		mStepJump += mSceneManager->GetDeltaTime();
		if (mStepJump < TIME_JUMP_IN)
		{
			mJumpPow = VScale(mGravityManager->GetDirUpGravity(), POW_JUMP);
		}
		mIsJump = true;
	}
	
	if (!isHitKey)
	{
		mStepJump = TIME_JUMP_IN;
	}

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

void Player::CalcGravityPow(void)
{
	VECTOR dirGravity = mGravityManager->GetDirUpGravity();
	//
	float gravityPow = mGravityManager->GetPower();

	VECTOR gravity = VScale(dirGravity, gravityPow);
	mJumpPow = VAdd(mJumpPow, gravity);

	//内積を使ってジャンプか落下中か見分ける
	float dot = VDot(dirGravity, mJumpPow);
	if (dot >= 0.0f)
	{
		mJumpPow = gravity;
	}

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

	CollisionCapsule();

	CollisionGravity();


	//移動
	
	mMoveDiff = VSub(mMovedPos, mTransform.pos);
	mTransform.pos = mMovedPos;

}

void Player::CollisionCapsule(void)
{
	//カプセルを移動後座標に移動させる
	Transform trans = Transform(&mTransform);
	trans.pos = mMovedPos;
	trans.Update();
	Capsule cap = mCapsule->Copy(&trans);

	//カプセルとの衝突
	for (auto c : mColliders)
	{
		auto hits = MV1CollCheck_Capsule(
			c->mModelId, -1,
			cap.GetPosDown(), cap.GetPosTop(), cap.GetRadius()
		);

		for (int i = 0; i < hits.HitNum; i++)
		{
			auto hit = hits.Dim[i];

			for (int tryCnt = 0; tryCnt < 10; tryCnt++)
			{
				int pHit = HitCheck_Capsule_Triangle(
					cap.GetPosDown(), cap.GetPosTop(), cap.GetRadius(),
					hit.Position[0], hit.Position[1], hit.Position[2]
				);

				if (pHit)
				{
					mMovedPos = VAdd(mMovedPos, VScale(hit.Normal, 1.0f));

					//カプセル移動
					trans.pos = mMovedPos;
					trans.Update();
					cap = mCapsule->Copy(&trans);
					continue;

				}
				break;
			}

		}
		MV1CollResultPolyDimTerminate(hits);
	}


}

void Player::CollisionGravity(void)
{
	
	//ジャンプ量を移動座標に加算
	mMovedPos = VAdd(mMovedPos, mJumpPow);

	VECTOR dirGravity =
		mGravityManager->GetDirGravity();

	VECTOR dirUpGravity =
		mGravityManager->GetDirUpGravity();

	float gravityPow =
		mGravityManager->GetPower();

	float checkPow = 10.0f;
	mGravHitUp = 
		VAdd(mMovedPos, VScale(dirUpGravity, gravityPow));
	mGravHitUp = 
		VAdd(mGravHitUp, VScale(dirUpGravity, checkPow * 2.0f));
	mGravHitDown = 
		VAdd(mMovedPos, VScale(dirGravity, checkPow));

	for (auto c : mColliders)
	{
		auto hit = 
			MV1CollCheck_Line(
				c->mModelId, -1, mGravHitUp, mGravHitDown);
		if (hit.HitFlag > 0)
		{// 衝突地点から、少し上に移動
			mMovedPos =
				VAdd(hit.HitPosition,
					VScale(dirUpGravity, 2.0f));
			// ジャンプリセット
			mJumpPow = AsoUtility::VECTOR_ZERO;
			mStepJump = 0.0f;

			if (mIsJump)
			{
				mAnimationController->Play((int)ANIM_TYPE::JUMP, false,
					29.0f, 45.0f, false, true);
			}
			mIsJump = false;
		}
	}

}

bool Player::IsEndLanding(void)
{
	bool ret = true;
	if (mAnimationController->GetPlayType() != (int)ANIM_TYPE::JUMP)
	{
		return ret;
	}
	if (mAnimationController->IsEnd())
	{
		return ret;
	}

	return false;
}

void Player::EffectFootSmoke(void)
{
	int playeHandle = -1;

	float len = AsoUtility::SqrMagnitude(mMoveDiff);
	if (mIsJump == false)

	{
		if (mStepFootSmoke % 15 == 0 && len != 0.0f)
		{
			//エフェクト発生
			playeHandle = PlayEffekseer3DEffect(mEffectSmoke);

			SetScalePlayingEffekseer3DEffect(playeHandle, 5.0f, 5.0f, 5.0f);

			SetPosPlayingEffekseer3DEffect(playeHandle, mTransform.pos.x, mTransform.pos.y, mTransform.pos.z);
		}
	}

	mStepFootSmoke++;

}
