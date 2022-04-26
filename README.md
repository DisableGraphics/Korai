# Korai
Next generation manga reader

![Korai](https://user-images.githubusercontent.com/48135147/163577957-b9fdc19d-827a-40d3-8cfc-e0f36930a505.png)
Note: screenshot is old and probably doesn't show all the features in Korai

# Features
- **Fast:** Despite using WebKitGtk, a port of an _entire_ browser engine to GTK, Korai is incredibly fast loading chapters.
- **Reliable:** Works with manga/comic names that contain unusual characters and doesn't crash for no reason.
- **Easy to use:** Everything is where it should be. No previous configuration required for almost anything.
- **Integrated manga downloader (Optional):** Just write the title and Voilà!

# File format support
Korai supports:
  - **CBZ/ZIP** files
  - **CBR/RAR** files
  - **Folders with images inside**
  - **MangaDex**: As Korai uses WebKitGTK, a browser engine, it can load manga web pages. Use the next chapter/previous chapter buttons to go back and forward.

Korai supports these types of images:
  - **PNG** files
  - **JPG/JFIF/JPEG** files
  - **GIF** files
  - **WebP** files
  - **SVG** files
# Command line options
For more information, please execute `korai -h`

- Run a tutorial: `korai -t`
- Maximize the window: `korai -m`
- Set default window size: `korai -s <width>x<height>` (Note the 'x')

# Shortcuts
To make life easier, Korai supports some shortcuts:
- Pressing 'o' will will open a dialog to open a file
- Pressing left arrow will change to the previous chapter
- Pressing right arrow will change to the next chapter 

# Dependencies
Korai requires: <br>
1. **A compiler that supports C++17 (GCC > 5.0)** Only if compiling from source<br>
2. **Gtkmm 3**
- Ubuntu & Debian: `sudo apt install libgtkmm-3.0-dev`
- Arch: `sudo pacman -S gtkmm3`<br>
3. **WebKit2Gtk**
- Ubuntu & Debian: `sudo apt install libwebkit2gtk-4.0-dev`
- Arch: `sudo pacman -S webkit2gtk`
4. **Libarchive**
- Ubuntu & Debian: `sudo apt install libarchive-dev`
- Arch: `sudo pacman -S libarchive` 
5. **VTE** (Only if downloader support is enabled)
- Ubuntu & Debian: `sudo apt install libvte-2.91-dev`
- Arch: `sudo pacman -S vte3`
6. **mangodl** (Only if downloader support is enabled)
- Ubuntu & Debian: <a href=https://github.com/Gyro7/mangodl/releases/>Please grab the latest Linux release here</a> and put it on your PATH
- Arch: `yay -S mangodl`

# Building
To build Korai, please enter the following commands:
```
git clone https://github.com/DisableGraphics/Korai
cd Korai
make
```
Two applications will appear on the `build/` directory:
- `korai-nodownload`: Korai without integrated downloader
- `korai`: Korai with integrated downloader <br>
**Note:** Compilation of `korai` will fail if downloader support is not wanted. `korai-nodownload` will appear in the `build/` directory as the only executable.

# Troubleshooting
1. The window is completely blank!
- There's an issue surrounding WebKitGTK that can be traced to a faulty MIME types database (<a href=https://stackoverflow.com/questions/71734719/webkitgtk-doesnt-load-local-files>More information here</a>). To correct this, please open Korai and click the button named "Reload MIME types" (Under the menu button).

2. Hey, a weird file named `index.html` and a folder named `tmp` have appeared!
- These two files are crucial for the operation of Korai. Don't worry, when the program is closed, they will be deleted.
3. Hey, a weird file named `chapter.conf` has appeared and doesn't get deleted after closing!
- This file is where Korai saves the last chapter the user was reading. Its path can be changed using the `korai -f /path/to/save_file.conf` command
4. Hey, there's no Windows version!
- WebKitGtk doesn't support Windows and so does Korai
5. Why did you name the releases after heavy metal/hard rock songs?
- Why not
