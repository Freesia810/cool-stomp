namespace coolstomp{
    namespace frame{
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
};