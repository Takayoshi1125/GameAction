#pragma once
#include <string>
#include <DxLib.h>
#include "Transform.h"
class SceneManager;

class SpeechBalloon
{

public:

	enum class TYPE
	{
		TEXT,
		SPEECH
	};

	SpeechBalloon(SceneManager* manager, TYPE type, Transform* parent);

	void Draw(void);
	void DrawTextOrg(void);
	void DrawSpeech(void);
	void Release(void);

	void SetText(std::string text);
	void SetTime(double time);

	void SetType(TYPE type);
	void SetVisiblePermanent(void);

	void SetRelativePos(VECTOR relativePos);

private:

	SceneManager* mSceneManager;

	TYPE mType;

	int mImage;

	// trueの場合、永続的に表示する
	bool mIsVisiblePermanent;
	double mVisibleTime;

	Transform* mTransformParent;
	VECTOR mPos;

	// 表示位置の調整用
	VECTOR mRelativePos;

	std::string mText;

};
