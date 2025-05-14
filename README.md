# Qubed

**Qubed**  (with a *very dignified* capital **Q**, because it's fancy like that ✨) is a custom-built Vulkan-based rendering engine, developed from scratch with love, caffeine, and cat-like curiosity. It's a personal playground and learning project, inspired very loosely by TheCherno’s *Cubed*—but with original architecture and implementation. This engine is entirely independent and not based on any external rendering framework.

This is a **Work-in-Progress (WIP)**, so things might break, crash, or spontaneously combust (hopefully not literally). Expect rapid changes, wild experiments, and possibly a sprinkle of magic dust ✨

---

## 🛠️ Building

### Minimum Requirements

To build Qubed you will need:
- A C++17 compatible compiler (like g++ or clang++)
- Vulkan SDK - Version 1.4 installed and configured
- CMake Version 3.1 or higher


### Cloning the repository

To get started with Qubed, you'll need to clone the repository with its submodules. This will ensure that GLFW and any other dependencies are set up correctly.

Run the following command to clone with the submodules:

```bash
git clone --recursive https://github.com/CrazyRocker/Qubed
```

In case you have already cloned the repository without the submodules, run:

```bash
git submodule update --init --recursive
```

### Running CMake

Once you have cloned the repository, create a folder called `build` (or anything else which suits you) inside the root directory of the repository.

Open a terminal inside this newly created folder, and run 
```bash
cmake ..
```
to generate the build files.

Alternatively, you may run `cmake .. -G {build_system}` where `{build_system}` is the name of your preferred build system (e.g., Unix Makefiles, Ninja).

### Compiling and Linking

Now, you may run `make`, `ninja` or any other applicable command depending on your system's default build system.

You should find the `Qubed` executable in the directory you just created. 

😺 Enjoy! 😺

---

## 🗂️ Structure

```
src/
├── device.cpp / device.hpp     → Physical & logical device setup
├── instance.cpp / instance.hpp → Vulkan instance & validation layers
├── utils.cpp / utils.hpp       → Utility / Helper Functions
├── window.cpp / window.hpp     → GLFW window creation & callbacks
├── main.cpp                    → Main setup logic
dependencies/
├── glfw                        → This folder should exist if the repository
                                  is cloned with its submodules.
```

---
