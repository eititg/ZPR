#ifndef BUFFERPACK_HPP
#define BUFFERPACK_HPP

#include <boost/cstdint.hpp>
#include <string>
#include <vector>

namespace util
{
    template <typename T> void pack(std::vector<boost::uint8_t> &buffer, T data);
    template <> void pack(std::vector<boost::uint8_t> &buffer, std::string data);

    template <typename T> T unpack(std::vector<boost::uint8_t> &buffer, unsigned &pointer);
    template <> std::string unpack(std::vector<boost::uint8_t> &buffer, unsigned &pointer);

    std::string unpack(std::vector<boost::uint8_t> &buffer, unsigned size, unsigned &pointer);
}

#include "BufferPack.tpp"

#endif // BUFFERPACK_HPP
