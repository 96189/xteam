
// 

#ifndef _TATANIC_HPP_
#define _TATANIC_HPP_

#include <uuid/uuid.h>

#include "mdwrkapi.hpp"
#include "mdcliapi.hpp"
#include "zfile.h"      // czmq/include/zfile.h

#define BUFF_SIZE 256
#define TITANIC_DIR ".titanic"

#define BROKER_ADDR "tcp://localhost:5555"
#define SERVICE_REQUEST "titanic.request"
#define SERVICE_REPLY "titanic.reply"
#define SERVICE_CLOSE "titanic.close"

namespace Helper
{

static char *GenerateUuid()
{
    char hex_char [] = "0123456789ABCDEF";
    char *uuidstr = (char*)zmalloc(sizeof(uuid_t) * 2 + 1);
    uuid_t uuid;
    uuid_generate(uuid);
    int byte_nbr;
    for (byte_nbr = 0; byte_nbr < sizeof(uuid_t); byte_nbr++) {
        uuidstr [byte_nbr * 2 + 0] = hex_char [uuid [byte_nbr] >> 4];
        uuidstr [byte_nbr * 2 + 1] = hex_char [uuid [byte_nbr] & 15];
    }
    return uuidstr;
}

static char *RequestFilename(char *uuid)
{
    char *filename = (char*)malloc(BUFF_SIZE);
    snprintf(filename, BUFF_SIZE, TITANIC_DIR "/%s.req", uuid);
    return filename;
}

static char *ReplyFilename(char *uuid)
{
    char *filename = (char*)malloc(BUFF_SIZE);
    snprintf(filename, BUFF_SIZE, TITANIC_DIR "/%s.rep", uuid);
    return filename;
}

};

class Titanic 
{
public:
    Titanic()
    {

    }
    ~Titanic()
    {

    }
public:
    static void Request(zsock_t *pipe, void *args)
    {   
        zsock_signal(pipe, 0);
        
        MdWork *worker = new MdWork(BROKER_ADDR, SERVICE_REQUEST, 0);
        worker->Connect2Broker();

        zmsg_t *reply = NULL;
        while (1)
        {
            zmsg_t *request = worker->Recv(&reply);
            if (!request) break;

            zfile_mkdir(TITANIC_DIR);

            char *uuid = Helper::GenerateUuid();
            char *filename = Helper::RequestFilename(uuid);
            FILE *file = fopen(filename, "w");
            assert(file);
            zmsg_save(request, file);
            fclose(file);
            zmsg_destroy(&request);

            reply = zmsg_new();
            zmsg_addstr(reply, uuid);
            zmsg_send(&reply, pipe);

            reply = zmsg_new();
            zmsg_addstr(reply, "200");
            zmsg_addstr(reply, uuid);
            free(uuid);
        }

        delete worker;
    }
    static void Reply(void *args)
    {
        MdWork *worker = new MdWork(BROKER_ADDR, SERVICE_REPLY, 0);
        worker->Connect2Broker();

        zmsg_t *reply = NULL;
        while (1)
        {
            zmsg_t *request = worker->Recv(&reply);
            if (!request) break;

            char *uuid = zmsg_popstr(request);
            char *req_filename = Helper::RequestFilename(uuid);
            char *rep_filename = Helper::ReplyFilename(uuid);
            if (zfile_exists(rep_filename))
            {
                FILE *file = fopen(rep_filename, "r");
                assert(file);
                reply = zmsg_load(file);
                zmsg_pushstr(reply, "200");
                fclose(file);
            }
            else 
            {
                reply = zmsg_new();
                if (zfile_exists(req_filename))
                {
                    zmsg_pushstr(reply, "300"); // pending
                }
                else 
                {
                    zmsg_pushstr(reply, "400"); // unknown
                }
            }
            zmsg_destroy(&request);
            free(uuid);
            free(req_filename);
            free(rep_filename);
        }

        delete worker;
    }
    static void Close(void *args)
    {
        MdWork *worker = new MdWork(BROKER_ADDR, SERVICE_CLOSE, 0);
        worker->Connect2Broker();

        zmsg_t *reply = NULL;
        while (1)
        {
            zmsg_t *request = worker->Recv(&reply);
            if (!request) break;

            char *uuid = zmsg_popstr(request);
            char *req_filename = Helper::RequestFilename(uuid);
            char *rep_filename = Helper::ReplyFilename(uuid);
            zfile_delete(req_filename);
            zfile_delete(rep_filename);
            free(uuid);
            free(req_filename);
            free(rep_filename);

            zmsg_destroy(&request);
            reply = zmsg_new();
            zmsg_addstr(reply, "200");
        }

        delete worker;
    }
    static int ServiceSuccess(char *uuid)
    {
        char *filename = Helper::RequestFilename(uuid);
        FILE *file = fopen(filename, "r");
        free(filename);

        if (!file) return 1;

        zmsg_t *request = zmsg_load(file);
        fclose(file);

        zframe_t *service = zmsg_pop(request);
        char *service_name = zframe_strdup(service);

        MdCli *client = new MdCli(BROKER_ADDR, 0);
        client->SetTimeOut(1000);
        client->SetRetries(1);

        zmsg_t *mmi_request = zmsg_new();
        zmsg_add(mmi_request, service);
        zmsg_t *mmi_reply = client->Send("mmi.service", &mmi_request);
        int service_ok = (mmi_reply && zframe_streq(zmsg_first(mmi_reply), "200"));
        zmsg_destroy(&mmi_request);

        int result = 0;
        if (service_ok)
        {
            zmsg_t *reply = client->Send(service_name, &request);
            if (reply)
            {
                filename = Helper::ReplyFilename(uuid);
                FILE *file = fopen(filename, "w");
                assert(file);
                zmsg_save(reply, file);
                fclose(file);
                free(filename);
                result = 1;
            }
            zmsg_destroy(&reply);
        }
        else 
        {
            zmsg_destroy(&request);
        }
        delete client;
        free(service_name);
        return result;
    }
};

#endif