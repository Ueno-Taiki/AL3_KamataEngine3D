#include "TitleScene.h"
#include "TextureManager.h"
#include <cassert>

TitleScene::~TitleScene() { 
	delete sprite_;
}

void TitleScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//ワールド変数の初期化
	worldTransfrom_.Initialize();
	worldTransfrom_.scale_ = {2, 2, 2};

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("Title.png");

	//BGM・SE読み込み
	//GameStart = audio_->LoadWave("BGM/GameStart.wav");

	//スプライトの生成
	sprite_ = Sprite::Create(textureHandle_, {0, 0});

	//音声再生
	//voiceHandle_ = audio_->PlayWave(GameStart, true);

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void TitleScene::Update() {
	//行列を更新
	worldTransfrom_.UpdateMatrix();
}

void TitleScene::Draw() {
	//コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	//背景スプライト描画前処理
	Sprite::PreDraw(commandList);
	sprite_->Draw();
	//スプライト描画後処理
	Sprite::PostDraw();
	//深度バッファクリア
	dxCommon_->ClearDepthBuffer();
}

void TitleScene::StopMusic() {
	//音声停止
	audio_->StopWave(voiceHandle_);
}

