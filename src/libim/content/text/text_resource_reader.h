#ifndef LIBIM_RESOURCE_READER_H
#define LIBIM_RESOURCE_READER_H
#include "../../text/tokenizer.h"
#include "../../log/log.h"
#include "../../utils/utils.h"

#include <vector>
#include <type_traits>

namespace libim::content::text {
    using namespace libim::text;

    class TextResourceReader final : public Tokenizer
    {
    public:
        using Tokenizer::Tokenizer;

        void assertLabel(std::string_view label);

        template<typename T>
        void assertKey(std::string_view key, T v)
        {
            if constexpr (std::is_enum_v<T>){
                return assertKey(key, utils::to_underlying(v));
            }

            bool bValid = false;
            readKey(key, cachedTkn_);
            if constexpr(std::is_arithmetic_v<T>) {
                bValid = cachedTkn_.getNumber<T>() == v;
            }
             else {
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
            using U = utils::underlying_type_t<T>;

            readKey(key, cachedTkn_);
            if constexpr(std::is_arithmetic_v<U>) {
                return static_cast<T>(cachedTkn_.getNumber<U>());
            } else {
                return std::move(cachedTkn_).value();
            }
        }

        void readKey(std::string_view key, Token& t);


        template<typename T, bool hasRowIdxs = true, typename Lambda>
        std::vector<T> readList(std::string_view expectedName, Lambda&& constructor)
        {
            auto len = readKey<std::size_t>(expectedName);
            std::vector<T> result;
            result.reserve(len);

            for([[maybe_unused]] std::size_t i = 0; i < len; i++)
            {
                if constexpr (hasRowIdxs)
                {
                    [[maybe_unused]]  const auto rowIdx = readRowIdx();
                    assert(i == rowIdx && "reading list row failed!");
                }

                T item;
                constructor(*this, item);
                result.push_back(std::move(item));
            }

            return result;
        }

        void assertSection(std::string_view section);
        std::string readSection();
        void readSection(Token& t);

    private:
        std::size_t readRowIdx();
    };
}
#endif // LIBIM_RESOURCE_READER_H
