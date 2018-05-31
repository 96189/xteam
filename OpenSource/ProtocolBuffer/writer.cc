#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include "addrBook.pb.h"

using namespace std;

void PromptForAddress(app::Person *person)
{
    cout << "Enter person ID number: ";
    int id;
    cin >> id;
    person->set_id(id);
    cin.ignore(256, '\n');

    cout << "Enter name: ";
    getline(cin, *person->mutable_name());

    cout << "Enter email address (blank for none): ";
    string email;
    getline(cin, email);
    if (!email.empty())
    {
        person->set_email(email);
    }

    while (true)
    {
        cout << "Enter a phone number (or leave blank to finish): ";
        string number;
        getline(cin, number);
        if (number.empty())
        {
            break;
        }

        app::Person::PhoneNumber *phone_number = person->add_phone();
        phone_number->set_number(number);

        cout << "Is this a mobile, home, or work phone? ";
        string type;
        getline(cin, type);
        if (type == "mobile")
        {
            phone_number->set_type(app::Person::MOBILE);
        }
        else if (type == "home")
        {
            phone_number->set_type(app::Person::HOME);
        }
        else if (type == "work")
        {
            phone_number->set_type(app::Person::WORK);
        }
        else
        {
            cout << "Unknown phone type.  Using default." << endl;
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
    // 由AddressBook创建Person子对象
    app::Person* pPerson = address_book.add_person();
    // 填充person成员
    PromptForAddress(pPerson);
    // 提供待写入文件
    fstream output(argv[1], ios::out | ios::trunc | ios::binary);
    // 序列化address_book对象到string
    std::string message;
    if (!address_book.SerializeToString(&message))
    {
        cerr << "Failed to write address book." << endl;
        return -1;        
    }
    // string内容写入文件
    output << message;


    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}