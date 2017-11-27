//
// Created by piotr on 27.11.17.
//

#ifndef SHEPHERD_ACCEPTOR_H
#define SHEPHERD_ACCEPTOR_H


#include <boost/asio/generic/stream_protocol.hpp>
#include <boost/function.hpp>

class Acceptor {
public:
    typedef boost::function<void(const boost::system::error_code &error)> AsyncAcceptor;
    virtual void accept(boost::asio::generic::stream_protocol::socket &socket, AsyncAcceptor function) = 0;
};


#endif //SHEPHERD_ACCEPTOR_H
