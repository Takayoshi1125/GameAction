#include <chrono>
#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Common/Fader.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/GravityManager.h"
#include "../Scene/TitleScene.h"
#include "../Scene/GameScene.h"
#include "Camera.h"
#include "SceneManager.h"

void SceneManager::Init()
{

	// 3D用の設定
	Init3D();

	mSceneID = SCENE_ID::TITLE;
	mWaitSceneID = SCENE_ID::NONE;

	mResourceManager = new ResourceManager();
	mResourceManager->Init();

	mFader = new Fader();
	mFader->Init();

	// 重力管理(Cameraより先に)
	mGravityManager = new GravityManager(this);
	mGravityManager->Init();

	mCamera = new Camera(this);
	mCamera->Init();

	mScene = new TitleScene(this);
	mScene->Init();

	mIsSceneChanging = false;

	// デルタタイム
	mTickCount = std::chrono::system_clock::now();


}

void SceneManager::Init3D(void)
{

	// Zバッファを有効にする
	SetUseZBuffer3D(true);

	// Zバッファへの書き込みを有効にする
	SetWriteZBuffer3D(true);

	// バックカリングを有効にする
	SetUseBackCulling(true);

	// ライトの設定
	ChangeLightTypeDir({ 0.3f, -0.7f, 0.8f });

	// 背景色設定
	SetBackgroundColor(0, 139, 139);

	// フォグ設定
	SetFogEnable(true);
	SetFogColor(5, 5, 5);
	SetFogStartEnd(10000.0f, 20000.0f);

}

void SceneManager::Update(void)
{

	if (mScene == nullptr)
	{
		return;
	}

	// デルタタイム
	auto tickCount = std::chrono::system_clock::now();
	mDeltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(tickCount - mTickCount).count() / 1000000000.0f;
	mTickCount = tickCount;

	// 重力方向の計算
	mGravityManager->Update();

	mFader->Update();
	if (mIsSceneChanging)
	{
		Fader::FADE_STATE fState = mFader->GetState();
		switch (fState)
		{
		case Fader::FADE_STATE::FADE_IN:
			if (mFader->IsEnd())
			{
				mFader->SetFade(Fader::FADE_STATE::NONE);
				mIsSceneChanging = false;
			}
			break;
		case Fader::FADE_STATE::FADE_OUT:
			if (mFader->IsEnd())
			{
				DoChangeScene();
				mFader->SetFade(Fader::FADE_STATE::FADE_IN);
			}
			break;
		default:
			break;
		}
	}
	else 
	{
		mScene->Update();
	}

	// カメラ更新ステップ
	mCamera->Update();

	// 描画先グラフィック領域の指定
	// (３Ｄ描画で使用するカメラの設定などがリセットされる)
	SetDrawScreen(DX_SCREEN_BACK);

	// 画面を初期化
	ClearDrawScreen();

	// カメラ設定
	mCamera->SetBeforeDraw();

	// 描画
	mScene->Draw();
	mCamera->Draw();

	// 最後
	mFader->Draw();
	
}

void SceneManager::Release(void)
{

	mScene->Release();
	delete mScene;

	mCamera->Release();
	delete mCamera;

	delete mGravityManager;

	mResourceManager->Release();
	delete mResourceManager;

}

void SceneManager::ChangeScene(SCENE_ID nextId, bool isFading)
{

	mWaitSceneID = nextId;

	if (isFading)
	{
		mFader->SetFade(Fader::FADE_STATE::FADE_OUT);
		mIsSceneChanging = true;
	}
	else 
	{
		DoChangeScene();
	}

}

float SceneManager::GetDeltaTime(void)
{
	//return 1.0f / 60.0f;
	return mDeltaTime;
}

Camera* SceneManager::GetCamera(void)
{
	return mCamera;
}

ResourceManager* SceneManager::GetResourceManager(void)
{
	return mResourceManager;
}

GravityManager* SceneManager::GetGravityManager(void)
{
	return mGravityManager;
}

void SceneManager::ResetDeltaTime(void)
{
	mDeltaTime = 0.016f;
	mTickCount = std::chrono::system_clock::now();
}

void SceneManager::DoChangeScene(void)
{

	mResourceManager->Release();

	mScene->Release();
	delete mScene;

	mSceneID = mWaitSceneID;

	switch (mSceneID)
	{
	case SCENE_ID::TITLE:
		mScene = new TitleScene(this);
		break;
	case SCENE_ID::GAME:
		mScene = new GameScene(this);
		break;
	}

	mScene->Init();

	ResetDeltaTime();

	mWaitSceneID = SCENE_ID::NONE;

}
