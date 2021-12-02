//#pragma once
#include <chrono>
#include <DxLib.h>
class SceneBase;
class ResourceManager;
class Fader;
class Camera;
class GravityManager;

// シーン管理用
enum class SCENE_ID
{
	NONE,
	TITLE,
	GAME
};

class SceneManager
{

public:

	void Init(void);
	void Init3D(void);
	void Update(void);
	void Release(void);

	void ChangeScene(SCENE_ID nextId, bool isFading);

	// デルタタイムの取得
	float GetDeltaTime(void);

	// カメラの取得
	Camera* GetCamera(void);

	// カメラの取得
	ResourceManager* GetResourceManager(void);

	// 重力方向の取得
	GravityManager* GetGravityManager(void);

private:

	SCENE_ID mSceneID;
	SCENE_ID mWaitSceneID;

	SceneBase* mScene;
	ResourceManager* mResourceManager;
	Fader* mFader;
	Camera* mCamera;

	bool mIsSceneChanging;

	// デルタタイム
	std::chrono::system_clock::time_point mTickCount;
	float mDeltaTime;

	// 重力方向の制御
	GravityManager* mGravityManager;

	void ResetDeltaTime(void);

	void DoChangeScene(void);

};