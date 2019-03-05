#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using namespace boost::program_options;
using namespace std;

vector<unsigned char> loadBinaryFile(string inputFileName);
void parseBinaryFile(const vector<unsigned char>& inputBuffer, ostream& outputStream);

int main(int argc, char* argv[])
{
	// Handle command line parameters
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
        return 0;
    }

    notify(vm);
	auto inputFileBuffer = loadBinaryFile(inputFileName);
	ofstream outputFile(outputFileName);
    return 0;
}

vector<unsigned char> loadBinaryFile(string inputFileName)
{
	ifstream inputFileStream(inputFileName, ios::binary);
	vector<unsigned char> inputFileBuffer(istreambuf_iterator<char>(inputFileStream), {});
	return inputFileBuffer;
}

void parseBinaryFile(const vector<unsigned char>& inputBuffer, ostream& outputStream)
{
	for (size_t i = 0; i < inputBuffer.size(); ++i)
	{

	}
}