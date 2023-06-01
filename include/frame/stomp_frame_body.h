#ifndef __STOMP_FRAME_BODY__
#define __STOMP_FRAME_BODY__

#include "buffer/stomp_buffer.h"

namespace coolstomp{
    namespace frame{
        class StompFrameBody{
        public:

        private:
            buffer::StompBuffer raw_body_;
        };
    };
};

#endif