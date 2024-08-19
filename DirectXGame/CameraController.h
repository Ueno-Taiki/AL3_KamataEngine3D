#pragma once

#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"

class Player;

class CameraController {
public:
	// 矩形
	struct Rect {
		float left = 0.0f;   // 左側
		float right = 1.0f;  // 右側
		float bottom = 0.0f; // 下側
		float top = 1.0f;    // 上側
	};

	// カメラの目的座標
	Vector3 objective;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	void SetTarget(Player* target) { target_ = target; }

	void Reset();

	const ViewProjection& GetViewProjection() const { return viewProjection_; }

	void SetMovableArea(Rect area) { movableArea_ = area; }

private:
	Player* target_ = nullptr;

	// 座標補間割合
	static inline const float kInterpolationRate = 0.1f;
	// 速度掛け率
	static inline const float kVelocityBias = 30.0f;
	// 追従対象の各方向へのカメラ移動範囲
	static inline const Rect margin = {-5.0f, 5.0f, -5.0f, 5.0f};

	// 追尾対策とカメラの座標の差
	Vector3 targetOffset_ = {0, 0, -15.0f};
	// カメラ移動範囲
	Rect movableArea_ = {0, 100, 0, 100};

	// ビュープロジェクション
	ViewProjection viewProjection_;
};
