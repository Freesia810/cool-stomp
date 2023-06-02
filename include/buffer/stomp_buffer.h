#ifndef __STOMP_BUFFER__
#define __STOMP_BUFFER__

#include <memory>
#include <vector>

namespace coolstomp{
    namespace buffer{
        using Byte = unsigned char;
        class StompBuffer{
        public:

        protected:
            StompBuffer() = default;
            Byte* raw_buffer_;
        };
    };
};

#endif