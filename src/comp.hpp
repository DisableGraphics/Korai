#include <iostream>
#include <filesystem>
#include <vector>

namespace comp
{
    inline bool isImage(std::filesystem::path path)
    {
        std::vector<std::string> supportedExtensions{".png", ".jpeg", ".jpg", ".jfif", ".gif", ".webp", ".svg"};
        for (auto extension : supportedExtensions)
        {
            if(path.extension() == extension)
            {
                return true;
            }
        }
        return false;
    }
    inline bool isCompressed(std::filesystem::path path)
    {
        std::vector<std::string> supportedExtensions{".cbz", ".cbr", ".rar", ".zip"};
        for (auto extension : supportedExtensions)
        {
            if(path.extension() == extension)
            {
                return true;
            }
        }
        return false;
    }
}