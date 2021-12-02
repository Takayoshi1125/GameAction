#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <math.h>
#include <DxLib.h>
#include "AsoUtility.h"

std::vector<std::string> AsoUtility::Split(std::string& line, char delimiter)
{

    std::istringstream stream(line);
    std::string field;
    std::vector<std::string> result;
    
    while (getline(stream, field, delimiter)) {
        result.push_back(field);
    }

    return result;

}

double AsoUtility::Rad2DegD(double rad)
{
    return rad * (180.0 / DX_PI);
}

float AsoUtility::Rad2DegF(float rad)
{
    return rad * (180.0f / DX_PI_F);
}

double AsoUtility::Deg2RadD(double deg)
{
    return deg * (DX_PI / 180.0);
}

float AsoUtility::Deg2RadF(float deg)
{
    return deg * (DX_PI_F / 180.0f);
}

double AsoUtility::DegIn360(double deg)
{
    deg = fmod(deg, 360.0);
    if (deg < 0.0f)
    {
        deg += 360.0;
    }
    return deg;
}

double AsoUtility::RadIn2PI(double rad)
{
    rad = fmod(rad, DX_TWO_PI);
    if (rad < 0.0)
    {
        rad += DX_TWO_PI;
    }
    return rad;
}

int AsoUtility::DirNearAroundRad(float from, float to)
{

    float ret = 1.0f;

    float diff = to - from;

    if (diff >= 0.0f)
    {

        // 比較元よりも時計回りに位置する

        if (diff > DX_PI_F)
        {
            // でも、180度以上離れているので、反時計回りの方が近い
            ret = -1.0f;
        }
        else
        {
            // 時計回り
            ret = 1.0f;
        }

    }
    else
    {

        // 比較元よりも反時計回りに位置する

        if (diff < -DX_PI_F)
        {
            // でも、180度以上離れているので、時計回りの方が近い
            ret = 1.0f;
        }
        else
        {
            // 反時計回り
            ret = -1.0f;
        }

    }

    return (int)ret;

}

int AsoUtility::DirNearAroundDeg(float from, float to)
{

    float ret = 1.0f;

    float diff = to - from;

    if (diff >= 0.0f)
    {

        // 比較元よりも時計回りに位置する

        if (diff > 180.0f)
        {
            // でも、180度以上離れているので、反時計回りの方が近い
            ret = -1.0f;
        }
        else
        {
            // 時計回り
            ret = 1.0f;
        }

    }
    else
    {

        // 比較元よりも反時計回りに位置する

        if (diff < -180.0f)
        {
            // でも、180度以上離れているので、時計回りの方が近い
            ret = 1.0f;
        }
        else
        {
            // 反時計回り
            ret = -1.0f;
        }

    }

    return (int)ret;

}

double AsoUtility::Lerp(double start, double end, double t)
{
    // 線形補間
    if (t >= 1.0f)
    {
        return end;
    }

    double ret = start;
    ret += t * (end - start);
    return ret;
}

VECTOR AsoUtility::Lerp(VECTOR start, VECTOR end, float t)
{

    // 線形補間
    if (t >= 1.0f)
    {
        return end;
    }


    VECTOR ret = start;
    ret.x += t * (end.x - start.x);
    ret.y += t * (end.y - start.y);
    ret.z += t * (end.z - start.z);

    return ret;
}

double AsoUtility::LerpDeg(double start, double end, double t)
{

    double ret;

    double diff = end - start;
    if (diff < -180.0)
    {
        end += 360.0f;
        ret = Lerp(start, end, t);
        if (ret >= 360.0f)
        {
            ret -= 360.0f;
        }
    }
    else if (diff > 180.0)
    {
        end -= 360.0f;
        ret = Lerp(start, end, t);
        if (ret < 0.0f)
        {
            ret += 360.0f;
        }
    }
    else
    {
        ret = Lerp(start, end, t);
    }

    return ret;

}

COLOR_F AsoUtility::Lerp(COLOR_F start, COLOR_F end, float t)
{

    // 線形補間
    if (t >= 1.0f)
    {
        return end;
    }

    COLOR_F ret = start;
    ret.r += t * (end.r - start.r);
    ret.g += t * (end.g - start.g);
    ret.b += t * (end.b - start.b);
    ret.a += t * (end.a - start.a);
    return ret;

}

VECTOR AsoUtility::RotXZPos(VECTOR centerPos, VECTOR radiusPos, float rad)
{
    // 実装しなくてよい
    float x = (float)((radiusPos.x - centerPos.x) * cosf(rad)) - ((radiusPos.z - centerPos.z) * sinf(rad));
    float z = (float)((radiusPos.x - centerPos.x) * sinf(rad)) + ((radiusPos.z - centerPos.z) * cosf(rad));
    return { centerPos.x + x, radiusPos.y, centerPos.z + z };
}

double AsoUtility::Magnitude(VECTOR v)
{
    return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

double AsoUtility::Distance(VECTOR v1, VECTOR v2)
{
    return sqrt(pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2) + pow(v2.z - v1.z, 2));
}

bool AsoUtility::IsHitSpheres(VECTOR pos1, float radius1, VECTOR pos2, float radius2)
{

    bool ret = false;


    // 球体同士の衝突判定

    // お互いの半径の合計
    float radius = radius1 + radius2;

    // 座標の差からお互いの距離を取る
    VECTOR diff = VSub(pos2, pos1);

    // 三平方の定理で比較
    float dis = (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z);
    if (dis < (radius * radius))
    {
        ret = true;
    }

    return ret;

}

bool AsoUtility::Equals(VECTOR v1, VECTOR v2)
{
    if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z)
    {
        return true;
    }
    return false;
}

bool AsoUtility::EqualsVZero(VECTOR v1)
{
    VECTOR v2 = VECTOR_ZERO;
    if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z)
    {
        return true;
    }
    return false;
}

VECTOR AsoUtility::VNormalize(VECTOR v)
{
	if (AsoUtility::EqualsVZero(v))
	{
        // Quaternion計算でゼロを渡して、
        // エラー(-1, -1, -1)が返ってくると困る
		return v;
	}
	return VNorm(v);
}

float AsoUtility::SqrMagnitude(VECTOR v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

float AsoUtility::AngleDeg(VECTOR from, VECTOR to)
{

    // sqrt(a) * sqrt(b) = sqrt(a * b) -- valid for real numbers
    float fLen = AsoUtility::SqrMagnitude(from);
    float tLen = AsoUtility::SqrMagnitude(to);
    float denominator = sqrt(fLen * tLen);
    if (denominator < kEpsilonNormalSqrt)
    {
        return 0.0f;
    }

    //float dot = std::clamp(Dot(from, to) / denominator, -1.0f, 1.0f);
    //auto dot = Dot(from, to) / denominator;
    auto dot = VDot(from, to) / denominator;
    if (dot < -1.0f)
    {
        dot = -1.0f;
    }
    if (dot > 1.0f)
    {
        dot = 1.0f;
    }

    return acosf(dot) * (180.0f / DX_PI_F);

}

void AsoUtility::DrawLineDir(VECTOR pos, VECTOR dir, int color, float len)
{

    auto nDir = AsoUtility::VNormalize(dir);
    auto sPos = VAdd(pos, VScale(nDir, -len));
    auto ePos = VAdd(pos, VScale(nDir, len));
    DrawLine3D(sPos, ePos, color);
    DrawSphere3D(ePos, 5.0f, 5, color, color, true);

}

void AsoUtility::DrawLineXYZ(VECTOR pos, Quaternion rot, float len)
{

    VECTOR dir;

    // X
    dir = rot.GetRight();
    DrawLineDir(pos, dir, 0xff0000, len);

    // Y
    dir = rot.GetUp();
    DrawLineDir(pos, dir, 0x00ff00, len);

    // Z
    dir = rot.GetForward();
    DrawLineDir(pos, dir, 0x0000ff, len);

}


