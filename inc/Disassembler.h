#pragma once

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <ostream>
#include <string>
#include <vector>

namespace CodexMachina
{
	class Disassembler
	{
	public:
		Disassembler(const std::string& filename) { }

		void parse(const std::string& filename)
		{
			std::filesystem::path path(filename);
			std::ifstream ifstream(path, std::ios::binary);
			parse(ifstream);
		}
		
		virtual void parse(const std::istream& istream) = 0;

	private:
		std::vector<std::byte> nextBuffer(std::istream& istream)
		{
			std::vector<char> buffer;
			buffer.reserve(BUFFER_SIZE);
			istream.read(buffer.data(), BUFFER_SIZE);
			std::vector<std::byte> bytes;
			bytes.reserve(istream.gcount());
			for (size_t i = 0; i < bytes.size(); ++i) bytes[i] = static_cast<std::byte>(buffer[i]);
			return bytes;
		}

		std::istream istream;
		static constexpr unsigned int BUFFER_SIZE = 1000000;
	};
}