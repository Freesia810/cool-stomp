#include <memory>
#include "frame/stomp_frame_headers.h"
#include "frame/stomp_frame_body.h"
#include "frame/stomp_frame_command.h"

namespace coolstomp{
    namespace frame{
        class StompFrame{
        public:

        private:
            StompFrameCommand command_;
            std::unique_ptr<StompFrameHeaders> headers_;
            std::unique_ptr<StompFrameBody> body_;
        };
    };
};