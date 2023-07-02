#include "MessageFactoryImpl.h"

namespace client::api
{
common::messages::Message MessageFactoryImpl::createMessage(common::messages::MessageId messageId,
                                                            const nlohmann::json& data,
                                                            const std::optional<std::string> token) const
{
    nlohmann::json jsonPayload;

    jsonPayload["data"] = data;

    if (token)
    {
        jsonPayload["token"] = token.value();
    }

    common::bytes::Bytes payload{jsonPayload.dump()};

    common::messages::Message message{messageId, payload};

    return message;
}
}