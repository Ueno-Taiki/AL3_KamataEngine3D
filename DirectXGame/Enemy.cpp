#include "Enemy.h"
#include <cassert>
#include <numbers>

void Enemy::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	// NULLポインタをチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	viewProjection_ = viewProjection;
	worldTransfrom_.translation_ = position;

	// ワールド変数の初期化
	worldTransfrom_.Initialize();
	worldTransfrom_.rotation_.y = std::numbers::pi_v<float> / -2.0f;

	//速度を設定する
	velocity_ = {-kWalkSpeed, 0, 0};

	walkTimer_ = 0.0f;
}

void Enemy::Update() {
	//移動
	worldTransfrom_.translation_ += velocity_;

	//タイマーを加算
	walkTimer_ += 1.0f / 60.0f;

	//回転アニメーション
	worldTransfrom_.rotation_.x = std::sin(std::numbers::pi_v<float> * 2.0f * walkTimer_ / kWalkMotionTime);

	//行列を更新
	worldTransfrom_.UpdateMatrix();
}

void Enemy::Draw() {
	// 3Dモデル描画
	model_->Draw(worldTransfrom_, *viewProjection_);
}

//ワールド座標を取得
Vector3  Enemy::GetWorldPosition()  { 
	// ワールド座標を入れる変数
	Vector3 worldPos = {};
	// ワールド行列の平行移動成文を取得
	worldPos.x = worldTransfrom_.matWorld_.m[3][0];
	worldPos.y = worldTransfrom_.matWorld_.m[3][1];
	worldPos.z = worldTransfrom_.matWorld_.m[3][2];

	return worldPos;
}

//AABB取得
AABB Enemy::GetAABB() { 
	Vector3 worldPos = GetWorldPosition();

	AABB aabb = {};

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

//衝突応答
void Enemy::OnCollision(const Player* player) { 
	(void)player; 
}

