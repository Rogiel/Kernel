//
// Created by Rogiel Sulzbach on 9/15/17.
//

#pragma once

#include <System/Driver/Driver.hpp>
#include <vector>
#include <memory>

namespace System::Driver {

	class DriverManager {
	private:
		std::vector<std::unique_ptr<Driver>> drivers;

	public:
		void addDriver(Driver* driver);

		void removeDriver(Driver* driver);

		Driver* findDriver(System::Bus::BusDevice& busDevice);
		Driver* findDriverByName(const std::string& name);
		Driver* findDriverByType(const std::type_info& typeInfo);

		template<typename T>
		T* findDriverByType();

	};

	template<typename T>
	T* DriverManager::findDriverByType() {
		return findDriverByType(typeid(T));
	}

#define KERNEL_REGISTER_DRIVER(DriverName, DriverClass)		\
	static DriverClass __ ## DriverName;					\
	__attribute__((constructor))							\
	static void __ ## DriverName ## RegisterFunction() {	\
		dynamicDriverRegistering( &__ ## DriverName );		\
	}

	extern size_t count;
	void dynamicDriverRegistering(Driver* driver);

}

