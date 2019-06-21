#include "../text_resource_reader.h"
#include "text_resource_literals.h"
#include "../../../text/impl/tokenizer_p.h"

#include <utility>

using namespace libim::content::text;
using namespace std::string_view_literals;

void TextResourceReader::assertLabel(std::string_view label)
{
    assertIdentifier(label);
    assertPunctuator(kResLabelPunc);
}

void TextResourceReader::readKey(std::string_view key, Token& t)
{
    assertKey(key);

    const bool bReportEol = reportEol();
    setReportEol(true);
    getNextToken(t);
    setReportEol(bReportEol);
}

void TextResourceReader::assertKey(std::string_view key)
{
    getString(cachedTkn_, key.size());
    if(!utils::iequal(key, cachedTkn_.value()))
    {
        LOG_DEBUG("assertKey: expected key '%', found '%'", key, cachedTkn_.value());
        throw TokenizerError("invalid key"sv, cachedTkn_.location());
    }
}

void TextResourceReader::assertSection(std::string_view section)
{
    readSection(cachedTkn_);
    if(!utils::iequal(cachedTkn_.value(), section))
    {
        LOG_DEBUG("expected '%', found '%'", section, cachedTkn_.value());
        throw TokenizerError("invalid section"sv, cachedTkn_.location());
    }
}

std::string TextResourceReader::readSection()
{
    readSection(cachedTkn_);
    return std::move(cachedTkn_).value();
}

void TextResourceReader::readSection(Token& t)
{
    assertLabel(kResName_Section);
    tp_->readLine(t);
}

std::size_t TextResourceReader::readRowIdx()
{
    auto num = getNumber<std::size_t>();
    assertPunctuator(kResLabelPunc);
    return num;
}
