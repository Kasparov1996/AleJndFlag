#ifndef UTIL_HPP
#define UTIL_HPP

namespace util
{
    /*
     * NOTE: This should be the place where all the utilities subroutine is implemented
     *
     * TODO: How can we make use of this callback?
     */

    std::size_t callback(const char *in, std::size_t size, std::size_t num, std::string *out)
    {
        const std::size_t totalBytes(size * num);
        out->append(in, totalBytes);
        return totalBytes;
    }


}

#endif
