//
// Created by Rogiel Sulzbach on 9/17/17.
//

#pragma once

#include <memory>
#include <functional>

namespace System::Interrupt {

	class InterruptDescriptor;

	/**
	 * A type representing a Interrupt Request
	 */
	using IRQ = uint32_t;

	/**
	 * A interrupt handler that will be called whenever the interrupt is fired
	 * by the processor.
	 */
	using InterruptHandler = std::function<void(
			/**
			 * The descriptor that has triggered the interrupt
			 */
			InterruptDescriptor&
	)>;

	class InterruptDescriptor {
	private:
		/**
		 * The interrupt request
		 */
		IRQ irq;

		/**
		 * The interrupt handler called called whenever the interrupt is fired
		 * by the processor.
		 */
		InterruptHandler handler;

	public:
		/**
		 * Creates a new interrupt descriptor
		 *
		 * @param irq 	the interrupt request
		 */
		InterruptDescriptor(IRQ irq);

		/**
		 * Destroys the interrupt descriptor
		 */
		~InterruptDescriptor();

	public:
		/**
		 * @return the interrupt request
		 */
		IRQ getIRQ() const;

		/**
		 * @return the interrupt handler called called whenever the interrupt
		 * is fired by the processor.
		 */
		const InterruptHandler& getHandler() const;

		/**
		 * Sets the interrupt handler.
		 *
		 * @param handler the interrupt handler called called whenever the
		 * interrupt is fired by the processor.
		 */
		void setHandler(const InterruptHandler& handler);

	};

	/**
	 * A pointer to a interrupt descriptor
	 */
	using InterruptDescriptorPtr = InterruptDescriptor*;

}
