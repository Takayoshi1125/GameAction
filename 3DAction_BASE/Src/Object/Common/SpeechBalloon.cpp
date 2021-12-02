#include <DxLib.h>
#include "Transform.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Resource.h"
#include "SpeechBalloon.h"

SpeechBalloon::SpeechBalloon(SceneManager* manager, TYPE type, Transform* parent)
{

	mSceneManager = manager;

	mTransformParent = parent;

	mType = type;

	Resource res = mSceneManager->GetResourceManager()->Load(ResourceManager::SRC::SPEECH_BALLOON);
	mImage = res.mHandleId;

	mIsVisiblePermanent = true;
	mVisibleTime = -1.0f;

	mPos = { 0.0f, 0.0f, 0.0f };
	mRelativePos = { 0.0f, 0.0f, 0.0f };

}

void SpeechBalloon::Draw(void)
{

	if (!mIsVisiblePermanent)
	{
		mVisibleTime -= mSceneManager->GetDeltaTime();
		if (mVisibleTime < 0.0f)
		{
			return;
		}
	}

	// 対象物の姿勢から見た、上方向に3D座標をずらす
	VECTOR relativePos = mTransformParent->quaRot.PosAxis(mRelativePos);
	VECTOR pos = VAdd(mTransformParent->pos, relativePos);
	mPos = ConvWorldPosToScreenPos(pos);

	switch (mType)
	{
	case SpeechBalloon::TYPE::TEXT:
		DrawTextOrg();
		break;
	case SpeechBalloon::TYPE::SPEECH:
		DrawSpeech();
		break;
	}

}

void SpeechBalloon::DrawTextOrg(void)
{

	// カメラのSetCameraNearFarから外れていた場合、表示しない
	if (mPos.z > 0.0f && mPos.z < 1.0f)
	{
		DrawFormatString((int)mPos.x, (int)mPos.y, 0xdd0000, "%s", mText.c_str());
	}

}

void SpeechBalloon::DrawSpeech(void)
{

	// カメラのSetCameraNearFarから外れていた場合、表示しない
	if (mPos.z > 0.0f && mPos.z < 1.0f)
	{
		DrawRotaGraph((int)mPos.x, (int)mPos.y, 0.5f, 0.0f, mImage, true);
		int len = (int)strlen(mText.c_str());
		int width = GetDrawStringWidth(mText.c_str(), len);
		DrawFormatString((int)mPos.x - (width / 2), (int)mPos.y - 20, 0x000000, "%s", mText.c_str());
	}

}

void SpeechBalloon::Release(void)
{
	//DeleteGraph(mImage);
}

void SpeechBalloon::SetText(std::string text)
{
	mText = text;
}

void SpeechBalloon::SetTime(double time)
{
	mIsVisiblePermanent = false;
	mVisibleTime = time;
}

void SpeechBalloon::SetType(TYPE type)
{
	mType = type;
}

void SpeechBalloon::SetVisiblePermanent(void)
{
	mIsVisiblePermanent = true;
}

void SpeechBalloon::SetRelativePos(VECTOR relativePos)
{
	mRelativePos = relativePos;
}

