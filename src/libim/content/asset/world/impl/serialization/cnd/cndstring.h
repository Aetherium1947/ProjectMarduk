#ifndef LIBIM_CNDSTRING_H
#define LIBIM_CNDSTRING_H
#include <array>
#include <cstdint>
#include <iostream>
#include <string>

#include "../../../../../../utils/utils.h"

namespace libim::content::asset {
    static constexpr std::size_t kCndMaxNameLen = 64;

    template<std::size_t N>
    struct CndString : std::array<char, N> {
        using base_type = std::array<char, N>;
        using base_type::array;
        constexpr CndString() : base_type{ 0 } {}
        constexpr CndString(base_type a) : base_type(std::move(a)) {}

        std::string toStdString() const
        {
            return utils::trim(*this);
        }

        operator std::string() const
        {
            return toStdString();
        }

        bool operator == (const std::string& rstr) const
        {
            if (rstr.size() > N) return false;
            for (std::size_t i = 0; i < N; i++)
            {
                if(this->at(i) != rstr.at(i)) {
                    return false;
                }
                else if(this->at(i) == 0) {
                    return true;
                }
            }
            return true;
        }
    };

    using ResourceName = CndString<kCndMaxNameLen>;

    template<std::size_t N>
    std::ostream& operator<<(std::ostream& os, const CndString<N>& cstr)
    {
        os << cstr.data();
        return os;
    }
}
#endif // LIBIM_CNDSTRING_H