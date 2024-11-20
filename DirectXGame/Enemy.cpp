#include <cassert>
#include "Enemy.h"
#include "MathUtilityForText.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle, const Vector3& position) {
	// NULLポインタチャック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}

void Enemy::Update() {
	//接近フェーズの速度
	Vector3 velocity(0, 0, kBulletSpeed);
	//離脱フェーズの速度
	Vector3 leaveVelocity(kBulletSpeed, -kBulletSpeed, 0);

	switch (phase_) {
	case Phase::Approach:
	default:
		Approach(velocity);
		break;
	case Phase::Leave:
		Leave(leaveVelocity);
		break;
	}

	//アフィン変換と転送
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_); 
}

//接近
void Enemy::Approach(const Vector3& vector) {
	// 移動(ベクトルを加算)
	worldTransform_.translation_ += vector;
	// 規定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

//離脱
void Enemy::Leave(const Vector3& vector) {
	// 移動(ベクトルを加算)
	worldTransform_.translation_ += vector;
}
