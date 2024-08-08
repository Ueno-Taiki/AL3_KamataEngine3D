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

