#include <DxLib.h>
#include "GameCommon.h"
#include "Fader.h"

Fader::FADE_STATE Fader::GetState(void)
{
	return mState;
}

bool Fader::IsEnd(void)
{
	return mIsEnd;
}

void Fader::SetFade(FADE_STATE state)
{
	mState = state;
	if (mState != FADE_STATE::NONE)
	{
		mIsPreEnd = false;
		mIsEnd = false;
	}
}

void Fader::Init(void)
{
	mState = FADE_STATE::NONE;
	mAlpha = 0;
	mIsPreEnd = true;
	mIsEnd = true;
}

void Fader::Update(void)
{

	if (mIsEnd)
	{
		return;
	}

	switch (mState)
	{
	case FADE_STATE::NONE:
		return;

	case FADE_STATE::FADE_OUT:
		mAlpha += SPEED_ALPHA;
		if (mAlpha > 255)
		{
			// フェード終了
			mAlpha = 255;
			if (mIsPreEnd)
			{
				// 1フレーム後(Draw後)に終了とする
				mIsEnd = true;
			}
			mIsPreEnd = true;
		}

		break;

	case FADE_STATE::FADE_IN:
		mAlpha -= SPEED_ALPHA;
		if (mAlpha < 0)
		{
			// フェード終了
			mAlpha = 0;
			if (mIsPreEnd)
			{
				// 1フレーム後(Draw後)に終了とする
				mIsEnd = true;
			}
			mIsPreEnd = true;
		}
		break;

	default:
		return;
	}

}

void Fader::Draw(void)
{

	switch (mState)
	{
	case FADE_STATE::NONE:
		return;
	case FADE_STATE::FADE_OUT:
	case FADE_STATE::FADE_IN:
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)mAlpha);
		DrawBox(0, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		break;
	}

}
