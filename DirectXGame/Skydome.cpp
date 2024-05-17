#include "Skydome.h"

void Skydome::Initialize() {}

void Skydome::Update() {}

void Skydome::Draw() {
	//3Dモデル描画
	model_->Draw(worldTransfrom_, viewProjection_);
}
