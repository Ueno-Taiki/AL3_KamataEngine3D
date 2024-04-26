#include "WorldTransform.h"

void UpdateMatrix() {
	//スケール、回転、平行移動を合成して行列を計算する
	matWorld_ = MakeAffineMatrix();
	TransferMatrix();
}