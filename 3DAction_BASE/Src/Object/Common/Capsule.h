#pragma once
#include <DxLib.h>
class Transform;

class Capsule
{

public :

	static constexpr int COLOR = 0xffffff;

	Capsule(Transform* parent);

	// 描画
	void Draw(void);

	// 親Transformからの相対位置をセット
	void SetRelativePosTop(VECTOR pos);
	void SetRelativePosDown(VECTOR pos);

	VECTOR GetPosTop(void);
	VECTOR GetPosDown(void);
	VECTOR GetRelativePos(VECTOR pos);

	// 半径
	float GetRadius(void);
	void SetRadius(float radius);

	// 高さ
	float GetHeight(void);

	// カプセルの中心座標
	VECTOR GetCenter(void);

	// コピー
	Capsule Copy(Transform* base);

	void SetTransformParent(Transform* parent);

private :

	Transform* mTransformParent;

	// 親Transformからの相対位置(上側)
	VECTOR mRelativePosTop;

	// 親Transformからの相対位置(下側)
	VECTOR mRelativePosDown;

	// 半径
	float mRadius;

};

