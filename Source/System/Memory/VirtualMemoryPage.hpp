//
// Created by Rogiel Sulzbach on 9/16/17.
//

#pragma once

#include <System/Memory/PhysicalMemoryPage.hpp>

#import <memory>

namespace System::Memory {

	enum class VirtualMemoryPagePermission : uint8_t {
		/**
		 * The page has the read permission
		 */
				READ = 1 << 0,

		/**
		 * The page has the write permission
		 */
				WRITE = 1 << 1,

		/**
		 * The page has the execute permission
		 */
				EXECUTE = 1 << 2
	};

	using VirtualMemoryPagePlatformData = void*;

	class VirtualMemoryPage {
	private:
		/**
		 * The physical memory page this virtual page refers to.
		 *
		 * Can be <tt>nullptr</tt> if the page is currently paged-out
		 * or is not allocated.
		 */
		PhysicalMemoryPagePtr physicalMemoryPage;

		/**
		 * The virtual memory page permissions
		 */
		VirtualMemoryPagePermission permissions;

		/**
		 * A opaque platform data object. This object should only be
		 * touched by platform-specific code and is completely managed
		 * by it.
		 */
		VirtualMemoryPagePlatformData platformData;

	public:
		/**
		 * Creates a new virtual memory page
		 */
		VirtualMemoryPage();

		/**
		 * Destroys a virtual memory page
		 */
		~VirtualMemoryPage();

	public:
		/**
		 * @return the memory page permissions
		 */
		VirtualMemoryPagePermission getPermissions() const;

		/**
		 * @return true if the memory page is readable
		 */
		bool canRead();

		/**
		 * @return true if the memory page is writeable
		 */
		bool canWrite();

		/**
		 * @return true if the memory page is executable
		 */
		bool canExecute();

		/**
		 * @return the physical memory page
		 */
		const PhysicalMemoryPagePtr& getPhysicalMemoryPage() const;

		/**
		 * Changes the physical memory page that backs the virtual page
		 *
		 * @param physicalMemoryPage 	the new physical memory page
		 */
		void setPhysicalMemoryPage(const PhysicalMemoryPagePtr& physicalMemoryPage);

		/**
		 * @return a opaque platform data object. This object should only
		 * be touched by platform-specific code and is completely managed
		 * by it.
		 */
		VirtualMemoryPagePlatformData getPlatformData() const;

		/**
		 * @param platformData a opaque platform data object. This object
		 * should only be touched by platform-specific code and is
		 * completely managed by it.
		 */
		void setPlatformData(VirtualMemoryPagePlatformData platformData);

	};

	using VirtualMemoryPagePtr = std::shared_ptr<VirtualMemoryPage>;
	using VirtualMemoryPageWeakPtr = std::weak_ptr<VirtualMemoryPage>;

	inline VirtualMemoryPagePermission operator|(VirtualMemoryPagePermission permission1,
												 VirtualMemoryPagePermission permission2) {
		return static_cast<VirtualMemoryPagePermission>(
				static_cast<uint8_t>(permission1) | static_cast<uint8_t>(permission2)
		);
	}

	inline bool operator&(VirtualMemoryPagePermission permission1,
						  VirtualMemoryPagePermission permission2) {
		return (static_cast<uint8_t>(permission1) & static_cast<uint8_t>(permission2)) ==
			   static_cast<uint8_t>(permission2);
	}
}
