#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <string>

void ListFiles(const std::string& path)
{
    DIR* directory = ::opendir(path.c_str());
    if (directory == NULL)
    {
        std::cerr << "Error opening directory: " << path << std::endl;
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(directory)) != NULL)
    {
        std::string filename = entry->d_name;
        std::string fullPath = path + "/" + filename;

        struct stat fileInfo;
        if (stat(fullPath.c_str(), &fileInfo) == -1)
        {
            std::cerr << "Error getting file info for: " << fullPath << std::endl;
            continue;
        }

        if (S_ISDIR(fileInfo.st_mode))
        {
            // Ignore "." and ".." directories
            if (filename != "." && filename != "..")
            {
                std::cout << "Directory: " << fullPath << std::endl;
                ListFiles(fullPath); // Recursive call to list files in subdirectory
            }
        }
        else
        {
            std::cout << "File: " << fullPath << std::endl;
        }
    }

    closedir(directory);
}

int main()
{
    

    std::string path = "/gogog/carrot";
    size_t slash_pos = path.find_last_of("/");
    if(slash_pos != std::string::npos && slash_pos == path.length() -1){
        std::cout << "its a directory !\n";
    }else {
        std::cout << "its not a directory\n";
    }

    return 0;
}