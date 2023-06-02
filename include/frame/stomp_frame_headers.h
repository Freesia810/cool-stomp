#ifndef __STOMP_FRAME_HEADER__
#define __STOMP_FRAME_HEADER__

#include <vector>
#include <string>
#include "buffer/stomp_buffer.h"

namespace coolstomp{
    namespace frame{
        class StompFrameHeaders{
        public:
            friend class StompFrame;
        private:
            StompFrameHeaders() = default;
            std::vector<std::pair<std::string, std::string>> header_kvs_;
        };
    };
};

#endif