#include "..\inc\Disassembler8080.h"

namespace CodexMachina
{
	void Disassembler8080::disassembleImpl(std::ostream& ostream)
	{
		const auto width = addressPrintingWidth(_addressBusSize);

		while (!_eof)
		{
			const auto& buffer = nextBuffer();
			std::size_t memOffset = 0;

			for (size_t i = 0; i < buffer.size(); ++i)
			{
				const auto fillChar = ostream.fill();
				ostream << std::setfill('0') << std::hex << memOffset;
				const auto printWidth = ostream.width();

				switch (const auto op = buffer[i]; op)
				{
				case 0x00:
					ostream << std::setfill(' ') << std::setw(3) << static_cast<char>(buffer[i]) << "       NOP\n";
				}
				ostream << std::setbase(0) << std::setfill(fillChar) << " ";
			}
		}
	}
}