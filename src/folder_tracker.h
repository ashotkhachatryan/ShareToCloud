#ifndef FOLDER_TRACKER_H
#define FOLDER_TRACKER_H

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
        std::function<void(fs::path changedFilePath)> callback);

private:
    fs::path FolderPath;
    std::function<void(fs::path)> Callback;
    std::vector<fs::path> filesInFolder;

    void ScanFolder();
    void RunThread();
    void Check();
};

#endif // FOLDER_TRACKER_H