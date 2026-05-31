#include <iostream>
#include "FileManager.hpp"
#include "CliArgs.hpp"

int main(int argc, const char *argv[])
{
    CliArgs cli(argc, argv);
    if (!cli.isGood())
    {
        cli.printUsage();
        return 1;
    }

    if (cli.getCliCommand().has_value())
    {
        switch (cli.getCliCommand().value())
        {
        case CliCommand::AddRecord:
            std::cout << "Agregar registro activado." << std::endl;
            break;

        case CliCommand::RemoveRecord:
            std::cout << "Eliminar registro activado." << std::endl;

            break;

        case CliCommand::SearchRecord:
            std::cout << "Buscar registro activado." << std::endl;

            break;

        case CliCommand::UpdateRecord:
            std::cout << "Actualizar registro activado." << std::endl;

            break;

        case CliCommand::CleanUp:
            std::cout << "Clean-Up  activado." << std::endl;

            break;

        default:

            break;
        }
    }

    return 0;
}