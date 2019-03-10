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
    Disassembler() = default;

    void disassemble(const std::vector<std::string>& inputFilenames, const std::string& outputFilename, const bool verbose = false)
    {
      for (const auto& filename : inputFilenames)
      {
        if (!std::filesystem::exists(filename))
        {
          std::string error(filename);
          error.append(" does not exist");
          throw std::invalid_argument(error);
        }
      }

      if (std::filesystem::exists(outputFilename))
      {
        std::string error(outputFilename);
        error.append(" already exists");
        throw std::invalid_argument(error);
      }

      _curIndex = 0;
      _eof = false;
      _filenames = inputFilenames;
      std::unique_ptr<std::istream> istreamPtr = std::make_unique<std::ifstream>(_filenames[_curIndex], std::ios::binary);
      std::ofstream ostream{ outputFilename };
      disassemble(istreamPtr, ostream, verbose);
    }

    void disassemble(std::unique_ptr<std::istream>& istreamPtr, std::ostream& ostream, const bool verbose = false)
    {
      _istreamPtr = std::move(istreamPtr);
      disassembleImpl(ostream, verbose);
    }

    virtual void disassembleImpl(std::ostream& ostream, bool verbose = false) = 0;

  protected:
    std::vector<unsigned char> nextBuffer()
    {
      std::vector<char> buffer(_bufferSize);
      std::streamsize countRead = 0;

      while (countRead < _bufferSize)
      {
        if (_istreamPtr->fail() && !_istreamPtr->eof())
        {
          std::string error{ "error reading file " };
          error.append(_filenames[_curIndex]);
          throw std::runtime_error(error);
        }

        if (_istreamPtr->eof())
        {
          if (++_curIndex == _filenames.size())
          {
            _eof = true;
            buffer.resize(countRead);
            return convertToUnsigned(buffer);
          }

          _istreamPtr = std::make_unique<std::ifstream>(_filenames[_curIndex], std::ios::binary);
        }

        _istreamPtr->read(&buffer[countRead], _bufferSize - countRead);
        countRead += _istreamPtr->gcount();
      }

      return convertToUnsigned(buffer);
    }

    std::size_t addressPrintingWidth(std::size_t addressBusSize)
    {
      std::size_t result = 1;
      auto memorySize = 1 << addressBusSize;

      while (memorySize > 16)
      {
        memorySize /= 16;
        ++result;
      }

      return result;
    }

    size_t _curIndex = 0;
    bool _eof = true;
    std::vector<std::string> _filenames;
    std::unique_ptr<std::istream> _istreamPtr;
    static constexpr unsigned int _bufferSize{ 1000000 };

  private:
    std::vector<unsigned char> convertToUnsigned(const std::vector<char>& v)
    {
      std::vector<unsigned char> bytes;
      bytes.reserve(v.size());
      for (const auto c : v) { bytes.push_back(c); }
      return bytes;
    }
  };
} // namespace CodexMachina