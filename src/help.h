#pragma once
#include <iostream>
#include <sys/ioctl.h>
#include <stdio.h>
#include <webkit2/webkit2.h>
#include <gtkmm.h>
#include "gtkmm/headerbar.h"
#include "web_contents.h"
#include <fstream>
#include <filesystem>

namespace help{
    inline void displayHelp() //Displays the help
    {
        struct winsize w;
        ioctl(0, TIOCGWINSZ, &w);
        for(int i{0}; i < w.ws_col; i++) //Writes '-' to fill the terminal (Looks pretty good)
        {
            std::cout << "-";
        }
        std::cout << "\n";
        std::cout << "Korai: Next generation manga reader\t Author: DisableGraphics\n\nLicensed under the GNU GPL v3.\n\n";
        std::cout << "Options:\n\t-s\t--size\t\tChange window size. In wxh format. Eg: 400x300\n";
        std::cout << "\t-f\t--save-file\tChange the default save file path\n";
        std::cout << "\t-m\t--maximized\tMaximize the window\n";
        std::cout << "\t-t\t--tutorial\tRun a tutorial\n";
        #ifndef NODOWNLOAD
        std::cout << "\t-c\t--check\t\tCheck if 'mangadex-downloader' is installed\n";
        #endif

        std::cout << "\t-h\t--help\t\tDisplay help\n";
        std::cout << "\n";
        for(int i{0}; i < w.ws_col; i++)
        {
            std::cout << "-";
        }
    }
    inline void tutorial(WebKitWebView * webView, Gtk::Popover * popover, int * position, std::string * file, std::string * folder, Gtk::HeaderBar * titlebar) //Creates the tutorial and loads it
    {
        *file = "";
        *position = -1;
        *folder = "";
        std::string tutorial{""};
        tutorial += "<html>";
        tutorial += "<style>" + css::css + "</style>";
        tutorial += "<head> </head>";
        tutorial += "<body>";
        tutorial += "<center><h1> Tutorial</h1></center>";
        tutorial += "Welcome! To open a manga, please click on the \"Open\" button (The one in the middle of the left buttons) <br><br>";
        tutorial += "<center><h3>Shortcuts</h3></center>";
        tutorial += "<ul> <li> Press \"o\" to open a manga</li>";
        tutorial += "<li> Press \"Right arrow\" to open the next chapter</li>";
        tutorial += "<li> Press \"Left arrow\" to open the previous chapter</li>";
        tutorial += "</ul> </body>";
        std::ofstream tutorial_file;
        tutorial_file.open((std::string)std::filesystem::current_path() + "/tutorial.html");
        tutorial_file << tutorial;
        tutorial_file.close();
        std::string tutorial_uri{"file://" + (std::string)std::filesystem::current_path() + "/tutorial.html"};
        webkit_web_view_load_uri(webView, tutorial_uri.c_str());
        popover->hide();
        titlebar->set_subtitle("Tutorial");

    }
}