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

		void disassemble(const std::vector<std::string>& filenames)
		{
			for (const auto& filename : filenames)
			{
				if (!std::filesystem::exists(filename))
				{
					std::string error(filename);
					error.append(" does not exist");
					throw new std::invalid_argument(error);
				}
			}

			_curIndex = 0;
			_eof = false;
			_filenames = filenames;
			std::unique_ptr<std::istream> istreamPtr(new std::ifstream(_filenames[_curIndex], std::ios::binary));
			disassemble(istreamPtr);
		}
		
		void disassemble(std::unique_ptr<std::istream>& istreamPtr)
		{
			_istreamPtr = std::move(istreamPtr);
		}

		virtual void disassembleImpl() = 0;

	protected:
		std::vector<std::byte> nextBuffer()
		{
			std::vector<char> buffer;
			buffer.reserve(BUFFER_SIZE);
			std::streamsize countRead = 0;

			while (countRead < BUFFER_SIZE)
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
						return convertToBytes(buffer);
					}

					_istreamPtr.reset(new std::ifstream(_filenames[_curIndex], std::ios::binary));
				}

				_istreamPtr->read(buffer.data() + countRead, BUFFER_SIZE - countRead);
				countRead += _istreamPtr->gcount();
			}

			return convertToBytes(buffer);
		}

		size_t _curIndex;
		bool _eof;
		std::vector<std::string> _filenames;
		std::unique_ptr<std::istream> _istreamPtr;
		static constexpr unsigned int BUFFER_SIZE = 1000000;

	private:
		std::vector<std::byte> convertToBytes(const std::vector<char>& v)
		{
			std::vector<std::byte> bytes;
			bytes.reserve(v.size());
			return bytes;
		}
	};
}