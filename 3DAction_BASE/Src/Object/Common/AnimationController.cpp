#include <DxLib.h>
#include "../../Manager/SceneManager.h"
#include "AnimationController.h"

AnimationController::AnimationController(SceneManager* manager, int modelId)
{
	mSceneManager = manager;
	mModelId = modelId;

	mPlayType = -1;
	mIsLoop = false;

	mIsStop = false;
	mSwitchLoopReverse = 0.0f;
	mEndLoopSpeed = 0.0f;
	mStepEndLoopStart = 0.0f;
	mStepEndLoopEnd = 0.0f;
}

void AnimationController::Add(int type, std::string path, float speed)
{

	Animation anim;

	anim.model = MV1LoadModel(path.c_str());
	anim.animIndex = type;
	anim.speed = speed;

	if (mAnimations.count(type) == 0)
	{
		// 入れ替え
		mAnimations.emplace(type, anim);
	}
	else
	{
		// 追加
		mAnimations[type].model = anim.model;
		mAnimations[type].animIndex = anim.animIndex;
		mAnimations[type].attachNo = anim.attachNo;
		mAnimations[type].totalTime = anim.totalTime;
	}

}

void AnimationController::Play(int type, bool isLoop, 
	float startStep, float endStep, bool isStop, bool isForce)
{

	if (mPlayType != type || isForce) {

		if (mPlayType != -1)
		{
			// モデルからアニメーションを外す
			mPlayAnim.attachNo = MV1DetachAnim(mModelId, mPlayAnim.attachNo);
		}

		// アニメーション種別を変更
		mPlayType = type;
		mPlayAnim = mAnimations[type];

		// 初期化
		mPlayAnim.step = startStep;

		// モデルにアニメーションを付ける
		mPlayAnim.attachNo = MV1AttachAnim(mModelId, 1, mPlayAnim.model);

		// アニメーション総時間の取得
		if (endStep > 0.0f)
		{
			mPlayAnim.totalTime = endStep;
		}
		else
		{
			mPlayAnim.totalTime = MV1GetAttachAnimTotalTime(mModelId, mPlayAnim.attachNo);
		}

		// アニメーションループ
		mIsLoop = isLoop;

		// アニメーションしない
		mIsStop = isStop;

		mStepEndLoopStart = -1.0f;
		mStepEndLoopEnd = -1.0f;
		mSwitchLoopReverse = 1.0f;
	}

}

void AnimationController::Update(void)
{

	// 経過時間の取得
	float deltaTime = mSceneManager->GetDeltaTime();

	if (!mIsStop)
	{
		// 再生
		mPlayAnim.step += (deltaTime * mPlayAnim.speed * mSwitchLoopReverse);

		// アニメーション終了判定
		bool isEnd = false;
		if (mSwitchLoopReverse > 0.0f)
		{
			// 通常再生の場合
			if (mPlayAnim.step > mPlayAnim.totalTime)
			{
				isEnd = true;
			}
		}
		else
		{
			// 逆再生の場合
			if (mPlayAnim.step < mPlayAnim.totalTime)
			{
				isEnd = true;
			}
		}

		if (isEnd)
		{
			// アニメーションが終了したら
			if (mIsLoop)
			{
				// ループ再生
				if (mStepEndLoopStart > 0.0f)
				{
					// アニメーション終了後の指定フレーム再生
					mSwitchLoopReverse *= -1.0f;
					if (mSwitchLoopReverse > 0.0f)
					{
						mPlayAnim.step = mStepEndLoopStart;
						mPlayAnim.totalTime = mStepEndLoopEnd;
					}
					else
					{
						mPlayAnim.step = mStepEndLoopEnd;
						mPlayAnim.totalTime = mStepEndLoopStart;
					}
					mPlayAnim.speed = mEndLoopSpeed;
					
				}
				else
				{
					// 通常のループ再生
					mPlayAnim.step = 0.0f;
				}
			}
			else
			{
				// ループしない
				mPlayAnim.step = mPlayAnim.totalTime;
			}

		}

	}

	// アニメーション設定
	MV1SetAttachAnimTime(mModelId, mPlayAnim.attachNo, mPlayAnim.step);

}

void AnimationController::Release(void)
{
	for (auto& anim : mAnimations)
	{
		MV1DeleteModel(anim.second.model);
	}
}

void AnimationController::SetEndLoop(float startStep, float endStep, float speed)
{
	mStepEndLoopStart = startStep;
	mStepEndLoopEnd = endStep;
	mEndLoopSpeed = speed;
}

int AnimationController::GetPlayType(void)
{
	return mPlayType;
}

bool AnimationController::IsEnd(void)
{

	bool ret = false;

	if (mIsLoop)
	{
		return ret;
	}

	if (mPlayAnim.step >= mPlayAnim.totalTime)
	{
		return true;
	}

	return ret;

}
