//
// Created by Rogiel Sulzbach on 9/18/17.
//

#pragma once

#include <System/Device/Processor.hpp>

namespace System::Machine::X86::Device {

	class X86Processor : public System::Device::Processor {
	public:
		/**
		 * @return a string representing the device name
		 */
		std::string getName() const final;
	};

}


