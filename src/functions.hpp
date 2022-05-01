#pragma once
#include <gtkmm.h>
#include <webkit2/webkit2.h>
#include <iostream>
#include <fstream>
#include "web_contents.h"
#include <filesystem>
#include "strnatcmp.hpp"
#include "comp.hpp"
#include "global_variables.hpp"
#include "zipextract.hpp"

inline void save()
{
  std::ofstream chapter_file;
  chapter_file.open(saveFile);

  chapter_file << file;
  chapter_file.close();
}

//Gets the chapter name from 'file'
inline std::string getChapterName()
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

//Gets the manga name from 'file'
inline std::string getMangaName()
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

//Returns the files in a folder 'folda'
inline std::vector<std::string> getFilesInFolder(std::string folda)
{
  std::vector<std::string> toreturn;
  for (const auto & file : std::filesystem::recursive_directory_iterator(folda))
  {
    if(!std::filesystem::is_directory(file.path()))
    {
      if(comp::isImage(file.path()) || comp::isCompressed(file.path()))
      {
        toreturn.push_back(file.path());
      }
    }
  }
  std::sort(toreturn.begin(), toreturn.end(), compareNat);
  return toreturn;
}

//Decompresses the filename
inline void decompress(std::string filename)
{
  std::string tmp_folder{(std::string)std::filesystem::current_path() + "/tmp/"};
  std::filesystem::remove_all(tmp_folder);
  std::filesystem::create_directory(tmp_folder);
  if(comp::isCompressed(file))
  {
    zipe::extract(filename, tmp_folder);
  }
  else if(comp::isImage(file))
  {
    std::vector<std::string> filesInFolda{getFilesInFolder(folder)};
    for(auto foyal : filesInFolda)
    {
      std::filesystem::copy(foyal, tmp_folder);
    }
  }
  
}

//Loads the homepage into 'webview'
inline void load_homepage(WebKitWebView * webview)
{
  std::string homepage_contents{html::homepage};
  std::ofstream homepage_file;
  homepage_file.open((std::string)std::filesystem::current_path() + "/index.html");
  homepage_file << homepage_contents;
  homepage_file.close();
  std::string homepage_uri{"file://" + (std::string)std::filesystem::current_path() + "/index.html"};

  webkit_web_view_load_uri(webview, homepage_uri.c_str());

}

//Generates the page for a chapter
inline std::string generateWebPage()
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

//Returns the folder in which 'filename' is
inline std::string getFolder(std::string filename)
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

inline std::vector<std::string> getFoldersInFolder(std::string folda)
{
  std::vector<std::string> toreturn;
  for (const auto & file : std::filesystem::recursive_directory_iterator(folda))
  {
    if(file.is_directory())
    {
      toreturn.push_back(file.path().string() + "/");
    }
  }
  std::sort(toreturn.begin(), toreturn.end(), compareNat);
  return toreturn;
}

inline std::string getMangaFolderForImages(std::string image_path)
{
  bool f{false};
  int p{-1};
  for(int i{static_cast<int>(image_path.size() -1)}; i >=0; i--)
  {
      if(image_path[i] == '/')
      {
        if(f)
        {
          p = i;
          break;
        }
        else
        {
          f = true;
        }
      }
  }
  std::string r{""};
  for(int i{0}; i <= p; i++)
  {
    r += image_path[i];
  }
  return r;
}

inline std::string findFolderInFolder(int pos)
{
  std::vector<std::string> folders{getFoldersInFolder(getMangaFolderForImages(folder))};
  return folders[pos];
}

//Finds a file in a folder given a position
inline std::string findFileInFolder(int pos)
{
  std::vector<std::string> files{getFilesInFolder(folder)};
  return files[pos];
}

//Sets the position.
inline int setPosition(std::string folda, std::string fayel)
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

inline int setImageFolderPosition(std::string upper_folder, std::string lower_folder)
{
  std::vector<std::string> foldersInFolder{getFoldersInFolder(upper_folder)};
  for(int i{0}; i < foldersInFolder.size(); i++)
  {
    if(lower_folder == foldersInFolder[i])
    {
      return i;
    }
  }
  return -1;
}