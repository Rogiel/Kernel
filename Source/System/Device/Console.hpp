//
// Created by Rogiel Sulzbach on 9/15/17.
//

#pragma once

#include <System/Device/Device.hpp>
#include <string>

namespace System::Device {

	class Console : public Device {
	public:
		virtual void write(const std::string& str) = 0;
		virtual void writeln(const std::string& str) = 0;

		virtual void clear() = 0;

	};

}
