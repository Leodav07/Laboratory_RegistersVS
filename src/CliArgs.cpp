#include "CliArgs.hpp"
#include <iostream>

void CliArgs::printUsage()
{
    std::cout << "Uso: " << program << " Interfaz de Linea de Comandos." << '\n'
              << "Agregar registro/s: --agregar | \"archivo.json\" -> (Obligatorio, sin comillas)." << '\n'
              << "Eliminar registro por No. de Cuenta: --eliminar | \"No. de Cuenta\" -> (Obligatorio, sin comillas)." << '\n'
              << "Buscar registro por No. de Cuenta: --buscar | \"No. de Cuenta\" -> (Obligatorio, sin comillas)." << '\n'
              << "Actualizar registro: --actualizar | \"archivo.json\" -> (Obligatorio, sin comillas)." << '\n'
              << "Clean-Up: --clean-up" << std::endl;
}

void CliArgs::parse(int argc, const char *argv[])
{
    program = argv[0];

    if (argc > 2 && argc <= 3)
    {
        std::string command = argv[1];

        if (command == "--agregar")
        {
            filePath = argv[2];
            cliCommand_ = CliCommand::AddRecord;
        }
        else if (command == "--eliminar")
        {
            accNumber = argv[2];
            cliCommand_ = CliCommand::RemoveRecord;
        }
        else if (command == "--buscar")
        {
            accNumber = argv[2];
            cliCommand_ = CliCommand::SearchRecord;
        }
        else if (command == "--actualizar")
        {
            filePath = argv[2];
            cliCommand_ = CliCommand::UpdateRecord;
        }
        else if (command == "--clean-up")
        {
            cliCommand_ = CliCommand::CleanUp;
        }
        else
        {
            std::cout << "Expected commands: " << '\n'
                      << std::endl;
            cliCommand_ = std::nullopt;
            good = false;
        }
    }
    else if (argc == 2)
    {
        std::string command = argv[1];
        if (command == "--clean-up")
        {
            cliCommand_ = CliCommand::CleanUp;
        }
        else
        {
            cliCommand_ = std::nullopt;
            good = false;
        }
    }
    else if (argc < 2)
    {
        std::cout << "Expected commands." << '\n'
                  << std::endl;
        cliCommand_ = std::nullopt;
        good = false;
    }
    else
    {
        cliCommand_ = std::nullopt;
        good = false;
    }
}
