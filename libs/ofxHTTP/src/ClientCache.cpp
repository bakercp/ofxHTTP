//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/ClientCache.h"


namespace ofx {
namespace HTTP {



SQLiteClientCacheStorage::~SQLiteClientCacheStorage()
{
}


void SQLiteClientCacheStorage::put(const std::string& key, ClientCacheEntry value)
{
 // noop
}


ClientCacheEntry SQLiteClientCacheStorage::get(const std::string& key)
{
    return ClientCacheEntry();
}


void SQLiteClientCacheStorage::remove(const std::string& key)
{
    // noop
}


void SQLiteClientCacheStorage::update(const std::string& key)
{
    // noop
}


} } // namespace ofx::HTTP
