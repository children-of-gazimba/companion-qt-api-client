#include "client_exceptions.h"

ApiException::ApiException(const QString &text) noexcept
    : message(text)
{}

ApiException::ApiException(const ApiException &e) noexcept
    : message(e.message)
{}

void ApiException::raise() const
{
    throw *this;
}

ApiException *ApiException::clone() const
{
    return new ApiException(*this);
}

const char *ApiException::what() const noexcept
{
    return message.toStdString().c_str();
}
