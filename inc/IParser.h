#pragma once

#include <ostream>
#include <vector>

namespace CodexMachina
{
	class IParser
	{
	public:
		virtual void parse(const std::vector<unsigned char>& inputBuffer, std::ostream& outputStream) = 0;
	};
}