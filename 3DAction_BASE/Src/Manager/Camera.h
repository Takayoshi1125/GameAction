#pragma once
#include <vector>
#include <DxLib.h>
#include "../Common/Quaternion.h"
class SceneManager;
class GravityManager;
class Transform;
class Collider;

class Camera
{

public:

	// FIXED_POINT : カメラの初期座標
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };

	// FOLLOW : カメラ位置(追従対象との相対座標)
	static constexpr VECTOR RELATIVE_CAMERA_POS_FOLLOW = { 0.0f, 50.0f, -400.0f };

	// FOLLOW : 注視点(追従対象との相対座標)
	static constexpr VECTOR RELATIVE_TARGET_POS = { 0.0f, 0.0f, 500.0f };

	// カメラのX回転上限度角
	static constexpr float LIMIT_X_UP_RAD = 40.0f * (DX_PI_F / 180.0f);
	static constexpr float LIMIT_X_DW_RAD = 15.0f * (DX_PI_F / 180.0f);
	
	// 追従対象との調整座標
	static constexpr VECTOR RELATIVE_TRANSFORM_POS = { 0.0f, 20.0f, 0.0f };

	// カメラモード
	enum class MODE
	{
		NONE,
		FIXED_POINT,
		FOLLOW
	};

	Camera(SceneManager* manager);
	~Camera();

	void Init(void);
	void Update(void);
	void SetBeforeDraw(void);
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFollow(void);
	void Draw(void);
	void Release(void);

	void ProcessRot(void);

	void SetTransform(Transform* transform);

	VECTOR GetPos(void);
	VECTOR GetAngles(void);
	VECTOR GetTargetPos(void);
	Quaternion GetQuaRot(void);
	Quaternion GetQuaRotOutX(void);

	VECTOR GetDir(void);

	void ChangeMode(MODE mode);

private:

	SceneManager* mSceneManager;
	GravityManager* mGravityManager;

	// カメラが追従対象とするTransform
	Transform* mSyncTransform;

	// カメラモード
	MODE mMode;

	// カメラの位置
	VECTOR mPos;

	// カメラ角度(rad)
	VECTOR mAngles;

	// X軸回転が無い角度
	Quaternion mQuaRotOutX;

	// カメラ角度
	Quaternion mQuaRot;

	// 注視点
	VECTOR mTargetPos;

	// カメラの上方向
	VECTOR mCameraUp;

	void SetDefault(void);

	// Transformの位置と同期を取る
	void SyncTransform(void);

};

