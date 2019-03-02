#pragma once

#include "IParser.h"

namespace CodexMachina
{
	class Parser8080 : public IParser
	{
	public:
		Parser8080() { }
		void parse(const std::vector<unsigned char>& inputBuffer, std::ostream& outputStream);
	}
}