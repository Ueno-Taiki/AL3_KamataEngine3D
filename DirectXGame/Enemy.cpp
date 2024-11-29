#include <cassert>
#include "Enemy.h"
#include "MathUtilityForText.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle, const Vector3& position) {
	// NULLポインタチャック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;

	//接近フェーズ初期化
	ApproachInitialize();

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}

Enemy::~Enemy() {
	//bullet_の解放
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Enemy::Update() {
	//接近フェーズの速度
	Vector3 velocity(0, 0, kBulletSpeed);

	//接近
	Approach(velocity);

	//弾更新
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}

	//アフィン変換と転送
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//弾描画
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

//接近
void Enemy::Approach(const Vector3& vector) {
	//指定時間に達した
	if (--timer <= 0) {
		//弾を発射
		Fire();
		//発射タイマーを初期化
		timer = kFireInterval;
	}

	//移動(ベクトルを加算)
	worldTransform_.translation_ += vector;
}

//弾発射
void Enemy::Fire() {
	//弾の速度
	const float kEnemyBulletSpeed = -1.0f;
	Vector3 velocity(0, 0, kEnemyBulletSpeed);

	//弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	//弾を登録する
	bullets_.push_back(newBullet);
}

//接近フェーズ初期化
void Enemy::ApproachInitialize() {
	//発射タイマーを初期化
	timer = kFireInterval;
}
