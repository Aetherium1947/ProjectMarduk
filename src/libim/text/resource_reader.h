#ifndef LIBIM_RESOURCE_READER_H
#define LIBIM_RESOURCE_READER_H
#include "tokenizer.h"
#include "../log/log.h"
#include "../utils/utils.h"

#include <functional>
#include <vector>
#include <type_traits>

namespace libim::text {
    class ResourceReader final : public Tokenizer
    {
    public:
        using Tokenizer::Tokenizer;

        template<typename T>
        void assertKey(std::string_view key, T v)
        {
            bool bValid = false;
            readKey(key, cachedTkn_);
            if constexpr(std::is_arithmetic_v<T>) {
                bValid = cachedTkn_.getNumber<T>() == v;
            } else {
                bValid = utils::iequal(cachedTkn_.value(), v);
            }

            if(!bValid)
            {
                using namespace std::string_view_literals;
                LOG_DEBUG("assertKey: expected value '%', found '%'", v, cachedTkn_.value());
                throw TokenizerError("invalid value"sv, cachedTkn_.location());
            }
        }

        template<typename T>
        T readKey(std::string_view key)
        {
            readKey(key, cachedTkn_);
            if constexpr(std::is_arithmetic_v<T>) {
                return cachedTkn_.getNumber<T>();
            } else {
                return std::move(cachedTkn_).value();
            }
        }

        void readKey(std::string_view key, Token& t);


        template<typename T, typename Lambda>
        std::vector<T> readList(std::string_view expectedName, Lambda&& readRow)
        {
            assertIdentifier(expectedName);

            auto len = getNumber<std::size_t>();
            std::vector<T> result;
            result.reserve(len);

            for(std::size_t i = 0; i < len; i++)
            {
                [[maybe_unused]]  const auto j = getNumber<std::size_t>();
                assert(i == j && "reading list row failed!");
                assertPunctuator(":");

                result.push_back(readRow(*this));
            }

            return result;
        }

        void assertSection(std::string_view section);
        std::string readSection();
        void readSection(Token& t);
    };
}
#endif // LIBIM_RESOURCE_READER_H
