//
// Created by Rogiel Sulzbach on 9/15/17.
//

#include <string.h>
#include "VGAConsole.hpp"

namespace System::Driver::Console {

	static inline void outb(uint16_t port, uint8_t val) {
		asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port));
		/* TODO: Is it wrong to use 'N' for the port? It's not a 8-bit constant. */
		/* TODO: Should %1 be %w1? */
	}

	static void update_cursor(int row, int col) {
		unsigned short position = (row * 80) + col;

		// cursor LOW port to vga INDEX register
		outb(0x3D4, 0x0F);
		outb(0x3D5, (unsigned char) (position & 0xFF));
		// cursor HIGH port to vga INDEX register
		outb(0x3D4, 0x0E);
		outb(0x3D5, (unsigned char) ((position >> 8) & 0xFF));
	}

	static uint16_t make_vgaentry(char c, uint8_t color) {
		uint16_t c16 = c;
		uint16_t color16 = color;
		return c16 | color16 << 8;
	}

	static size_t strlen(const char* str) {
		size_t ret = 0;
		while(str[ret] != 0)
			ret++;
		return ret;
	}

	// -----------------------------------------------------------------------------------------------------------------

	VGAConsoleDevice::VGAConsoleDevice(System::Bus::BusDevice& busDevice) {
		vga = busDevice.allocateMemoryResource();

		color = 7;
		clear();
	}

	void VGAConsoleDevice::write(const std::string& str) {
		writeString(str.c_str());
	}

	void VGAConsoleDevice::writeln(const std::string& str) {
		writeString(str.c_str());
		putChar('\n');
	}

	void VGAConsoleDevice::clear() {
		row = 0;
		column = 0;
		for(size_t y = 0; y < VGA_HEIGHT; y++) {
			for(size_t x = 0; x < VGA_WIDTH; x++) {
				const size_t index = y * VGA_WIDTH + x;

				vga[index].character = ' ';
				vga[index].background = 0;
				vga[index].foreground = 7;
			}
		}
		update_cursor(0, 0);
	}

	// -----------------------------------------------------------------------------------------------------------------

	void VGAConsoleDevice::onAttach() {
		write(getName());
		write(": ");
		write(__func__);
		write(": console device attached\n");
	}

	// -----------------------------------------------------------------------------------------------------------------

	std::string VGAConsoleDevice::getName() const {
		return "VGAConsole";
	}

	// -----------------------------------------------------------------------------------------------------------------

	void VGAConsoleDevice::setColor(uint8_t color) {
		VGAConsoleDevice::color = color;
	}

	void VGAConsoleDevice::putEntryAt(char c, uint8_t color, size_t x, size_t y) {
		const size_t index = y * VGA_WIDTH + x;
		vga[index].character = c;
		vga[index].background = 0;
		vga[index].foreground = 7;
	}

	void VGAConsoleDevice::putChar(char c) {
		if(row >= VGA_HEIGHT) {
			memcpy((void*) &vga[0], (void*) &vga[VGA_WIDTH], 2 * VGA_WIDTH * (VGA_HEIGHT - 1));
			row--;
		}

		if(c == '\n') {
			update_cursor(row, column);
			row += 1;
			column = 0;

			return;
		}

		putEntryAt(c, color, column, row);
		if(++column == VGA_WIDTH) {
			column = 0;
			if(++row == VGA_HEIGHT) {
				row = 0;
			}
		}
		update_cursor(row, column);
	}

	void VGAConsoleDevice::writeString(const char* data) {
		size_t datalen = strlen(data);
		for(size_t i = 0; i < datalen; i++)
			putChar(data[i]);
	}

	// -----------------------------------------------------------------------------------------------------------------

	static bool processed = false;

	Driver::ProbeStatus VGAConsoleDriver::probe(System::Bus::BusDevice& busDevice) {
		if(processed == false) {
			processed = true;
			return ProbeStatus::SUPPORTED;
		} else {
			return ProbeStatus::NOT_SUPPORTED;
		}
	}

	VGAConsoleDevice* VGAConsoleDriver::attach(System::Bus::BusDevice& busDevice) {
		return new VGAConsoleDevice(busDevice);
	}

	const char* VGAConsoleDriver::getName() {
		return "VGAConsole";
	}

}