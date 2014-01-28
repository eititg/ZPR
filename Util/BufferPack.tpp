#include <algorithm>

namespace util
{
    template <typename T>
    void pack(std::vector<boost::uint8_t> &buffer, T data)
    {
        boost::uint8_t *mem = reinterpret_cast<boost::uint8_t*>(&data);

        buffer.insert(buffer.end(), mem, mem + sizeof(T));
    }

    template <>
    void pack(std::vector<boost::uint8_t> &buffer, std::string data)
    {
        buffer.insert(buffer.end(), data.begin(), data.end());
    }

    template <typename T>
    T unpack(std::vector<boost::uint8_t> &buffer, unsigned &pointer)
    {
        T data;

        std::copy(&buffer[pointer], &buffer[pointer + sizeof(T)], &data);

        pointer += sizeof(T);

        return data;
    }

    std::string unpack(std::vector<boost::uint8_t> &buffer, unsigned size, unsigned &pointer)
    {
        std::string data(&buffer[pointer], &buffer[pointer + size]);

        pointer += size;

        return data;
    }
}
