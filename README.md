# Korai
Next generation manga reader

# Features
- **Fast:** Despite using WebKitGtk, a port of an _entire_ browser engine to GTK, Korai is incredibly fast loading chapters.
- **Reliable:** Works with manga/comic names that contain unusual characters and doesn't crash for no reason.
- **Easy to use:** Everything is where it should be. No previous configuration required for almost anything.

# Command line options
For more information, please execute `korai -h`

- Run a tutorial: `korai -t`
- Maximize the window: `korai -m`
- Set default window size: `korai <width>x<height>` (Note the 'x')

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

# Screenshots

![Korai](https://user-images.githubusercontent.com/48135147/163577957-b9fdc19d-827a-40d3-8cfc-e0f36930a505.png)
