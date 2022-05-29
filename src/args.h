#pragma once
#include <cstring>
#include <filesystem>
#include <iostream>
#include "comp.hpp"
#include "help.h"
#include "global_variables.hpp"
#include "functions.hpp"
namespace args{
    //Two dimensional int
    typedef struct{
        int x, y;
    } vector2d; //Used as a "cleaner" way to select the size of the korai window

    inline vector2d extractSizes(char * text)
    {
        //Gets the sizes from a string in the form <width>x<height>
        //Finds an 'x' and then all the numbers before it are the width and all the numbers after it are the height
        int xpos{0};
        for(int i{0}; i < sizeof(text); i++)
        {
            if (text[i] == 'x')
            {
                xpos = i;
            }
        }
        vector2d r;
        std::string tmp{""};
        for(int i{0}; i < xpos; i++)
        {
            tmp += text[i];
        }
        r.x = std::stoi(tmp);
        tmp.clear();

        for(int i{xpos + 1}; i < sizeof(text); i++)
        {
            tmp += text[i];
        }
        r.y = std::stoi(tmp);
        return r;
    }
    //Parses the arguments. Much better here than in the main.cpp file
    inline void parseArgs(int argc, char ** argv, bool &fullscreen, bool &tutorial, vector2d &defsize, std::string &saveFile, bool &goBack, bool &checkdownloader)
    {
        for(int i{1}; i < argc; i++)
        {
            if(strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--size") == 0)
            {
                i++; //To obtain the correct value. (So that the size is not "-s")
                defsize = args::extractSizes(argv[i]);
            }
            else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
            {
                help::displayHelp();
                goBack = true; //This variable tells Korai to "return 0;" in main() if it's true
            }
            else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--save-file") == 0)
            {
                i++;
                saveFile = argv[i];
            }
            else if(strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "--maximized") == 0)
            {
                fullscreen = true;
            }
            else if(strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--tutorial") == 0)
            {
                tutorial = true;
            }
            else if(strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "--extension") == 0)
            {
                std::filesystem::create_directory((std::string) std::filesystem::current_path() + "/korai-extensions");
                for (const auto & entry : std::filesystem::directory_iterator((std::string) std::filesystem::current_path()))
                {
                    
                    if(entry.path().extension() == ".so")
                    {
                        std::cout << entry.path() << std::endl;
                        std::filesystem::copy(entry.path(), (std::string) std::filesystem::current_path() + "/korai-extensions");
                        std::filesystem::remove(entry.path());
                    }
                }
            }
            #ifndef NODOWNLOAD
            else if(strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--check") == 0)
            {
                checkdownloader = true;
            }
            #endif
            else if(std::filesystem::exists(argv[i]) && !std::filesystem::is_directory(argv[i]))
            {
                if(comp::isCompressed(argv[i]))
                {
                    file = argv[i];
                    folder = getFolder(file);
                    position = setPosition(folder, file);
                }

            }
            else //Error
            {
                std::cout << "Argument \"" << argv[i] << "\" not recognized\n";
                goBack = true;
            }
        }
    }
}