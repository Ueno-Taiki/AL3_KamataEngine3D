#include <cassert>
#include "Player.h"

void Player::Initialize(Model* model, uint32_t textureHandle) {
	//NULLポインタチャック
	assert(model);

	//メンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;

	//ワールド変換の初期化
	worldTransform_.Initialize();
}

void Player::Update() {}

void Player::Draw() {}

