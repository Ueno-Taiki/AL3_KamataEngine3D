#include "DeathParticles.h"
#include <cassert>

void DeathParticles::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	// NULLポインタをチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	viewProjection_ = viewProjection;

	//ワールド変数の初期化
	for (auto& worldTransform : worldTransforms_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}
}

void DeathParticles::Update() { 
	//行列の更新
	for (auto& worldTransform : worldTransforms_) {
		worldTransform.UpdateMatrix();
	}
}

void DeathParticles::Draw() {
	for (auto& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform, *viewProjection_);
	}
}

