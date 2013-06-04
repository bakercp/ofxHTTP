#include "WebSocketFrame.h"


namespace ofx {
namespace HTTP {


//------------------------------------------------------------------------------
ofxWebSocketFrame::ofxWebSocketFrame(const ofBuffer& _buffer, int _flags)
: flags(_flags) { set(_buffer); }

//------------------------------------------------------------------------------
ofxWebSocketFrame::ofxWebSocketFrame(const string& _text, int _flags)
: flags(_flags) { set(_text); }

//------------------------------------------------------------------------------
ofxWebSocketFrame::ofxWebSocketFrame(const unsigned char* _buffer, unsigned int _size, int _flags)
: flags(_flags) { set(reinterpret_cast<const char*>(_buffer),_size); }

//------------------------------------------------------------------------------
ofxWebSocketFrame::ofxWebSocketFrame(const char* _buffer, unsigned int _size, int _flags)
: flags(_flags) { set(_buffer,_size); }

//------------------------------------------------------------------------------
ofxWebSocketFrame::~ofxWebSocketFrame() { }

//------------------------------------------------------------------------------
int ofxWebSocketFrame::getFlags() const { return flags; }

//------------------------------------------------------------------------------
bool ofxWebSocketFrame::isContinuation() const {
    return (flags & Poco::Net::WebSocket::FRAME_OP_BITMASK) == Poco::Net::WebSocket::FRAME_OP_CONT;
}

//------------------------------------------------------------------------------
bool ofxWebSocketFrame::isText() const {
    return (flags & Poco::Net::WebSocket::FRAME_OP_BITMASK) == Poco::Net::WebSocket::FRAME_OP_TEXT;
}

//------------------------------------------------------------------------------
bool ofxWebSocketFrame::isBinary() const {
    return (flags & Poco::Net::WebSocket::FRAME_OP_BITMASK) == Poco::Net::WebSocket::FRAME_OP_BINARY;
}

//------------------------------------------------------------------------------
bool ofxWebSocketFrame::isClose() const {
    return (flags & Poco::Net::WebSocket::FRAME_OP_BITMASK) == Poco::Net::WebSocket::FRAME_OP_CLOSE;
}

//------------------------------------------------------------------------------
bool ofxWebSocketFrame::isPing() const {
    return (flags & Poco::Net::WebSocket::FRAME_OP_BITMASK) == Poco::Net::WebSocket::FRAME_OP_PING;
}

//------------------------------------------------------------------------------
bool ofxWebSocketFrame::isPong() const {
    return (flags & Poco::Net::WebSocket::FRAME_OP_BITMASK) == Poco::Net::WebSocket::FRAME_OP_PONG;
}

//------------------------------------------------------------------------------
bool ofxWebSocketFrame::isFinal() const {
    return flags & Poco::Net::WebSocket::FRAME_FLAG_FIN;
}

//------------------------------------------------------------------------------
bool ofxWebSocketFrame::isRSV1() const {
    return flags & Poco::Net::WebSocket::FRAME_FLAG_RSV1;
}

//------------------------------------------------------------------------------
bool ofxWebSocketFrame::isRSV2() const {
    return flags & Poco::Net::WebSocket::FRAME_FLAG_RSV2;
}

//------------------------------------------------------------------------------
bool ofxWebSocketFrame::isRSV3() const {
    return flags & Poco::Net::WebSocket::FRAME_FLAG_RSV3;
}

//------------------------------------------------------------------------------
string ofxWebSocketFrame::toString() const {
    stringstream ss;
    ss << "Flags:";
    ss << " CONT="    << (isContinuation() ? "Y" : "N");
    ss << ", TXT="    << (isText()         ? "Y" : "N");
    ss << ", BIN="    << (isBinary()       ? "Y" : "N");
    ss << ", CLOSE="  << (isClose()        ? "Y" : "N");
    ss << ", PING="   << (isPing()         ? "Y" : "N");
    ss << ", PONG="   << (isPong()         ? "Y" : "N");
    ss << ", FINAL="  << (isFinal()        ? "Y" : "N");
    ss << ", RSV1="   << (isRSV1()         ? "Y" : "N");
    ss << ", RSV2="   << (isRSV2()         ? "Y" : "N");
    ss << ", RSV3="   << (isRSV3()         ? "Y" : "N");
    ss << ", nBytes=" << size();
    ss << ", bytes="  << getText();
    
    return ss.str();
}

    
} }
