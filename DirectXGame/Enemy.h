#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "MathUtilityForText.h"
#include "ViewProjection.h"

class Enemy {
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
	// ワールドトランスフォーム
	WorldTransform worldTransfrom_;
	// 3Dモデルデータ
	Model* model_ = nullptr;
	// ビュープロジェクション
	ViewProjection* viewProjection_ = nullptr;

};
