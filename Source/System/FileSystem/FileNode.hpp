//
// Created by Rogiel Sulzbach on 9/15/17.
//

#pragma once

#include <memory>

namespace System::FileSystem {

	class FileNode {

	};

	using FileNodePtr = std::shared_ptr<FileNode>;
	using FileNodeWeakPtr = std::weak_ptr<FileNode>;

}
