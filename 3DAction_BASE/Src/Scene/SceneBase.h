#pragma once
class SceneManager;

class SceneBase
{
public:

	SceneBase(SceneManager* _manager);
	~SceneBase();

	// 初期化処理
	virtual void Init(void);

	// 更新ステップ
	virtual void Update(void);

	// 描画処理
	virtual void Draw(void);

	// 解放処理
	virtual void Release(void);

	SceneManager* GetSceneManager(void);

protected:
	SceneManager* mSceneManager;
};

