//
// Created by Rogiel Sulzbach on 9/15/17.
//

#pragma once

#include <System/Memory/MemoryController.hpp>
#include <System/Device/DeviceManager.hpp>
#include <System/Driver/DriverManager.hpp>
#include <System/Machine/Machine.hpp>

#include <System/Support/Utility/Formatting.hpp>

#include <memory>
#include <utility>

namespace System {

	class Kernel {
	private:
		Machine::Machine* machine;
		Memory::MemoryController* memoryController;
		Driver::DriverManager* driverManager;
		Device::DeviceManager* deviceManager;

	public:
		explicit Kernel(Memory::MemoryController* memoryController);

		Kernel(const Kernel& other) = delete;
		Kernel& operator=(const Kernel& other) = delete;

		Kernel(Kernel&& other) noexcept;
		Kernel& operator=(Kernel&& other) noexcept;

	public:
		void initialize();

		template<typename Arg>
		void write(const Arg& arg);

		template<typename Arg, typename... Args>
		void write(const Arg& arg, const Args&... str);

		template<typename... Args>
		void writeln(const Args&... str);

		template<typename... Args>
		void debug(const Args&... args);

	private:
		void print(const std::string& str);

	public:
		void setMachine(Machine::Machine* machine);
		Machine::Machine* getMachine();
		
		void setMemoryController(Memory::MemoryController* memoryController);
		Memory::MemoryController* getMemoryController();

		void setDriverManager(Driver::DriverManager* driverManager);
		Driver::DriverManager* getDriverManager();
		
		void setDeviceManager(Device::DeviceManager* deviceManager);
		Device::DeviceManager* getDeviceManager();

	};

	extern Kernel* kernel;

	template<typename Arg>
	void Kernel::write(const Arg& arg) {
		print(Support::Utility::Formatter<Arg>::toString(arg));
	}

	template<typename Arg, typename... Args>
	void Kernel::write(const Arg& arg, const Args&... args) {
		print(Support::Utility::Formatter<Arg>::toString(arg));
		write(args...);
	}

	template<typename... Args>
	void Kernel::writeln(const Args&... args) {
		write(args...);
		print("\n");
	}

	template<typename... Args>
	void Kernel::debug(const Args&... args) {
#if !defined(NDEBUG)
		writeln<Args...>(args...);
#endif
	}

}

using System::kernel;
