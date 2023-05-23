#include "folder_tracker.h"
#include "google_authenticator.h"
#include "constants.h"
#include "google_drive.h"
#include "filesystem"

#include <queue>

namespace fs = std::filesystem;

void PrintHelp()
{
    std::stringstream ss;
    ss << "Usage: ShareToCloud [OPTION]...\n";
    ss << "  -s, --client-secrets       Client Secrets json file path\n";
    ss << "  -d, --directory            Local directory that needs to be tracked\n";
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

    std::string directory_path;
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
        else if (command == "-d" || command == "--directory")
        {
            directory_path = commands.front();
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

    if (client_secrets_path.empty())
    {
        fs::path exePath(argv[0]);
        for (const auto& file : fs::directory_iterator(exePath.parent_path()))
        {
            if (file.path().filename().string().starts_with("client_secret"))
            {
                client_secrets_path = file.path().string();
            }
        }
    }

    std::cout << client_secrets_path << std::endl;
    if (directory_path.empty() || client_secrets_path.empty())
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

        FolderTracker tracker(directory_path, [&drive, &gdrive_path](std::filesystem::path file) {
            drive.UploadFile(file.string(), gdrive_path);
        });
    }

    return 0;
}
