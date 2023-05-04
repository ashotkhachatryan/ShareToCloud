#include <iostream>
#include <filesystem>
#include <functional>
#include <vector>
#include <thread>
#include <chrono>
#include <cassert>

namespace fs = std::filesystem;

class FolderTracker {
public:
    FolderTracker(fs::path path,
                  std::function<void(fs::path changedFilePath)> callback)
        : FolderPath(path), Callback(callback)
    {
        ScanFolder();

        std::thread t(&FolderTracker::RunThread, this);
        t.join();
    }

private:
    fs::path FolderPath;
    std::function<void(fs::path)> Callback;
    std::vector<fs::path> filesInFolder;

    void ScanFolder() {
        for (const auto& entry : fs::directory_iterator(FolderPath)) {
            filesInFolder.push_back(entry);
        }
    }

    void RunThread() {
        using namespace std::chrono_literals;
        while (true) {
            Check();
            std::this_thread::sleep_for(2000ms);
        }
    }

    void Check() {
        for (const auto& entry : fs::directory_iterator(FolderPath)) {
            if (std::find(filesInFolder.begin(), filesInFolder.end(), entry) == filesInFolder.end()) {
                // Check if entry is a file
                Callback(entry);
                filesInFolder.push_back(entry);
            }
        }
    }
};