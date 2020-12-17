/********************************************************************************
 * Copyright (C) 2020 Leonid Kuzin
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *********************************************************************************/

#ifndef SRC_ZEROCONFDNSSD_HPP_
#define SRC_ZEROCONFDNSSD_HPP_

#ifdef __cplusplus
extern "C" {
#endif

#include <avahi-client/client.h>
#include <avahi-client/publish.h>

#include <avahi-common/alternative.h>
#include <avahi-common/thread-watch.h>
#include <avahi-common/malloc.h>
#include <avahi-common/error.h>
#include <avahi-common/timeval.h>

#ifdef __cplusplus
}
#endif

namespace OBSRemote {

class ZeroconfDNSSD {
    
private:
	AvahiClient *avahi_client_;
	AvahiEntryGroup *avahi_group_;
	AvahiThreadedPoll *avahi_poll_;
	char *name_;
	char *service_type_;
	int port_;
	
	int error;
    
	void create_services(AvahiClient *c) noexcept;
	static void avahi_entry_group_callback(AvahiEntryGroup *g, AvahiEntryGroupState state, AVAHI_GCC_UNUSED void *userdata) noexcept;
	void entry_group_callback(AvahiEntryGroup *g, AvahiEntryGroupState state) noexcept;
	static void avahi_client_callback(AvahiClient *c, AvahiClientState state, AVAHI_GCC_UNUSED void * userdata) noexcept;
	void client_callback(AvahiClient *c, AvahiClientState state) noexcept;
    
public:
	ZeroconfDNSSD(const char *Name, const char *ServiceType, int Port) ;
	virtual ~ZeroconfDNSSD();
	void Run();
};

} /* namespace OBSRemote */

#endif /* SRC_ZEROCONFDNSSD_HPP_ */
