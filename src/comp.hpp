#pragma once
#include <iostream>
#include <filesystem>
#include <vector>
//Compares if the file is from a supported extension
namespace comp
{
    inline std::vector<std::string> supportedImageExtensions{".png", ".jpeg", ".jpg", ".jfif", ".gif", ".webp", ".svg"};
    inline std::vector<std::string> supportedCompressedExtensions{".cbz", ".cbr", ".rar", ".zip"};
    inline bool isImage(std::filesystem::path path)
    {
        
        for (auto extension : supportedImageExtensions)
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
        
        for (auto extension : supportedCompressedExtensions)
        {
            if(path.extension() == extension)
            {
                return true;
            }
        }
        return false;
    }
}