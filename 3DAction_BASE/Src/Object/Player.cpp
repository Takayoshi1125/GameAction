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

	// ���f���̊�{�ݒ�
	mTransform.SetModel(mResourceManager->LoadModelDuplicate(
		ResourceManager::SRC::PLAYER));
	mTransform.scl = AsoUtility::VECTOR_ONE;
	mTransform.pos = { 0.0f, -30.0f, 0.0f };
	mTransform.quaRot = Quaternion();
	mTransform.quaRotLocal = 
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	mTransform.Update();

	//�Z�e�摜�̓ǂݐ_
	mImgShadow = LoadGraph("Data/Image/Shadow.png");

	//�����G�t�F�N�g
	mEffectSmoke = mResourceManager->Load(
		ResourceManager::SRC::FOOT_SMOKE).mHandleId;

	mIsJump = false;
	mStepJump = 0.0f;
	mJumpPow = AsoUtility::VECTOR_ZERO;

	mCapsule = new Capsule(&mTransform);
	mCapsule->SetRelativePosTop({0.0f,110.0f,0.0f});
	mCapsule->SetRelativePosDown({ 0.0f,0.0f,0.0f});

	// �A�j���[�V�����̐ݒ�
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

	
	//��]��Transform�ɔ��f
	mTransform.quaRot = mGravityManager->GetTransform()->quaRot; 
	mTransform.quaRot = mTransform.quaRot.Mult(mPlayerRotY);

	//�����G�t�F�N�g
	EffectFootSmoke();

}

void Player::Draw(void)
{
	
	// ���f���̕`��
	MV1DrawModel(mTransform.modelId);

	//
	DrawShadow();

	// �f�o�b�O�p�`��
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

	// ���C�e�B���O�𖳌��ɂ���
	SetUseLighting(FALSE);

	// �y�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(TRUE);

	// �e�N�X�`���A�h���X���[�h�� CLAMP �ɂ���( �e�N�X�`���̒[����͒[�̃h�b�g�����X���� )
	SetTextureAddressMode(DX_TEXADDRESS_CLAMP);

	// �e�𗎂Ƃ����f���̐������J��Ԃ�
	for (auto c:mColliders)
	{
		// �`�F�b�N���郂�f���́Aj��0�̎��̓X�e�[�W���f���A1�ȏ�̏ꍇ�̓R���W�������f��
		ModelHandle = c->mModelId;
		
		// �v���C���[�̒����ɑ��݂���n�ʂ̃|���S�����擾
		HitResDim = MV1CollCheck_Capsule(ModelHandle, -1,
			mTransform.pos, 
			VAdd(mTransform.pos, VGet(0.0f, -PLAYER_SHADOW_HEIGHT, 0.0f)),
			PLAYER_SHADOW_SIZE);

		// ���_�f�[�^�ŕω��������������Z�b�g
		Vertex[0].dif = GetColorU8(255, 255, 255, 255);
		Vertex[0].spc = GetColorU8(0, 0, 0, 0);
		Vertex[0].su = 0.0f;
		Vertex[0].sv = 0.0f;
		Vertex[1] = Vertex[0];
		Vertex[2] = Vertex[0];

		// ���̒����ɑ��݂���|���S���̐������J��Ԃ�
		HitRes = HitResDim.Dim;
		for (i = 0; i < HitResDim.HitNum; i++, HitRes++)
		{
			// �|���S���̍��W�͒n�ʃ|���S���̍��W
			Vertex[0].pos = HitRes->Position[0];
			Vertex[1].pos = HitRes->Position[1];
			Vertex[2].pos = HitRes->Position[2];

			// ������Ǝ����グ�ďd�Ȃ�Ȃ��悤�ɂ���
			SlideVec = VScale(HitRes->Normal, 0.5f);
			Vertex[0].pos = VAdd(Vertex[0].pos, SlideVec);
			Vertex[1].pos = VAdd(Vertex[1].pos, SlideVec);
			Vertex[2].pos = VAdd(Vertex[2].pos, SlideVec);

			// �|���S���̕s�����x��ݒ肷��
			Vertex[0].dif.a = 0;
			Vertex[1].dif.a = 0;
			Vertex[2].dif.a = 0;
			if (HitRes->Position[0].y > mTransform.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[0].dif.a = 128 * (1.0f - fabs(HitRes->Position[0].y - mTransform.pos.y) / PLAYER_SHADOW_HEIGHT);

			if (HitRes->Position[1].y > mTransform.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[1].dif.a = 128 * (1.0f - fabs(HitRes->Position[1].y - mTransform.pos.y) / PLAYER_SHADOW_HEIGHT);

			if (HitRes->Position[2].y > mTransform.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[2].dif.a = 128 * (1.0f - fabs(HitRes->Position[2].y - mTransform.pos.y) / PLAYER_SHADOW_HEIGHT);

			// �t�u�l�͒n�ʃ|���S���ƃv���C���[�̑��΍��W���犄��o��
			Vertex[0].u = (HitRes->Position[0].x - mTransform.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[0].v = (HitRes->Position[0].z - mTransform.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[1].u = (HitRes->Position[1].x - mTransform.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[1].v = (HitRes->Position[1].z - mTransform.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[2].u = (HitRes->Position[2].x - mTransform.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[2].v = (HitRes->Position[2].z - mTransform.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;

			// �e�|���S����`��
			DrawPolygon3D(Vertex, 1, mImgShadow, TRUE);
		}

		// ���o�����n�ʃ|���S�����̌�n��
		MV1CollResultPolyDimTerminate(HitResDim);
	}

	// ���C�e�B���O��L���ɂ���
	SetUseLighting(TRUE);

	// �y�o�b�t�@�𖳌��ɂ���
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

	// �L������{���
	//-------------------------------------------------------
	// �L�������W
	v = mTransform.pos;
	DrawFormatString(20, 60, black, "�L�������W �F (%0.2f, %0.2f, %0.2f)",
		v.x, v.y, v.z
	);
	//-------------------------------------------------------
	DrawLine3D(mGravHitUp, mGravHitDown, 0x000000);

	//�J�v�Z���R���C�_
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

	//x�����������J�����p�x
	Quaternion cameraRot=mSceneManager->GetCamera()->GetQuaRotOutX();

	VECTOR dir = AsoUtility::VECTOR_ZERO;

	double rotRad = 0.0;

	//�v���C���[�ړ�
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

	//�ړ������ɉ�������]
	Rotate();

	//mMovePow = VScale(dir, mSpeed);
}

void Player::ProcessJamp(void)
{

	//�W�����v�{�^�������ꂽ��W�����v
	bool isHitKey = CheckHitKey(KEY_INPUT_BACKSLASH);
	if (isHitKey&&(mIsJump||IsEndLanding()))
	{
		if (mIsJump)
		{
			//�W�����v�A�j���[�V����
	//mAnimationController->Play((int)ANIM_TYPE::JUMP);
	//mAnimationController->Play((int)ANIM_TYPE::JUMP,false);
	/*mAnimationController->Play(
		(int)ANIM_TYPE::JUMP,false,13.0f,25.0f);*/

		//�؂���Đ����I�������
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

	//���݂̃S�[����]�ƐV���ȃS�[����]�̊p�x�������߂�
	double angleDiff = Quaternion::Angle(axis, mGoalQuaRotY);

	//�������l�Ń��Z�b�g���邩���f
	if(mStepRotTime>=0.)
//�J�X���ԃ��Z�b�g
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

	//���ς��g���ăW�����v������������������
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


	//�ړ�
	
	mMoveDiff = VSub(mMovedPos, mTransform.pos);
	mTransform.pos = mMovedPos;

}

void Player::CollisionCapsule(void)
{
	//�J�v�Z�����ړ�����W�Ɉړ�������
	Transform trans = Transform(&mTransform);
	trans.pos = mMovedPos;
	trans.Update();
	Capsule cap = mCapsule->Copy(&trans);

	//�J�v�Z���Ƃ̏Փ�
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

					//�J�v�Z���ړ�
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
	
	//�W�����v�ʂ��ړ����W�ɉ��Z
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
		{// �Փ˒n�_����A������Ɉړ�
			mMovedPos =
				VAdd(hit.HitPosition,
					VScale(dirUpGravity, 2.0f));
			// �W�����v���Z�b�g
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
			//�G�t�F�N�g����
			playeHandle = PlayEffekseer3DEffect(mEffectSmoke);

			SetScalePlayingEffekseer3DEffect(playeHandle, 5.0f, 5.0f, 5.0f);

			SetPosPlayingEffekseer3DEffect(playeHandle, mTransform.pos.x, mTransform.pos.y, mTransform.pos.z);
		}
	}

	mStepFootSmoke++;

}
