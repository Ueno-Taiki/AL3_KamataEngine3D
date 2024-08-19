#pragma once

#include <math.h>
#include "Matrix4x4.h"
#include "Vector3.h"

// AABB
struct AABB {
	Vector3 min; // 最小点
	Vector3 max; // 最大点
};

//3次元アフィン変換
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

//Vector3の足算
Vector3& operator+=(Vector3& lhv, const Vector3& rhv);
const Vector3 operator+(const Vector3& v1, const Vector3& v2);

//Vector3の掛け算
Vector3& operator*=(Vector3& v, float s);
const Vector3 operator*(const Vector3& v, float s);

//EaseInOut関数
float EaseInOut(float x1, float x2, float t);
float Lerp(float x1, float x2, float t);
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

// 衝突判定
bool IsCollision(const AABB& aabb1, const AABB& aabb2);

//Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian);

//座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);