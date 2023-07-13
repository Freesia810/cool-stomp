#ifndef _TYPE_HEADER_
#define _TYPE_HEADER_

#include <stdint.h>

namespace coolstomp{
    struct Message {
        char** headers;
        uint64_t header_sz;
        char* buffer;
    };

    enum class StompFrameCommand{
        //client command
        SEND,
        SUBSCRIBE,
        UNSUBSCRIBE,
        BEGIN,
        COMMIT,
        ABORT,
        ACK,
        NACK,
        DISCONNECT,
        CONNECT,
        STOMP,

        //server command
        CONNECTED,
        MESSAGE,
        RECEIPT,
        ERROR
    };
};

#endif