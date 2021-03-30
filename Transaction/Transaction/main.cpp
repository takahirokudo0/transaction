#include <iostream>

#include "Client.h"
#include "DataManager.h"
#include "Item.h"
#include "Money.h"
#include "Packet.h"
#include "Transaction.h"

int main(int argc, char** argv)
{
	auto&& client = std::make_shared<Client>();

	client->OnItemCreateReq(ItemCreateReq(1, 2, 0));

	system("pause");
	return 0;
}

void Example()
{
	ItemDataManager manager;
	// トランザクション①：新規登録
	auto&& t = Transaction::Create();
	int item_masterid = 10;
	int count = 10;
	int level = 1;
	manager.Change(t.GetID(), ItemData(item_masterid, count, level));
	manager.Commit(t.GetID());
	std::cout << "count:" << manager.GetData(item_masterid).count << " level:" << manager.GetData(item_masterid).level << std::endl;

	// トランザクション②：個数変更
	t = Transaction::Create();
	item_masterid = 10;
	int add_count = 20;
	manager.ChangeCount(t.GetID(), item_masterid, add_count);
	manager.Commit(t.GetID());
	std::cout << "count:" << manager.GetData(item_masterid).count << " level:" << manager.GetData(item_masterid).level << std::endl;

	// トランザクション③：レベル変更
	t = Transaction::Create();
	int add_level = 1;
	manager.ChangeLevel(t.GetID(), item_masterid, add_level);
	manager.Commit(t.GetID());
	std::cout << "count:" << manager.GetData(item_masterid).count << " level:" << manager.GetData(item_masterid).level << std::endl;

	// トランザクション④：両方変更
	t = Transaction::Create();
	add_count = 1;
	add_level = 1;
	manager.Change(t.GetID(), item_masterid, add_count, add_level);
	manager.Commit(t.GetID());
	std::cout << "count:" << manager.GetData(item_masterid).count << " level:" << manager.GetData(item_masterid).level << std::endl;

	// トランザクション⑤：両方上書き
	t = Transaction::Create();
	count = 10;
	level = 20;
	manager.Change(t.GetID(), ItemData(item_masterid, count, level));
	manager.Commit(t.GetID());
	std::cout << "count:" << manager.GetData(item_masterid).count << " level:" << manager.GetData(item_masterid).level << std::endl;

	// トランザクション⑥：削除
	t = Transaction::Create();
	manager.Delete(t.GetID(), item_masterid);
	manager.Commit(t.GetID());
	std::cout << "count:" << manager.GetData(item_masterid).count << " level:" << manager.GetData(item_masterid).level << std::endl;
}