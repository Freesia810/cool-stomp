#include "buffer/stomp_buffer.h"
namespace coolstomp{
    namespace buffer{
        class StompUtf8Buffer: public StompBuffer{
        public:

        };

        class StompBinaryBuffer: public StompBuffer{
        public:
            StompBinaryBuffer()
        };
    };
};