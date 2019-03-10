#include <boost/program_options.hpp>
#include "Disassembler8080.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

namespace po = boost::program_options;

int main(int argc, char* argv[])
{
  // Handle command line parameters
  std::vector<std::string> inputFileNames;
  std::string outputFileName;
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help", "produce help message")
    ("inputFile,i", po::value<std::vector<std::string>>(&inputFileNames)->required(), "file to be disassembled")
    ("outputFile", po::value<std::string>(&outputFileName)->required(), "assembly output file")
    ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);

  if (vm.count("help") != 0) {
    std::cout << desc << "\n";
    return 0;
  }

  po::notify(vm);

  CodexMachina::Disassembler8080 d8080;
  d8080.disassemble(inputFileNames, outputFileName, true);

  return 0;
}