---
slug: /advanced/combine-systems
---

![Advanced](https://user-images.githubusercontent.com/98862735/178853223-10d53d46-3d80-4d44-95d6-3cd345a730e1.png)


<table><td>

Download the files used in this example:  
- [combined-systems-example.zip](https://github.com/OnionUI/Onion/files/9717818/combined-systems-example.zip)

</td></table>

Below is an example `launch.sh` script for combining different systems or cores inside one Emu/Roms folder. To get started you can copy any of the subfolders inside `Emu`, as long as it contains a `launch.sh` and `config.json` file, or you can download the zip provided below.

To set the name of the system/collection edit the label inside `config.json`, replacing `My Label` with the name you want. You also need to make sure the rom and image paths are pointing to the right folder in `Roms`. You can change the names of the folders `Emu/EXAMPLE` and `Roms/EXAMPLE`, replacing `EXAMPLE` with whatever name you want (the name shouldn't contain any spaces, preferably you should only use the characters `A-Z` or `0-9`).

The example is made up of three different core selection methods, you can include one or more of them depending on your needs. To find the correct core names, you can lookup the filename in `RetroArch/.retroarch/cores`.

## Step 1: Set the default core

Edit the `launch.sh` to contain the following (changing out the core names and keywords to fit your specific use-case):

`Emu/EXAMPLE/launch.sh`
```sh
#!/bin/sh
echo $0 $*
progdir=`dirname "$0"`
homedir=`dirname "$1"`

# Set the default core
core="mednafen_supafaust_libretro.so"
```

The `core` variable is initialized with the default core name, which will be used for unknown file extensions. Remember to either clear the `extlist` in the `config.json` file, or add all the supported extensions (be sure to add the lower- and uppercase versions).

## Step 2.a: Select core based on file extension

Below is the first method of choosing a core for the rom you want to launch. It's purely based on the file extension of the rom, and will only work if the cores you are choosing between use different file extensions. E.g. here we're creating a games folder that can contain PSX, SNES, and PC-Engine games.

`Emu/EXAMPLE/launch.sh` *(continued)*
```sh
# Select core based on file extension
case `echo "$(basename "$1")" | awk -F. '{print tolower($NF)}'` in
    iso|cue|img|mdf|pbp|toc|cbn|m3u|ccd|chd)
        core="pcsx_rearmed_libretro.so"
        ;;
    sfc|smc)
        core="mednafen_supafaust_libretro.so"
        ;;
    pce)
        core="mednafen_pce_fast_libretro.so"
        ;;
    *)
        ;;
esac
```

## Step 2.b: Select core based on subfolders

Another way to select cores is by checking the name of the subfolder it is in. This will solve the problem of different cores accepting the same files. In our example we can know detect which core to use for games located in different subfolders. The PlayStation core will be used for roms located in `Roms/EXAMPLE/PS` or `Roms/EXAMPLE/PSX`, and the SNES core will be used for the `SFC` or `SNES` subfolders, etc.

`Emu/EXAMPLE/launch.sh` *(continued)*
```sh
# Select core based on name of containing folder
case `echo "$(basename "$homedir")" | awk '{print toupper($0)}'` in
    PS|PSX)
        core="pcsx_rearmed_libretro.so"
        ;;
    SFC|SNES)
        core="mednafen_supafaust_libretro.so"
        ;;
    PCE|PCECD)
        core="mednafen_pce_fast_libretro.so"
        ;;
    *)
        ;;
esac
```

## Step 2.c: Select core based on keywords

A third way to select a core, is to check for keywords in the filename. Below I've made a switch case that selects a core based on different abbreviations in parentheses. To add more abbreviations be sure to add `*"([ABBR])"*` with a `|` as separator.

`Emu/EXAMPLE/launch.sh` *(continued)*
```sh
# Select core based on parentheses in filename
case `echo "$(basename "$1")" | awk '{print toupper($0)}'` in
    *"(PS)"*|*"(PSX)"*)
        core="pcsx_rearmed_libretro.so"
        ;;
    *"(SFC)"*|*"(SNES)"*)
        core="mednafen_supafaust_libretro.so"
        ;;
    *"(PCE)"*|*"(PCECD)"*)
        core="mednafen_pce_fast_libretro.so"
        ;;
    *)
        ;;
esac
```

## Step 3: Launch RetroArch with the selected core

Lastly, we need to launch RetroArch with the selected core.

`Emu/EXAMPLE/launch.sh` *(continued)*
```sh
cd /mnt/SDCARD/RetroArch/
HOME=/mnt/SDCARD/RetroArch/ ./retroarch -v -L ".retroarch/cores/$core" "$1"
```
