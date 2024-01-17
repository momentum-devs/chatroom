#include "S3ServiceImpl.h"

namespace server::application
{

S3ServiceImpl::S3ServiceImpl()
{
    //    Aws::SDKOptions options;
    //    Aws::InitAPI(options);
}

void S3ServiceImpl::putObject(const PutObjectPayload&) const {}

std::string S3ServiceImpl::getObject(const GetObjectPayload&) const
{
    return "";
}
}
