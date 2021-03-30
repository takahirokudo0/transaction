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
	// �g�����U�N�V�����@�F�V�K�o�^
	auto&& t = Transaction::Create();
	int item_masterid = 10;
	int count = 10;
	int level = 1;
	manager.Change(t.GetID(), ItemData(item_masterid, count, level));
	manager.Commit(t.GetID());
	std::cout << "count:" << manager.GetData(item_masterid).count << " level:" << manager.GetData(item_masterid).level << std::endl;

	// �g�����U�N�V�����A�F���ύX
	t = Transaction::Create();
	item_masterid = 10;
	int add_count = 20;
	manager.ChangeCount(t.GetID(), item_masterid, add_count);
	manager.Commit(t.GetID());
	std::cout << "count:" << manager.GetData(item_masterid).count << " level:" << manager.GetData(item_masterid).level << std::endl;

	// �g�����U�N�V�����B�F���x���ύX
	t = Transaction::Create();
	int add_level = 1;
	manager.ChangeLevel(t.GetID(), item_masterid, add_level);
	manager.Commit(t.GetID());
	std::cout << "count:" << manager.GetData(item_masterid).count << " level:" << manager.GetData(item_masterid).level << std::endl;

	// �g�����U�N�V�����C�F�����ύX
	t = Transaction::Create();
	add_count = 1;
	add_level = 1;
	manager.Change(t.GetID(), item_masterid, add_count, add_level);
	manager.Commit(t.GetID());
	std::cout << "count:" << manager.GetData(item_masterid).count << " level:" << manager.GetData(item_masterid).level << std::endl;

	// �g�����U�N�V�����D�F�����㏑��
	t = Transaction::Create();
	count = 10;
	level = 20;
	manager.Change(t.GetID(), ItemData(item_masterid, count, level));
	manager.Commit(t.GetID());
	std::cout << "count:" << manager.GetData(item_masterid).count << " level:" << manager.GetData(item_masterid).level << std::endl;

	// �g�����U�N�V�����E�F�폜
	t = Transaction::Create();
	manager.Delete(t.GetID(), item_masterid);
	manager.Commit(t.GetID());
	std::cout << "count:" << manager.GetData(item_masterid).count << " level:" << manager.GetData(item_masterid).level << std::endl;
}