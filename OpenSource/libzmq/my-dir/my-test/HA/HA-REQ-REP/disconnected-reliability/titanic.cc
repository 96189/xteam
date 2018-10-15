
//

#include "titanic.hpp"
#include <thread>
#include <functional>

int main(int argc, char const *argv[])
{
    int verbose = (argc > 1 && streq(argv[1], "-v"));

    zactor_t *request_pipe = zactor_new(Titanic::Request, NULL);
    assert(request_pipe);
    
    Titanic reply;
    std::thread reply_thread(std::bind(&Titanic::Reply, &reply));
    reply_thread.detach();

    Titanic close;
    std::thread close_thread(std::bind(&Titanic::Close, &close));
    close_thread.detach();

    Titanic::Loop(request_pipe, verbose);

    zactor_destroy(&request_pipe);
    return 0;
}
