#pragma once

#include "Model.h"
#include "Input.h"
#include "WorldTransform.h"
#include "PlayerBullet.h"

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

	//旋回
	void Rotate();

	//攻撃
	void Attack();

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//キーボード入力
	Input* input_ = nullptr;

	//弾
	PlayerBullet* bullet_ = nullptr;

	std::vector<WorldTransform*> worldTransformBlocks_;
};
