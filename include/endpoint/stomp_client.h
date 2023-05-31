#ifndef _STOMP_CLIENT_HEADER_
#define _STOMP_CLIENT_HEADER_

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>

namespace coolstomp{
    namespace endpoint{
        using no_tls_client = websocketpp::client<websocketpp::config::asio_client>;
        using tls_client = websocketpp::client<websocketpp::config::asio_tls_client>;

        class StompClient{
        private:
            union {
                no_tls_client no_tls_client_;
                tls_client tls_client_;
            } ws_client_;
            bool enableTls{false};
        }
    };
}

#endif