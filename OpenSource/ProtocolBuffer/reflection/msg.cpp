
#include "msg"

// 反射机制提供的接口
// descriptor.h
// class DescriptorPool
// Get a pointer to the generated pool.  Generated protocol message classes
// which are compiled into the binary will allocate their descriptors in
// this pool.  Do not add your own descriptors to this pool.
static const DescriptorPool* generated_pool();
// Looking up descriptors ------------------------------------------
// These find descriptors by fully-qualified name.  These will find both
// top-level descriptors and nested descriptors.  They return NULL if not
// found.
const Descriptor* FindMessageTypeByName(const string& name) const;

// message.h
// Abstract interface for a factory for message objects.
// class MessageFactory
// Gets a MessageFactory which supports all generated, compiled-in messages.
// In other words, for any compiled-in type FooMessage, the following is true:
//   MessageFactory::generated_factory()->GetPrototype(
//     FooMessage::descriptor()) == FooMessage::default_instance()
// This factory supports all types which are found in
// DescriptorPool::generated_pool().  If given a descriptor from any other
// pool, GetPrototype() will return NULL.  (You can also check if a
// descriptor is for a generated message by checking if
// descriptor->file()->pool() == DescriptorPool::generated_pool().)
//
// This factory is 100% thread-safe; calling GetPrototype() does not modify
// any shared data.
//
// This factory is a singleton.  The caller must not delete the object.
static MessageFactory* generated_factory();
// Given a Descriptor, gets or constructs the default (prototype) Message
// of that type.  You can then call that message's New() method to construct
// a mutable message of that type.
//
// Calling this method twice with the same Descriptor returns the same
// object.  The returned object remains property of the factory.  Also, any
// objects created by calling the prototype's New() method share some data
// with the prototype, so these must be destoyed before the MessageFactory
// is destroyed.
//
// The given descriptor must outlive the returned message, and hence must
// outlive the MessageFactory.
//
// Some implementations do not support all types.  GetPrototype() will
// return NULL if the descriptor passed in is not supported.
//
// This method may or may not be thread-safe depending on the implementation.
// Each implementation should document its own degree thread-safety.
virtual const Message *GetPrototype(const Descriptor *type) = 0;

// message.h
// class Message : public MessageLite
// Construct a new instance of the same type.  Ownership is passed to the
// caller.  (This is also defined in MessageLite, but is defined again here
// for return-type covariance.)
virtual Message *New() const = 0;


// 反射机制应用
google::protobuf::Message* CMsgProtobuf::GetInstance()
{
	if (NULL == m_pInstance)
	{
		const google::protobuf::Descriptor* descriptor = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(Name());
		if (descriptor)
		{
			const google::protobuf::Message* prototype = google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
			if (prototype)
			{
				m_pInstance = prototype->New();     // 原型模式
			}
		}
	}

	return m_pInstance;
}


// 反射机制的初始化
// 向MessageFactory中添加 Descriptor => Message的映射关系
void protobuf_RegisterTypes(const ::std::string &)
{
    protobuf_AssignDescriptorsOnce();
    ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
        Person_descriptor_, &Person::default_instance());
    ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
        Person_PhoneNumber_descriptor_, &Person_PhoneNumber::default_instance());
    ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
        AddressBook_descriptor_, &AddressBook::default_instance());
}

void protobuf_AddDesc_addressbook_2eproto()
{
    static bool already_here = false;
    if (already_here)
        return;
    already_here = true;
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
        "\n\021addressbook.proto\022\010tutorial\"\332\001\n\006Person"
        "\022\014\n\004name\030\001 \002(\t\022\n\n\002id\030\002 \002(\005\022\r\n\005email\030\003 \001("
        "\t\022+\n\005phone\030\004 \003(\0132\034.tutorial.Person.Phone"
        "Number\032M\n\013PhoneNumber\022\016\n\006number\030\001 \002(\t\022.\n"
        "\004type\030\002 \001(\0162\032.tutorial.Person.PhoneType:"
        "\004HOME\"+\n\tPhoneType\022\n\n\006MOBILE\020\000\022\010\n\004HOME\020\001"
        "\022\010\n\004WORK\020\002\"/\n\013AddressBook\022 \n\006person\030\001 \003("
        "\0132\020.tutorial.PersonB)\n\024com.example.tutor"
        "ialB\021AddressBookProtos",
        342);
    ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
        "addressbook.proto", &protobuf_RegisterTypes);
    Person::default_instance_ = new Person();
    Person_PhoneNumber::default_instance_ = new Person_PhoneNumber();
    AddressBook::default_instance_ = new AddressBook();
    Person::default_instance_->InitAsDefaultInstance();
    Person_PhoneNumber::default_instance_->InitAsDefaultInstance();
    AddressBook::default_instance_->InitAsDefaultInstance();
    ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_addressbook_2eproto);
}

// 生成Descriptor
// Person_descriptor_
// AddressBook_descriptor_
// 通过文件建立name => Descriptor的映射关系
void protobuf_AssignDesc_addressbook_2eproto()
{
    protobuf_AddDesc_addressbook_2eproto();
    const ::google::protobuf::FileDescriptor *file =
        ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
            "addressbook.proto");   // 内部TryFindFileInFallbackDatabase建立 name => Descriptor的映射关系
    GOOGLE_CHECK(file != NULL);
    Person_descriptor_ = file->message_type(0);
    static const int Person_offsets_[4] = {
        GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Person, name_),
        GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Person, id_),
        GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Person, email_),
        GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Person, phone_),
    };
    Person_reflection_ =
        ::google::protobuf::internal::GeneratedMessageReflection::NewGeneratedMessageReflection(
            Person_descriptor_,
            Person::default_instance_,
            Person_offsets_,
            GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Person, _has_bits_[0]),
            -1,
            -1,
            sizeof(Person),
            GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Person, _internal_metadata_),
            -1);
    Person_PhoneNumber_descriptor_ = Person_descriptor_->nested_type(0);
    static const int Person_PhoneNumber_offsets_[2] = {
        GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Person_PhoneNumber, number_),
        GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Person_PhoneNumber, type_),
    };
    Person_PhoneNumber_reflection_ =
        ::google::protobuf::internal::GeneratedMessageReflection::NewGeneratedMessageReflection(
            Person_PhoneNumber_descriptor_,
            Person_PhoneNumber::default_instance_,
            Person_PhoneNumber_offsets_,
            GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Person_PhoneNumber, _has_bits_[0]),
            -1,
            -1,
            sizeof(Person_PhoneNumber),
            GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Person_PhoneNumber, _internal_metadata_),
            -1);
    Person_PhoneType_descriptor_ = Person_descriptor_->enum_type(0);
    AddressBook_descriptor_ = file->message_type(1);
    static const int AddressBook_offsets_[1] = {
        GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AddressBook, person_),
    };
    AddressBook_reflection_ =
        ::google::protobuf::internal::GeneratedMessageReflection::NewGeneratedMessageReflection(
            AddressBook_descriptor_,
            AddressBook::default_instance_,
            AddressBook_offsets_,
            GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AddressBook, _has_bits_[0]),
            -1,
            -1,
            sizeof(AddressBook),
            GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AddressBook, _internal_metadata_),
            -1);
}
