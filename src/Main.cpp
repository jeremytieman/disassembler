#include <cxxopts.hpp>
#include "Disassembler8080.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

int main(int argc, char* argv[])
{
  // Handle command line parameters
  std::vector<std::string> inputFileNames;
  std::string outputFileName;
  cxxopts::Options options(argv[0], "Allowed options");
  options.add_options()
    ("help", "produce help message")
    ("i,inputFiles", "file to be disassembled (required)", cxxopts::value<std::vector<std::string>>(inputFileNames))
    ("outputFile", "assembly output file (required)", cxxopts::value<std::string>(outputFileName))
    ;

  auto result = options.parse(argc, argv);

  if (result.count("help"))
  {
    std::cout << options.help() << "\n";
    return 0;
  }

  if (result.count("inputFiles") == 0)
  {
    std::cerr << "Missing input file(s)\n";
    std::cerr << options.help() << "\n";
    return 0;
  }

  if (result.count("outputFile") == 0)
  {
    std::cerr << "Missing output file\n";
    std::cerr << options.help() << "\n";
    return 0;
  }

  CodexMachina::Disassembler8080 d8080;
  d8080.disassemble(inputFileNames, outputFileName, true);

  return 0;
}