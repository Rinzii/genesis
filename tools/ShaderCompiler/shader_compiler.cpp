// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include <algorithm>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

namespace
{
    namespace fs = std::filesystem;

	constexpr auto compiler_v =
    #if defined(DXC_PATH)
        DXC_PATH;
    #else
		"dxc";
        #error "DXC_PATH not defined"
    #endif

    // TODO: Implement this
}
