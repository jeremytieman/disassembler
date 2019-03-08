#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

namespace po = boost::program_options;
using namespace std;

int main(int argc, char* argv[])
{
	// Handle command line parameters
	vector<string> inputFileName;
	string outputFileName;
    po::options_description desc("Allowed options");
	desc.add_options()
        ("help", "produce help message")
        ("inputFile", po::value<std::vector<std::string>>(&inputFileName)->required(), "file to be disassembled")
        ("outputFile", po::value<std::string>(&outputFileName)->required(), "assembly output file")
        ;

    po::variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);

    if (vm.count("help")) {
        cout << desc << "\n";
        return 0;
    }

    notify(vm);
    return 0;
}