#pragma once
#include <DxLib.h>
#include "../../Common/Quaternion.h"
#include "Collider.h"

/// <summary>
/// モデル制御の基本情報
/// 大きさ：VECTOR基準
/// 回転　：Quaternion基準
/// 位置　：VECTOR基準
/// </summary>
class Transform
{

public:

	Transform(void);
	Transform(int model);
	Transform(Transform* base);

	// モデルのハンドルID
	int modelId;

	// 大きさ
	VECTOR scl;
	// 回転
	VECTOR rot;
	// 位置
	VECTOR pos;

	MATRIX matScl;
	MATRIX matRot;
	MATRIX matPos;

	// 回転
	Quaternion quaRot;

	// ローカル回転
	Quaternion quaRotLocal;

	// コライダ
	Collider* collider;

	/// <summary>
	/// モデル制御の基本情報更新
	/// </summary>
	/// <param name=""></param>
	void Update(void);

	void Release(void);

	void SetModel(int modelHId);
	void MakeCollider(Collider::TYPE type);

	// 前方方向を取得
	VECTOR GetForward(void);

	// 後方方向を取得
	VECTOR GetBack(void);

	// 右方向を取得
	VECTOR GetRight(void);

	// 左方向を取得
	VECTOR GetLeft(void);

	// 上方向を取得
	VECTOR GetUp(void);

	// 下方向を取得
	VECTOR GetDown(void);

	// 対象方向を取得
	VECTOR GetDir(VECTOR vec);

	// 値コピー
	void Copy(Transform* base);

};

