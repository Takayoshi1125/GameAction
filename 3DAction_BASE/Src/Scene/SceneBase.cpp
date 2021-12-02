#include "../Manager/SceneManager.h"
#include "SceneBase.h"

SceneBase::SceneBase(SceneManager* _manager)
{
	mSceneManager = _manager;
}

SceneBase::~SceneBase()
{
}

void SceneBase::Init(void)
{
}

void SceneBase::Update(void)
{
}

void SceneBase::Draw(void)
{
}

void SceneBase::Release(void)
{
}

SceneManager* SceneBase::GetSceneManager(void)
{
	return mSceneManager;
}
