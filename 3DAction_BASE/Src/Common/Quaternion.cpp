#include <math.h>
#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "Quaternion.h"

Quaternion::Quaternion(void)
{
    w = 1;
    x = y = z = 0;
}

Quaternion::Quaternion(VECTOR rad)
{
    Quaternion q = Euler(rad.x, rad.y, rad.z);
    w = q.w;
    x = q.x;
    y = q.y;
    z = q.z;
}

Quaternion::Quaternion(double ww, double wx, double wy, double wz)
{
    w = ww;
    x = wx;
    y = wy;
    z = wz;
}

Quaternion Quaternion::Euler(VECTOR rad)
{
    return Euler(rad.x, rad.y, rad.z);
}

Quaternion Quaternion::Euler(double radX, double radY, double radZ)
{

    Quaternion ret = Quaternion();

    radX = AsoUtility::RadIn2PI(radX);
    radY = AsoUtility::RadIn2PI(radY);
    radZ = AsoUtility::RadIn2PI(radZ);

    double cosZ = cos(radZ / 2.0f);
    double sinZ = sin(radZ / 2.0f);
    double cosX = cos(radX / 2.0f);
    double sinX = sin(radX / 2.0f);
    double cosY = cos(radY / 2.0f);
    double sinY = sin(radY / 2.0f);

    //ret.w = cosZ * cosX * cosY + sinZ * sinX * sinY;
    //ret.x = sinZ * cosX * cosY - cosZ * sinX * sinY;
    //ret.y = cosZ * sinX * cosY + sinZ * cosX * sinY;
    //ret.z = cosZ * cosX * sinY - sinZ * sinX * cosY;

    ret.w = cosX * cosY * cosZ + sinX * sinY * sinZ;
    ret.x = sinX * cosY * cosZ + cosX * sinY * sinZ;
    ret.y = cosX * sinY * cosZ - sinX * cosY * sinZ;
    ret.z = cosX * cosY * sinZ - sinX * sinY * cosZ;

    return ret;

}

Quaternion Quaternion::Mult(Quaternion q1, Quaternion q2)
{

    Quaternion ret = Quaternion();
    double d1, d2, d3, d4;

    // wÇÃåvéZ 
    d1 = q1.w * q2.w;
    d2 = -q1.x * q2.x;
    d3 = -q1.y * q2.y;
    d4 = -q1.z * q2.z;
    ret.w = d1 + d2 + d3 + d4;

    // xÇÃåvéZ 
    d1 = q1.w * q2.x;
    d2 = q2.w * q1.x;
    d3 = q1.y * q2.z;
    d4 = -q1.z * q2.y;
    ret.x = d1 + d2 + d3 + d4;

    // yÇÃåvéZ
    d1 = q1.w * q2.y;
    d2 = q2.w * q1.y;
    d3 = q1.z * q2.x;
    d4 = -q1.x * q2.z;
    ret.y = d1 + d2 + d3 + d4;

    // zÇÃåvéZ
    d1 = q1.w * q2.z;
    d2 = q2.w * q1.z;
    d3 = q1.x * q2.y;
    d4 = -q1.y * q2.x;
    ret.z = d1 + d2 + d3 + d4;

    return ret;

}

Quaternion Quaternion::Mult(Quaternion q)
{
    return Mult(Quaternion(w, x, y, z), q);
}

Quaternion Quaternion::AngleAxis(double rad, VECTOR axis)
{

    Quaternion ret = Quaternion();

    double norm;
    double c, s;

    // UnityÇ…çáÇÌÇπÇÈ
    //ret.w = ret.x = ret.y = ret.z = 0.0;
    ret.w = 1.0;
    ret.x = ret.y = ret.z = 0.0;

    norm = (double)axis.x * (double)axis.x + (double)axis.y * (double)axis.y + (double)axis.z * (double)axis.z;
    if (norm <= 0.0f)
    {
        return ret;
    }

    norm = 1.0 / sqrt(norm);
    axis.x = (float)(axis.x * norm);
    axis.y = (float)(axis.y * norm);
    axis.z = (float)(axis.z * norm);

    c = cos(0.5f * rad);
    s = sin(0.5f * rad);

    ret.w = c;
    ret.x = s * axis.x;
    ret.y = s * axis.y;
    ret.z = s * axis.z;

    return ret;

}

VECTOR Quaternion::PosAxis(Quaternion q, VECTOR pos)
{
    // à íuèÓïÒÇ…âÒì]èÓïÒÇîΩâfÇ≥ÇπÇÈ
    // pos' = qÅEposÅEq(-1)
    Quaternion tmp = Quaternion();
    tmp = tmp.Mult(q);
    tmp = tmp.Mult(Quaternion(0.0f, pos.x, pos.y, pos.z));
    tmp = tmp.Mult(q.Inverse());
    return { (float)tmp.x, (float)tmp.y, (float)tmp.z };
}

VECTOR Quaternion::PosAxis(VECTOR pos)
{
    return PosAxis(Quaternion(w, x, y, z), pos);
}

VECTOR Quaternion::ToEuler(Quaternion q)
{

    VECTOR ret;

    double r11 = 2 * (q.x * q.z + q.w * q.y);
    double r12 = q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z;
    double r21 = -2 * (q.y * q.z - q.w * q.x);
    double r31 = 2 * (q.x * q.y + q.w * q.z);
    double r32 = q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z;

    ret.x = (float)asin(r21);
    ret.y = (float)atan2(r11, r12);
    ret.z = (float)atan2(r31, r32);

    return ret;

}

VECTOR Quaternion::ToEuler(void)
{
    return ToEuler(Quaternion(w, x, y, z));
}

MATRIX Quaternion::ToMatrix(Quaternion q)
{

    float sqw = (float)(q.w * q.w);
    float sqx = (float)(q.x * q.x);
    float sqy = (float)(q.y * q.y);
    float sqz = (float)(q.z * q.z);
    float invs = 1.0f / (sqx + sqy + sqz + sqw);

    MATRIX matrix = MGetIdent();

    matrix.m[0][0] = (sqx - sqy - sqz + sqw) * invs;
    matrix.m[1][1] = (-sqx + sqy - sqz + sqw) * invs;
    matrix.m[2][2] = (-sqx - sqy + sqz + sqw) * invs;

    float tmp1 = (float)(q.x * q.y);
    float tmp2 = (float)(q.z * q.w);
    matrix.m[0][1] = 2.0f * (tmp1 + tmp2) * invs;
    matrix.m[1][0] = 2.0f * (tmp1 - tmp2) * invs;

    tmp1 = (float)(q.x * q.z);
    tmp2 = (float)(q.y * q.w);
    matrix.m[0][2] = 2.0f * (tmp1 - tmp2) * invs;
    matrix.m[2][0] = 2.0f * (tmp1 + tmp2) * invs;

    tmp1 = (float)(q.y * q.z);
    tmp2 = (float)(q.x * q.w);
    matrix.m[1][2] = 2.0f * (tmp1 + tmp2) * invs;
    matrix.m[2][1] = 2.0f * (tmp1 - tmp2) * invs;

    return matrix;

}

MATRIX Quaternion::ToMatrix(void)
{
    return ToMatrix(Quaternion(w, x, y, z));
}

Quaternion Quaternion::LookRotation(VECTOR dir)
{
    VECTOR up = { 0.0f, 1.0f, 0.0f };
    return LookRotation(dir, up);
}

Quaternion Quaternion::LookRotation(VECTOR dir, VECTOR up)
{

    dir = AsoUtility::VNormalize(dir);
    VECTOR right = AsoUtility::VNormalize(VCross(up, dir));
    up = VCross(dir, right);
    auto m00 = right.x;
    auto m01 = right.y;
    auto m02 = right.z;
    auto m10 = up.x;
    auto m11 = up.y;
    auto m12 = up.z;
    auto m20 = dir.x;
    auto m21 = dir.y;
    auto m22 = dir.z;


    float num8 = (m00 + m11) + m22;
    auto quaternion = Quaternion();
    if (num8 > 0.0f)
    {
        double num = sqrt(num8 + 1.0);
        quaternion.w = num * 0.5;
        num = 0.5 / num;
        quaternion.x = ((double)m12 - m21) * num;
        quaternion.y = ((double)m20 - m02) * num;
        quaternion.z = ((double)m01 - m10) * num;
        return quaternion.Normalized();
    }
    if ((m00 >= m11) && (m00 >= m22))
    {
        // xÇ∆wÇ™ãtÅH
        //auto num7 = sqrt(((1.0f + m00) - m11) - m22);
        //auto num4 = 0.5f / num7;
        //quaternion.x = 0.5f * num7;
        //quaternion.y = (m01 + m10) * num4;
        //quaternion.z = (m02 + m20) * num4;
        //quaternion.w = (m12 - m21) * num4;
        //return quaternion.Normalized();
        auto num7 = sqrt(((1.0f + m00) - m11) - m22);
        auto num4 = 0.5f / num7;
        quaternion.x = ((double)m12 - m21) * num4;
        quaternion.y = ((double)m01 + m10) * num4;
        quaternion.z = ((double)m02 + m20) * num4;
        quaternion.w = 0.5 * num7;
        return quaternion.Normalized();
    }
    if (m11 > m22)
    {
        auto num6 = sqrt(((1.0f + m11) - m00) - m22);
        auto num3 = 0.5f / num6;
        quaternion.x = ((double)m10 + m01) * num3;
        quaternion.y = 0.5 * num6;
        quaternion.z = ((double)m21 + m12) * num3;
        quaternion.w = ((double)m20 - m02) * num3;
        return quaternion.Normalized();
    }
    auto num5 = sqrt(((1.0f + m22) - m00) - m11);
    auto num2 = 0.5f / num5;
    quaternion.x = ((double)m20 + m02) * num2;
    quaternion.y = ((double)m21 + m12) * num2;
    quaternion.z = 0.5 * num5;
    quaternion.w = ((double)m01 - m10) * num2;
    return quaternion.Normalized();

}

Quaternion Quaternion::GetRotation(MATRIX mat)
{

    float elem[4];
    elem[0] = mat.m[0][0] - mat.m[1][1] - mat.m[2][2] + 1.0f;
    elem[1] = -mat.m[0][0] + mat.m[1][1] - mat.m[2][2] + 1.0f;
    elem[2] = -mat.m[0][0] - mat.m[1][1] + mat.m[2][2] + 1.0f;
    elem[3] = mat.m[0][0] + mat.m[1][1] + mat.m[2][2] + 1.0f;

    int biggestIdx = 0;
    for (int i = 0; i < 4; i++)
    {
        if (elem[i] > elem[biggestIdx])
        {
            biggestIdx = i;
        }
    }

    if (elem[biggestIdx] < 0)
    {
        return Quaternion();
    }

    float q[4];
    float v = sqrt(elem[biggestIdx]) * 0.5f;
    q[biggestIdx] = v;
    float mult = 0.25f / v;

    switch (biggestIdx)
    {
    case 0:
        q[1] = (mat.m[1][0] + mat.m[0][1]) * mult;
        q[2] = (mat.m[0][2] + mat.m[2][0]) * mult;
        q[3] = (mat.m[2][1] - mat.m[1][2]) * mult;
        break;
    case 1:
        q[0] = (mat.m[1][0] + mat.m[0][1]) * mult;
        q[2] = (mat.m[2][1] + mat.m[1][2]) * mult;
        q[3] = (mat.m[0][2] - mat.m[2][0]) * mult;
        break;
    case 2:
        q[0] = (mat.m[0][2] + mat.m[2][0]) * mult;
        q[1] = (mat.m[2][1] + mat.m[1][2]) * mult;
        q[3] = (mat.m[1][0] - mat.m[0][1]) * mult;
        break;
    case 3:
        q[0] = (mat.m[2][1] - mat.m[1][2]) * mult;
        q[1] = (mat.m[0][2] - mat.m[2][0]) * mult;
        q[2] = (mat.m[1][0] - mat.m[0][1]) * mult;
        break;
    }

    return Quaternion(q[3], q[0], q[1], q[2]);

}

VECTOR Quaternion::GetDir(VECTOR dir)
{
    return PosAxis(dir);
}

VECTOR Quaternion::GetForward(void)
{
    return GetDir(AsoUtility::DIR_F);
}

VECTOR Quaternion::GetBack(void)
{
    return GetDir(AsoUtility::DIR_B);
}

VECTOR Quaternion::GetRight(void)
{
    return GetDir(AsoUtility::DIR_R);
}

VECTOR Quaternion::GetLeft(void)
{
    return GetDir(AsoUtility::DIR_L);
}

VECTOR Quaternion::GetUp(void)
{
    return GetDir(AsoUtility::DIR_U);
}

VECTOR Quaternion::GetDown(void)
{
    return GetDir(AsoUtility::DIR_D);
}

double Quaternion::Dot(Quaternion q1, Quaternion q2)
{
    return (q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z);
}

double Quaternion::Dot(Quaternion q)
{
    return (w * q.w + x * q.x + y * q.y + z * q.z);
}

Quaternion Quaternion::Normalized(void)
{

    double mag = sqrt(w * w + x * x + y * y + z * z);
    return Quaternion(w / mag, x / mag, y / mag, z / mag);

}

void Quaternion::Normalize(void)
{

    double mag = sqrt(w * w + x * x + y * y + z * z);

    w /= mag;
    x /= mag;
    y /= mag;
    z /= mag;

}

Quaternion Quaternion::Inverse(void)
{

    double n = 1.0f / (w * w + x * x + y * y + z * z);
    Quaternion tmp = Quaternion(w, -x, -y, -z);
    return Quaternion(tmp.w * n, tmp.x * n, tmp.y * n, tmp.z * n);;

}


Quaternion Quaternion::Slerp(Quaternion from, Quaternion to, double t)
{
    if (t > 1) t = 1;
    if (t < 0) t = 0;
    return SlerpUnclamped(from, to, (float)t);

}

inline float SIGN(float x) {
    return (x >= 0.0f) ? +1.0f : -1.0f;
}

inline float NORM(float a, float b, float c, float d) {
    return sqrt(a * a + b * b + c * c + d * d);
}

Quaternion Quaternion::FromToRotation(VECTOR fromDir, VECTOR toDir)
{

	VECTOR axis = VCross(fromDir, toDir);
	float angle = AsoUtility::AngleDeg(fromDir, toDir);
	if (angle >= 179.9196f)
	{
		auto r = VCross(fromDir, AsoUtility::DIR_R);
		axis = VCross(r, fromDir);
		//if (axis.sqrMagnitude < 0.000001f)
		float len = axis.x * axis.x + axis.y * axis.y + axis.z * axis.z;
		if (len < 0.000001f)
		{
			axis = AsoUtility::DIR_U;
		}
	}

	axis = AsoUtility::VNormalize(axis);
	return Quaternion::AngleAxis(AsoUtility::Deg2RadF(angle), axis);

}

Quaternion Quaternion::RotateTowards(Quaternion from, Quaternion to, float maxDegreesDelta)
{
    double num = Quaternion::Angle(from, to);
    if (num == 0.0)
    {
        return to;
    }
    float t = min(1.0f, maxDegreesDelta / (float)num);
    return Quaternion::SlerpUnclamped(from, to, t);
}

double Quaternion::Angle(Quaternion q1, Quaternion q2)
{
	double cos = Quaternion::Dot(q1, q2);
    double ac = acos(cos);
	return ac * (180.0 / DX_PI);
}

Quaternion Quaternion::SlerpUnclamped(Quaternion& a, Quaternion& b, float t)
{

    // if either input is zero, return the other.
    if (a.LengthSquared() == 0.0f)
    {
        if (b.LengthSquared() == 0.0f)
        {
            return Identity();
        }
        return b;
    }
    else if (b.LengthSquared() == 0.0f)
    {
        return a;
    }


    float cosHalfAngle = (float)(a.w * b.w) + VDot(a.xyz(), b.xyz());

    if (cosHalfAngle >= 1.0f || cosHalfAngle <= -1.0f)
    {
        // angle = 0.0f, so just return one input.
        return a;
    }
    else if (cosHalfAngle < 0.0f)
    {
        //b.xyz() = -b.xyz();
		b.x = b.x * -1.0f;
		b.y = b.y * -1.0f;
		b.z = b.z * -1.0f;
		b.w = -b.w;
        cosHalfAngle = -cosHalfAngle;
    }

    float blendA;
    float blendB;
    if (cosHalfAngle < 0.99f)
    {
        // do proper slerp for big angles
        float halfAngle = acosf(cosHalfAngle);
        float sinHalfAngle = sinf(halfAngle);
        float oneOverSinHalfAngle = 1.0f / sinHalfAngle;
        blendA = sinf(halfAngle * (1.0f - t)) * oneOverSinHalfAngle;
        blendB = sinf(halfAngle * t) * oneOverSinHalfAngle;
    }
    else
    {
        // do lerp if angle is really small.
        blendA = 1.0f - t;
        blendB = t;
    }

    //Quaternion result = Quaternion(blendA * a.xyz() + blendB * b.xyz(), blendA * a.w + blendB * b.w);
    VECTOR v = VAdd(VScale(a.xyz(), blendA), VScale(b.xyz(), blendB));
    //Quaternion result = Quaternion(v.x, v.y, v.z, blendA * a.w + blendB * b.w);
	Quaternion result = Quaternion(blendA * a.w + blendB * b.w, v.x, v.y, v.z);
    if (result.LengthSquared() > 0.0f)
    {
        return Normalize(result);
    }
    else
    {
        return Identity();
    }

}

Quaternion Quaternion::Identity(void)
{
    return Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
}

double Quaternion::Length(void)
{
    return sqrt(x * x + y * y + z * z + w * w);
}

double Quaternion::LengthSquared(void)
{
    return x * x + y * y + z * z + w * w;
}

VECTOR Quaternion::xyz(void)
{
    return { (float)x, (float)y, (float)z };
}

Quaternion Quaternion::Normalize(Quaternion q)
{
    Quaternion result = Quaternion();
    Normalize(q, &result);
    return result;
}

void Quaternion::Normalize(Quaternion q, Quaternion* result)
{

	float scale = 1.0f / (float)q.Length();
	VECTOR v = VScale(q.xyz(), scale);
	//Quaternion* ret = new Quaternion(v.x, v.y, v.z, q.w * scale);
	Quaternion* ret = new Quaternion(q.w * scale, v.x, v.y, v.z);
	result->x = ret->x;
	result->y = ret->y;
	result->z = ret->z;
	result->w = ret->w;

}

void Quaternion::ToAngleAxis(float* angle, VECTOR* axis)
{

	if (abs(this->w) > 1.0f)
	{
		this->Normalize();
	}
	*angle = 2.0f * acosf((float)this->w); // angle

    if (x == 0 && y == 0 && z == 0)
    {
        *angle = 0.0f;
    }

	float den = sqrtf(1.0f - (float)(this->w * this->w));
	if (den > 0.0001f)
	{
		//axis = q->xyz / den;
		auto v = this->xyz();
		axis->x = v.x / den;
		axis->y = v.y / den;
		axis->z = v.z / den;
	}
	else
	{
		// This occurs when the angle is zero. 
		// Not a problem: just set an arbitrary normalized axis.
		*axis = { 1.0f, 0.0f, 0.0f };
	}

}

Quaternion Quaternion::operator*(float& f) {
    return Quaternion(w * f, x * f, y * f, z * f);
}

const Quaternion Quaternion::operator*(const float& f) {
    return Quaternion(w * f, x * f, y * f, z * f);
}

Quaternion Quaternion::operator+(Quaternion& rhs) {
    return Quaternion(w + rhs.w, x + rhs.x, y + rhs.y, z + rhs.z);
}

const Quaternion Quaternion::operator+(const Quaternion& rhs) {
    return Quaternion(w + rhs.w, x + rhs.x, y + rhs.y, z + rhs.z);
}
//
//Quaternion Quaternion::operator*(Quaternion rhs)
//{
//	return Quaternion(
//		this->w * rhs.x + this->x * rhs.w + this->y * rhs.z - this->z * rhs.y,
//		this->w * rhs.y + this->y * rhs.w + this->z * rhs.x - this->x * rhs.z,
//		this->w * rhs.z + this->z * rhs.w + this->x * rhs.y - this->y * rhs.x,
//		this->w * rhs.w - this->x * rhs.x - this->y * rhs.y - this->z * rhs.z);
//}
//
//VECTOR Quaternion::operator*(VECTOR rhs)
//{
//
//	float num = this->x * 2.0f;
//	float num2 = this->y * 2.0f;
//	float num3 = this->z * 2.0f;
//	float num4 = this->x * num;
//	float num5 = this->y * num2;
//	float num6 = this->z * num3;
//	float num7 = this->x * num2;
//	float num8 = this->x * num3;
//	float num9 = this->y * num3;
//	float num10 = this->w * num;
//	float num11 = this->w * num2;
//	float num12 = this->w * num3;
//	VECTOR result;
//	result.x = (1.0f - (num5 + num6)) * rhs.x + (num7 - num12) * rhs.y + (num8 + num11) * rhs.z;
//	result.y = (num7 + num12) * rhs.x + (1.0f - (num4 + num6)) * rhs.y + (num9 - num10) * rhs.z;
//	result.z = (num8 - num11) * rhs.x + (num9 + num10) * rhs.y + (1.0f - (num4 + num5)) * rhs.z;
//	return result;
//}
