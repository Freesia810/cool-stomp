#include "frame/stomp_frame.h"

#include <sstream>
#include <unordered_map>
#include <cassert>

std::unordered_map<std::string, coolstomp::frame::StompFrameCommand> coolstomp::frame::StompFrame::cmd_map_ = {
    { "MESSAGE", StompFrameCommand::MESSAGE },
    { "ERROR", StompFrameCommand::ERROR },
    { "CONNECTED", StompFrameCommand::CONNECTED },
    { "RECEIPT", StompFrameCommand::RECEIPT },
    { "CONNECT", StompFrameCommand::CONNECT },
    { "DISCONNECT", StompFrameCommand::DISCONNECT },
    { "SEND", StompFrameCommand::SEND },
    { "STOMP", StompFrameCommand::STOMP },
    { "SUBSCRIBE", StompFrameCommand::SUBSCRIBE },
    { "UNSUBSCRIBE", StompFrameCommand::UNSUBSCRIBE },
    { "ACK", StompFrameCommand::ACK },
    { "NACK", StompFrameCommand::NACK },
    { "BEGIN", StompFrameCommand::BEGIN },
    { "COMMIT", StompFrameCommand::COMMIT },
    { "ABORT", StompFrameCommand::ABORT }
};

coolstomp::frame::StompFrame::StompFrame(const char* raw){
    //assert header code is ascii

    std::stringstream ss;
    ss << raw;

    std::string command;
	std::getline(ss, command);

    auto iter = cmd_map_.find(command);
    if(iter == cmd_map_.end())
        assert(false);
    this->command_ = iter->second;
    this->headers_ = std::make_unique<StompFrameHeaders>();
    while (true) {
		std::string line;
		std::getline(ss, line);

		if (line == "") {
			break;
		}

		auto tmp = line.find(':');
		this->headers_->header_kvs_.emplace_back(line.substr(0, tmp), line.substr(tmp + 1));
	}

}