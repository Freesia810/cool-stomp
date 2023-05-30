#include <memory>
#include <vector>

namespace coolstomp{
    namespace buffer{
        using Byte = unsigned char;
        class StompBuffer{
        public:

        private:
            std::vector<Byte> raw_buffer_;
        };
    };
};