#pragma once

#include <memory>
#include <boost/optional.hpp>
#include "DataManager.h"

struct MoneyData {
	int count;
	MoneyData() : count{} {}
	MoneyData(int count) : count(count) {}
	MoneyData& operator+=(MoneyData data) {
		count += data.count;
		return *this;
	}
	MoneyData& operator-=(MoneyData data) {
		count -= data.count;
		return *this;
	}
	MoneyData operator+(MoneyData data) const {
		return MoneyData(count + data.count);
	}
	MoneyData operator-(MoneyData data) const {
		return MoneyData(count - data.count);
	}
};
class MoneyDataManager : public SingleDataManager<MoneyData> {
};