#include <fstream>
#include <functional>
#include <webkit2/webkit-web-extension.h>
#include <JavaScriptCore/JavaScript.h>
#include <iostream>
#include <thread>
#include <filesystem>

#define SLEEP_PERIOD 5

using namespace std::filesystem;

bool t{false};

static void save_pos(WebKitWebPage * web_page)
{
  std::ofstream tmp_file;
  tmp_file.open((std::string) std::filesystem::current_path() + "/poslck");
  tmp_file << "e";
  tmp_file.close();
  sleep(SLEEP_PERIOD + 1);
  std::filesystem::remove((std::string) std::filesystem::current_path() + "/poslck");
  WebKitDOMDocument * doc = webkit_web_page_get_dom_document(web_page);
  WebKitDOMDOMWindow * win = webkit_dom_document_get_default_view(doc);

  std::ofstream o; 

  while(true && !std::filesystem::exists((std::string) std::filesystem::current_path() + "/poslck"))
  {
    sleep(SLEEP_PERIOD);
    o.open(std::filesystem::current_path().string() + "/pos.conf");
    o <<  webkit_dom_dom_window_get_scroll_y(win);
    o.close();
    std::cout << "Y: " << webkit_dom_dom_window_get_scroll_y(win) << "\nX: " << webkit_dom_dom_window_get_scroll_x(win) << "\n";

  }
  
}

//
//  Boilerplate code / signal callback for attaching methods when a
//  new javascript context is created.
//
static void 
window_object_cleared_callback (WebKitScriptWorld *world, 
                                WebKitWebPage *web_page, 
                                WebKitFrame *frame, 
                                gpointer user_data)
{ 

  if(!t)
  {
    std::thread dothesaving(std::bind(save_pos, web_page));
    dothesaving.detach();
  }
}

static void
web_page_created_callback (WebKitWebExtension *extension,
                           WebKitWebPage      *web_page,
                           gpointer            user_data)
{
  g_print ("Page %d created for %s\n",
            webkit_web_page_get_id (web_page),
            webkit_web_page_get_uri (web_page));
  
}

//
//  Extension initialization thing.
//
extern "C" G_MODULE_EXPORT void
webkit_web_extension_initialize (WebKitWebExtension *extension)
{
    std::cout << "EXTENSION INITIALIZED\n";
    
    g_signal_connect (webkit_script_world_get_default(),
                      "window-object-cleared", 
                      G_CALLBACK (window_object_cleared_callback), 
                      NULL);
    g_signal_connect (extension, "page-created",
                      G_CALLBACK (web_page_created_callback),
                      NULL);

    
}
