//
// Created by Rogiel Sulzbach on 9/15/17.
//

#pragma once

#include <string>

namespace System::Device {

	class Device {
	public:
		virtual void onAttach();
		virtual void onDetach();

	public:
		virtual void onInterrupt();

	public:
		virtual Device* getParent();

		/**
		 * @return a string representing the device name
		 */
		virtual std::string getName() const = 0;

	};

}
