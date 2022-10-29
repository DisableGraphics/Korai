#pragma once
#include "args.h"
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <gtkmm.h>
#include <webkit2/webkit2.h>
#include "icon.xpm"
#include "functions.hpp"
#include "global_variables.hpp"
#include "comp.hpp"
#include "signal_functions.hpp"
#include "downloader.hpp"
#include "web_contents.h"

class MainWindow : public Gtk::Window
{
    public:
        MainWindow(args::vector2d defsize, bool tutorial, bool fullscreen, Glib::RefPtr<Gtk::Application> app)
        {
            titleBar.set_title("Korai");
            titleBar.pack_start(buttonsBox);

            menubutton.set_image_from_icon_name("open-menu-symbolic");
            
            menuBox.pack_start(closeButton );
            menuBox.pack_start(deleteButton );
            menuBox.pack_start(sep1);
            menuBox.pack_start(reloadMIMEbutton);
            menuBox.pack_start(sep2);
            menuBox.pack_start(tutorialButton);
            menuBox.pack_start(mangadexButton);

            #ifndef NODOWNLOAD
            menuBox.pack_start(downloadButton);
            #endif 

            menuBox.pack_end(aboutButton); //This button must be at the end
            menuBox.pack_end(sepabout); //Preceeded by a separator

            menu.add(menuBox);
            menu.show_all();
            menu.hide();

            menubutton.set_popover(menu);

            titleBar.pack_end(menubutton);

            nextButton.set_image_from_icon_name("go-next-symbolic");
            previousButton.set_image_from_icon_name("go-next-symbolic-rtl");
            openButton.set_image_from_icon_name("folder-new-symbolic");

            deleteButton.set_label("Delete manga");
            closeButton.set_label("Close manga");
            reloadMIMEbutton.set_label("Reload MIME types");
            aboutButton.set_label("About");
            tutorialButton.set_label("Tutorial");
            mangadexButton.set_label("MangaDex");
            #ifndef NODOWNLOAD
            downloadButton.set_label("Download manga");
            #endif
            
            buttonsBox.pack_start(previousButton);
            buttonsBox.pack_start(openButton);
            buttonsBox.pack_start(nextButton);

            webkit_web_context_set_web_extensions_directory(webkit_web_context_get_default(), "/usr/share/korai/korai-extensions");

            WebKitSettings * settings = WEBKIT_SETTINGS(webkit_settings_new());
            webkit_settings_set_enable_smooth_scrolling(settings, TRUE);
            webkit_settings_set_allow_file_access_from_file_urls(settings, TRUE);
            webkit_settings_set_allow_universal_access_from_file_urls(settings, TRUE);
            webkit_settings_set_auto_load_images(settings, TRUE);
            webkit_settings_set_enable_javascript(settings, TRUE);
            titleBar.pack_start(spin);

            if(defsize.x == -1 && defsize.y == -1)
            {
                defsize.x = (double)(get_screen()->get_width() * 0.3925);
                defsize.y = (double)(get_screen()->get_height() * 0.722222222);
            }
            else if(defsize.x > get_screen()->get_width() || defsize.y > get_screen()->get_height())
            {
                std::cout << " \t\t\t[ERROR] Hey hey! [ERROR]\n You're trying to open a window bigger than your computer screen!\n";
                std::cout << " Your screen dimensions are: " << get_screen()->get_width() << "x" << get_screen()->get_height() << "\n";
                exit(1);
            }
            
            set_default_size( defsize.x, defsize.y );
            if(fullscreen)
            {
                maximize(); //Will maximize the window. Pretty self-explanatory
            }

            move((get_screen()->get_width()/2) - defsize.x/2, (get_screen()->get_height() / 2) - (defsize.y/2));
            
            WebKitWebView * webview =  WEBKIT_WEB_VIEW( webkit_web_view_new_with_settings(settings) );

            /*
            * the next line does some tricks :
            * GTK_WIDGET( one ) -> convert WebKitWebView to GtkWidget (one->two)
            * Glib::wrap( GTK_WIDGET( one ) ) -> convert GtkWidget to Gtk::Widget (two->three)
            */
            Gtk::Widget * webview_widget = Glib::wrap( GTK_WIDGET( webview ) );
            
            add( *webview_widget );
            Glib::RefPtr<Gdk::Pixbuf> pix = Gdk::Pixbuf::create_from_xpm_data(icon);
            set_icon(pix);

            set_titlebar(titleBar);
            titleBar.set_show_close_button();

            

            if(tutorial)
            {
                //Yes, I need these absolute f*ckton of arguments. And there's no argument copied by value. (All of them are pointers and references)
                help::tutorial(webview, &menu, &position, &file, &folder, &titleBar);
            }
            else
            {
                load_homepage(webview);
                if(file == "")
                {
                    on_load(webview, titleBar);
                }
                else
                {
                    open_chapter(webview);
                    titleBar.set_subtitle(getMangaName() + " - " + getChapterName());
                }
                
            } 

            show_all();
            
            webkit_web_view_reload(webview);

            g_signal_connect_object(webview,"load-changed",G_CALLBACK(on_load_changed), spin.gobj(),  G_CONNECT_AFTER);
            //These all the goddamn buttons. Man, not using Glade didn't pay off...
            openButton.signal_clicked().connect(sigc::bind(sigc::ptr_fun(open), webview, &titleBar));
            nextButton.signal_clicked().connect(sigc::bind(sigc::ptr_fun(next_chapter), webview, &titleBar));
            previousButton.signal_clicked().connect(sigc::bind(sigc::ptr_fun(previous_chapter), webview, &titleBar));
            deleteButton.signal_clicked().connect(sigc::bind(sigc::ptr_fun(delete_manga), webview, &titleBar, &menu));
            closeButton.signal_clicked().connect(sigc::bind(sigc::ptr_fun(close_manga), webview, &titleBar, &menu));
            reloadMIMEbutton.signal_clicked().connect(sigc::bind(sigc::ptr_fun(reloadMIME), app));
            mangadexButton.signal_clicked().connect(sigc::bind(sigc::ptr_fun(open_mangadex), webview, &titleBar, ""));
            tutorialButton.signal_clicked().connect(sigc::bind(sigc::ptr_fun(help::tutorial), webview, &menu, &position, &file, &folder, &titleBar));
            #ifndef NODOWNLOAD
            downloadButton.signal_clicked().connect(sigc::bind(sigc::ptr_fun(download_manga), webview));
            #endif

            aboutButton.signal_clicked().connect(sigc::ptr_fun(about));
            signal_key_press_event().connect(sigc::bind(sigc::ptr_fun(on_key_pressed), webview, &titleBar), false);

            signal_delete_event().connect(sigc::bind(sigc::ptr_fun(&on_close), app, webview));

        }
    private:
        Gtk::Spinner spin;

        Gtk::HeaderBar titleBar;
        Gtk::HBox buttonsBox;
        Gtk::Popover menu;
        Gtk::MenuButton menubutton;

        Gtk::Button nextButton, previousButton, openButton;
        Gtk::ModelButton deleteButton, closeButton;
        Gtk::ModelButton reloadMIMEbutton;
        Gtk::ModelButton aboutButton, tutorialButton;
        Gtk::ModelButton mangadexButton;
        #ifndef NODOWNLOAD
        Gtk::ModelButton downloadButton;
        #endif

        Gtk::VBox menuBox;
        Gtk::Separator sep1, sepabout, sep2;

};