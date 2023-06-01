#ifndef __STOMP_FRAME_HEADER__
#define __STOMP_FRAME_HEADER__

#include <vector>
#include "buffer/stomp_buffer.h"

namespace coolstomp{
    namespace frame{
        class StompFrameHeaders{
        public:

        private:
            std::vector<std::pair<buffer::StompBuffer, buffer::StompBuffer>> header_kvs_;
        };
    };
};

#endif