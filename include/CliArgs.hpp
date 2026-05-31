#include <optional>
#include "CliCommand.hpp"

class CliArgs
{
public:
    CliArgs(int argc, const char *argv[]);
    ~CliArgs();
    void printUsage();
    bool isGood;

private:
    std::optional<CliCommand> cliCommand_;
};
