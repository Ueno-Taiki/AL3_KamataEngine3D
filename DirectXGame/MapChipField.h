#pragma once


#include "Audio.h"
#include "DirectXCommon.h"
#include <vector>
#include <Vector3.h>

enum class MapChipType {
	kBlank,  //空白
	kBlock,  //ブロック
};

struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};

class MapChipField {
public:
	struct IndexSet {
		uint32_t xIndex;
		uint32_t yIndex;
	};

	//範囲矩形
	struct Rect {
		float left;
		float right;
		float bottom;
		float top;
	};

	//リセット
	void ResetMapChipData();

	//読み込み
	void LoadMapChipCsv(const std::string& filePath);

	uint32_t GetNumBlockVirtical() const { return kNumBlockVirtical; }
	uint32_t GetNumBlockHorizontal() const { return kNumBlockHorizontal; }

	//マップチップ種別の取得
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

	//マップチップ座標の取得
	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

	//座標からマップチップ番号を計算
	IndexSet GetMapChipIndexSetByPosition(const Vector3& position);

	//ブロックの範囲取得関数
	Rect GetRectByIndex(uint32_t xIndex, uint32_t yIndex);

private:
	//1ブロックのサイズ
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;
	//ブロックの個数
	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

	MapChipData mapChipData_;
};
