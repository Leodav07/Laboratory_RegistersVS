#include "CliArgs.hpp"
#include <iostream>

void CliArgs::printUsage()
{
    std::cout << "Usage: " << program << " --data <file.data> [--index <file.idx>] <command> [argument]\n"
              << "\nCommands:\n"
              << "  --add   <file.json>    Add one or more student records from a JSON file.\n"
              << "  --search    <account>      Search a student record by account number.\n"
              << "  --remove  <account>      Soft-delete a student record by account number.\n"
              << "  --update <file.json>   Update an existing student record from a JSON file.\n"
              << "  --clean-up                 Rebuild the data file removing all soft-deleted records.\n"
              << "\nExamples:\n"
              << "  " << program << " --data student.data --add students.json\n"
              << "  " << program << " --data student.data --index student.idx --search 2020-00001\n"
              << "  " << program << " --data student.data --remove 2020-00001\n"
              << "  " << program << " --data student.data --clean-up\n"
              << std::endl;
}

void CliArgs::parse(int argc, const char *argv[])
{
    program = argv[0];

    if (argc < 2)
    {
        std::cerr << "Error: no arguments provided." << std::endl;
        printUsage();
        good = false;
        return;
    }

    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];

        if (arg == "--data")
        {
            if (i + 1 >= argc)
            {
                std::cerr << "Error: '--data' requires a file path." << std::endl;
                good = false;
                return;
            }
            dataFile = argv[++i];
        }
        else if (arg == "--index")
        {
            if (i + 1 >= argc)
            {
                std::cerr << "Error: '--index' requires a file path." << std::endl;
                good = false;
                return;
            }
            indexFile = argv[++i];
        }
        else if (arg == "--add")
        {
            if (i + 1 >= argc)
            {
                std::cerr << "Error: '--add' requires a JSON file." << std::endl;
                good = false;
                return;
            }
            cliCommand_ = CliCommand::AddRecord;
            filePath = argv[++i];
        }
        else if (arg == "--remove")
        {
            if (i + 1 >= argc)
            {
                std::cerr << "Error: '--remove' requires an account number." << std::endl;
                good = false;
                return;
            }
            cliCommand_ = CliCommand::RemoveRecord;
            accNumber = argv[++i];
        }
        else if (arg == "--search")
        {
            if (i + 1 >= argc)
            {
                std::cerr << "Error: '--search' requires an account number." << std::endl;
                good = false;
                return;
            }
            cliCommand_ = CliCommand::SearchRecord;
            accNumber = argv[++i];
        }
        else if (arg == "--update")
        {
            if (i + 1 >= argc)
            {
                std::cerr << "Error: '--update' requires a JSON file." << std::endl;
                good = false;
                return;
            }
            cliCommand_ = CliCommand::UpdateRecord;
            filePath = argv[++i];
        }
        else if (arg == "clean-up")
        {
            cliCommand_ = CliCommand::CleanUp;
        }
        else
        {
            std::cerr << "Error: unrecognized argument '" << arg << "'." << std::endl;
            good = false;
            return;
        }
    }

    if (dataFile.empty())
    {
        std::cerr << "Error: '--data' flag is required." << std::endl;
        good = false;
        return;
    }

    if (indexFile.empty())
    {
        size_t dotPos = filePath.rfind('.');
        if (dotPos != std::string::npos)
            indexFile = filePath.substr(0, dotPos) + ".idx";
        else
            indexFile = filePath + ".idx";
    }

    if (!cliCommand_.has_value())
    {
        std::cerr << "Error: no command provided." << std::endl;
        printUsage();
        good = false;
        return;
    }
}
