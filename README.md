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

- **Ubuntu & Debian:** 
 ```
sudo apt install libgtkmm-3.0-dev libwebkit2gtk-4.0-dev libarchive-dev libvte-2.91-dev
``` 
- **Arch:** The AUR source is automatically updated with GitHub and automatically grabs the dependencies
```
yay -S korai
```
**Note:** The default chapter file path is `/home/$USER/.local/share/chapter.conf` if korai has been installed using this method <br>
Although you can still build Korai from source:
```
sudo pacman -S gtkmm3 webkit2gtk libarchive vte3
```

**If the integrated downloader is wanted:**
- Ubuntu & Debian: <a href=https://github.com/Gyro7/mangodl/releases/>Please grab the latest Linux release here</a> and put it on your PATH
- Arch: Automatically downloaded using the AUR (If you want to build Korai from source: `yay -S mangodl`)

# Building
To build Korai, please enter the following commands:
```
git clone https://github.com/DisableGraphics/Korai
cd Korai
make
```
**Note:** If the integrated downloader is not wanted, then write:
```
git clone https://github.com/DisableGraphics/Korai
cd Korai
make korai-nodownload
```
instead

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
