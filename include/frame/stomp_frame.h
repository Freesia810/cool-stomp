#include <memory>
#include "frame/stomp_frame_headers.h"
#include "frame/stomp_frame_body.h"
#include "frame/stomp_frame_command.h"

namespace coolstomp{
    namespace frame{
        class StompFrame{
        public:
            friend class endpoint::StompClient;
        private:
            StompFrame(const char* raw);
            StompFrameCommand command_;
            std::unique_ptr<StompFrameHeaders> headers_;
            std::unique_ptr<StompFrameBody> body_;

            static std::unordered_map<std::string, StompFrameCommand> cmd_map_;
        };
    };
};