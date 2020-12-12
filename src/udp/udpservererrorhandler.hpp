/*
 * udpservererrorhandler.h
 *
 *  Created on: 12 дек. 2020 г.
 *      Author: dg
 */

#ifndef SRC_UDP_UDPSERVERERRORHANDLER_HPP_
#define SRC_UDP_UDPSERVERERRORHANDLER_HPP_

#include <stdexcept>
#include <sys/types.h>
#include <unistd.h>

namespace UDPHandShake {

class udp_runtime_error : public std::runtime_error
{
public:
    udp_runtime_error(const std::string & errmsg) : std::runtime_error(errmsg) {}
};

class udp_parameter_error : public std::logic_error
{
public:
    udp_parameter_error(const std::string & errmsg) : std::logic_error(errmsg) {}
};

} /* namespace UDPHandShake */

#endif /* SRC_UDP_UDPSERVERERRORHANDLER_HPP_ */
