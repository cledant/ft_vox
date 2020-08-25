# ft_vox
![screenshot](./screenshots/ft_vox.png?raw=true)  

## Features

ft_vox is a 42 project that aims to create a voxel engine, inspired by Minecraft, in which a world is randomly generated.  

## Compiling

Make sure you have the libraries by running `git submodule init && git submodule update`.  
You may compile `ft_vox` binary by running `mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Release && make -j8`.  

## Usage

Run `./ft_vox`  
Options:  
--seed=\[positive number\] to set a specific seed for world generation  
--invertMouseY to invert mouse Y axis  
-h | --help to display help  