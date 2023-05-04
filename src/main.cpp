#include "FolderTracker.h"
#include "GoogleManager/google_authenticator.h"
#include "GoogleManager/constants.h"
#include "GoogleManager/google_drive.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Please provide client secrets json file path\n";
        return -1;
    }

    GoogleAuthenticator auth{
        ClientSecret::FromJson(argv[1]),
        {Scopes::DriveMetadataReadonly, Scopes::DrivePhotosReadonly}
    };

    std::string FolderPath = ""; // THIS IS GOING TO BE THE FOLDER PATH TO TRACK
    std::string GdrivePath = "/root"; // THIS IS GOING TO BE THE GOOGLE DRIVE PATH
    auto credentials = auth.Authenticate();
    if (credentials.has_value()) {
        GDrive drive(credentials.value());

        FolderTracker tracker(FolderPath, [&drive, &GdrivePath](std::filesystem::path file) {
            drive.UploadFile(file.string(), GdrivePath);
        });
    }

    return 0;
}
