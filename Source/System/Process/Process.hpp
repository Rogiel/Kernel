//
// Created by Rogiel Sulzbach on 9/17/17.
//

#pragma once

#include <System/Process/Thread.hpp>
#include <System/Process/Syscall/SyscallController.hpp>
#include <System/Memory/VirtualMemorySpace.hpp>

#include <list>

namespace System::Process {

	class Process {
	private:
		/**
		 * A list of threads for the process
		 */
		std::list<Thread> threads;

		/**
		 * The process virtual memory space
		 */
		Memory::VirtualMemorySpacePtr memorySpace;

		/**
		 * The process syscall controller
		 */
		Syscall::SyscallController* syscallController;

	public:
		/**
		 * Creates a new process with the given memory space
		 *
		 * @param memorySpace		the process memory space
		 * @param syscallController	the process syscall controller
		 */
		explicit Process(Memory::VirtualMemorySpacePtr memorySpace,
						 Syscall::SyscallController& syscallController);

		/**
		 * Destroys the process
		 */
		~Process();

	public:
		/**
		 * Create a new thread.
		 *
		 * @return a pointer to the newly created thread.
		 */
		ThreadPtr createThread();

		/**
		 * Destroys a process thread
		 *
		 * @param thread 	the thread to be destroyed
		 */
		void destroyThread(const Thread& thread);

	public:
		/**
		 * @return a list of threads for the process
		 */
		const std::list<Thread>& getThreads() const;

		/**
		 * @return the process virtual memory space
		 */
		const Memory::VirtualMemorySpacePtr& getMemorySpace() const;

		/**
		 * @return the process syscall controller
		 */
		Syscall::SyscallController& getSyscallController() const;

	};

	using ProcessPtr = Process*;

}


