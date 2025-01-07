#include <cassert>
#include "Enemy.h"
#include "Player.h"
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
	if (!isDead_) {
		model_->Draw(worldTransform_, viewProjection, textureHandle_);
	}

	//弾描画
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

//接近
void Enemy::Approach(const Vector3& vector) {
	//指定時間に達した
	if (!isDead_) {
		if (--timer <= 0) {
			// 弾を発射
			Fire();
			// 発射タイマーを初期化
			timer = kFireInterval;
		}
	}

	//移動(ベクトルを加算)
	worldTransform_.translation_ += vector;
}

//弾発射
void Enemy::Fire() {
	assert(player_);

	//弾の速度
	const float kEnemyBulletSpeed = 0.03f;

	//自キャラのワールド座標を取得する
	Vector3 playerPosition = player_->GetWorldPosition();

	//敵キャラのワールド座標を取得する
	Vector3 enemyPosition = GetWorldPosition();

	//敵キャラから自キャラへの差分ベクトルを求める
	Vector3 direction = playerPosition - enemyPosition;

	//ベクトルの正規化
	Normalize(direction);

	//ベクトルの長さを、速さに合わせる
	Vector3 velocity = direction * kEnemyBulletSpeed;

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

//ワールド座標を取得
Vector3 Enemy::GetWorldPosition() const { 
	//ワールド座標を入れる変数
	Vector3 worldPos = {};
	//ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos; 
}

//衝突を検出したら呼び出されるコールバック関数
void Enemy::OnCollision() { 
	isDead_ = true;
}
