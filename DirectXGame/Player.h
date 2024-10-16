#pragma once

#include "Model.h"
#include "WorldTransform.h"

//Vector3の足算
Vector3& operator+=(Vector3& lhv, const Vector3& rhv);
const Vector3 operator+(const Vector3& v1, const Vector3& v2);

class Player {
public:
	/// </summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle);

	/// </summary>
	/// 初期化
	/// </summary>
	void Update();

	/// </summary>
	/// 初期化
	/// </summary>
	void Draw(ViewProjection& viewProjection);

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//キーボード入力
	Input* input_ = nullptr;
};
