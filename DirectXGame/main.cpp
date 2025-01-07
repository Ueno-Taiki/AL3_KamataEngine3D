#include "Audio.h"
#include "AxisIndicator.h"
#include "DirectXCommon.h"
#include "GameScene.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "WinApp.h"
#include "TitleScene.h"
#include "GameClear.h"
#include "GameOver.h"

GameScene* gameScene = nullptr;
TitleScene* titleScene = nullptr;
GameClear* gameClear = nullptr;
GameOver* gameOver = nullptr;

// シーン
enum class Scene {

	kUnknown = 0,

	kTitle,
	kGame,
	kGameCler,
	kGameOver
};

//現在シーン
Scene scene = Scene::kUnknown;

//シーン切り替え処理
void ChangeScene();

//シーンの更新
void UpdateScene();

//シーンの描画
void DrawScene();

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	// 汎用機能
	Input* input = nullptr;
	Audio* audio = nullptr;
	AxisIndicator* axisIndicator = nullptr;
	PrimitiveDrawer* primitiveDrawer = nullptr;

	// ゲームウィンドウの作成
	win = WinApp::GetInstance();
	win->CreateGameWindow(L"LE2D_01_ウエノ_タイキ_AL3");

	// DirectX初期化処理
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

#pragma region 汎用機能初期化
	// ImGuiの初期化
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	imguiManager->Initialize(win, dxCommon);

	// 入力の初期化
	input = Input::GetInstance();
	input->Initialize();

	// オーディオの初期化
	audio = Audio::GetInstance();
	audio->Initialize();

	// テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(dxCommon->GetDevice());
	TextureManager::Load("white1x1.png");

	// スプライト静的初期化
	Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::kWindowWidth, WinApp::kWindowHeight);

	// 3Dモデル静的初期化
	Model::StaticInitialize();

	// 軸方向表示初期化
	axisIndicator = AxisIndicator::GetInstance();
	axisIndicator->Initialize();

	primitiveDrawer = PrimitiveDrawer::GetInstance();
	primitiveDrawer->Initialize();
#pragma endregion

	//最初のシーンの初期化
	scene = Scene::kTitle;
	titleScene = new TitleScene;
	titleScene->Initialize();

	// メインループ
	while (true) {
		// メッセージ処理
		if (win->ProcessMessage()) {
			break;
		}

		// ImGui受付開始
		imguiManager->Begin();
		// 入力関連の毎フレーム処理
		input->Update();
		//シーン切り替え
		ChangeScene();
		//現在のシーン
		UpdateScene();
		// 軸表示の更新
		axisIndicator->Update();
		// ImGui受付終了
		imguiManager->End();

		// 描画開始
		dxCommon->PreDraw();
		//現在のシーンの描画
		DrawScene();
		// 軸表示の描画
		axisIndicator->Draw();
		// プリミティブ描画のリセット
		primitiveDrawer->Reset();
		// ImGui描画
		imguiManager->Draw();
		// 描画終了
		dxCommon->PostDraw();
	}

	// 各種解放
	delete gameScene;
	// 3Dモデル解放
	Model::StaticFinalize();
	audio->Finalize();
	// ImGui解放
	imguiManager->Finalize();
	delete titleScene;

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();

	return 0;
}

void ChangeScene() {
	switch (scene) {
	case Scene::kTitle:
		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			titleScene->StopMusic();
			//シーンの変更
			scene = Scene::kGame;
			//旧シーンの解放
			delete titleScene;
			titleScene = nullptr;
			//新シーンの生成と初期化
			gameScene = new GameScene;
			gameScene->Initialize();
		}
		break;
	case Scene::kGame:
		// 生き残った時
		if (gameScene->IsCleared()) {
			// シーンの変更
			scene = Scene::kGameCler;
			// 旧シーンの解放
			delete gameScene;
			gameScene = nullptr;
			// 新シーンの生成と初期化
			gameClear = new GameClear();
			gameClear->Initialize();
		}
		// 死んだ時
		else if (gameScene->IsFinished()) {
			// シーンの変更
			scene = Scene::kGameOver;
			// 旧シーンの解放
			delete gameScene;
			gameScene = nullptr;
			// 新シーンの生成と初期化
			gameOver = new GameOver();
			gameOver->Initialize();
		}

		break;
	case Scene::kGameCler:

		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			// シーンの変更
			scene = Scene::kTitle;
			// 旧シーンの解放
			delete gameClear;
			gameClear = nullptr;
			// 新シーンの生成と初期化
			titleScene = new TitleScene;
			titleScene->Initialize();
		}

		break;
	case Scene::kGameOver:

		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			// シーンの変更
			scene = Scene::kTitle;
			// 旧シーンの解放
			delete gameOver;
			gameOver = nullptr;
			// 新シーンの生成と初期化
			titleScene = new TitleScene;
			titleScene->Initialize();
		}

		break;
	}
}

void UpdateScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Update();
		break;
	case Scene::kGame:
		gameScene->Update();
		break;
	case Scene::kGameCler:
		gameClear->Update();
		break;
	case Scene::kGameOver:
		gameOver->Update();
		break;
	}
}

void DrawScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Draw();
		break;
	case Scene::kGame:
		gameScene->Draw();
		break;
	case Scene::kGameCler:
		gameClear->Draw();
		break;
	case Scene::kGameOver:
		gameOver->Draw();
		break;
	}
}
