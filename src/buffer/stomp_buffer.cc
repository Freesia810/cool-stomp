#include "buffer/stomp_buffer.h"
#include <string_view>

namespace coolstomp{
    namespace buffer{
        class StompTextBuffer: public StompBuffer{
        public:
        private:
            std::string_view text_;
        };

        class StompBinaryBuffer: public StompBuffer{
        public:
            StompBinaryBuffer(const Byte* loc, uint64_t sz): 
                StompBuffer(std::vector<Byte>(loc, loc + sz)) {};
            StompBinaryBuffer(const char* loc, uint64_t sz): 
                StompBuffer(std::vector<Byte>(loc, loc + sz)) {};
            StompBinaryBuffer(std::vector<Byte>&& vec): 
                StompBuffer(std::move(vec)) {};
            StompBinaryBuffer(std::vector<char>&& vec): 
                StompBuffer(std::vector<Byte>(vec.begin(), vec.end())) {};
        };
    };
};