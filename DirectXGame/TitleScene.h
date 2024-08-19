#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class TitleScene {
public:
	void Initialize();

	void Update();

	void Draw();

	//デスフラグのgetter
	bool IsFinished() const { return finished_; }

private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransfrom_;
	// ビュープロジェクション
	ViewProjection viewProjection_;
	//タイトルシーン
	TitleScene* titleScene_ = nullptr;
	//3Dモデルデータ
	Model* model_ = nullptr;

	//終了フラグ
	bool finished_ = false;
};