#include "include/utility.hpp"
#include <zlib.h>
#include <cstring>
#include <iostream>
#include <sstream>
#include <stdexcept>

std::vector<std::string> SplitMessage(const std::string& message, const std::string& delim) {
    std::vector<std::string> tokens;
    size_t start = 0, end = 0;
    while ((end = message.find(delim, start)) != std::string::npos) {
        tokens.push_back(message.substr(start, end - start));
        start = end + delim.size();
    }
    tokens.push_back(message.substr(start));
    return tokens;
}

std::string CompressGzip(const std::string& str) {
    z_stream zs;
    memset(&zs, 0, sizeof(zs));

    if (deflateInit2(&zs, Z_BEST_COMPRESSION, Z_DEFLATED, 15 | 16, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
        throw(std::runtime_error("deflateInit2 failed while compressing."));
    }

    zs.next_in = (Bytef*)str.data();
    zs.avail_in = str.size();

    int ret;
    char outbuffer[32768];
    std::string outstring;

    do {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);

        ret = deflate(&zs, Z_FINISH);

        if (outstring.size() < zs.total_out) {
            outstring.append(outbuffer, zs.total_out - outstring.size());
        }
    } while (ret == Z_OK);

    deflateEnd(&zs);

    if (ret != Z_STREAM_END) {
        std::ostringstream oss;
        oss << "Exception during zlib compression: (" << ret << ") " << zs.msg;
        throw(std::runtime_error(oss.str()));
    }

    return outstring;
}
