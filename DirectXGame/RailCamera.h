#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"

class RailCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& positon, const Vector3& radian);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

private:
	//ワールド変換データ
	WorldTransform worldTransfrom_;
	//ビュープロジェクション
	ViewProjection viewProjection_;

	//デバックカメラ
	DebugCamera* debugCamera_ = nullptr;
};
