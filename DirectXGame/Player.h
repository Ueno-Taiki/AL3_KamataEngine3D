#pragma once

#include <list>
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
	/// デストラクタ
	/// </summary>
	~Player();

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

	//ワールド座標を取得
	Vector3 GetWorldPosition() const;

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	//自機の半径
	float GetRadius() const { return 1.5f; }

	//弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

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
	std::list<PlayerBullet*> bullets_;

	std::vector<WorldTransform*> worldTransformBlocks_;
};
