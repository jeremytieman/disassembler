#pragma once

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace CodexMachina
{
	class Disassembler
	{
	public:
		Disassembler() { }

		void disassemble(const std::vector<std::string>& inputFilenames, const std::string& outputFilename)
		{
			for (const auto& filename : inputFilenames)
			{
				if (!std::filesystem::exists(filename))
				{
					std::string error(filename);
					error.append(" does not exist");
					throw new std::invalid_argument(error);
				}
			}

			if (std::filesystem::exists(outputFilename))
			{
				std::string error(outputFilename);
				error.append(" already exists");
				throw new std::invalid_argument(error);
			}

			_curIndex = 0;
			_eof = false;
			_filenames = inputFilenames;
			std::unique_ptr<std::istream> istreamPtr(new std::ifstream(_filenames[_curIndex], std::ios::binary));
			std::ofstream ostream{ outputFilename };
			disassemble(istreamPtr, ostream);
		}
		
		void disassemble(std::unique_ptr<std::istream>& istreamPtr, std::ostream& ostream)
		{
			_istreamPtr = std::move(istreamPtr);
		}

		virtual void disassembleImpl(std::ostream& ostream) = 0;

	protected:
		std::vector<unsigned char> nextBuffer()
		{
			std::vector<char> buffer;
			buffer.reserve(_bufferSize);
			std::streamsize countRead = 0;

			while (countRead < _bufferSize)
			{
				if (_istreamPtr->fail())
				{
					std::string error{ "error reading file " };
					error.append(_filenames[_curIndex]);
					throw new std::runtime_error(error);
				}

				if (_istreamPtr->eof())
				{
					if (++_curIndex == _filenames.size())
					{
						_eof = true;
						return convertToUnsigned(buffer);
					}

					_istreamPtr.reset(new std::ifstream(_filenames[_curIndex], std::ios::binary));
				}

				_istreamPtr->read(buffer.data() + countRead, _bufferSize - countRead);
				countRead += _istreamPtr->gcount();
			}

			return convertToUnsigned(buffer);
		}

		std::size_t addressPrintingWidth(std::size_t addressBusSize)
		{
			std::size_t result = 0;
			auto memorySize = 1 << addressBusSize;
			while (memorySize > 16) memorySize /= 16;
			return result;
		}

		size_t _curIndex;
		bool _eof;
		std::vector<std::string> _filenames;
		std::unique_ptr<std::istream> _istreamPtr;
		static constexpr unsigned int _bufferSize{ 1000000 };

	private:
		std::vector<unsigned char> convertToUnsigned(const std::vector<char>& v)
		{
			std::vector<unsigned char> bytes;
			bytes.reserve(v.size());
			for (const auto c : v) bytes.push_back(c);
			return bytes;
		}
	};
}