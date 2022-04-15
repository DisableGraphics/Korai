# Korai
Next generation manga reader

# Usage
Getting used to Korai's command line options is practically a must. You can just run `korai` from the command line and the program won't have any issues with it, but there are some pre-defined options that may not suit the user's preferences. For more information, please run `korai -h` to see all the available options.

# Dependencies
Korai requires:
**Gtkmm 3**
- Ubuntu & Debian: `sudo apt install libgtkmm-3.0-dev`
- Arch: `sudo pacman -S gtkmm3`
**WebKit2Gtk**
- Ubuntu & Debian: `sudo apt install libwebkit2gtk-4.0-dev`
- Arch `sudo pacman -S webkit2gtk`

# Building
To build Korai, please enter the following commands:
```
git clone https://github.com/DisableGraphics/Korai
cd Korai
make
```
The program will be in the `build/` directory
