#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using namespace boost::program_options;
using namespace std;

int main(int argc, char* argv[])
{
	// Handle command line parameters
	vector<string> inputFileName;
	string outputFileName;
    options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("inputFile", value<vector<string>>(&inputFileName)->required(), "file to be disassembled")
        ("outputFile", value<string>(&outputFileName)->required(), "assembly output file")
        ;

    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);

    if (vm.count("help")) {
        cout << desc << "\n";
        return 0;
    }

    notify(vm);
    return 0;
}