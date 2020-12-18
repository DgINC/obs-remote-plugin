/*
 * Copyright (c) 2020 <copyright holder> <email>
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "config.hpp"
namespace OBSRemote {
Config::Config() : server_enable_(false), dns_sd_enable_(false) {
	const char* path = obs_module_file("config.json");
	buf = obs_data_create_from_json_file(path);
}

Config::~Config(){
}

void Config::LoadConfig() {
	server_enable_ = obs_data_get_bool(buf, "ServerEnable");
	dns_sd_enable_ = obs_data_get_bool(buf, "DNSSDEnable");
	port_ = obs_data_get_int(buf, "ServerRPCPort");
}

void Config::SaveConfig() {
	obs_data_set_bool(buf, "ServerEnable", server_enable_ );
	obs_data_set_bool(buf, "DNSSDEnable", dns_sd_enable_);
	obs_data_set_int(buf, "ServerRPCPort", port_);
	const char* path = obs_module_file("config.json");
	obs_data_save_json(buf, path);
}

void Config::SetDefault() {
	obs_data_set_default_bool(buf, "ServerEnable", false);
	obs_data_set_default_bool(buf, "DNSSDEnable", false);
	obs_data_set_default_int(buf, "ServerRPCPort", 585);
	const char* path = obs_module_file("config.json");
	obs_data_save_json(buf, path);
}


}
