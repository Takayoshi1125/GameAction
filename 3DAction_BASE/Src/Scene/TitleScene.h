#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"
class SceneManager;
class SkyDome;
class AnimationController;

class TitleScene : public SceneBase
{

public:

	TitleScene(SceneManager* manager);
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	int mImgTitle;
	int mImgPush;

	SkyDome* mSkyDome;
	Transform mPlanet;
	Transform mMovePlanet;
	Transform mChara;
	AnimationController* mAnimationController;

};
