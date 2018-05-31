#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <assert.h>
#include "addrBook.pb.h"

using namespace std;

void ListPeople(const app::AddressBook &address_book)
{
    for (int i = 0; i < address_book.person_size(); i++)
    {
        const app::Person &person = address_book.person(i);

        cout << "Person ID: " << person.id() << endl;
        cout << "  Name: " << person.name() << endl;
        if (person.has_email())
        {
            cout << "  E-mail address: " << person.email() << endl;
        }

        for (int j = 0; j < person.phone_size(); j++)
        {
            const app::Person::PhoneNumber &phone_number = person.phone(j);

            switch (phone_number.type())
            {
            case app::Person::MOBILE:
                cout << "  Mobile phone #: ";
                break;
            case app::Person::HOME:
                cout << "  Home phone #: ";
                break;
            case app::Person::WORK:
                cout << "  Work phone #: ";
                break;
            }
            cout << phone_number.number() << endl;
        }
    }
}

int main(int argc, char* argv[])
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    if (argc != 2)
    {
        cerr << "Usage:  " << argv[0] << " ADDRESS_BOOK_FILE" << endl;
        return -1;
    }
    // 创建AddressBook对象
    app::AddressBook address_book;
    // 提供pb数据文件
    // 打开pb二进制数据文件
    FILE* fp = ::fopen(argv[1], "rb");
    assert(fp);
    char buffer[1024] = { '\0' };
    size_t nread = ::fread(buffer, sizeof(char), 1024, fp);
    // 拷贝二进制数据到string
    std::string message;
    message.assign(buffer, nread);
    // 反序列化pb二进制数据到address_book对象
    if (!address_book.ParseFromString(message))
    {
        cerr << "Failed to parse address book." << endl;
        return -1;
    }
    // 打印address_book对象
    ListPeople(address_book);

    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}