
#include "abstractClass.h"
#include "concreteClass.h"

int main(int argc, char *argv[])
{
    Account *cAccount = new CurrentAccount();
    cAccount->Handle("user", "password");
    Account *sAccount = new SavingAccount();
    sAccount->Handle("user", "password");
    return 0;
}