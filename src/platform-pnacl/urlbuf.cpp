#include "urlbuf.hpp"
#include <stdexcept>
#include <ppapi_simple/ps_interface.h>
#include <ppapi_simple/ps_instance.h>

urlbuf::urlbuf(const std::string& url) :
	buffer(4096)
{
	urlLoaderInterface = PSInterfaceURLLoader();
	urlLoader = urlLoaderInterface->Create(PSInstance::GetInstance()->pp_instance());

	auto ppVarInterface = PSInterfaceVar();
	auto urlVar = ppVarInterface->VarFromUtf8(url.data(), url.length());

	auto urlRequestInfoInterface = PSInterfaceURLRequestInfo();
	auto urlRequestInfo = urlRequestInfoInterface->Create(PSInstance::GetInstance()->pp_instance());
	urlRequestInfoInterface->SetProperty(urlRequestInfo, PP_URLREQUESTPROPERTY_URL, urlVar);

	PP_CompletionCallback blockingCompletionCallback;
	blockingCompletionCallback.func = nullptr;
	blockingCompletionCallback.user_data = nullptr;
	blockingCompletionCallback.flags = 0;

	urlLoaderInterface->Open(urlLoader, urlRequestInfo, blockingCompletionCallback);
}

urlbuf::~urlbuf() { }

urlbuf::int_type urlbuf::underflow() {
	PP_CompletionCallback blockingCompletionCallback;
	blockingCompletionCallback.func = nullptr;
	blockingCompletionCallback.user_data = nullptr;
	blockingCompletionCallback.flags = 0;

	auto bytesRead = urlLoaderInterface->ReadResponseBody(urlLoader, buffer.data(), buffer.size(), blockingCompletionCallback);
	if (bytesRead < 0) throw std::runtime_error("urlLoaderInterface->ReadResponseBody returned an error code");

	setg(buffer.data(), buffer.data(), buffer.data() + bytesRead);

	return bytesRead ? traits_type::to_int_type(buffer[0]) : traits_type::eof();
}
