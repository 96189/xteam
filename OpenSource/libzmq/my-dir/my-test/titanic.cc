
//

#include "titanic.hpp"
#include <thread>
#include <functional>

int main(int argc, char const *argv[])
{
    int verbose = (argc > 1 && streq(argv[1], "-v"));

    zactor_t *request_pipe = zactor_new(Titanic::Request, NULL);
    assert(request_pipe);
    
    Titanic titanic;
    std::thread reply_thread(std::bind(&Titanic::Reply, &titanic));
    reply_thread.detach();

    std::thread close_thread(std::bind(&Titanic::Close, &titanic));
    close_thread.detach();

    while (1)
    {
        zmq_pollitem_t items[] = 
        {
            { request_pipe, 0, ZMQ_POLLIN, 0 }
        };
        int ret = zmq_poll(items, sizeof(items)/sizeof(items[0]), 1000);
        if (ret == -1) break;

        if (items[0].revents & ZMQ_POLLIN)
        {
            zfile_mkdir(TITANIC_DIR);
            zmsg_t *msg = zmsg_recv(request_pipe);
            if (!msg) break;
            FILE* file = fopen(TITANIC_DIR"/queue", "a");
            char *uuid = zmsg_popstr(msg);
            fprintf(file, "-%s\n", uuid);
            fclose(file);
            free(uuid);
            zmsg_destroy(&msg);
        }

        char entry[] = "?…….:…….:…….:…….:";
        FILE *file = fopen(TITANIC_DIR"/queue", "r+");
        while (file && fread(entry, 33, 1, file) == 1)
        {
            if (entry[0] == '-')
            {
                if (verbose)
                    printf ("I: processing request %s\n", entry + 1);
                if (Titanic::ServiceSuccess(entry + 1))
                {
                    fseek(file, -33, SEEK_CUR);
                    fwrite("+", 1, 1, file);
                    fseek(file, 32, SEEK_CUR);
                }
            }

            if (fgetc(file) == '\r')
            {
                fgetc(file);
            }
            if (zctx_interrupted) break;
        }
        if (file)
        {
            fclose(file);
        }
    }
    return 0;
}
