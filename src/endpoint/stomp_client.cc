#include "endpoint/stomp_client.h"

void coolstomp::endpoint::StompClient::Connect(const char* uri, bool sync, bool enable_tls){
	this->enable_tls_ = enable_tls;
    this->uri_ = uri;
    if(this->enable_tls_){
        // tls init
		tls_client_ = new tls_client();

		tls_client_->clear_access_channels(websocketpp::log::alevel::all);
		tls_client_->clear_error_channels(websocketpp::log::elevel::all);
		tls_client_->init_asio();
		tls_client_->set_tls_init_handler(websocketpp::lib::bind(&StompClient::on_tls_init, this));

		//connect
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

    auto tmp = uri_.substr(5);
	auto host = tmp.substr(0, tmp.find_first_of('/'));

	// send connect frame
	no_tls_con_->send(nullptr, 0, websocketpp::frame::opcode::TEXT);
}

void coolstomp::endpoint::StompClient::on_open_tls(tls_client* c, websocketpp::connection_hdl hdl){
    //websocket connected
    this->onWebsocketConnected();

    auto tmp = uri_.substr(5);
	auto host = tmp.substr(0, tmp.find_first_of('/'));

	// send connect frame
	tls_con_->send(nullptr, 0, websocketpp::frame::opcode::TEXT);
}

void coolstomp::endpoint::StompClient::Disconnect(){
    // send disconnect frame
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