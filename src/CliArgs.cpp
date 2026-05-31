#include "CliArgs.hpp"
#include <format>
#include <iostream>

void CliArgs::printUsage()
{
}

void CliArgs::parse(int argc, const char *argv[])
{
    program = argv[0];

    if (argc > 2 && argc <= 3)
    {
        std::string command = argv[1];

        if (command == "agregar")
        {
            filePath = argv[2];
            cliCommand_ = CliCommand::AddRecord;
        }
        else if (command == "eliminar")
        {
            accNumber = argv[2];
            cliCommand_ = CliCommand::RemoveRecord;
        }
        else if (command == "buscar")
        {
            accNumber = argv[2];
            cliCommand_ = CliCommand::SearchRecord;
        }
        else if (command == "actualizar")
        {
            filePath = argv[2];
            cliCommand_ = CliCommand::UpdateRecord;
        }
        else if (command == "clean-up")
        {
            cliCommand_ = CliCommand::CleanUp;
        }
        else
        {
            std::cout << "Expected commands: " << '\n'
                      << std::endl;
            good = false;
        }
    }
    else if (argc == 2)
    {
        std::string command = argv[1];
        if (command == "clean-up")
        {
            cliCommand_ = CliCommand::CleanUp;
        }
        else
        {
            good = false;
        }
    }
    else if (argc < 2)
    {
        std::cout << "Expected commands." << '\n'
                  << std::endl;
        good = false;
    }
    else
    {
        good = false;
    }
}
