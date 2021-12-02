#pragma once
#include <string>
#include <vector>
#include <DxLib.h>
#include "../Common/Quaternion.h"
class AsoUtility
{

public:

	static constexpr float RAD2DEG = (180.0f / DX_PI_F);

	static constexpr VECTOR VECTOR_ZERO = { 0.0f, 0.0f, 0.0f };
	static constexpr VECTOR VECTOR_ONE = { 1.0f, 1.0f, 1.0f };

	// 回転軸
	static constexpr VECTOR AXIS_X = { 1.0f, 0.0f, 0.0f };
	static constexpr VECTOR AXIS_Y = { 0.0f, 1.0f, 0.0f };
	static constexpr VECTOR AXIS_Z = { 0.0f, 0.0f, 1.0f };

	// 方向
	static constexpr VECTOR DIR_F = { 0.0f, 0.0f, 1.0f };
	static constexpr VECTOR DIR_B = { 0.0f, 0.0f, -1.0f };
	static constexpr VECTOR DIR_R = { 1.0f, 0.0f, 0.0f };
	static constexpr VECTOR DIR_L = { -1.0f, 0.0f, 0.0f };
	static constexpr VECTOR DIR_U = { 0.0f, 1.0f, 0.0f };
	static constexpr VECTOR DIR_D = { 0.0f, -1.0f, 0.0f };

	static constexpr float kEpsilonNormalSqrt = 1e-15F;

	static std::vector <std::string> Split(std::string& line, char delimiter);

	// ラジアン(rad)から度(deg)
	static double Rad2DegD(double rad);
	static float Rad2DegF(float rad);

	// 度(deg)からラジアン(rad)
	static double Deg2RadD(double deg);
	static float Deg2RadF(float deg);

	// 0～360度の範囲に収める
	static double DegIn360(double deg);

	// 0(0)～2π(360度)の範囲に収める
	static double RadIn2PI(double rad);

	// 回転が少ない方の回転向きを取得する(時計回り:1、反時計回り:-1)
	static int DirNearAroundRad(float from, float to);
	
	// 回転が少ない方の回転向きを取得する(時計回り:1、反時計回り:-1)
	static int DirNearAroundDeg(float from, float to);
	
	// 線形補間
	static double Lerp(double start, double end, double t);
	static VECTOR Lerp(VECTOR start, VECTOR end, float t);

	// 角度の線形補間
	static double LerpDeg(double start, double end, double t);

	// 色の線形補間
	static COLOR_F Lerp(COLOR_F start, COLOR_F end, float t);

	static VECTOR RotXZPos(VECTOR centerPos, VECTOR radiusPos, float rad);

	static double Magnitude(VECTOR v);
	static double Distance(VECTOR v1, VECTOR v2);

	// 球体同士の衝突判定
	static bool IsHitSpheres(VECTOR pos1, float radius1, VECTOR pos2, float radius2);

	static bool Equals(VECTOR v1, VECTOR v2);
	static bool EqualsVZero(VECTOR v1);

	static VECTOR VNormalize(VECTOR v);

	// 2点間のベクトルの2乗の長さ
	static float SqrMagnitude(VECTOR v);

	// 2つのベクトルの間の角度
	static float AngleDeg(VECTOR from, VECTOR to);

	// 描画系
	static void DrawLineDir(VECTOR pos, VECTOR dir, int color, float len = 50.0f);
	static void DrawLineXYZ(VECTOR pos, Quaternion rot, float len = 50.0f);

};

