# Korai
Next generation manga reader

![Korai](https://user-images.githubusercontent.com/48135147/163577957-b9fdc19d-827a-40d3-8cfc-e0f36930a505.png)

# Features
- **Fast:** Despite using WebKitGtk, a port of an _entire_ browser engine to GTK, Korai is incredibly fast loading chapters.
- **Reliable:** Works with manga/comic names that contain unusual characters and doesn't crash for no reason.
- **Easy to use:** Everything is where it should be. No previous configuration required for almost anything.

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
1. **A compiler that supports C++17 (GCC > 5.0)**<br>
2. **Gtkmm 3**
- Ubuntu & Debian: `sudo apt install libgtkmm-3.0-dev`
- Arch: `sudo pacman -S gtkmm3`<br>
3. **WebKit2Gtk**
- Ubuntu & Debian: `sudo apt install libwebkit2gtk-4.0-dev`
- Arch: `sudo pacman -S webkit2gtk-4.0`
4. **Unzip**
- Ubuntu & Debian: `sudo apt install unzip`
- Arch: `sudo pacman -S unzip` 

# Building
To build Korai, please enter the following commands:
```
git clone https://github.com/DisableGraphics/Korai
cd Korai
make
```
The program will be in the `build/` directory

# Troubleshooting
1. The window is completely blank!
- There's an issue surrounding WebKitGtk that produces this problem, to solve it, please run the following commands:
```
rm ~/.local/share/mime/packages/user-extension-html.xml
update-mime-database ~/.local/share/mime
```
2. Hey, a weird file named `index.html` and a folder named `tmp` have appeared!
- These two files are crucial for the operation of Korai. Don't worry, when the program is closed, they will be deleted.
3. Hey, a weird file named `chapter.conf` has appeared and doesn't get deleted after closing!
- This file is where Korai saves the last chapter the user was reading. Its path can be changed using the `korai -f /path/to/save_file.conf` command
4. Hey, there's no Windows version!
- WebKitGtk doesn't support Windows and so does Korai
