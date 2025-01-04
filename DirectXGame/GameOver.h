#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Input.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class GameOver {
public:
	~GameOver();

	void Initialize();

	void Update();

	void Draw();

private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransfrom_;
	//ビュープロジェクション
	ViewProjection viewProjection_;

	//ゲームオーバーシーン
	GameOver* gameOver_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	//スプライト
	Sprite* sprite_ = nullptr;

	//サウンドデータハンドル
	uint32_t Gameover = 0;

	//音声再生ハンドル
	uint32_t voiceHandle_ = 0;
};
