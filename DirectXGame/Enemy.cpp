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
}

void Enemy::Update() {
	//行列を更新
	worldTransfrom_.UpdateMatrix();
}

void Enemy::Draw() {
	// 3Dモデル描画
	model_->Draw(worldTransfrom_, *viewProjection_);
}

