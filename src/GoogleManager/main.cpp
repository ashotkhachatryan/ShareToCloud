#include <iostream>
#include "client_secret.h"
#include "google_authenticator.h"
#include "httplib.h"
#include "constants.h"
#include "google_drive.h"
#include <filesystem>

using namespace std::filesystem;

//std::vector<std::string> split(const std::string& str, char delimeter)
//{
//    std::vector<std::string> result;
//    size_t begin = 0;
//    int end = str.find(delimeter, 0);
//    while (end != -1) {
//        std::string substr = str.substr(begin, end - begin);
//        result.push_back(substr);
//
//        begin = end + 1;
//        end = begin;
//
//        end = str.find(delimeter, end);
//    }
//    result.push_back(str.substr(begin, str.size() - begin));
//    return result;
//}


int main(int argc, char** argv) {
    GoogleAuthenticator auth {
        ClientSecret::FromJson(argv[1]),
        {Scopes::DriveMetadataReadonly, Scopes::DrivePhotosReadonly, Scopes::Drive}
    };

    auto credentials = auth.Authenticate();
    if (credentials.has_value()) {
        GDrive drive(credentials.value());
        std::vector<GFile> files = drive.GetFileList();
        std::cout << files.size() << std::endl;
        for (const auto& f : files) {
            std::cout << f.name << "\n";
        }

        //drive.GetFileId("/root/");

        drive.UploadFile("C:\\Users\\ashot\\Desktop\\Books\\guide-t-cp.pdf");
        //auto id = drive.GetFileId("TestFolder", "root");
        //if (id.has_value())
        //    std::cout << id.value() << std::endl;
        //else
        //    std::cout << "failed to get the value" << std::endl;
    }
}
