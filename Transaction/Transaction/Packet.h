#pragma once

enum Result {
	SUCCESS = 0,
	FAILURE = 1
};

class PacketBase {
public:
};
class ItemCreateReq : public PacketBase {
public:
	int masterid{};
	int use_item_masterid{};
	int use_money{};
	ItemCreateReq() {}
	ItemCreateReq(int masterid, int use_item_masterid, int use_money) : masterid(masterid), use_item_masterid(use_item_masterid), use_money(use_money) {}
};
class ItemCreateRes : public PacketBase {
public:
	unsigned char result;
	ItemCreateRes(unsigned char result) : result(result) {}
};
class ItemCreateApiReq : public PacketBase {
public:
	int masterid{};
};
class ItemCreateApiRes : public PacketBase {
public:
	unsigned char result;
};