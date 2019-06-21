#ifndef LIBIM_TOKENIZER_H
#define LIBIM_TOKENIZER_H
#include "token.h"
#include "../io/stream.h"

#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <type_traits>

namespace libim::text {

    class Tokenizer
    {
    public:
        Tokenizer(const InputStream& s);
        virtual ~Tokenizer();

        // Explicitly delete copy and move ctors
        Tokenizer(const Tokenizer&) = delete;
        Tokenizer(Tokenizer&&) noexcept = delete;
        Tokenizer& operator=(const Tokenizer&) = delete;
        Tokenizer operator=(Tokenizer&&) noexcept = delete;

        const Token& currentToken() const;
        const Token& getNextToken();
        const Token& peekNextToken();


        template <typename T, typename DT = std::decay_t<T>>
        DT getNumber()
        {
            auto tkn = getNextToken();
            return tkn.getNumber<DT>();
        }

        std::string getIdentifier();
        std::string getStringLiteral();
        std::string getSpaceDelimitedString();
        std::string getDelimitedString(const std::function<bool(char)>& isDelim);


        std::string getString(std::size_t len);

        void assertIdentifier(std::string_view id);
        void assertPunctuator(std::string_view punc);
        void assertEndOfFile();

        void skipToNextLine();

        void setReportEol(bool report);
        bool reportEol() const;

        const InputStream& istream() const;

    protected:
        void getNextToken(Token& out);
        void peekNextToken(Token& out);
        void getSpaceDelimitedString(Token& out);
        void getDelimitedString(Token& out, const std::function<bool(char)>& isDelim);
        void getString(Token& out, std::size_t len);

    protected:
        Token cachedTkn_;
        class TokenizerPrivate;
        std::unique_ptr<
            TokenizerPrivate
        > tp_;
    };

}
#endif // LIBIM_TOKENIZER_H
