#include <cassert>
#include "PlayerBullet.h"
#include "TextureManager.h"
#include "MathUtilityForText.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	//NULlポインタチェック
	assert(model);

	model_ = model;
	velocity_ = velocity;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("PlayerBullet.png");

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
}

void PlayerBullet::Update() { 
	//座標を移動させる
	worldTransform_.translation_ += velocity_;

	//時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
	
	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix(); 
}

void PlayerBullet::Draw(ViewProjection& viewProjection) {
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

//ワールド座標を取得
Vector3 PlayerBullet::GetWorldPosition() const {
	// ワールド座標を入れる変数
	Vector3 worldPos = {};
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

//衝突を検出したら呼び出されるコールバック関数
void PlayerBullet::OnCollision() { 
	isDead_ = true; 
}
