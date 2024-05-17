#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Skydome {
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

private:
	//ワールド変換データ
	WorldTransform worldTransfrom_;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//モデム
	Model* model_ = nullptr;
};
