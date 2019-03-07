#pragma once

#include "Disassembler.h"

namespace CodexMachina
{
	class Disassembler8080 : public Disassembler
	{
	public:
		Disassembler8080() { }
		void disassembleImpl();
	};
}