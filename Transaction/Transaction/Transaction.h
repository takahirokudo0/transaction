#pragma once

class Transaction {
public:
	static Transaction Create() {
		static int next_transaction_id_{};
		return Transaction(next_transaction_id_++);
	}
	int GetID() const { return transaction_id_; }
private:
	Transaction(int transaction_id) : transaction_id_(transaction_id) {}
	int transaction_id_{};
};