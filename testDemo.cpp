//testDemo.cpp

#include <gtest/gtest.h>

struct bankAccount
{
   int balance = 0;

   bankAccount() {

   }

   explicit bankAccount(const int balance)
      : balance{ balance }
   {

   }

   void deposit(int amount)
   {
      balance += amount;
   }

   bool withdraw(int amount)
   {
      if (amount <= balance)
      {
         balance -= amount;
         return true;
      }
      return false;
   }
};

struct bankAccountTest : testing::Test
{
   bankAccount* account;

   bankAccountTest()
   {
      account = new bankAccount;
   }

   virtual ~bankAccountTest() 
   {
      delete account;
   }
};

TEST_F(bankAccountTest, bankAccountStartsEmpty)
{
   EXPECT_EQ(0, account->balance);
}

TEST_F(bankAccountTest, CanDepositMoney)
{
   account->deposit(100);
   EXPECT_EQ(100, account->balance);
}

struct account_state
{
   int initial_balance;
   int withdraw_amount;
   int final_balance;
   bool success;

   friend std::ostream& operator<<(std::ostream& os, const account_state& obj)
   {
      return os
         << "initial_balance: " << obj.initial_balance
         << " withdraw_amount: " << obj.withdraw_amount
         << " final_balance: " << obj.final_balance
         << " success: " << obj.success;
   }
};

struct withdrawAccountTest : bankAccountTest, testing::WithParamInterface<account_state>
{
   withdrawAccountTest()
   {
      account->balance = GetParam().initial_balance;
   }
};

TEST_P(withdrawAccountTest, FinalBalance)
{
   auto as = GetParam();
   auto success = account->withdraw(as.withdraw_amount);
   EXPECT_EQ(as.final_balance, account->balance);
   EXPECT_EQ(as.success, success);
}

INSTANTIATE_TEST_CASE_P(Default, withdrawAccountTest, testing::Values(
   account_state{ 100, 50, 50, true },
   account_state{ 100, 200, 100, false }));

int main(int argc, char* argv[]) {

   testing::InitGoogleTest(&argc, argv);

   return RUN_ALL_TESTS();
}
