#pragma once
#include "Model.h"
#include "Input.h"
#include "WorldTransform.h"

class Enemy {
public:
	// 行動フェーズ
	enum class Phase {
		Approach, // 接近する
		Leave,    // 離脱する
	};

	// フェーズ
	Phase phase_ = Phase::Approach;

	/// </summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle, const Vector3& position);

	/// </summary>
	/// 初期化
	/// </summary>
	void Update();

	/// </summary>
	/// 初期化
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	//接近
	void Approach(const Vector3& vector);

	//離脱
	void Leave(const Vector3& vector);

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//敵の速度
	static inline const float kBulletSpeed = -0.5f;
};
