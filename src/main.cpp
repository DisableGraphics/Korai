#include "args.h"
#include "main_window.hpp"
#include <cstdlib>

int main(int argc, char ** argv)
{
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create();
    args::vector2d defsize{-1, -1};
    bool tutorial{false}, fullscreen{false}, goBack{false};
    args::parseArgs(argc, argv, fullscreen, tutorial, defsize, saveFile, goBack);
    if(goBack)
    {
        return EXIT_SUCCESS;
    }
    MainWindow win{defsize, tutorial, fullscreen, app};
    return app->run(win);
}