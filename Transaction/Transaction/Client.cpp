#include <iostream>

#include "Client.h"
#include "Packet.h"
#include "Transaction.h"

/*
1.OnItemCreateReq		:�p�P�b�g��M
2.ValidateItemCreate	:���N�G�X�g���e�̃`�F�b�N
3.Transaction::Create	:�g�����U�N�V����ID���s
4.XXDataManager::Change	:�f�[�^�̕ύX
5.SendWebApi			:WebAPI���M
6.DataManager::Commit	:�ύX�̃R�~�b�g
7.DataManager::Rollback	:�ύX�̃��[���o�b�N
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

// �p�P�b�g���M����
void Client::SendPacket(const PacketBase& packet)
{
}

// ItemCreateReq��M����
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
// �A�C�e����������
bool Client::ItemCreate(const ItemCreateReq& req, const boost::function<void(const ItemCreateRes&)> success_callback, const boost::function<void(const ItemCreateRes&)> failure_callback)
{
	bool result = false;
	// ����������Ȃǂ��`�F�b�N
	if (ValidateItemCreate(req.use_item_masterid, req.use_money))
	{
		// �g�����U�N�V����ID�𔭍s
		auto&& transaction = Transaction::Create();

		// �f�[�^�̕ϓ������itry-catch����Ɖǐ��͏オ�邪�p�t�H�[�}���X��������...�j
		auto&& rollback_process = [this, transaction, failure_callback]() {
			money_manager.Rollback(transaction.GetID());
			item_manager.Rollback(transaction.GetID());
			failure_callback(ItemCreateRes(Result::FAILURE));
		};
		// �����̏���
		if (money_manager.Change(transaction.GetID(), -req.use_money) == false) {
			rollback_process();
			return false;
		}
		// �A�C�e���̏���
		if (item_manager.ChangeCount(transaction.GetID(), req.use_item_masterid, -1) == false) {
			rollback_process();
			return false;
		}
		// �A�C�e���̐���
		if (item_manager.Change(transaction.GetID(), ItemData(req.masterid, 1, 1)) == false) {
			rollback_process();
			return false;
		}

		// �ύX��WebAPI�ɑ��M(1�̃g�����U�N�V�����ɑ΂���WebAPI���N�G�X�g��1���܂łɂ���)
		auto&& share = shared_from_this();
		result = SendWebApi<ItemCreateApiReq, ItemCreateApiRes>(ItemCreateApiReq(), [share, transaction, success_callback, failure_callback](const ItemCreateApiRes& res) {
			if (res.result == Result::SUCCESS)
			{
				// WebAPI�ŏ����ɐ��������ꍇ�̓R�~�b�g����
				share->money_manager.Commit(transaction.GetID());
				share->item_manager.Commit(transaction.GetID());
				success_callback(ItemCreateRes(res.result));
			}
			else
			{
				// WebAPI�ŏ����Ɏ��s�����ꍇ�̓��[���o�b�N����
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
// �A�C�e�������̏����`�F�b�N�i���N�G�X�g���e��f�ނۗ̕L���`�F�b�N�j
bool Client::ValidateItemCreate(int use_item_masterid, int use_money)
{
	int use_item_count = 1;	// �Ƃ肠����1�K�v�Ƃ������ƂŁB
	// �����̉ߕs���`�F�b�N
	//if (money_manager.GetData().count <= use_money) return false;
	// �A�C�e���̉ߕs���`�F�b�N
	//if (item_manager.GetData(use_item_masterid).count < use_item_count) return false;
	return true;
}