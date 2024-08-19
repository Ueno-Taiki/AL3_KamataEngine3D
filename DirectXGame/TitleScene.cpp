#include "TitleScene.h"
#include <cassert>

void TitleScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ワールド変数の初期化
	worldTransfrom_.Initialize();
	worldTransfrom_.scale_ = {2, 2, 2};

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// 3Dモデルデータの生成
	model_ = Model::CreateFromOBJ("titleFont", true);
}

void TitleScene::Update() {
	//タイトルシーンの終了条件
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}

	//行列を更新
	worldTransfrom_.UpdateMatrix();
}

void TitleScene::Draw() {
	//コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	//3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	//3Dモデル描画
	model_->Draw(worldTransfrom_, viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
}

