#pragma once
#include <gtkmm.h>
#include <webkit2/webkit2.h>
#include "functions.hpp"
#include <thread>
#include "icon.xpm"

//Opens a chapter and loads it into webView
inline void open_chapter(WebKitWebView * webView)
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

//Opens a chapter and loads it into 'webview'. Changes 'titlebar's subtitle to the name of the manga and the chapter name
inline void open(WebKitWebView * webview, Gtk::HeaderBar * titlebar)
{
  Glib::RefPtr<Gtk::FileFilter> supported_file_types = Gtk::FileFilter::create(); 
  supported_file_types->set_name("All supported file types");
  for(std::string ext : comp::supportedCompressedExtensions)
  {
    supported_file_types->add_pattern("*" + ext);
  }
  for(std::string ext : comp::supportedImageExtensions)
  {
    supported_file_types->add_pattern("*" + ext);
  }
  Gtk::FileChooserDialog o{"Select a manga/comic file"};
  Glib::RefPtr<Gtk::FileFilter> cbz = Gtk::FileFilter::create();
  cbz->set_name("CBZ archive");
  cbz->add_pattern("*.cbz");
  Glib::RefPtr<Gtk::FileFilter> cbr = Gtk::FileFilter::create();
  cbr->set_name("CBR archive");
  cbr->add_pattern("*.cbr");
  o.add_filter(supported_file_types);
  o.add_filter(cbz);
  o.add_filter(cbr);

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
      std::thread t(save);
      t.detach();
      break;
  }
}

//Goes to the next chapter and loads it into 'webView'. Changes 'titlebar's subtitle to the name of the manga and the chapter name
inline void next_chapter(WebKitWebView * webView, Gtk::HeaderBar * titleBar)
{
  if(position > -1){
    if(comp::isCompressed(file)){
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
    else if(comp::isImage(file))
    {
      std::string folders{getMangaFolderForImages(file)};
      std::vector <std::string> foldersInFolder{getFoldersInFolder(folders)};
      position++;
      if(position == foldersInFolder.size())
      {
        position = 0;
      }
      folder = findFolderInFolder(position);
      
      file = findFileInFolder(0);
      open_chapter(webView);
      titleBar->set_subtitle(getMangaName() + " - " + getChapterName());
      gtk_widget_grab_focus(GTK_WIDGET(webView));
    }
    std::thread t(save);
    t.detach();
  }
  else if(position == -2)
  {
    webkit_web_view_go_forward(webView);
  }
}

//Goes to the previous chapter and loads it into 'webView'. Changes 'titlebar's subtitle to the name of the manga and the chapter name
inline void previous_chapter(WebKitWebView * webView, Gtk::HeaderBar * titleBar)
{
  if(position > -1){
    if(comp::isCompressed(file)){
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
    else if(comp::isImage(file))
    {
      std::string folders{getMangaFolderForImages(file)};
      std::vector <std::string> foldersInFolder{getFoldersInFolder(folders)};
      position--;
      if(position < 0)
      {
        position = foldersInFolder.size() - 1;
      }
      folder = findFolderInFolder(position);
      
      file = findFileInFolder(0);
      open_chapter(webView);
      titleBar->set_subtitle(getMangaName() + " - " + getChapterName());
      gtk_widget_grab_focus(GTK_WIDGET(webView));
    }
    std::thread t(save);
    t.detach();
  } 
  else if (position == -2)
  {
    webkit_web_view_go_back(webView);
  }
}

//Deletes the manga
inline void delete_manga(WebKitWebView * webview, Gtk::HeaderBar * headbar, Gtk::Popover * menu)
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
      if(comp::isCompressed(file)){
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
      else if(comp::isImage(file))
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
      std::thread t(save);
      t.detach();
    } 
  }
    
  menu->hide();
}
//Closes the manga
inline void close_manga(WebKitWebView * webView, Gtk::HeaderBar * headbar, Gtk::Popover * menu)
{
  file = "";
  folder = "";
  position = -1;
  headbar->set_subtitle("");

  std::thread t(save);
  t.detach();

  load_homepage(webView);
  menu->hide();
}
//Executed when the user clicks on the "about" button
inline void about()
{
  //Pretty much miscellaneous things
  Gtk::AboutDialog dialog;
  Gtk::Box * box = dialog.get_content_area();
  dialog.set_authors({"DisableGraphics"});
  dialog.set_website("https://github.com/DisableGraphics/Korai");
  dialog.set_program_name("Korai");
  Glib::RefPtr<Gdk::Pixbuf> pix = Gdk::Pixbuf::create_from_xpm_data(icon);
  dialog.set_logo(pix);
  dialog.set_license_type(Gtk::LICENSE_GPL_3_0);
  dialog.set_copyright("Made by DisableGraphics");
  Gtk::Label label;
  //It's funny that the longest line on this method is the link to my Patreon
  label.set_markup("<a href ='https://www.patreon.com/DisableGraphics' title =''>Donate</a>");
  box->pack_start(label);
  dialog.show_all();
  Gtk::Widget * but = dialog.get_action_area()->get_children()[1]; //The "License" button wouldn't hide automatically, so I forced it to
  but->hide();
  dialog.run();
}
//Reloads the MIME database, which has been failing in recent versions of WebKitGTK
inline void reloadMIME(WebKitWebView * webView)
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
//Pretty self-explanatory
inline void open_mangadex(WebKitWebView * webView, Gtk::HeaderBar * titleBar)
{
  webkit_web_view_load_uri(webView, "https://mangadex.org");
  position = -2; //Needs to be different from -1. 
  file = "";
  folder = "";
  titleBar->set_subtitle("MangaDex");
}

//This is executed when there's a keypress
inline bool on_key_pressed(GdkEventKey* event, WebKitWebView * webView, Gtk::HeaderBar * titleBar)
{
  switch(event->hardware_keycode)
  {
    case 114: //Right arrow
      if(position > -2)
      {
        next_chapter(webView, titleBar);
      }
      break;
    case 113: //Left arrow
      if(position > -2)
      {
        previous_chapter(webView, titleBar);
      }
      break;
    case 32: //'o' key
      if(position > -2){
        open(webView, titleBar);
      }
      break;
  }
  return false;
}

//When korai is closed, this is executed
inline bool on_close(GdkEventAny* event, Glib::RefPtr<Gtk::Application> app)
{
  //Saves the chapter in chapter_file
  save();

  //Deletes unnecesary folders and files
  std::filesystem::remove_all((std::string)std::filesystem::current_path() + "/tmp/");
  std::filesystem::remove((std::string)std::filesystem::current_path() + "/index.html");
  std::filesystem::remove((std::string)std::filesystem::current_path() + "/tutorial.html");

  //Closes the application
  app->quit();
  //This was needed for something, but I don't remember why
  return false;
}