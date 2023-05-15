#include "folder_tracker.h"

FolderTracker::FolderTracker(fs::path path,
    std::function<void(fs::path changedFilePath)> callback)
    : FolderPath(path), Callback(callback)
{
    ScanFolder();

    std::thread t(&FolderTracker::RunThread, this);
    t.join();
}

void FolderTracker::ScanFolder() {
    for (const auto& entry : fs::directory_iterator(FolderPath)) {
        filesInFolder.push_back(entry);
    }
}

void FolderTracker::RunThread() {
    using namespace std::chrono_literals;
    while (true) {
        Check();
        std::this_thread::sleep_for(2000ms);
    }
}

void FolderTracker::Check() {
    for (const auto& entry : fs::directory_iterator(FolderPath)) {
        if (std::find(filesInFolder.begin(), filesInFolder.end(), entry) == filesInFolder.end()) {
            // Check if entry is a file
            Callback(entry);
            filesInFolder.push_back(entry);
        }
    }
}