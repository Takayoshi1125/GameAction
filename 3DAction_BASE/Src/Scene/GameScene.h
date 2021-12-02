#pragma once
#include "SceneBase.h"
class SceneManager;
class Stage;
class SkyDome;
class Player;

class GameScene : public SceneBase
{

public:

	GameScene(SceneManager* manager);
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void DrawDebug(void);
	void Release(void) override;

private:

	Stage* mStage;
	SkyDome* mSkyDome;
	Player* mPlayer;

};
