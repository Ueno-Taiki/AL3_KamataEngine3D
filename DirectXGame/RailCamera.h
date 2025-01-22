#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

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

	//ビュープロジェクションを取得
	const ViewProjection& GetViewProjection() { return viewProjection_; }

private:
	//ワールド変換データ
	WorldTransform worldTransfrom_;
	//ビュープロジェクション
	ViewProjection viewProjection_;
};
