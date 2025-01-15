#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include "MathUtilityForText.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	//自キャラの解放
	delete player_;
	//敵キャラの解放
	delete enemy_;
	//天球の解放
	delete modelSkydome_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//3Dモデルデータの生成
	model_ = Model::Create();
	modelSkydome_ = Model::CreateFromOBJ("SkyDome", true);

	//ファイルを指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	enemyHandle_ = TextureManager::Load("enemy.jpg");

	//天球を生成
	skydome_ = new Skydome();
	//天球の初期化
	skydome_->Initialize(modelSkydome_, textureHandle_, &viewProjection_);

	//自キャラの生成
	player_ = new Player();
	//自キャラの初期化
	player_->Initialize(model_, textureHandle_);

	//敵キャラの生成
	enemy_ = new Enemy();
	//敵キャラの座標
	Vector3 enemyPosition = {20, 2, 40};
	//敵キャラの初期化
	enemy_->Initialize(model_, enemyHandle_, enemyPosition);

	//敵キャラに自キャラのアドレスを表す
	enemy_->SetPlayer(player_);

	//レールカメラの生成
	railCamera_ = new RailCamera();
	//レールカメラの座標
	Vector3 railCameraPosition = {0, 0, -20};
	//レールカメラの角度
	Vector3 railCameraRadius = {0, 0, -20};
	//レールカメラの初期化
	railCamera_->Initialize(railCameraPosition, railCameraRadius);

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void GameScene::Update() {
	//自キャラの更新
	player_->Update();
	
	//敵キャラの更新
	enemy_->Update();

	//レールカメラの更新
	viewProjection_.matView = 
	viewProjection_.matProjection = viewProjection_.matProjection;
	railCamera_->Update();

	//大親分による判定
	CheckAllCollisions();
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
	
	//天球の描画
	skydome_->Draw();

	//自キャラの描画
	player_->Draw(viewProjection_);

	//敵キャラの描画
	enemy_->Draw(viewProjection_);

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

//大親分による判定
void GameScene::CheckAllCollisions() {
	//判定対象AとBの座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	//敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

	#pragma region 自キャラと敵弾の当たり判定
	//自キャラの座標
	posA = player_->GetWorldPosition();

	//自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
		//敵弾の座標
		posB = bullet->GetWorldPosition();

		//座標AとBの距離を求める
		float distance = CalculateDistanceSquared(posA, posB);

		//球と球の交差判定
		if (distance <= player_->GetRadius() + bullet->GetRadius()) {
			//自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			//敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
	#pragma endregion

	#pragma region 自弾と敵キャラの当たり判定
	//敵キャラの座標
	posA = enemy_->GetWorldPosition();

	//自弾と敵キャラ全ての当たり判定
	for (PlayerBullet* bullet : playerBullets) {
		//自弾の座標
		posB = bullet->GetWorldPosition();

		//座標AとBの距離を求める
		float distance = CalculateDistanceSquared(posA, posB);

		//球と球の交差判定
		if (distance <= enemy_->GetRadius() + bullet->GetRadius()) {
			//敵キャラの衝突時コールバックを呼び出す
			enemy_->OnCollision();
			//自弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
    #pragma endregion

	#pragma region 自弾と敵弾の当たり判定
	//自弾の座標
	for (PlayerBullet* playerBullet : playerBullets) {
		posA = playerBullet->GetWorldPosition();

		//自弾と敵弾全ての当たり判定
		for (EnemyBullet* enemyBullet : enemyBullets) {
			// 敵弾の座標
			posB = enemyBullet->GetWorldPosition();

			// 座標AとBの距離を求める
			float distance = CalculateDistanceSquared(posA, posB);

			// 球と球の交差判定
			if (distance <= playerBullet->GetRadius() + enemyBullet->GetRadius()) {
				// 自キャラの衝突時コールバックを呼び出す
				playerBullet->OnCollision();
				// 敵弾の衝突時コールバックを呼び出す
				enemyBullet->OnCollision();
			}
		}
	}
    #pragma endregion
}
