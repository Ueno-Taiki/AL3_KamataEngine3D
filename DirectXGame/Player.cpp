#define NOMINMAX
#include "Player.h"
#include "Input.h"
#include "MathUtilityForText.h"
#include <cassert>
#include <numbers>
#include <algorithm>

void Player::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {

	// NULLポインタをチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	viewProjection_ = viewProjection;
	// ワールド変数の初期化
	worldTransfrom_.Initialize();
	worldTransfrom_.translation_ = position;

	//初期回転
	worldTransfrom_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

void Player::Update() {
	if (onGround_) {
		// 左右移動
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
			// 左右加速
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				// 左移動中の右入力
				if (velocity_.x < 0.0f) {
					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - KAttenuation);
				}
				acceleration.x += kAcceleration;
				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					// 旋回開始時の角度を記録する
					velocity_.y = turnFirstRotationY_;
					// 旋回タイマーに時間を設定
					turnTimer_ = kTimeTurn;
				}
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				// 右移動中の左入力
				if (velocity_.x > 0.0f) {
					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - KAttenuation);
				}
				acceleration.x -= kAcceleration;
				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					// 旋回開始時の角度を記録する
					velocity_.y = turnFirstRotationY_;
					// 旋回タイマーに時間を設定
					turnTimer_ = kTimeTurn;
				}
			}
			// 加速／減速
			velocity_.x += acceleration.x;

			// 最大速度制限
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);

			// 旋回制御
			if (turnTimer_ > 0.0f) {
				// 旋回タイマーをカウントダウンする
				turnTimer_ -= 1.0f / 60.0f;

				// 左右の自キャラ角度デーブル
				float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};
				// 状態に応じた角度を取得する
				float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
				// 自キャラの角度を設定する
				worldTransfrom_.rotation_.y = EaseInOut(destinationRotationY, turnFirstRotationY_, turnTimer_ / kTimeTurn);
			}
		} else {
			// 非入力時は移動減衰をかける
			velocity_.x *= (1.0f - KAttenuation);
		}
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			//ジャンプ初速
			velocity_ += Vector3(0, kJumpAcceleration, 0);
		}
		//空中
	} else {
		//落下速度
		velocity_ += Vector3(0, -kGravityAcceleration, 0);
		//落下速度
		velocity_.y = std::max(velocity_.y, -kLimitRunSpeed);
	}

	//移動
	worldTransfrom_.translation_ += velocity_;

	// 行列を定数バッファに転送
	worldTransfrom_.UpdateMatrix();

	//着地フラグ
	bool landing = false;

	//地面との当たり判定
	if (velocity_.y < 0) {
		//Y座標が地面以下になったら着地
		if (worldTransfrom_.translation_.y <= 1.0f) {
			landing = true;
		}
	}

	//接地判定
	if (onGround_) {
		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			// 空中状態に移行
			onGround_ = false;
		}
	} else {
		//着地
		if (landing) {
			//めり込み
			worldTransfrom_.translation_.y = 1.0f;
			//摩擦で横方向速度が減衰する
			velocity_.x *= (1.0f - KAttenuation);
			//下方向速度をリセット
			velocity_.y = 0.0f;
			//接地状態に移行
			onGround_ = true;
		}
	}
}

void Player::Draw() {
	//3Dモデル描画
	model_->Draw(worldTransfrom_, *viewProjection_, textureHandle_);
}
