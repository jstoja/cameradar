// Copyright 2016 Etix Labs
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <string>        // std::string
#include <utility>       // std::pair
#include <logger.h>      // _LOG_
#include <json/value.h>  // Json::Value
#include <json/reader.h> // Json::Value

namespace etix {

namespace cameradar {

static const std::string default_configuration_path = "conf/cameradar.conf.json";
static const std::string default_ids_file_path_ = "conf/ids.json";
static const std::string default_urls_file_path_ = "conf/url.json";

struct configuration {
    std::string thumbnail_storage_path;
    std::string subnets;
    std::string rtsp_url_file;
    std::string rtsp_ids_file;
    std::string ports;
    std::string cache_manager_path;
    std::string cache_manager_name;
    std::vector<std::string> paths;
    std::vector<std::string> usernames;
    std::vector<std::string> passwords;

    Json::Value raw_conf;

    configuration() = default;
    configuration(const std::string& thumbnail_storage_path,
                  const std::string& subnets,
                  const std::string& rtsp_url_file,
                  const std::string& rtsp_ids_file,
                  const std::string& cache_manager_path,
                  const std::string& cache_manager_name,
                  const std::string& ports = "1-65535")
    : thumbnail_storage_path(thumbnail_storage_path)
    , subnets(subnets)
    , rtsp_url_file(rtsp_url_file)
    , rtsp_ids_file(rtsp_ids_file)
    , ports(ports)
    , cache_manager_path(cache_manager_path)
    , cache_manager_name(cache_manager_name) {}

    static const std::string name_;

    bool load_ids();
    bool load_url();

    Json::Value get_raw() const;
};

std::pair<bool, std::string> read_file(const std::string& path);
std::pair<bool, configuration> load(const std::string& path);
}
}
