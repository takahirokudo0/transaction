#pragma once

#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>

template<class Data>
class SingleDataManager {
public:
	// データを変更する場合はdiffを作成するメソッドを利用しないといけない
	template <class... Args>
	bool Change(int transaction_id, Args... args) {
		if (IsLocked()) {
			//throw std::runtime_error("data was locked.");
			return false;
		}
		diff = Data(args...);
		return true;
	}
	bool Change(int transaction_id, const Data& value) {
		if (IsLocked()) {
			return false;
		}
		diff = value - data;
		return true;
	}
	void Commit(int transaction_id) {
		if (diff) { data += *diff; }
		Unlock();
	}
	void Rollback(int transaction_id) {
		Unlock();
	}
	// 参照しても良いが、直接変更することは禁止する
	const Data& GetData() {
		return data;
	}
private:
	bool IsLocked() {
		return diff != boost::none;
	}
	void Unlock() {
		diff = boost::none;
	}
	boost::optional<Data> diff;
	Data data;
};

template<class Data, class Key>
class MultiDataManager {
public:
	// データを変更する場合はdiffを作成するメソッドを利用しないといけない
	template <class... Args>
	bool Change(int transaction_id, const Key& key, Args... args) {
		if (IsLocked(key)) {
			//throw std::runtime_error("data was locked.");
			return false;
		}
		diff[transaction_id][key] = Data(key, args...);
		return true;
	}
	bool Change(int transaction_id, const Data& value) {
		if (IsLocked(value.GetKey())) {
			//throw std::runtime_error("data was locked.");
			return false;
		}
		diff[transaction_id][value.GetKey()] = value - data[value.GetKey()];
		return true;
	}
	bool Delete(int transaction_id, const Key& key) {
		if (IsLocked(key)) {
			//throw std::runtime_error("data was locked.");
			return false;
		}
		del[transaction_id].insert(key);
		return true;
	}
	void Commit(int transaction_id) {
		for (auto&& value : diff[transaction_id]) {
			data[value.second.GetKey()] += value.second;
		}
		for (auto&& key : del[transaction_id]) {
			data.erase(key);
		}
		Unlock(transaction_id);
	}
	void Rollback(int transaction_id) {
		Unlock(transaction_id);
	}
	bool IsExist(const Key& key) {
		return data.find(key) != data.end();
	}
	// 参照しても良いが、直接変更することは禁止する
	const Data& GetData(const Key& key) {
		return data[key];
	}
protected:
	bool IsLocked(const Key& key) {
		for (auto&& transaction : diff) {
			if (transaction.second.find(key) != transaction.second.end()) {
				return true;
			}
		}
		return false;
	}
	void Unlock(int transaction_id) {
		diff.erase(transaction_id);
		del.erase(transaction_id);
	}
	// key:TransactionID, key:Key
	boost::unordered_map<int, boost::unordered_map<Key, Data>> diff;
	// key:TransactionID
	boost::unordered_map<int, boost::unordered_set<Key>> del;
	// key:Key
	boost::unordered_map<Key, Data> data;
};