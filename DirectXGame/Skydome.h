#pragma once
#include "Model.h"
#include "WorldTransform.h"

class Skydome {
private:
	//ワールド変換データ
	WorldTransform worldTransfrom_;
	//モデム
	Model* model_ = nullptr;
};
