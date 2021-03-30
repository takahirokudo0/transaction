#include <iostream>

#include "Client.h"
#include "Packet.h"
#include "Transaction.h"

/*
1.OnItemCreateReq		:パケット受信
2.ValidateItemCreate	:リクエスト内容のチェック
3.Transaction::Create	:トランザクションID発行
4.XXDataManager::Change	:データの変更
5.SendWebApi			:WebAPI送信
6.DataManager::Commit	:変更のコミット
7.DataManager::Rollback	:変更のロールバック
*/

template<class Req, class Res>
bool SendWebApi(const Req& packet, const boost::function<void(const Res&)> callback = boost::function<void(const Res&)>()) {
	if (true) {
		callback(Res());
	}
	else {
		throw std::runtime_error("Error:Network");
		return false;
	}
	return true;
}

// パケット送信処理
void Client::SendPacket(const PacketBase& packet)
{
}

// ItemCreateReq受信処理
void Client::OnItemCreateReq(const ItemCreateReq& req)
{
	auto&& share = shared_from_this();
	auto&& callback = [share](const ItemCreateRes& res) {
		std::cout << (res.result == Result::SUCCESS ? "Success" : "Failure") << std::endl;
		share->SendPacket(res);
	};
	bool result = ItemCreate(req, callback, callback);

	if (result == false) {
		std::cout << "Error:OnItemCreateReq" << std::endl;
	}
}
// アイテム生成処理
bool Client::ItemCreate(const ItemCreateReq& req, const boost::function<void(const ItemCreateRes&)> success_callback, const boost::function<void(const ItemCreateRes&)> failure_callback)
{
	bool result = false;
	// 権限や条件などをチェック
	if (ValidateItemCreate(req.use_item_masterid, req.use_money))
	{
		// トランザクションIDを発行
		auto&& transaction = Transaction::Create();

		// データの変動処理（try-catchすると可読性は上がるがパフォーマンスが下がる...）
		auto&& rollback_process = [this, transaction, failure_callback]() {
			money_manager.Rollback(transaction.GetID());
			item_manager.Rollback(transaction.GetID());
			failure_callback(ItemCreateRes(Result::FAILURE));
		};
		// お金の消費
		if (money_manager.Change(transaction.GetID(), -req.use_money) == false) {
			rollback_process();
			return false;
		}
		// アイテムの消費
		if (item_manager.ChangeCount(transaction.GetID(), req.use_item_masterid, -1) == false) {
			rollback_process();
			return false;
		}
		// アイテムの生成
		if (item_manager.Change(transaction.GetID(), ItemData(req.masterid, 1, 1)) == false) {
			rollback_process();
			return false;
		}

		// 変更をWebAPIに送信(1つのトランザクションに対してWebAPIリクエストは1件までにする)
		auto&& share = shared_from_this();
		result = SendWebApi<ItemCreateApiReq, ItemCreateApiRes>(ItemCreateApiReq(), [share, transaction, success_callback, failure_callback](const ItemCreateApiRes& res) {
			if (res.result == Result::SUCCESS)
			{
				// WebAPIで処理に成功した場合はコミットする
				share->money_manager.Commit(transaction.GetID());
				share->item_manager.Commit(transaction.GetID());
				success_callback(ItemCreateRes(res.result));
			}
			else
			{
				// WebAPIで処理に失敗した場合はロールバックする
				share->money_manager.Rollback(transaction.GetID());
				share->item_manager.Rollback(transaction.GetID());
				failure_callback(ItemCreateRes(res.result));
			}
		});
	}
	if (result == false)
	{
		failure_callback(ItemCreateRes(Result::FAILURE));
	}
	return result;

}
// アイテム生成の条件チェック（リクエスト内容や素材の保有数チェック）
bool Client::ValidateItemCreate(int use_item_masterid, int use_money)
{
	int use_item_count = 1;	// とりあえず1個必要ということで。
	// お金の過不足チェック
	//if (money_manager.GetData().count <= use_money) return false;
	// アイテムの過不足チェック
	//if (item_manager.GetData(use_item_masterid).count < use_item_count) return false;
	return true;
}