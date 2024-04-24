#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

/// <summary>
/// ゲームシーン
/// </summary>
class Player {
public: // メンバ関数
		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection);

		/// <summary>
		/// 毎フレーム処理
		/// </summary>
		void Update();

		/// <summary>
		/// 描画
		/// </summary>
		void Draw();

private: // メンバ変数
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

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
