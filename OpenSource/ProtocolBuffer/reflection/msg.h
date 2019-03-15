
#ifndef _MSG_H_
#define _MSG_H_

#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>

class CMsgProtobuf : public CMsgBase
{
public:
	CMsgProtobuf();
	virtual ~CMsgProtobuf();

public:
	google::protobuf::Message* GetInstance();

protected:
	int Parse(uint8* pBuf, uint32& dwSize);
	int Pack(uint8* pBuf, uint32& dwSize);

private:
	google::protobuf::Message* m_pInstance;

	DECLARE_CLASS_CREATE(CMsgProtobuf)
};

#endif
