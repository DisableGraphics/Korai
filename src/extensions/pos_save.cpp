#include <fstream>
#include <functional>
#include <webkit2/webkit-web-extension.h>
#include <JavaScriptCore/JavaScript.h>
#include <iostream>
#include <thread>
#include <filesystem>

#define SLEEP_PERIOD 5

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
  
  /*  I beg to dream and differ from the hollow lies - the first person who guesses where is this quote from will be followed by me in GitHub
      
      This fuckery that appears here is mostly a crude attempt at making a way to communicate between extensions
      (Telling the other extensions to close, mostly)
  */
  while(true && !std::filesystem::exists((std::string) std::filesystem::current_path() + "/poslck"))
  {
    sleep(SLEEP_PERIOD);

    o.open(std::filesystem::current_path().string() + "/pos.conf");
    //Yes, I know it's deprecated, but the JavaScriptCore documentation is far too lacking
    o <<  webkit_dom_dom_window_get_scroll_y(win);
    o.close();
  } 
}

static void 
window_object_cleared_callback (WebKitScriptWorld *world, 
                                WebKitWebPage *web_page, 
                                WebKitFrame *frame, 
                                gpointer user_data)
{ 

  std::thread dothesaving(std::bind(save_pos, web_page));
  dothesaving.detach();
}

extern "C" G_MODULE_EXPORT void
webkit_web_extension_initialize (WebKitWebExtension *extension)
{
    std::cout << "[INFO] Extension initialized: savepos\n";
    
    g_signal_connect (webkit_script_world_get_default(),
                      "window-object-cleared", 
                      G_CALLBACK (window_object_cleared_callback), 
                      NULL);
}
