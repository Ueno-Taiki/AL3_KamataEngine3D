#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "MapChipField.h"
#include "CameraController.h"
#include <vector>

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

	void GenerateBlocks();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// 3Dモデルデータ
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	//自キャラ
	Player* player_ = nullptr;
	//自キャラの3Dモデル
	Model* modelPlayer_ = nullptr;

	//敵キャラ
	Enemy* enemy_ = nullptr;
	//敵キャラの3Dモデル
	Model* modelEnemy_ = nullptr;

	// 天球
	Skydome* skydome_ = nullptr;
	//天球の3Dモデル
	Model* modelSkydome_ = nullptr;

	//マップチップフィールド
	MapChipField* mapChipField_;

	//デバックカメラ
	DebugCamera* debugCamera_ = nullptr;

	//カメラコントローラー
	CameraController* cameraController_ = nullptr;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// ワールドトランスフォーム
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	//デバックカメラ有効
	bool isDebugCameraActive_ = false;
	
	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
