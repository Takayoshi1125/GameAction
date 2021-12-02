#pragma once
#include "./Manager/SceneManager.h"

class Application
{
public:

	bool Init(void);
	void Run(void);
	bool Release(void);

private:
	
	// インスタンスの生成
	SceneManager mSceneManager;

};