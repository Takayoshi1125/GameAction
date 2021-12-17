#include <math.h>
#include <DxLib.h>
#include<EffekseerForDXLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/GravityManager.h"
#include "../Object/Common/Transform.h"
#include "../Object/Planet.h"
#include "Camera.h"

Camera::Camera(SceneManager* manager)
{
	mSceneManager = manager;
	mGravityManager = manager->GetGravityManager();

	mAngles = VECTOR();
	mCameraUp = VECTOR();
	mMode = MODE::NONE;
	mPos = AsoUtility::VECTOR_ZERO;
	mTargetPos = AsoUtility::VECTOR_ZERO;
	mSyncTransform = nullptr;
}

Camera::~Camera()
{
}

void Camera::Init()
{

	ChangeMode(MODE::FIXED_POINT);

}

void Camera::Update()
{
}

void Camera::SetBeforeDraw(void)
{

	// クリップ距離を設定する(SetDrawScreenでリセットされる)
	SetCameraNearFar(10.0f, 30000.0f);

	switch (mMode)
	{
	case Camera::MODE::FIXED_POINT:
		SetBeforeDrawFixedPoint();
		break;
	case Camera::MODE::FOLLOW:
		SetBeforeDrawFollow();
		break;
	}

	// カメラの設定
	SetCameraPositionAndTargetAndUpVec(
		mPos,
		mTargetPos,
		mCameraUp
	);

	Effekseer_Sync3DSetting();

}

void Camera::SetBeforeDrawFixedPoint(void)
{
}

void Camera::SetBeforeDrawFollow(void)
{

	// カメラ操作
	ProcessRot();

	// 追従対象との相対位置を同期
	SyncTransform();

}

void Camera::Draw()
{
}

void Camera::Release(void)
{
}

void Camera::ProcessRot(void)
{
	//VECTOR axisDeg = AsoUtility::VECTOR_ZERO;

	//カメラの左右回転
	if (CheckHitKey(KEY_INPUT_LEFT)) { mAngles.y += AsoUtility::Deg2RadF(-1.0f); }
	if (CheckHitKey(KEY_INPUT_RIGHT)) { mAngles.y += AsoUtility::Deg2RadF(1.0f); }

	//カメラの上下回転
	if (CheckHitKey(KEY_INPUT_UP)) { mAngles.x += AsoUtility::Deg2RadF(-1.0f);}
	if (mAngles.x > LIMIT_X_UP_RAD)
	{
		mAngles.x = LIMIT_X_UP_RAD;
	}
	if (CheckHitKey(KEY_INPUT_DOWN)) { mAngles.x += AsoUtility::Deg2RadF(1.0f);}
	if (mAngles.x < LIMIT_X_DW_RAD)
	{
		mAngles.x = LIMIT_X_DW_RAD;
	}


	//if (!AsoUtility::EqualsVZero(axisDeg)) {
	//	VECTOR axisRad;
	//	axisRad.x = AsoUtility::Deg2RadF(axisDeg.x);
	//	axisRad.y = AsoUtility::Deg2RadF(axisDeg.y);
	//	axisRad.z = AsoUtility::Deg2RadF(axisDeg.z);

	//	// 回転量を合成
	//	Quaternion axisQ = Quaternion::Euler(axisRad);
	//	mQuaRot = mQuaRot.Mult(axisQ);

	//	// 位置情報に回転情報加える
	//	// pos = q ・ pos ・ q(-1)
	//	Quaternion tmp;
	//	tmp = tmp.Mult(mQuaRot);
	//	tmp = tmp.Mult(Quaternion(0.0f, RELATIVE_TARGET_POS.x,
	//		RELATIVE_TARGET_POS.y, RELATIVE_TARGET_POS.z));
	//	tmp = tmp.Mult(mQuaRot.Inverse());

	//	// 注視点を更新
	//	VECTOR localPos = { (float)tmp.x,(float)tmp.y,(float)tmp.z };
	//	mTargetPos = VAdd(mPos, localPos);
	//}

}

void Camera::SetTransform(Transform* transform)
{
	mSyncTransform = transform;
}

VECTOR Camera::GetPos(void)
{
	return mPos;
}

VECTOR Camera::GetAngles(void)
{
	return mAngles;
}

VECTOR Camera::GetTargetPos(void)
{
	return mTargetPos;
}

Quaternion Camera::GetQuaRot(void)
{
	return mQuaRot;
}

Quaternion Camera::GetQuaRotOutX(void)
{
	return mQuaRotOutX;
}

VECTOR Camera::GetDir(void)
{
	return VNorm(VSub(mTargetPos, mPos));
}

void Camera::ChangeMode(MODE mode)
{

	SetDefault();

	mMode = mode;
	switch (mMode)
	{
	case Camera::MODE::FIXED_POINT:
		break;
	case Camera::MODE::FOLLOW:
		break;
	}

}

void Camera::SetDefault(void)
{

	// カメラの初期設定
	mPos = DEFAULT_CAMERA_POS;
	mTargetPos = AsoUtility::VECTOR_ZERO;

	mAngles.x = AsoUtility::Deg2RadF(30.0f);
	mAngles.y = 0.0f;
	mAngles.z = 0.0f;

	mQuaRot = Quaternion();

	mCameraUp = AsoUtility::DIR_U;

}

void Camera::SyncTransform(void)
{

	// 同期先の位置
	VECTOR pos = mSyncTransform->pos;

	// 重力の方向制御に従う
	Quaternion gRot = mGravityManager->GetTransform()->quaRot;

	// 正面から設定されたY軸分、回転させる
	mQuaRotOutX = gRot.Mult(Quaternion::AngleAxis(mAngles.y, AsoUtility::AXIS_Y));

	// 正面から設定されたX軸分、回転させる
	mQuaRot = mQuaRotOutX.Mult(Quaternion::AngleAxis(mAngles.x, AsoUtility::AXIS_X));

	VECTOR localPos;

	// 注視点(通常重力でいうところのY値を追従対象と同じにする)
	localPos = mQuaRotOutX.PosAxis(RELATIVE_TARGET_POS);
	mTargetPos = VAdd(pos, localPos);

	// カメラ位置
	localPos = mQuaRot.PosAxis(RELATIVE_CAMERA_POS_FOLLOW);
	mPos = VAdd(pos, localPos);

	// カメラの上方向
	mCameraUp = gRot.GetUp();

}
