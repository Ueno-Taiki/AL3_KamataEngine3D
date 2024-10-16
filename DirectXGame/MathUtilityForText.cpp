#include <cmath>
#include <numbers>
#include <assert.h>
#include "MathUtilityForText.h"

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
{ 
	Matrix4x4 result = {0};

	result.m[0][0] = scale.x * (std::cos(rotate.y) * std::cos(rotate.z));
	result.m[0][1] = scale.x * (std::cos(rotate.y) * std::sin(rotate.z));
	result.m[0][2] = scale.x * -std::sin(rotate.y);
	result.m[0][3] = 0;

	result.m[1][0] = scale.y * (std::sin(rotate.x) * std::sin(rotate.y) * std::cos(rotate.z) - std::cos(rotate.x) * std::sin(rotate.z));
	result.m[1][1] = scale.y * (std::sin(rotate.x) * std::sin(rotate.y) * std::sin(rotate.z) + std::cos(rotate.x) * std::cos(rotate.z));
	result.m[1][2] = scale.y * (std::sin(rotate.x) * std::cos(rotate.y));
	result.m[1][3] = 0;

	result.m[2][0] = scale.z * (std::cos(rotate.x) * std::sin(rotate.y) * std::cos(rotate.z) + std::sin(rotate.x) * std::sin(rotate.z));
	result.m[2][1] = scale.z * (std::cos(rotate.x) * std::sin(rotate.y) * std::sin(rotate.z) - std::sin(rotate.x) * std::cos(rotate.z));
	result.m[2][2] = scale.z * (std::cos(rotate.x) * std::cos(rotate.y));
	result.m[2][3] = 0;

	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	result.m[3][3] = 1;

	return result;
}

Vector3& operator+=(Vector3& lhv, const Vector3& rhv){ 
	lhv.x += rhv.x;
	lhv.y += rhv.y;
	lhv.z += rhv.z;
	return lhv;
}

const Vector3 operator+(const Vector3& v1, const Vector3& v2) { 
	Vector3 temp(v1);
	return temp += v2;
}

Vector3& operator*=(Vector3& v, float s) { 
	v.x *= s;
	v.y *= s;
	v.z *= s;
	return v;
}

const Vector3 operator*(const Vector3& v, float s) { 
	Vector3 temp(v);
	return temp *= s;
}

float EaseInOut(float x1, float x2, float t) { 
	float easedT = -(std::cosf(std::numbers::pi_v<float> *t) - 1.0f) / 2.0f;
	return Lerp(x1, x2, easedT);
}

float Lerp(float x1, float x2, float t) { return (1.0f - t) * x1 + t * x2; }

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) { 
	return Vector3(Lerp(v1.x, v2.x, t), Lerp(v1.y, v2.y, t), Lerp(v1.z, v2.z, t)); 
}

// 衝突判定
bool IsCollision(const AABB& aabb1, const AABB& aabb2) {
	// x軸方向の判定
	if (aabb1.max.x < aabb2.min.x || aabb1.min.x > aabb2.max.x) {
		return false;
	}

	// y軸方向の判定
	if (aabb1.max.y < aabb2.min.y || aabb1.min.y > aabb2.max.y) {
		return false;
	}

	// z軸方向の判定
	if (aabb1.max.z < aabb2.min.z || aabb1.min.z > aabb2.max.z) {
		return false;
	}

	// 全ての軸で重なっていれば衝突している
	return true;
}

//Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 result = {0};

	result.m[0][0] = std::cos(radian);
	result.m[0][1] = std::sin(radian);
	result.m[0][2] = 0;
	result.m[0][3] = 0;

	result.m[1][0] = -std::sin(radian);
	result.m[1][1] = std::cos(radian);
	result.m[1][2] = 0;
	result.m[1][3] = 0;

	result.m[2][0] = 0;
	result.m[2][1] = 0;
	result.m[2][2] = 1;
	result.m[2][3] = 0;

	result.m[3][0] = 0;
	result.m[3][1] = 0;
	result.m[3][2] = 0;
	result.m[3][3] = 1;

	return result;
}

//座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result = {0};
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}
