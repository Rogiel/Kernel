//
// Created by Rogiel Sulzbach on 9/17/17.
//

#pragma once

#include <memory>

namespace System::Process {

	class Process;

	enum class ThreadState {
		RUNNING,
		WAITING,
		CRASH
	};

	class Thread {
	private:
		/**
		 * The process owning the thread
		 */
		Process* process;

		/**
		 * The thread state
		 */
		ThreadState state;

	public:
		/**
		 * Create a new thread for the given process
		 *
		 * @param process 	the process to create a new thread for
		 */
		explicit Thread(Process& process);

		/**
		 * Destroys the thread
		 */
		~Thread();

	public:
		/**
		 * @return the process owning the thread
		 */
		Process* getProcess() const;

		/**
		 * @return the thread state
		 */
		ThreadState getState() const;

	};

	using ThreadPtr = Thread*;

}
