//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/ClientState.h"
#include <map>


namespace ofx {
namespace HTTP {


const std::map<ClientState, std::string> CLIENT_STATE_STRINGS = {
    { ClientState::NONE, "NONE" },
    { ClientState::DETECTING_PROXY, "DETECTING_PROXY" },
    { ClientState::RESOLVING_NAME, "RESOLVING_NAME" },
    { ClientState::CONNECTING_TO_SERVER, "CONNECTING_TO_SERVER" },
    { ClientState::NEGOTIATING_SSL, "NEGOTIATING_SSL" },
    { ClientState::SENDING_HEADERS, "SENDING_HEADERS" },
    { ClientState::SENDING_CONTENTS, "SENDING_CONTENTS" },
    { ClientState::WAITING_FOR_RESPONSE, "WAITING_FOR_RESPONSE" },
    { ClientState::RECEIVING_HEADERS, "RECEIVING_HEADERS" },
    { ClientState::RECEIVING_CONTENTS, "RECEIVING_CONTENTS" },
    { ClientState::REDIRECTING, "REDIRECTING" }
};


std::string to_string(ClientState state)
{
    return CLIENT_STATE_STRINGS.at(state);
}


bool from_string(const std::string text, ClientState& state)
{
    for (const auto& entry: CLIENT_STATE_STRINGS)
    {
        if (text == entry.second)
        {
            state = entry.first;
            return true;
        }
    }
    
    state = ClientState::NONE;
    return false;
}


} } // namespace ofx::HTTP
