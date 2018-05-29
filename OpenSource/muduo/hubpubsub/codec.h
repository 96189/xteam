#include <muduo/base/Types.h>
#include <muduo/net/Buffer.h>
#include <boost/noncopyable.hpp>
#include <algorithm>

using namespace muduo;
using namespace muduo::net;

namespace pubsub
{
enum ParseResult
{
    kError,
    kSuccess,
    kContinue,
};

ParseResult parseMessage(Buffer *buf, string &cmd, string &topic, string &content)
{
    ParseResult result = kError;
    const char *crlf = buf->findCRLF();
    if (crlf)
    {
        const char *space = std::find(buf->peek(), crlf, ' ');
        if (space != crlf)
        {
            cmd.assign(buf->peek(), space);
            topic.assign(space + 1, crlf);
            if (cmd == "pub")
            {
                const char *start = crlf + 2;
                crlf = buf->findCRLF(start);
                if (crlf)
                {
                    content.assign(start, crlf);
                    buf->retrieveUntil(crlf + 2);
                    result = kSuccess;
                }
                else
                {
                    result = kContinue;
                }
            }
            else
            {
                buf->retrieveUntil(crlf + 2);
                result = kSuccess;
            }
        }
        else
        {
            result = kError;
        }
    }
    else
    {
        result = kContinue;
    }
    return result;
}
}; // namespace pubsub