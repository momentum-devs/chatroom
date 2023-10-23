#include "EmailServiceImpl.h"

#include <format>
#include <utility>

namespace server::application
{
namespace
{
const auto sendGridApiUrl = "https://api.sendgrid.com/v3/mail/send";
}

EmailServiceImpl::EmailServiceImpl(std::shared_ptr<common::httpClient::HttpClient> httpClientInit,
                                   std::string sendGridApiKeyInit, std::string sendGridEmailInit)
    : httpClient{std::move(httpClientInit)},
      sendGridApiKey{std::move(sendGridApiKeyInit)},
      sendGridEmail{std::move(sendGridEmailInit)}
{
}

void EmailServiceImpl::sendEmail(const SendEmailPayload& payload) const
{
    const std::map<std::string, std::string> headers{{"Authorization", std::format("Bearer {}", sendGridApiKey)},
                                                     {"Content-Type", "application/json"}};

    const auto data =
        std::format("{{\"personalizations\": [{{\"to\": [{{\"email\": \"{}\"}}]}}],\"from\": {{\"email\": "
                    "\"{}\"}},\"subject\": \"{}\",\"content\": [{{\"type\": \"text/plain\", \"value\": \"{}\"}}]}}",
                    payload.to, sendGridEmail, payload.subject, payload.data);

    httpClient->post({sendGridApiUrl, headers, data});
}
}
