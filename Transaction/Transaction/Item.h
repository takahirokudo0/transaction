#pragma once

#include <vector>
#include <memory>
#include <boost/unordered_map.hpp>
#include <boost/optional.hpp>
#include "DataManager.h"

struct ItemData {
	int masterid;
	int count;
	int level;
	ItemData() : masterid{}, count{}, level{} {}
	ItemData(int masterid, int count = 0, int level = 0) : masterid(masterid), count(count), level(level) {}
	ItemData& operator+=(ItemData data) {
		count += data.count;
		level += data.level;
		return *this;
	}
	ItemData& operator-=(ItemData data) {
		count -= data.count;
		level -= data.level;
		return *this;
	}
	ItemData operator+(ItemData data) const {
		return ItemData(masterid, count + data.count, level + data.level);
	}
	ItemData operator-(ItemData data) const {
		return ItemData(masterid, count - data.count, level - data.level);
	}
	int GetKey() const { return masterid; }
};

class ItemDataManager : public MultiDataManager<ItemData, int> {
public:
	// 例外処理（Rollback）はメインロジックで実行する。
	bool ChangeCount(int transaction_id, int key, int count) {
		if (IsLocked(key)) {
			throw std::runtime_error("data was locked.");
			return false;
		}
		diff[transaction_id][key] = ItemData(key, count, 0);
		return true;
	}
	bool ChangeLevel(int transaction_id, int key, int level) {
		if (IsLocked(key)) {
			throw std::runtime_error("data was locked.");
			return false;
		}
		diff[transaction_id][key] = ItemData(key, 0, level);
		return true;
	}
	bool DeleteItemList(int transaction_id, const std::vector<int>& keys) {
		for (auto&& key : keys) {
			if (Delete(transaction_id, key) == false) return false;
		}
		return true;
	}
};
