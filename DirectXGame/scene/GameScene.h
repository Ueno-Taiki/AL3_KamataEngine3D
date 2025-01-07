#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//大親分による判定
	void CheckAllCollisions();

	//クリアフラグのgetter
	bool IsCleared() const { return cleared_; }

	//デスフラグのgetter
	bool IsFinished() const { return finished_; }

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	//3Dモデル
	Model* model_ = nullptr;

	//自キャラ
	Player* player_ = nullptr;

	//敵キャラ
	Enemy* enemy_ = nullptr;
	//敵キャラのテクスチャハンドル
	uint32_t enemyHandle_ = 0;

	//天球
	Skydome* skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;

	//サウンドデータハンドル
	uint32_t BGM = 0;

	//音声再生ハンドル
	uint32_t voiceHandle_ = 0;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//終了フラグ
	bool cleared_ = false;
	bool finished_ = false;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
