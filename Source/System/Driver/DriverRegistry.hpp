//
// Created by Rogiel Sulzbach on 9/17/17.
//

#pragma once

#include <System/Driver/Driver.hpp>

#include <vector>

namespace System::Driver {

	class DriverRegistry {
	private:
//		std::vector<Driver*> registry;

	public:
		/**
		 * Creates a new empty driver registry
		 */
		DriverRegistry();

		/**
		 * Destroys the driver registry
		 */
		~DriverRegistry();

	public:
		/**
		 * Registers a new driver
		 *
		 * @param driver 	the driver to be registered
		 */
		void registerDriver(Driver* driver);

	};

}

