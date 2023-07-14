#ifndef FRAME_WRAPPER_HEADER
#define FRAME_WRAPPER_HEADER

#include "type.h"
#include <cstring>
#include <unordered_map>
#include <string>

#include "endpoint/stomp_client.h"

namespace coolstomp{
    namespace utils{
        class FrameWrapper{
        public:
            friend class coolstomp::endpoint::StompClient;
        private:
            inline static std::unordered_map<std::string, StompFrameCommand> cmd_map_ = {
                { "MESSAGE", StompFrameCommand::MESSAGE },
                { "ERROR", StompFrameCommand::ERROR },
                { "CONNECTED", StompFrameCommand::CONNECTED },
                { "RECEIPT", StompFrameCommand::RECEIPT },
                { "CONNECT", StompFrameCommand::CONNECT },
                { "DISCONNECT", StompFrameCommand::DISCONNECT },
                { "SEND", StompFrameCommand::SEND },
                { "STOMP", StompFrameCommand::STOMP },
                { "SUBSCRIBE", StompFrameCommand::SUBSCRIBE },
                { "UNSUBSCRIBE", StompFrameCommand::UNSUBSCRIBE },
                { "ACK", StompFrameCommand::ACK },
                { "NACK", StompFrameCommand::NACK },
                { "BEGIN", StompFrameCommand::BEGIN },
                { "COMMIT", StompFrameCommand::COMMIT },
                { "ABORT", StompFrameCommand::ABORT }
            };
            inline static char* lst[15] = {
                "SEND", "SUBSCRIBE", "UNSUBSCRIBE", "BEGIN",
                "COMMIT", "ABORT", "ACK", "NACK", "DISCONNECT", "CONNECT",
                "STOMP", "CONNECTED", "MESSAGE", "RECEIPT","ERROR"
            };

            FrameWrapper() { };
            ~FrameWrapper() { };
            FrameWrapper(const FrameWrapper&) = delete;
            FrameWrapper& operator=(const FrameWrapper&) = delete;

            static FrameWrapper& getInstance() {
                static FrameWrapper instance;
                return instance;
            }

            static void WrapFrame(StompFrameCommand type, const char* headers, const char* body, char*& buf, size_t& len){

            };

            static void UnwrapFrame(){

            };
        };
    };
};

#endif