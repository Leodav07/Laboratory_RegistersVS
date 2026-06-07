#include <optional>
#include "CliCommand.hpp"
#include <string>

class CliArgs
{
public:
    CliArgs(int argc, const char *argv[]) { parse(argc, argv); };

    void printUsage();

    std::string getFile()
    {
        return filePath;
    }

    std::string getDataFile()
    {
        return dataFile;
    }

    std::string getIndexFile()
    {
        return indexFile;
    }

    std::string getAccount()
    {
        return accNumber;
    }

    std::optional<CliCommand> getCliCommand() const
    {
        return cliCommand_;
    }

    bool isGood() { return good; };

private:
    std::optional<CliCommand> cliCommand_;
    void parse(int argc, const char *argv[]);
    bool good = true;
    std::string program;
    std::string filePath;
    std::string dataFile;
    std::string indexFile;
    std::string accNumber;
};
