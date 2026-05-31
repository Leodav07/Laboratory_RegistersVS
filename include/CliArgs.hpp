#include <optional>
#include "CliCommand.hpp"
#include <string>

class CliArgs
{
public:
    CliArgs(int argc, const char *argv[]) { parse(argc, argv); };
    ~CliArgs();
    void printUsage();

    std::string getFile()
    {
        return filePath;
    }

    std::string getAccount()
    {
        return accNumber;
    }
    bool isGood() { return good; };

private:
    std::optional<CliCommand> cliCommand_;
    void parse(int argc, const char *argv[]);
    bool good = true;
    std::string program;
    std::string filePath;
    std::string accNumber;
};
