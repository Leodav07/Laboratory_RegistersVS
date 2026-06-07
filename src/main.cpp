#include <iostream>
#include "FileManager.hpp"
#include "CliArgs.hpp"
#include <format>

int main(int argc, const char *argv[])
{
    CliArgs cli(argc, argv);
    if (!cli.isGood())
    {
        cli.printUsage();
        return 1;
    }

    FileManager fM;

    if (cli.getCliCommand().has_value())
    {
        switch (cli.getCliCommand().value())
        {
        case CliCommand::AddRecord:
            if (fM.addRecords(cli.getFile(), cli.getDataFile(), cli.getIndexFile()))
            {
                std::cout << "Student/s added successfully!" << std::endl;
            }
            else
            {
                std::cerr << "Error ocurred" << std::endl;
            }
            break;

        case CliCommand::RemoveRecord:
            if (fM.removeStudentByCode(cli.getDataFile(), cli.getAccount(), cli.getIndexFile()))
            {
                std::cout << "Student with account: (" << cli.getAccount() << ") has been eliminated successfully." << std::endl;
            }
            else
            {
                std::cerr << "Something went wrong." << std::endl;
            }

            break;

        case CliCommand::SearchRecord:
        {
            auto st = fM.searchByCode(cli.getDataFile(), cli.getAccount(), cli.getIndexFile());
            if (!st.has_value())
            {
                std::cerr << "Error.";
            }
            else
            {
                std::string header = std::format("{:<10} {:^12} {:^14} {:^5} {:<10}", "Account", "Student Name", "Phone Number", "Age", "Entry Date");
                std::cout << header << std::endl;

                std::string line = std::format("{:<10} {:^12} {:^14} {:^5} {:<10}", std::string(st.value().account, sizeof(Student::account)),
                                               st.value().name,
                                               std::string(st.value().phoneNumber, sizeof(Student::phoneNumber)),
                                               st.value().age,
                                               std::string(st.value().entryDate, sizeof(Student::entryDate)));
                std::cout << line << std::endl;
            }
            break;
        }
        case CliCommand::UpdateRecord:
            if (fM.updateStudent(cli.getDataFile(), cli.getFile(), cli.getIndexFile()))
            {
                std::cout << "Student/s updated successfully" << std::endl;
            }
            else
            {
                std::cerr << "Something went wrong updating" << std::endl;
            }

            break;

        case CliCommand::CleanUp:
            if (fM.cleanUp(cli.getDataFile(), cli.getIndexFile()))
            {
                std::cout << "DataBase in: (" << cli.getDataFile() << ") cleaned correctly." << std::endl;
            }
            else
            {
                std::cerr << "DataBase in: (" << cli.getDataFile() << ") not cleaned." << std::endl;
            }

            break;

        default:

            break;
        }
    }

    return 0;
}