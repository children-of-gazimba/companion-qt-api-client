#ifndef CLIENT_EXCEPTIONS_H
#define CLIENT_EXCEPTIONS_H

#include <QException>

class ApiException : public QException
{
public:
    ApiException(QString const& text=" ") noexcept;
    ApiException(const ApiException &e) noexcept;
    ~ApiException() override = default;

    void raise() const override;
    ApiException *clone() const override;
    const char *what() const noexcept override;

private:
    QString message;
};

#endif // CLIENT_EXCEPTIONS_H
