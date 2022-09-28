#pragma once
#include <stdexcept>

namespace dg::wii {
	class Exception : public std::runtime_error {
		using runtime_error::runtime_error;
	};
	class Init_Fail : public Exception {
		using Exception::Exception;
	};
}
