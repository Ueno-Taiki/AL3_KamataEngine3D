#include <imgui.h>
#include "RailCamera.h"
#include "MathUtilityForText.h"

void RailCamera::Initialize(const Vector3& positon, const Vector3& radian) {

	//ワールドトランスフォームの初期設定
	worldTransfrom_.translation_ = positon;
	worldTransfrom_.rotation_ = radian;

	//デバックカメラの生成
	debugCamera_ = new DebugCamera();
	
	//ビュープロジェクションの初期化
	//viewProjection_.farZ = 600;
	viewProjection_.Initialize();
}

void RailCamera::Update() {
	//ワールドトランスフォームの座標の数値を加算
	worldTransfrom_.translation_.x += 0.1f;
	worldTransfrom_.translation_.y += 0.1f;
	worldTransfrom_.translation_.z += 0.1f;

	//ワールドトランスフォームの角度の数値を加算
	worldTransfrom_.rotation_.x += 1.0f;
	worldTransfrom_.rotation_.y += 1.0f;
	worldTransfrom_.rotation_.z += 1.0f;

	//カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransfrom_.matWorld_);

	//カメラの座標を画面表示する処理
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("Translation", &viewProjection_.translation_.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("Rotation", &viewProjection_.rotation_.x, -180.0f, 180.0f);
	ImGui::End();

	//アフィン変換と転送
	worldTransfrom_.UpdateMatrix();
}
