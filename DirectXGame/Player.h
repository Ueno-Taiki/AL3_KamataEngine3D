#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

//左右
enum class LRDirection {
	kRight,
	kLeft,
};

/// <summary>
/// ゲームシーン
/// </summary>
class Player {
public: // メンバ関数
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
	// 慣性移動
	static inline const float kAcceleration = 0.01f;
	static inline const float KAttenuation = 0.05f;
	static inline const float kLimitRunSpeed = 0.2f;
	//旋回時間
	static inline const float kTimeTurn = 0.3f;
	//重力加速度
	static inline const float kGravityAcceleration = 0.05f;
	//最大落下速度
	static inline const float kLimitFallSpeed = 0.5f;
	//ジャンプ初速
	static inline const float kJumpAcceleration = 0.5f;

	// メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	//モデル
	Model* model_ = nullptr;
	ViewProjection* viewProjection_ = nullptr;
	//ワールド変換データ
	WorldTransform worldTransfrom_;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//速度
	Vector3 velocity_ = {};

	//左右
	LRDirection lrDirection_ = LRDirection::kRight;

	//旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	//旋回タイマー
	float turnTimer_ = 0.0f;

	//接地状態フラグ
	bool onGround_ = true;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
