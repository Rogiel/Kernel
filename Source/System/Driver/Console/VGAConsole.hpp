//
// Created by Rogiel Sulzbach on 9/15/17.
//

#pragma once

#include <stdbool.h> /* C doesn't have booleans by default. */
#include <stddef.h>
#include <stdint.h>

#include <System/Device/Console.hpp>
#include <System/Driver/Driver.hpp>


namespace System::Driver::Console {

	class VGAConsoleDevice : public Device::Console {
	private:
		struct VGAEntry {
			char character;
			uint8_t foreground:4;
			uint8_t background:4;
		} __attribute__((packed));

		System::Bus::MemoryResource<VGAEntry> vga;

	public:
		VGAConsoleDevice(System::Bus::BusDevice& busDevice);

		void write(const std::string& str) final;
		void writeln(const std::string& str) final;

		void clear() final;

	public:
		void onAttach() final;

	public:
		/**
		 * @return a string representing the device name
		 */
		std::string getName() const final;

	private:
		static const size_t VGA_WIDTH = 80;
		static const size_t VGA_HEIGHT = 25;

		size_t row;
		size_t column;
		uint8_t color;

		void setColor(uint8_t color);
		void putEntryAt(char c, uint8_t color, size_t x, size_t y);
		void putChar(char c);
		void writeString(const char* data);
	};

	class VGAConsoleDriver : public Driver {
	public:
		ProbeStatus probe(System::Bus::BusDevice& busDevice) final;

		/**
		 * Attaches the device.
		 *
		 * @param busDevice 	the bus device to attach
		 *
		 * @return a newly created device from the driver
		 */
		VGAConsoleDevice* attach(System::Bus::BusDevice& busDevice) final;

		/**
		 * @return a constant string with the driver name
		 */
		const char* getName() final;


	};

}

