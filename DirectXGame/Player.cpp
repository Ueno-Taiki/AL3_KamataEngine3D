#include <cassert>
#include "Player.h"
#include "Input.h"

void Player::Initialize(Model* model, uint32_t textureHandle) {
	//NULLポインタチャック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();
}

void Player::Update() {
	//キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	//キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	//押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	//押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_UP)) {
		move.y -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y += kCharacterSpeed;
	}

	//座標移動(ベクトルの加算)
	worldTransform_.translation_ += move;

	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {

	}

	//定数バッファに転送する
	

	//行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Player::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

