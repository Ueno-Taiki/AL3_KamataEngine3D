#include "Skydome.h"
#include <cassert>

void Skydome::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection) {
	// NULLポインタをチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = textureHandle;
	viewProjection_ = viewProjection;
	// ワールド変数の初期化
	worldTransfrom_.Initialize();
}

void Skydome::Update() {}

void Skydome::Draw() {
	//3Dモデル描画
	model_->Draw(worldTransfrom_, *viewProjection_);
}
