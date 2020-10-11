//
// Created by Rogiel Sulzbach on 9/17/17.
//

#include "Process.hpp"

#include <utility>

namespace System::Process {

	Process::Process(Memory::VirtualMemorySpacePtr memorySpace,
					 Syscall::SyscallController& syscallController) :
			memorySpace(std::move(memorySpace)),
			syscallController(&syscallController) {

	}

	Process::~Process() = default;

	// -----------------------------------------------------------------------------------------------------------------

	ThreadPtr Process::createThread() {
		return &threads.emplace_back(*this);
	}

	void Process::destroyThread(const Thread& thread) {
		// TODO
	}

	// -----------------------------------------------------------------------------------------------------------------

	const std::list<Thread>& Process::getThreads() const {
		return threads;
	}

	const Memory::VirtualMemorySpacePtr& Process::getMemorySpace() const {
		return memorySpace;
	}

	Syscall::SyscallController& Process::getSyscallController() const {
		return *syscallController;
	}

}
