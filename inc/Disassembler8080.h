#pragma once

#include "Disassembler.h"

namespace CodexMachina
{
	class Disassembler8080 : public Disassembler
	{
	public:
		Disassembler8080() { }
		void disassembleImpl(std::ostream& ostream) override;
	private:
		constexpr static std::size_t _addressBusSize{ 16 };
		constexpr static std::size_t _maxOperationSize{ 3 };
	};
}