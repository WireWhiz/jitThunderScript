#pragma once
#include "ThunderScript.h"

namespace ts
{
	void DisplayBytecode(tsBytecode& bytecode)
	{
		for (size_t i = 0; i < bytecode.bytes.size(); i++)
		{
			std::cout << i << ":  " << (int)bytecode.bytes.read<std::byte>(i) << std::endl;
			
		}
	}
}
