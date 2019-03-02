#include <boost/program_options.hpp>
#include <iostream>
#include <string>

using namespace boost::program_options;
using namespace std;

int main(int argc, char* argv[])
{
	string inputFileName;
	string outputFileName;
    options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("inputFile", value<string>(&inputFileName)->required(), "file to be disassembled")
        ("outputFile", value<string>(&outputFileName)->required(), "assembly output file")
        ;

    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);

    if (vm.count("help")) {
        cout << desc << "\n";
        return 1;
    }

    notify(vm);
    return 0;
}