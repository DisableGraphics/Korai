#pragma once
#include <iostream>

namespace css
{
    //Default CSS in the web page
    inline std::string css{"" 
        "body\n"
        "{\n"
            "background-color: #1E1E1E;\n"
            "color: #DEE0DA;\n"
        "}\n"
        "img\n"
        "{\n"
            "display: block;\n"
            "margin-left: auto;\n"
            "margin-right: auto;\n"
            "width: 95%;\n"
            "max-height: calc((29.7 / 21) * 95%);\n"
        "}\n"
    };
}
namespace html
{
    //The home page when you load korai
    inline char homepage[] = "<html> <style>\nbody\n{\n\tbackground-color: #1E1E1E;\n\tcolor: #DEE0DA\n}\n</style>"
    "<body> <center><h1>Welcome to Korai</h1><h3>The next generation manga reader</h3></center> </body>"
    "</html>" 
    ;

    /*window.addEventListener("scroll", (event) => {
    let scroll = this.scrollY;
    document.writeline(scroll)
    });
    */
    //Used to create a new chapter page
    inline std::string initial_html(std::string scrollto){return "<html> <style>"
     + css::css +
    "</style>"
    "<body>"
    "<script>"
    "function jumpToTop() "
    "{"
        "window.scrollTo({ top: " + scrollto +", behavior: 'smooth' });"
    "}"
    "window.onload = jumpToTop;"
     "</script>"
     "</html>";};

}