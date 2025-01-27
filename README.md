# HoMMybeer 3

## Introduction

A simplified version of the acclaimed Heroes of Might and Magic III game, developed as an assignment for the Advanced C++ Programming course at Warsaw University of Technology.

## Project status

The project has finished, it now offers most of the aimed for basic functionality of the game, while retaining potential for future expansion of functionality.

## How to build and run

### Automated script (Ubuntu)

For building and running the game in the release version on Ubuntu, run the _install-requirements.sh_ script found in the _scripts_ directory and then run the provided _build-and-run-game.sh_ script. For building different configurations and further information about the dependencies and used utilities, see the instructions below.

### Installing requirements
The project is built using _CMake_ and - by default - the _Ninja_ generator. The generator can be changed by adjusting the CMake presets found in _CMakePresets.json_, though other generators were not tested. The C++ compiler of choice during development was the GNU compiler and so that is also what we would recommend.

You can install the required programs mentioned above from the terminal or via the provided scripts in the _scripts_ directory (powershell scripts for Windows and a bash script for Ubuntu).

Note that when running on Ubuntu, several additional libraries required by SFML need to be installed. These will be installed with the provided scripts and include:
- xorg-dev
- libgl1-mesa-dev
- libudev-dev
- libalut-dev
- libvorbis-dev
- libflac-dev

SFML and GoogleTest will be automatically fetched via git when running CMake configuration.

### Building from inside Visual Studio Code

The development environment is setup to be used in VS Code so the building process is also simplified when using it. With the _CMake Tools_ extension installed, you can open the command palette (standard shortcut: _ctrl+shift+P_) and find "_CMake: Select Configure Preset_". After pressing _Enter_, you will be prompted to choose a preset. A recommended option for simply trying out the app would be the _Main release_ preset. After the configuration is complete, you can use the default build task with _ctrl+shift+B_ to build the main target.

### Building via the terminal

In the top level project directory, run:
```bash
cmake --preset="<preset-name>" .
cmake --build ./build/<preset-name>
```
For example:
```bash
cmake --preset="main-release" .
cmake --build ./build/main-release
```
(you can also swap the preset for any from: main-debug, test-release, test-debug)

### Running (Unix-based)

To run the built application, simply run:
```bash
./build/<chosen_preset_name>/bin/HoMMybeer3
```

### Running (Windows)

Unfortunately, as of now, copying the dynamic libraries provided by SFML to the directory from which the executable is being run is necessary. We are working on fixing that issue. The library files can be found in _build/<chosen_preset_name>/\_deps/sfml-build/lib_. After they have been copied to the top level directory, we can run the application with 
```powershell
.\build\<chosen_preset_name>\bin\HoMMybeer3.exe
```

### Running tests

To run the tests you simply have to select test-release or test-debug configure preset, build the project and run the tests with:
```bash
ctest --preset default
```
You can also take advantage of _ctest_ functionality, such as using regular expressions to find the tests of interest.

## Setting up the development environment (Windows and VS Code)

- When opening the project in VS Code, you will be asked whether to install the recommended extensions. Press 'Yes'.
- Install requirements with the provided Powershell scripts found in the _scripts_ directory. If you already have _scoop_, simply run the _install-requirements-scoop.ps1_. If not, run _install-scoop.ps1_ first. Be wary of unwanted interactions between already present, different installations of some of the packages.
- Configure project with CMake Tools - Press 'Yes' when prompted to configure project or open the command palette (standard shortcut: _ctrl+shift+P_) and find "_CMake: Select Configure Preset_".
- Make sure that you have autoformatting enabled (for example on file save) in settings. The formatting provider should be clang-format.
- Furthermore, you can specify some useful user settings for the workspace, for example your name to be filled by Doxygen when creating a file comment.

## User instruction

For instructions on how to play the game, refer to the complete PDF documentation found in the _doc/_ directory

## Contributing

- Wojciech Przybylski @wprzyby1 wojciech.przybylski2.stud@pw.edu.pl

- Piotr Kluba @pkluba piotr.kluba.stud@pw.edu.pl

