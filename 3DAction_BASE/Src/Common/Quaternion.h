#pragma once
#include <DxLib.h>
#include <iostream>
#include <algorithm>
class Quaternion
{

public:
	
	static constexpr float kEpsilonNormalSqrt = 1e-15F;

	double w;
	double x;
	double y;
	double z;

	Quaternion(void);
	Quaternion(VECTOR rad);
	Quaternion(double w, double x, double y, double z);

	// オイラー角からクォータニオンへ変換
	static Quaternion Euler(VECTOR rad);
	static Quaternion Euler(double radX, double radY, double radZ);

	// クォータニオンの合成
	static Quaternion Mult(Quaternion q1, Quaternion q2);
	Quaternion Mult(Quaternion q);

	// 指定軸を指定角分、回転させる
	static Quaternion AngleAxis(double rad, VECTOR axis);

	// 座標を回転させる
	static VECTOR PosAxis(Quaternion q, VECTOR pos);
	VECTOR PosAxis(VECTOR pos);

	// クォータニオンからオイラー角へ変換
	static VECTOR ToEuler(Quaternion q);
	VECTOR ToEuler(void);

	// クォータニオンから行列へ変換
	static MATRIX ToMatrix(Quaternion q);
	MATRIX ToMatrix(void);

	// ベクトルからクォータニオンに変換
	static Quaternion LookRotation(VECTOR dir);
	static Quaternion LookRotation(VECTOR dir, VECTOR up);

	// 行列からクォータニオンに変換
	static Quaternion GetRotation(MATRIX mat);

	// 基本ベクトルを取得
	VECTOR GetForward(void);
	VECTOR GetBack(void);
	VECTOR GetRight(void);
	VECTOR GetLeft(void);
	VECTOR GetUp(void);
	VECTOR GetDown(void);

	static double Dot(Quaternion q1, Quaternion q2);
	double Dot(Quaternion b);

	// 正規化
	Quaternion Normalized(void);
	void Normalize(void);

	// 逆クォータニオン
	Quaternion Inverse(void);

	// 球面補間
	static Quaternion Slerp(Quaternion from, Quaternion to, double t);


	// ２つのベクトル間の回転量を取得する
	static Quaternion FromToRotation(VECTOR fromDir, VECTOR toDir);
	static Quaternion RotateTowards(Quaternion from, Quaternion to, float maxDegreesDelta);
	static double Angle(Quaternion q1, Quaternion q2);
	static Quaternion SlerpUnclamped(Quaternion& a, Quaternion& b, float t);
	static Quaternion Identity(void);

	// 対象方向の回転
	double Length(void);
	double LengthSquared(void);
	VECTOR xyz(void);
	static Quaternion Normalize(Quaternion q);
	static void Normalize(Quaternion q, Quaternion* result);

	void ToAngleAxis(float* angle, VECTOR* axis);


	//Quaternion operator*(float& rhs);
	//const Quaternion operator*(const float& rhs);
	//Quaternion operator+(Quaternion& rhs);
	//const Quaternion operator+(const Quaternion& rhs);

	//Quaternion operator*(Quaternion rhs);
	//VECTOR operator*(VECTOR rhs);

private:

	// 基本ベクトルを取得
	VECTOR GetDir(VECTOR dir);

	Quaternion operator*(float& rhs);
	const Quaternion operator*(const float& rhs);
	Quaternion operator+(Quaternion& rhs);
	const Quaternion operator+(const Quaternion& rhs);
	
};
