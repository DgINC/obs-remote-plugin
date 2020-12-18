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

#include <iostream>
#include <stdexcept>
#include <functional>

#include "obsremote.hpp"
#include "zeroconfdnssd.hpp"
#include "errorhandler.hpp"

namespace OBSRemote {

using namespace OBSRemote::OBSRemoteErrorHandler;
using namespace std;

ZeroconfDNSSD::ZeroconfDNSSD(const char *Name, const char *ServiceType, int Port) : avahi_client_(nullptr), avahi_group_(nullptr), avahi_poll_(nullptr) {
	name_ = avahi_strdup(Name);
	service_type_ = avahi_strdup(ServiceType);
	avahi_poll_ = avahi_threaded_poll_new();
	port_ = Port;
	
	if (avahi_poll_ == nullptr) {
		blog(LOG_ERROR, "Failed to create simple poll object.");
		throw OBSRemoteRuntimeError("Failed to create simple poll object.");
	}
}

ZeroconfDNSSD::~ZeroconfDNSSD() {
	if (avahi_client_ != nullptr) {
		avahi_client_free( avahi_client_);
	}
	if (avahi_poll_ != nullptr) {
		avahi_threaded_poll_stop(avahi_poll_);
		avahi_threaded_poll_free( avahi_poll_);
	}
	avahi_free( name_);
	avahi_free( service_type_);
}

void ZeroconfDNSSD::Run() noexcept(false) {
	avahi_client_ = avahi_client_new(avahi_threaded_poll_get(avahi_poll_), AVAHI_CLIENT_NO_FAIL, avahi_client_callback, NULL, &error);
	error = avahi_threaded_poll_start(avahi_poll_);
	if (error < 0) {
		blog(LOG_ERROR, "Failed to start threaded poll object. %s", avahi_strerror(error));
		throw OBSRemoteErrorHandler::OBSRemoteRuntimeError("Failed to start threaded poll object. " + static_cast<string>(avahi_strerror(error)));
	}
}

void ZeroconfDNSSD::create_services(AvahiClient *c) noexcept {
	avahi_group_ = avahi_entry_group_new(c, avahi_entry_group_callback, NULL);

	if (avahi_group_ == nullptr) {
		blog(LOG_ERROR, "avahi_entry_group_new() failed: %s", avahi_strerror(avahi_client_errno(c)));
	}
	
	if (avahi_entry_group_is_empty(avahi_group_)) {
		error = avahi_entry_group_add_service(
				avahi_group_,
				AVAHI_IF_UNSPEC,
				AVAHI_PROTO_UNSPEC,
				AVAHI_PUBLISH_USE_MULTICAST,
				name_,
				service_type_,
				NULL,
				NULL,
				port_,
				NULL);

		if (error < 0) {
			if (error == AVAHI_ERR_COLLISION) {
				avahi_entry_group_reset(avahi_group_);
				create_services(c);
			} else {
				blog(LOG_ERROR, "Could not add service '%s' (%s) to avahi group (%s)", name_, service_type_, avahi_strerror(error));
				return;
			}
		}
		error = avahi_entry_group_commit(avahi_group_);
		if (error < 0) {
			blog(LOG_ERROR, "Could not commit avahi group '%s' (%s)", name_, avahi_strerror(error));
		}
	}
}

void ZeroconfDNSSD::avahi_client_callback(AvahiClient *c, AvahiClientState state, [[maybe_unused]] void* userdata) noexcept {
	ZeroconfDNSSD *discoveryManager = static_cast<ZeroconfDNSSD*>(userdata);
	discoveryManager->client_callback(c, state);
}

void ZeroconfDNSSD::client_callback(AvahiClient* c, AvahiClientState state) noexcept {
	switch (state) {	//Avahi server state has changed
		case AVAHI_CLIENT_S_RUNNING: {
			create_services(c);
			break;
		}
		case AVAHI_CLIENT_FAILURE: {
			blog(LOG_ERROR, "Client failure: %s", avahi_strerror(avahi_client_errno(c)));
			avahi_threaded_poll_quit(avahi_poll_);
			break;
		}
		case AVAHI_CLIENT_S_COLLISION:
		case AVAHI_CLIENT_S_REGISTERING: {
			if (avahi_group_ != nullptr) {
				avahi_entry_group_reset(avahi_group_);
			}
			break;
		}
		case AVAHI_CLIENT_CONNECTING: {
			break;
		}
	}
}

void ZeroconfDNSSD::avahi_entry_group_callback(AvahiEntryGroup* g, AvahiEntryGroupState state, [[maybe_unused]] void* userdata) noexcept {
	ZeroconfDNSSD *discoveryManager = static_cast<ZeroconfDNSSD*>(userdata);
	discoveryManager->entry_group_callback(g, state);
}

void ZeroconfDNSSD::entry_group_callback(AvahiEntryGroup* g, AvahiEntryGroupState state) noexcept {
	avahi_group_ = g;

	switch (state) {
		case AVAHI_ENTRY_GROUP_ESTABLISHED : {
			blog(LOG_INFO, "Service '%s' successfully established.", name_);
			break;
		}
		case AVAHI_ENTRY_GROUP_COLLISION : {
			char *n;
			n = avahi_alternative_service_name(name_);
			avahi_free(name_);
			name_ = n;

			blog(LOG_INFO, "Service name collision, renaming service to '%s'", name_);
			create_services(avahi_entry_group_get_client(g));
			break;
		}
		case AVAHI_ENTRY_GROUP_FAILURE : {
			blog(LOG_ERROR, "Entry group failure: %s", avahi_strerror(avahi_client_errno(avahi_entry_group_get_client(g))));
			avahi_threaded_poll_quit(avahi_poll_);
			break;
		}
		case AVAHI_ENTRY_GROUP_UNCOMMITED:
		case AVAHI_ENTRY_GROUP_REGISTERING: {
			break;
		}
	}
}

} /* namespace OBSRemote */
