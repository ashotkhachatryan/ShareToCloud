#include "FolderTracker.h"
#include "GoogleManager/google_authenticator.h"
#include "GoogleManager/constants.h"
#include "GoogleManager/google_drive.h"

#include <queue>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Please provide client secrets json file path\n";
        return -1;
    }

    std::queue<std::string> commands;
    for (int i = 0; i < argc; i++)
    {
        commands.push(argv[i]);
    }

    std::string FolderPath;
    std::string GdrivePath = "/root";
    while (!commands.empty())
    {
        std::string command = commands.front();
        commands.pop();

        if (command == "-h" || command == "--help")
        {

        }
        else if (command == "-s" || command == "--client-secrets")
        {

        }
        else if (command == "-f" || command == "--folder")
        {
            FolderPath = commands.front();
            commands.pop();
        }
        else if (command == "-p" || command == "--google-drive-path")
        {
            GdrivePath = commands.front();
            commands.pop();
        }
        else
        {
        }
    }

    GoogleAuthenticator auth{
        ClientSecret::FromJson(argv[1]),
        {Scopes::DriveMetadataReadonly, Scopes::DrivePhotosReadonly}
    };

    auto credentials = auth.Authenticate();
    if (credentials.has_value()) {
        GDrive drive(credentials.value());

        FolderTracker tracker(FolderPath, [&drive, &GdrivePath](std::filesystem::path file) {
            drive.UploadFile(file.string(), GdrivePath);
        });
    }

    return 0;
}
