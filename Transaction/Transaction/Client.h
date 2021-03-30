#pragma once

#include <memory>
#include <boost/function.hpp>
#include "Item.h"
#include "Money.h"

class PacketBase;
class ItemCreateReq;
class ItemCreateRes;

class Client : public std::enable_shared_from_this<Client> {
public:
	void SendPacket(const PacketBase& packet);

	void OnItemCreateReq(const ItemCreateReq& req);
	bool ItemCreate(const ItemCreateReq& req, const boost::function<void(const ItemCreateRes&)> success_callback = boost::function<void(const ItemCreateRes&)>(), const boost::function<void(const ItemCreateRes&)> failure_callback = boost::function<void(const ItemCreateRes&)>());
	bool ValidateItemCreate(int use_item_masterid, int use_money);
private:
	ItemDataManager item_manager;
	MoneyDataManager money_manager;
};