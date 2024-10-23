#include <cassert>
#include "PlayerBullet.h"
#include "TextureManager.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position) {
	//NULlポインタチェック
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("uvChecker.png");

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
}

void PlayerBullet::Update() { 
	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix(); 
}

void PlayerBullet::Draw(ViewProjection& viewProjection) {
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
