#ifndef _STOMP_CLIENT_HEADER_
#define _STOMP_CLIENT_HEADER_

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <atomic>
#include <functional>
#include <thread>

#include "macros.h"
#include "type.h"

namespace coolstomp{
    namespace endpoint{
        using no_tls_client = websocketpp::client<websocketpp::config::asio_client>;
        using tls_client = websocketpp::client<websocketpp::config::asio_tls_client>;
        using subscribe_callback = std::function<void(coolstomp::Message)>;

        class COOL_STOMP_PUBLIC StompClient{
        private:
            union {
                no_tls_client* no_tls_client_;
                tls_client* tls_client_;
            };

            union {
                no_tls_client::connection_ptr no_tls_con_;
                tls_client::connection_ptr tls_con_;
            };

            bool enable_tls_;
            std::string uri_;

            std::atomic<bool> is_connected_ {false};
            uint64_t subscribe_id_gen_ {0};
            std::unordered_map<std::string, uint64_t> topic_id_map_ {};
            std::unordered_map<std::string, subscribe_callback> topic_callback_map_ {};

            // lister and dispatcher 
            void on_open_no_tls(no_tls_client* c, websocketpp::connection_hdl hdl);
            void on_open_tls(tls_client* c, websocketpp::connection_hdl hdl);
            void message_dispatcher_no_tls(websocketpp::connection_hdl hdl, no_tls_client::message_ptr msg);
            void message_dispatcher_tls(websocketpp::connection_hdl hdl, tls_client::message_ptr msg);

            // tls context
            std::shared_ptr<asio::ssl::context> on_tls_init();

            // thread
            std::thread* run_thread_;
        public:
            StompClient() = default;
            StompClient(const StompClient&) = delete;
            StompClient& operator=(const StompClient&) = delete;
            virtual ~StompClient();

            void Connect(const char* uri, bool sync = true, bool enable_tls = false);
            void Disconnect();
            void Subscribe(const char* destination, subscribe_callback callback);
            void Unsubscribe(const char* destination);

            void SendEmptyFrame(const char* destination);
            void SendSimpleFrame(const char* destination, const char* msg);
            void SendJsonFrame(const char* destination, const char* msg);
            void SendByteFrame(const char* destination, const char* buffer, uint64_t sz);
            void SendFrame(const char* destination, const char* msg, std::initializer_list<const char*> headers);


            virtual void onWebsocketConnected() {};
            virtual void onStompConnected() {};
            virtual void onStompDisconnected() {};
            virtual void onWebsocketDisconnected() {};
        };
    };
};

#endif