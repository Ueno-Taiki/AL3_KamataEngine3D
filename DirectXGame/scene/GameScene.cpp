#include "GameScene.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "AxisIndicator.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete sprite_;
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	//テクスチャーの読み込み
	textureHandle_ = TextureManager::Load("mario.jpg");
	//サウンドデータの読み込み
	soundDataHandle_ = audio_->LoadWave("fanfare.wav");
	//スプライトの生成
	sprite_ = Sprite::Create(textureHandle_, {100, 50});
	//3Dモデルの生成
	model_ = Model::Create();
	//ワールドトランスフォームの初期化
	worldTransfrom_.Initialize();
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//音声再生
	voiceHandle_ = audio_->PlayWave(soundDataHandle_, true);
	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

}

void GameScene::Update() {

	//スプライトの今の座標を取得
	Vector2 position = sprite_->GetPosition();
	//座標{ 2,1 }移動
	position.x += 2.0f;
	position.y += 1.0f;
	//移動した座標をスプライトに反映
	sprite_->SetPosition(position);

	//スペースキーを押した瞬間
	if (input_->TriggerKey(DIK_SPACE)) {
		// 音声停止
		audio_->StopWave(voiceHandle_);
	}

	//デバッグカメラの更新
	debugCamera_->Update();

#ifdef _DEBUG
	ImGui::Begin("Debug1");
	//デバックテキストの表示
	ImGui::Text("Kamata Tarou %d.%d.%d", 2050, 12, 31);
	//float3入力ボックス
	ImGui::InputFloat3("InputFloat3", inputFloat3);
	//float3スライダー
	ImGui::SliderFloat3("SliderFloat3", inputFloat3, 0.0f, 1.0f); 
	ImGui::End();
	//デモウインドウの表示を有効化
	ImGui::ShowDemoWindow();
#endif // DEBUG

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	sprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	// 3Dモデル描画
	model_->Draw(worldTransfrom_, debugCamera_->GetViewProjection(), textureHandle_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
