#include "args.h"
#include "global_variables.hpp"
#include "main_window.hpp"
#include <cstdlib>
#include <filesystem>
#include <string>

int main(int argc, char ** argv)
{
    
    saveFile = (std::string)std::filesystem::current_path() + "/chapter.conf";
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create();
    args::vector2d defsize{-1, -1};
    bool tutorial{false}, fullscreen{false}, goBack{false}, checkdownloader{false};
    args::parseArgs(argc, argv, fullscreen, tutorial, defsize, saveFile, goBack, checkdownloader);
    tmp_folder = (std::string)std::filesystem::current_path() + "/tmp/";
    if(goBack)
    {
        return EXIT_SUCCESS;
    }

    #ifndef NODOWNLOAD
    if(checkdownloader)
    {
        
        system("yes y | pip install mangadex-downloader");
        
    }
    #endif

    MainWindow win{defsize, tutorial, fullscreen, app};
    return app->run(win);
}