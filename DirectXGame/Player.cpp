#include <cassert>
#include "Player.h"
#include "Input.h"
#include "MathUtilityForText.h"
#include "ImGuiManager.h"

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
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	//座標移動(ベクトルの加算)
	worldTransform_.translation_ += move;

	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {
		//アフィン変換作成
		Matrix4x4 matWorld = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
		worldTransformBlock->matWorld_ = matWorld;

		//定数バッファに転送する
		worldTransformBlock->TransferMatrix();
	}

	//キャラクターの座標を画面表示する処理
	ImGui::Begin(" ");
	ImGui::SliderFloat3("player", &worldTransform_.translation_.x, 0.0f, 1.0f);
	ImGui::End();

	//移動限界座標
	const float kMoveLimitX = 35.0f;
	const float kMoveKimitY = 19.0f;

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveKimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveKimitY);

	//アフィン変換と転送
	worldTransform_.UpdateMatrix();
}

void Player::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

