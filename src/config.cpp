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
	Config::Config() : server_enable_(true), dns_sd_enable_(true), port_(585) {
		if(!os_file_exists(obs_module_config_path("config.json"))) {
			BPtr<char> modulePath = obs_module_config_path("");
			os_mkdirs(modulePath);
			settings = obs_data_create();
			this->Save();
		}
		this->Load();
	}

	Config::~Config(){}

	void Config::Load() {
		BPtr<char> config_path_ = obs_module_config_path("config.json");
		settings = obs_data_create_from_json_file(config_path_);
		server_enable_ = obs_data_get_bool(settings, "ServerEnable");
		dns_sd_enable_ = obs_data_get_bool(settings, "DNSSDEnable");
		port_ = obs_data_get_int(settings, "ServerRPCPort");
	}

	void Config::Save() {
		obs_data_set_bool(settings, "ServerEnable", server_enable_);
		obs_data_set_bool(settings, "DNSSDEnable", dns_sd_enable_);
		obs_data_set_int(settings, "ServerRPCPort", port_);
		BPtr<char> config_path_ = obs_module_config_path("config.json");
		obs_data_save_json_safe(settings, config_path_, "tmp", "bak");
	}
}
