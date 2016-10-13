// =============================================================================
//
// Copyright (c) 2014-2016 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================


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
