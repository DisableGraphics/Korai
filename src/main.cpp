#include "gdkmm/pixbuf.h"
#include "gdkmm/rgba.h"
#include "glibmm/refptr.h"
#include "gtkmm/aboutdialog.h"
#include "gtkmm/accelgroup.h"
#include "gtkmm/dialog.h"
#include "gtkmm/enums.h"
#include "gtkmm/filechooser.h"
#include "gtkmm/filechooserdialog.h"
#include "gtkmm/filefilter.h"
#include "gtkmm/headerbar.h"
#include <chrono>
#include <cstdlib>
#include <gtkmm.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <tuple>
#include <webkit2/webkit2.h>
#include <webkitdom/webkitdom.h>
#include "gtkmm/image.h"
#include "gtkmm/label.h"
#include "gtkmm/menu.h"
#include "gtkmm/menubutton.h"
#include "gtkmm/menuitem.h"
#include "gtkmm/modelbutton.h"
#include "gtkmm/popover.h"
#include "gtkmm/scrolledwindow.h"
#include "help.h"
#include "web_contents.h"
#include "args.h"
#include "sigc++/functors/ptr_fun.h"
#include "externalFunctions.h"
#include "strnatcmp.hpp"

//Position in the folder
int position{-1};
//Variables to know different details
std::string file{""}, folder{""}, saveFile;

//Returns the files in a folder 'folda'
std::vector<std::string> getFilesInFolder(std::string folda)
{
  std::vector<std::string> toreturn;
  for (const auto & file : std::filesystem::recursive_directory_iterator(folda))
  {
    if(!std::filesystem::is_directory(file.path()))
    {
      if(file.path().extension() == ".cbz" || file.path().extension() == ".jpg" || file.path().extension() == ".png" || file.path().extension() == ".gif" || file.path().extension() == ".jfif" || file.path().extension() == ".webp" || file.path().extension() == ".jpeg" || file.path().extension() == ".svg")
      {
        toreturn.push_back(file.path());
      }
    }
  }
  std::sort(toreturn.begin(), toreturn.end(), compareNat);
  return toreturn;
}

//Sets the position.
int setPosition(std::string folda, std::string fayel)
{
  //Searches for a file 'fayel' in a folder 'folda' and returns its position
  std::vector<std::string> filesInFolda{getFilesInFolder(folda)};
  for(int i{0}; i < filesInFolda.size(); i++)
  {
    if(fayel == filesInFolda[i])
    {
      return i;
    }
  }
  return -1;
}

//Returns the folder in which 'filename' is
std::string getFolder(std::string filename)
{
  int slashPos{0};
  for(int i{static_cast<int>(filename.size() -1)}; i >= 0; i--)
  {
    if(filename[i] == '/')
    {
      slashPos = i;
      break;
    }
  }
  std::string tmp{""};
  for(int i{0}; i < slashPos; i++)
  {
    tmp += filename[i];
  }
  tmp += "/";
  return tmp;
}

//Decompresses the filename
void decompress(std::string filename)
{
  std::string delete_command{"rm -rf " + (std::string)std::filesystem::current_path() + "/tmp/*"};
  system(delete_command.c_str());
  std::string command{"unzip -oqq \'" + filename + "\' -d \'" + (std::string)std::filesystem::current_path() + "/tmp/\'"};
  system(command.c_str());
}

//Generates the page for a chapter
std::string generateWebPage()
{
  decompress(file);
  std::vector<std::string> filesInFolda{getFilesInFolder((std::string)std::filesystem::current_path() + "/tmp/")};
  std::string generated_html{html::initial_html};
  for(auto & fayel : filesInFolda)
  {
    generated_html += "<img src=\"" + fayel + "\" loading=\"lazy\"><br>\n"; //Lazy loading to improve performance
  }
  generated_html += "</body> </html>";
  return generated_html;
}

//Opens a chapter and loads it into webView
void open_chapter(WebKitWebView * webView)
{
  std::string web_contents{generateWebPage()};

  std::ofstream web_file;
  std::string currentFolder{std::filesystem::current_path()};
  web_file.open(currentFolder + "/tmp/page.html");
  web_file << web_contents;
  web_file.close();

  std::string uri{"file://" + currentFolder + "/tmp/page.html"};

  webkit_web_view_load_uri(webView, uri.c_str());
  //webkit_web_view_reload(webView);
}

//Finds a file in a folder given a position
std::string findFileInFolder(int pos)
{
  std::vector<std::string> files{getFilesInFolder(folder)};
  return files[pos];
}

//Gets the manga name from 'file'
std::string getMangaName()
{
  int firstSlashPos{-1}, secondSlashPos{-1};
  for(int i{static_cast<int>(file.size())}; i >= 0; i--)
  {
    if(file[i] == '/')
    {
      if(firstSlashPos == -1)
      {
        firstSlashPos = i;
      }
      else 
      {
        secondSlashPos = i;
        break;
      }
    }
  }
  std::string r{""};
  for(int i{secondSlashPos + 1}; i < firstSlashPos; i++)
  {
    r += file[i];
  }
  return r;
}

//Gets the chapter name from 'file'
std::string getChapterName()
{
  int pos{-1};
  int dotpos{-1};
  bool searchDot{true};
  for(int i{static_cast<int>(file.size())}; i >= 0; i--)
  {
    if(file[i] == '.' && searchDot)
    {
      searchDot = false;
      dotpos = i;
    }
    if(file[i] == '/')
    {
      pos = i;
      break;
    }
  }
  std::string r;
  for(int i{pos + 1}; i < dotpos; i++)
  {
    r += file[i];
  }
  return r;
}

//Goes to the next chapter and loads it into 'webView'. Changes 'titlebar's subtitle to the name of the manga and the chapter name
void next_chapter(WebKitWebView * webView, Gtk::HeaderBar * titleBar)
{
  if(position != -1){
    std::vector<std::string> filesInFolder{getFilesInFolder(folder)};
    position++;
    if(position == filesInFolder.size())
    {
      position = 0;
    }
    file = findFileInFolder(position);
    open_chapter(webView);
    titleBar->set_subtitle(getMangaName() + " - " + getChapterName());
    gtk_widget_grab_focus(GTK_WIDGET(webView));
  }
}

//Goes to the previous chapter and loads it into 'webView'. Changes 'titlebar's subtitle to the name of the manga and the chapter name
void previous_chapter(WebKitWebView * webView, Gtk::HeaderBar * titleBar)
{
  if(position != -1){
    std::vector<std::string> filesInFolder{getFilesInFolder(folder)};
    position--;
    if(position < 0)
    {
      position = filesInFolder.size() -1;
    }
    file = findFileInFolder(position);
    open_chapter(webView);
    titleBar->set_subtitle(getMangaName() + " - " + getChapterName());
    gtk_widget_grab_focus(GTK_WIDGET(webView));
  }
}

//Opens a chapter and loads it into 'webview'. Changes 'titlebar's subtitle to the name of the manga and the chapter name
void open(WebKitWebView * webview, Gtk::HeaderBar * titlebar)
{
  Gtk::FileChooserDialog o{"Select a cbz file"};
  Glib::RefPtr<Gtk::FileFilter> filt = Gtk::FileFilter::create();
  filt->set_name("CBZ archive");
  filt->add_pattern("*.cbz");
  o.set_filter(filt);

  o.set_action((Gtk::FileChooserAction)GTK_FILE_CHOOSER_ACTION_OPEN);
  o.add_button("Open", GTK_RESPONSE_ACCEPT);
  o.add_button("Cancel", GTK_RESPONSE_CANCEL);
  switch(o.run())
  {
    case GTK_RESPONSE_ACCEPT:
      file = o.get_filename();
      folder = getFolder(file);
      position = setPosition(folder,file);

      open_chapter(webview);
      titlebar->set_subtitle(getMangaName() + " - " + getChapterName());
      gtk_widget_grab_focus(GTK_WIDGET(webview));
      break;
  }
}

//Loads the homepage into 'webview'
void load_homepage(WebKitWebView * webview)
{
  std::string homepage_contents{html::homepage};
  std::ofstream homepage_file;
  homepage_file.open((std::string)std::filesystem::current_path() + "/index.html");
  homepage_file << homepage_contents;
  homepage_file.close();
  std::string homepage_uri{"file://" + (std::string)std::filesystem::current_path() + "/index.html"};

  webkit_web_view_load_uri(webview, homepage_uri.c_str());

}

//Deletes the manga
void delete_manga(WebKitWebView * webview, Gtk::HeaderBar * headbar, Gtk::Popover * menu)
{
  if(position != -1){
    Gtk::Dialog delete_dialog {"Are you sure?"};
    Gtk::Box * box = delete_dialog.get_content_area();

    Gtk::Image img;
    img.set_from_icon_name("dialog-warning", (Gtk::IconSize)GTK_ICON_SIZE_DIALOG);

    box->pack_start(img);

    Gtk::Label label{"Are you sure?"};
    box->pack_end(label);

    
    delete_dialog.add_button("Cancel", GTK_RESPONSE_CANCEL);
    delete_dialog.add_button("Delete", GTK_RESPONSE_ACCEPT);

    delete_dialog.show_all();
  
    if(delete_dialog.run() == GTK_RESPONSE_ACCEPT)
    {
      if(std::filesystem::exists(folder) && folder != "")
      {
        std::filesystem::remove_all(folder);
        file = "";
        folder = "";
        position = -1;
        headbar->set_subtitle("");

        load_homepage(webview);
      }
    }
  }
  menu->hide();
}

//Closes the manga
void close_manga(WebKitWebView * webView, Gtk::HeaderBar * headbar, Gtk::Popover * menu)
{
  file = "";
  folder = "";
  position = -1;
  headbar->set_subtitle("");

  load_homepage(webView);
  menu->hide();
}

//When korai is closed, this is executed
bool on_close(GdkEventAny* event, Glib::RefPtr<Gtk::Application> app)
{
  //Saves the chapter in chapter_file
  std::ofstream chapter_file;
  chapter_file.open(saveFile);

  chapter_file << file;
  chapter_file.close();

  //Deletes unnecesary folders and files
  std::filesystem::remove_all((std::string)std::filesystem::current_path() + "/tmp/");
  std::filesystem::remove((std::string)std::filesystem::current_path() + "/index.html");
  std::filesystem::remove((std::string)std::filesystem::current_path() + "/tutorial.html");

  //Closes the application
  app->quit();
  //This was needed for something, but I don't remember why
  return false;
}

//When korai is opened, this is executed
void on_load(WebKitWebView * webView, Gtk::HeaderBar& titlebar)
{
  //Loads the chapter file
  std::ifstream chapter_file;
  chapter_file.open(saveFile);
  //If it exists, sets the manga to the one dictated in the chapter_file
  if(chapter_file.good()){
    getline(chapter_file, file);
    if(std::filesystem::exists(file))
    {
      folder = getFolder(file);
      position = setPosition(folder,file);
      open_chapter(webView);
      titlebar.set_subtitle(getMangaName() + " - " + getChapterName());
    }
  }
  chapter_file.close();
  //Creates the 'tmp' directory
  std::filesystem::create_directory(((std::string)std::filesystem::current_path() + "/tmp/"));
}

//Executed when the user clicks on the "about" button
void about()
{
  Gtk::AboutDialog dialog;
  dialog.set_authors({"DisableGraphics"});
  dialog.set_license_type(Gtk::LICENSE_GPL_3_0);
  dialog.set_website("https://github.com/DisableGraphics/Korai");
  dialog.set_program_name("Korai");
  dialog.set_license("Licensed under the GPL v3");
  dialog.set_comments("Next generation manga reader");
  dialog.show_all();
  dialog.run();
}

//Reloads the MIME database, which has been failing in recent versions of WebKitGTK
void reloadMIME(WebKitWebView * webView)
{
  system("rm ~/.local/share/mime/packages/user-extension-html.xml");
  system("update-mime-database ~/.local/share/mime");

  //Notifies the user that the database has finished reloading
  Gtk::Dialog okdialog{"Finished reloading"};
  Gtk::Button * button = okdialog.add_button("OK", GTK_RESPONSE_ACCEPT);
  button->set_hexpand();
  Gtk::Image img;
  img.set_from_icon_name("dialog-information", (Gtk::IconSize)GTK_ICON_SIZE_DIALOG);
  Gtk::Box * box;

  box = okdialog.get_content_area();
  
  box->pack_start(img);
  Gtk::Label okmessage{"Finished reloading the database"};
  box->pack_start(okmessage);
  
  okdialog.show_all();

  switch(okdialog.run())
  {
    case GTK_RESPONSE_ACCEPT:
      okdialog.close();
      break;
  }
  webkit_web_view_reload(webView);
}

//This is executed when there's a keypress
bool on_key_pressed(GdkEventKey* event, WebKitWebView * webView, Gtk::HeaderBar * titleBar)
{
  switch(event->hardware_keycode)
  {
    case 114: //Right arrow
      next_chapter(webView, titleBar);
      break;
    case 113: //Left arrow
      previous_chapter(webView, titleBar);
      break;
    case 32: //'o' key
      open(webView, titleBar);
      break;
  }
  return false;
}

int main( int argc, char **argv)
{
  //Bunch of bools used for some things

  //Loads the tutorial if true
  bool tutorial{false};
  //Maximizes the window if true
  bool fullscreen{false};
  //Returns 0 if true
  bool goBack{false};

  std::string currentFolder{std::filesystem::current_path()};
  saveFile = (std::string)std::filesystem::current_path() + "/chapter.conf";
  //Default size. I find 628x650 very comfortable.
  args::vector2d defsize{628, 650};
  //Looks at the args
  if(argc >= 2)
  {
    args::parseArgs(argc, argv, fullscreen, tutorial, defsize, saveFile, goBack);
  }
  if(goBack)
  {
    return 0;
  }
  
  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create();
  Gtk::HeaderBar titleBar;
  titleBar.set_title("Korai");
  Gtk::HBox buttonsBox;
  Gtk::Popover menu;
  titleBar.pack_start(buttonsBox);
  Gtk::MenuButton menubutton;
  
  menubutton.set_image_from_icon_name("open-menu-symbolic");
  Gtk::Button nextButton, previousButton, openButton;
  Gtk::Button deleteButton, closeButton;
  Gtk::Button reloadMIMEbutton;
  Gtk::Button aboutButton;
  deleteButton.set_relief(Gtk::RELIEF_NONE);
  closeButton.set_relief(Gtk::RELIEF_NONE);
  reloadMIMEbutton.set_relief(Gtk::RELIEF_NONE);
  aboutButton.set_relief(Gtk::RELIEF_NONE);
  Gtk::VBox menuBox;

  menuBox.pack_start(closeButton);
  menuBox.pack_start(deleteButton);
  menuBox.pack_start(reloadMIMEbutton);

  menuBox.pack_end(aboutButton); //This button must be always on the end

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
  
  buttonsBox.pack_start(previousButton);
  buttonsBox.pack_start(openButton);
  buttonsBox.pack_start(nextButton);

  WebKitSettings * settings = WEBKIT_SETTINGS(webkit_settings_new());
  webkit_settings_set_enable_smooth_scrolling(settings, TRUE);
  webkit_settings_set_allow_file_access_from_file_urls(settings, TRUE);
  webkit_settings_set_allow_universal_access_from_file_urls(settings, TRUE);
  webkit_settings_set_auto_load_images(settings, TRUE);
  webkit_settings_set_enable_javascript(settings, TRUE);
  Gtk::Window window;
  window.set_default_size( defsize.x, defsize.y );

  if(fullscreen)
  {
    window.maximize();
  }
  
  WebKitWebView * webview =  WEBKIT_WEB_VIEW( webkit_web_view_new_with_settings(settings) );
  /*
   * the next line does some tricks :
   * GTK_WIDGET( one ) -> convert WebKitWebView to GtkWidget (one->two)
   * Glib::wrap( GTK_WIDGET( one ) ) -> convert GtkWidget to Gtk::Widget (two->three)
   */
  Gtk::Widget * webview_widget = Glib::wrap( GTK_WIDGET( webview ) );
  if(tutorial)
  {
     help::tutorial(webview);
     titleBar.set_subtitle("Tutorial");
  }
  else
  {
    load_homepage(webview);
    on_load(webview, titleBar);
  }
  titleBar.set_border_width(1);
  
  window.add( *webview_widget );

  window.set_titlebar(titleBar);
  titleBar.set_show_close_button();

  window.show_all();
  
  webkit_web_view_reload(webview);
  openButton.signal_clicked().connect(sigc::bind(sigc::ptr_fun(open), webview, &titleBar));
  nextButton.signal_clicked().connect(sigc::bind(sigc::ptr_fun(next_chapter), webview, &titleBar));
  previousButton.signal_clicked().connect(sigc::bind(sigc::ptr_fun(previous_chapter), webview, &titleBar));
  deleteButton.signal_clicked().connect(sigc::bind(sigc::ptr_fun(delete_manga), webview, &titleBar, &menu));
  closeButton.signal_clicked().connect(sigc::bind(sigc::ptr_fun(close_manga), webview, &titleBar, &menu));
  reloadMIMEbutton.signal_clicked().connect(sigc::bind(sigc::ptr_fun(reloadMIME), webview));
  aboutButton.signal_clicked().connect(sigc::ptr_fun(about));
  window.signal_key_press_event().connect(sigc::bind(sigc::ptr_fun(on_key_pressed), webview, &titleBar), false);

  window.signal_delete_event().connect(sigc::bind(sigc::ptr_fun(&on_close), app));

  app->run( window );
  exit( 0 );
}

