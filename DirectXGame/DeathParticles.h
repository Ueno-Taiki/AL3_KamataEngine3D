#pragma once

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <array>

class DeathParticles {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	//パーティクルの個数
	static inline const uint32_t kNumParticles = 8;
	//ワールド変数データ
	std::array<WorldTransform, kNumParticles> worldTransforms_;

	//モデル
	Model* model_ = nullptr;
	
	ViewProjection* viewProjection_ = nullptr;

};