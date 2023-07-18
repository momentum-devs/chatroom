#include "EmailServiceImpl.h"

#include <format>

#include "gtest/gtest.h"

#include "httpClient/HttpClientMock.h"

#include "faker-cxx/Internet.h"
#include "faker-cxx/Lorem.h"
#include "faker-cxx/Word.h"

using namespace ::testing;
using namespace server;
using namespace server::application;

class EmailServiceImplTest : public Test
{
public:
    std::shared_ptr<common::httpClient::HttpClientMock> httpClient =
        std::make_shared<StrictMock<common::httpClient::HttpClientMock>>();

    const std::string sendGridSecret = faker::Internet::password();

    EmailServiceImpl emailService{httpClient, sendGridSecret};
};

TEST_F(EmailServiceImplTest, shouldSendEmail)
{
    const auto to = faker::Internet::email();
    const auto from = faker::Internet::email();
    const auto subject = faker::Word::noun();
    const auto emailData = faker::Lorem::paragraph();

    const std::map<std::string, std::string> headers{{"Authorization", std::format("Bearer {}", sendGridSecret)},
                                                     {"Content-Type", "application/json"}};

    const auto sendGridApiUrl = "https://api.sendgrid.com/v3/mail/send";

    const auto body =
        std::format("{{\"personalizations\": [{{\"to\": [{{\"email\": \"{}\"}}]}}],\"from\": {{\"email\": "
                    "\"{}\"}},\"subject\": \"{}\",\"content\": [{{\"type\": \"text/plain\", \"value\": \"{}\"}}]}}",
                    to, from, subject, emailData);

    const common::httpClient::HttpPostRequestPayload postRequestPayload{sendGridApiUrl, headers, body};

    const common::httpClient::HttpResponse response{200, ""};

    EXPECT_CALL(*httpClient, post(postRequestPayload)).WillOnce(Return(response));

    emailService.sendEmail({to, from, subject, emailData});
}
