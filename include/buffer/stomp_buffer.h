#include <memory>
#include <vector>

namespace coolstomp{
    namespace buffer{
        using Byte = unsigned char;
        class StompBuffer{
        public:

        protected:
            StompBuffer() = default;
            StompBuffer(std::vector<Byte>&& vec): raw_buffer_(vec) {};
            std::vector<Byte> raw_buffer_;
        };
    };
};