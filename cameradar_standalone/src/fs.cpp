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

#include "fs.h"

#include <vector>     // for std::vector
#include <sstream>    // for std::stringstream
#include <pwd.h>      // for getpwuid, passwd
#include <stddef.h>   // for size_t
#include <sys/stat.h> // for stat, mkdir, S_ISDIR
#include <unistd.h>   // for getuid
#include <fstream>    // for std::ifstream

namespace etix {

namespace tool {

std::vector<std::string>
split(const std::string& s, char delim) {
    std::vector<std::string> elems;
    std::stringstream ss(s);

    std::string item;
    while (std::getline(ss, item, delim)) elems.push_back(item);

    return elems;
}

namespace fs {

fs_error
is_folder(const std::string& folder) {
    struct stat sb;

    if (stat(folder.c_str(), &sb) == 0) {
        if (S_ISDIR(sb.st_mode))
            return fs_error::is_dir;
        else
            return fs_error::is_not_dir;
    }
    return fs_error::dont_exist;
}

bool
get_or_create_folder(const std::string& folder) {
    bool status = false;

    switch (is_folder(folder)) {
    case fs_error::is_dir: status = true; break;
    case fs_error::is_not_dir: status = false; break;
    case fs_error::dont_exist: status = create_recursive_folder(folder); break;
    }
    return status;
}

bool
create_folder(const std::string& folder) {
    if (mkdir(folder.c_str(), 0755) == 0) { return true; }

    return false;
}

bool
create_recursive_folder(const std::string& folder) {
    auto path_elems = split(folder, '/');
    std::string current_path = folder[0] == '/' ? "/" : "";

    for (const auto& elem : path_elems) {
        current_path += elem;

        if (is_folder(current_path) == fs_error::dont_exist) create_folder(current_path);

        current_path += '/';
    }
    return true;
}

std::string
get_file_folder(std::string full_file_path) {
    //! remove ending slash
    if (full_file_path.back() == '/') full_file_path.pop_back();

    size_t last_slash_position = full_file_path.find_last_of('/');

    //! it there is no slash, there is no folder to return
    if (last_slash_position == std::string::npos) return "";

    return std::string(full_file_path, 0, last_slash_position);
}

std::string
home(void) {
    struct passwd* passwdEnt = getpwuid(getuid());
    return { passwdEnt->pw_dir };
}

bool
copy(const std::string& src, const std::string& dst) {
    std::ifstream src_stream(src, std::ios::binary);
    std::ofstream dst_stream(dst, std::ios::binary);

    if (not src_stream.is_open()) return false;

    dst_stream << src_stream.rdbuf();

    return true;
}

} // fs

} // tool

} // etix
