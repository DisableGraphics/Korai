#include "zip.hpp"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

inline std::string reverse_string(std::string s)
{
    std::string tmp;
    for(int i{static_cast<int>(s.size() - 1)}; i > 0; i--)
    {
        tmp += s[i];
    }
    return tmp;
}

namespace zipe
{
    void extract(std::string filename, std::string folder)
    {
        libzip::archive archive(filename);
        

        for(auto & file : archive)
        {
            std::string file_n{file.name};
            if(file_n[file_n.size() - 1] == '/')
            {
                std::filesystem::create_directory(folder + file.name);
            }
            else {
                libzip::stat_info stat = archive.stat(file.name);
                libzip::file file_t = archive.open(file.name);
                std::ofstream out{folder + file.name};
                out << file_t.read(stat.size);
                out.close();
            }
        }
    }
}