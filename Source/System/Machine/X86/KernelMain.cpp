//
// Created by Rogiel Sulzbach on 9/15/17.
//

#include "X86Machine.hpp"

#include <System/Kernel.hpp>
#include <string>

namespace System::Machine::X86 {

	static void* const MEMORY_CONTROLLER_ALLOC_BASE_ADDRESS = (void*) (0x01000000 + 0 * 1024 * 1024);
	static void* const KERNEL_ALLOC_BASE_ADDRESS =
			(char*) MEMORY_CONTROLLER_ALLOC_BASE_ADDRESS + sizeof(Memory::MemoryController);
	static void* const MEMORY_CONTROLLER_BASE_ADDRESS = (char*) KERNEL_ALLOC_BASE_ADDRESS + sizeof(Kernel);

	extern "C" void KernelMain() {
		kernel = new(KERNEL_ALLOC_BASE_ADDRESS) Kernel(
				new(MEMORY_CONTROLLER_ALLOC_BASE_ADDRESS) Memory::MemoryController(
						MEMORY_CONTROLLER_BASE_ADDRESS, 10 * 1024 * 1024
				)
		);

		kernel->setMachine(new X86Machine());
		kernel->initialize();
		kernel->getMachine()->halt();
	}

}
