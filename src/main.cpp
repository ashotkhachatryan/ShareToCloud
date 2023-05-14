#include "FolderTracker.h"
#include "google_authenticator.h"
#include "constants.h"
#include "google_drive.h"

#include <queue>

void PrintHelp()
{
    std::stringstream ss;
    ss << "Usage: ShareToCloud [OPTION]...\n";
    ss << "  -s, --client-secrets       Client Secrets json file path\n";
    ss << "  -f, --folder               Local folder that needs to be tracked\n";
    ss << "  -p, --google-drive-path    Google Drive folder path where files will be uploaded\n";
    ss << "  -h, --help                 Display this help and exit\n";
    std::cout << ss.str();
}

int main(int argc, char** argv) {
    std::queue<std::string> commands;
    for (int i = 1; i < argc; i++)
    {
        commands.push(argv[i]);
    }

    std::string folder_path;
    std::string gdrive_path = "/root";
    std::string client_secrets_path;

    while (!commands.empty())
    {
        std::string command = commands.front();
        commands.pop();

        if (command == "-h" || command == "--help")
        {
            PrintHelp();
            return 0;
        }
        else if (command == "-s" || command == "--client-secrets")
        {
            client_secrets_path = commands.front();
            commands.pop();
        }
        else if (command == "-f" || command == "--folder")
        {
            folder_path = commands.front();
            commands.pop();
        }
        else if (command == "-p" || command == "--google-drive-path")
        {
            gdrive_path = commands.front();
            commands.pop();
        }
        else
        {
            PrintHelp();
            return -1;
        }
    }

    std::cout << folder_path << std::endl;
    std::cout << client_secrets_path << std::endl;
    if (folder_path.empty() || client_secrets_path.empty())
    {
        PrintHelp();
        return -1;
    }

    GoogleAuthenticator auth{
        ClientSecret::FromJson(client_secrets_path),
        {Scopes::DriveMetadataReadonly, Scopes::DrivePhotosReadonly, Scopes::Drive}
    };

    auto credentials = auth.Authenticate();
    if (credentials.has_value()) {
        GDrive drive(credentials.value());

        FolderTracker tracker(folder_path, [&drive, &gdrive_path](std::filesystem::path file) {
            drive.UploadFile(file.string(), gdrive_path);
        });
    }

    return 0;
}
