#include "StartUp.h"

CStart gStart;

// Function to read lines from a file
std::vector<std::string> readFileLines(const std::string& filename) {
    std::vector<std::string> lines;
    std::ifstream file(filename);
    std::string line;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        file.close();
    }
    else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }

    return lines;
}
// Function to create a file and add a few lines if it doesn't exist
void createFileIfNotExist(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::ofstream newFile(filename);
        if (newFile.is_open()) {
            newFile << "engine_no_focus_sleep 0\n";
            newFile << "fps_max 30\n";
            newFile << "mat_queue_mode 1\n";
            newFile << "mat_viewportscale 0.001\n";
            newFile << "mat_filterlightmaps 0\n";
            newFile << "mat_filtertextures 0\n";
            newFile << "echo RealityhookTM\n";
            newFile.close();
            std::cout << "Created file: " << filename << " with a few lines" << std::endl;
        }
        else {
            std::cerr << "Unable to create file: " << filename << std::endl;
        }
    }
}
// Function to execute commands from a list of files
void CStart::executeCommandsFromFileList(const std::string& fileList) {
    createFileIfNotExist(fileList);
    // Read the main file to get the list of file names
    std::vector<std::string> files = readFileLines(fileList);

    for (const std::string& filename : files) {
        // Read each file from the list
        std::vector<std::string> commands = readFileLines(filename);

        for (const std::string& command : commands) {
            gInts.Engine->ClientCmd_Unrestricted(command.c_str());
        }
    }
}