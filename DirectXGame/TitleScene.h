#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Input.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class TitleScene {
public:
	~TitleScene();

	void Initialize();

	void Update();

	void Draw();

	void StopMusic();

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
	
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	//スプライト
	Sprite* sprite_ = nullptr;

	//サウンドデータハンドル
	uint32_t GameStart = 0;

	//音声再生ハンドル
	uint32_t voiceHandle_ = 0;
};