//
// Created by Rogiel Sulzbach on 9/17/17.
//

#pragma once

#include <System/Interrupt/InterruptDescriptor.hpp>

#include <unordered_map>

namespace System::Interrupt {

	class InterruptController {
	private:
		/**
		 * A map of all known interrupts
		 */
		std::unordered_map<
				IRQ, std::unique_ptr<InterruptDescriptor>
		> interrupts;

	public:
		/**
		 * Creates a new interrupt controller
		 */
		InterruptController();

		/**
		 * Destroys the interrupt controller
		 */
		~InterruptController();

	public:
		/**
		 * Gets a interrupt based on its IRQ
		 *
		 * @param irq	the interrupt IRQ
		 *
		 * @return a pointer to the interrupt given by the
		 * <tt>irq</tt>
		 */
		InterruptDescriptorPtr getInterrupt(IRQ irq);

		/**
		 * Adds a new interrupt descriptor to the interrupt controller
		 *
		 * The controller will assume ownership of the descriptor.
		 *
		 * @param interruptDescriptorPtr 	the interrupt descriptor pointer
		 */
		void registerInterrupt(InterruptDescriptorPtr interruptDescriptorPtr);

	};

}