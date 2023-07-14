#include "endpoint/stomp_client.h"
#include "utils/frame_wrapper.hpp"
#include "type.h"
#include <cstring>

void coolstomp::endpoint::StompClient::Connect(const char* uri, bool sync, bool enable_tls){
	this->enable_tls_ = enable_tls;
    this->uri_ = uri;

	// check uri matching enable_tls config?


    if(this->enable_tls_){
        // tls init
		tls_client_ = new tls_client();

		tls_client_->clear_access_channels(websocketpp::log::alevel::all);
		tls_client_->clear_error_channels(websocketpp::log::elevel::all);
		tls_client_->init_asio();
		tls_client_->set_tls_init_handler(websocketpp::lib::bind(&StompClient::on_tls_init, this));

		// connect
        websocketpp::lib::error_code ec{};
        tls_con_ = tls_client_->get_connection(uri, ec);
		if (ec) {
            // todo
			assert(false);
		}

		// connect websocket
		tls_con_->set_open_handler(websocketpp::lib::bind(&StompClient::on_open_tls, this, tls_client_, websocketpp::lib::placeholders::_1));
		tls_con_->set_message_handler(websocketpp::lib::bind(&StompClient::message_dispatcher_tls, this, websocketpp::lib::placeholders::_1, websocketpp::lib::placeholders::_2));
		tls_client_->connect(tls_con_);

        //run
        if(sync){
            tls_client_->run();
        }
        else{
            this->run_thread_ = new std::thread(&tls_client::run, tls_client_);
        }
	}
	else{
		// no_tls init
		no_tls_client_ = new no_tls_client();

		no_tls_client_->clear_access_channels(websocketpp::log::alevel::all);
		no_tls_client_->clear_error_channels(websocketpp::log::elevel::all);
		no_tls_client_->init_asio();

        //connect
        websocketpp::lib::error_code ec{};
        no_tls_con_ = no_tls_client_->get_connection(uri, ec);
		if (ec) {
            // todo
			assert(false);
		}

		// connect websocket
		no_tls_con_->set_open_handler(websocketpp::lib::bind(&StompClient::on_open_no_tls, this, no_tls_client_, websocketpp::lib::placeholders::_1));
		no_tls_con_->set_message_handler(websocketpp::lib::bind(&StompClient::message_dispatcher_no_tls, this, websocketpp::lib::placeholders::_1, websocketpp::lib::placeholders::_2));
		no_tls_client_->connect(no_tls_con_);

        //run
        if(sync){
            no_tls_client_->run();
        }
        else{
            this->run_thread_ = new std::thread(&no_tls_client::run, no_tls_client_);
        }
	}
}

std::shared_ptr<asio::ssl::context> coolstomp::endpoint::StompClient::on_tls_init(){
    // establishes a SSL connection
    std::shared_ptr<asio::ssl::context> ctx = std::make_shared<asio::ssl::context>(asio::ssl::context::sslv23);

    try {
        ctx->set_options(asio::ssl::context::default_workarounds |
                         asio::ssl::context::no_sslv2 |
                         asio::ssl::context::no_sslv3 |
                         asio::ssl::context::single_dh_use);
    } catch (std::exception &e) {
        std::cout << "Error in context pointer: " << e.what() << std::endl;
    }
    return ctx;
}

coolstomp::endpoint::StompClient::~StompClient(){
    websocketpp::lib::error_code ec{};
    if(enable_tls_){
		if (is_connected_.load()) {
			tls_client_->stop_perpetual();
			tls_client_->close(tls_con_->get_handle(), websocketpp::close::status::going_away, "", ec);
		}
	}
	else{
		if (is_connected_.load()) {
			no_tls_client_->stop_perpetual();
			no_tls_client_->close(no_tls_con_->get_handle(), websocketpp::close::status::going_away, "", ec);
		}
	}

    if (run_thread_ != nullptr) {
		run_thread_->join();
		delete run_thread_;
	}

	if(enable_tls_){
		delete tls_client_;
	}
	else{
		delete no_tls_client_;
	}
}

void coolstomp::endpoint::StompClient::on_open_no_tls(no_tls_client* c, websocketpp::connection_hdl hdl){
    //websocket connected
    this->onWebsocketConnected();

    auto tmp = uri_.substr(5); //ws://
	auto host = tmp.substr(0, tmp.find_first_of('/'));

	// send connect frame
	no_tls_con_->send(nullptr, 0, websocketpp::frame::opcode::TEXT);
}

void coolstomp::endpoint::StompClient::on_open_tls(tls_client* c, websocketpp::connection_hdl hdl){
    //websocket connected
    this->onWebsocketConnected();

    auto tmp = uri_.substr(6); //wss://
	auto host = tmp.substr(0, tmp.find_first_of('/'));

	// send connect frame
	tls_con_->send(nullptr, 0, websocketpp::frame::opcode::TEXT);
}

void coolstomp::endpoint::StompClient::Disconnect(){
    // send disconnect frame
	char* buf = nullptr;
	size_t len = 0;

	coolstomp::utils::FrameWrapper::getInstance().WrapFrame(StompFrameCommand::DISCONNECT, "receipt:disconnect", nullptr, buf, len);
	if(this->enable_tls_){
		tls_con_->send(nullptr, 0, websocketpp::frame::opcode::TEXT);
	}
	else{
		no_tls_con_->send(nullptr, 0, websocketpp::frame::opcode::TEXT);
	}

	while (true) {
		if (!is_connected_.load()) {
			if(this->enable_tls_){
				tls_con_->close(websocketpp::close::status::normal, "close");
			}
			else{
				no_tls_con_->close(websocketpp::close::status::normal, "close");
			}
            this->onWebsocketDisconnected();
			break;
		}
	}
}

void coolstomp::endpoint::StompClient::Subscribe(const char* destination, subscribe_callback callback){
	auto it = this->topic_id_map_.find(destination);
	if (it != this->topic_id_map_.end()) return;

	// send subscribe frame
	if(this->enable_tls_){
		this->tls_con_->send(nullptr, 0, websocketpp::frame::opcode::TEXT);
	}
	else{
		this->no_tls_con_->send(nullptr, 0, websocketpp::frame::opcode::TEXT);
	}

	this->topic_id_map_.insert(std::make_pair(destination, subscribe_id_gen_++));
	this->topic_callback_map_.insert(std::make_pair(destination, callback));
}

void coolstomp::endpoint::StompClient::Unsubscribe(const char* destination){
	auto it = this->topic_id_map_.find(destination);
	if (it == this->topic_id_map_.end()) return;

	// send unsubscribe
	char* buf = nullptr;
	size_t len = 0;

	std::string header = "id:";
	header = header + std::to_string(it->second);

	coolstomp::utils::FrameWrapper::getInstance().WrapFrame(StompFrameCommand::UNSUBSCRIBE, header.c_str(), nullptr, buf, len);
	if(this->enable_tls_){
		this->tls_con_->send(buf, len, websocketpp::frame::opcode::TEXT);
	}
	else{
		this->no_tls_con_->send(buf, len, websocketpp::frame::opcode::TEXT);
	}
	delete buf;

	this->topic_id_map_.erase(destination);
	this->topic_callback_map_.erase(destination);
}

void coolstomp::endpoint::StompClient::message_dispatcher_no_tls(websocketpp::connection_hdl hdl, no_tls_client::message_ptr msg){

}

void coolstomp::endpoint::StompClient::SendEmptyFrame(const char* destination){
	SendFrame(destination, nullptr, {});
}

void coolstomp::endpoint::StompClient::SendSimpleFrame(const char* destination, const char* msg){
	std::string tmp = "content-length:";
	tmp = tmp + std::to_string(strlen(msg));
	SendFrame(destination, msg, {"content-type:text/plain", tmp.c_str()});
}

void coolstomp::endpoint::StompClient::SendJsonFrame(const char* destination, const char* msg){
	std::string tmp = "content-length:";
	tmp = tmp + std::to_string(strlen(msg));
	SendFrame(destination, msg, {"content-type:application/json", tmp.c_str()});
}

void coolstomp::endpoint::StompClient::SendByteFrame(const char* destination, const char* buffer, uint64_t sz){
	SendFrame(destination, buffer, {"content-type:application/octet-stream", std::to_string(sz).c_str()});
}

void coolstomp::endpoint::StompClient::SendFrame(const char* destination, const char* msg, std::initializer_list<const char*> headers){
	char* buf = nullptr;
	size_t len = 0;

	std::string header = "destination:";
	header += destination;
	for(auto str: headers){
		header += str;
	}

	coolstomp::utils::FrameWrapper::getInstance().WrapFrame(StompFrameCommand::SEND, header.c_str(), msg, buf, len);
	if(enable_ssl){
		tls_con_->send(buf, len, websocketpp::frame::opcode::TEXT);
	}
	else{
		no_tls_con_->send(buf, len, websocketpp::frame::opcode::TEXT);
	}

	delete buf;
}