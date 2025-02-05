#include <imgui.h>
#include "RailCamera.h"
#include "MathUtilityForText.h"

void RailCamera::Initialize(const Vector3& positon, const Vector3& radian) {

	//引数で受け取った初期座標をセット
	worldTransfrom_.translation_ = positon;
	worldTransfrom_.rotation_ = radian;

	// ワールドトランスフォームの初期設定
	worldTransfrom_.Initialize();

	//ビュープロジェクションの初期化
	viewProjection_.farZ = 600;
	viewProjection_.Initialize();
}

void RailCamera::Update() {
	//移動
	worldTransfrom_.translation_ += Vector3(0.0f, 0.0f, 1.0f);
	//ワールド行列
	worldTransfrom_.matWorld_ = MakeAffineMatrix(worldTransfrom_.scale_, worldTransfrom_.rotation_, worldTransfrom_.translation_);

	//カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransfrom_.matWorld_);

	//ビュープロジェクションを転送
	viewProjection_.TransferMatrix();

	//カメラの座標を画面表示する処理
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("Translation", &viewProjection_.translation_.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("Rotation", &viewProjection_.rotation_.x, -180.0f, 180.0f);
	ImGui::End();

	//アフィン変換と転送
	worldTransfrom_.UpdateMatrix();
}
