# brisk
Compiles on Linux & Mac (using Make) and Windows (using MinGW-make or the toolchain of your choice)

## Background
```brisk``` is a header-only library that I've written myself for myself. 
This is not full featured and is just a pet project but it could be a valuable learning
tool for those who need illustration on how the STL actually works under the hood without
the mangled symbols, crazy template parameters. 

The goal is to rewrite the STL using the latest standard in readable code.

## Usage
First, refer to the Build section for your platform. So, now, you officially have set it up I'll assume.

All the libraries are split up into their respective headers. The libraries in ```brisk``` are:
- ```algorithm```, a WIP library including copy functions for ```array``` and ```vector```
- ```array```, a replacement for ```std::array```
- ```functional```, a replacement for the ```functional``` header
- ```logger```, a pretty good wrapper around ```std::cin``` and ```std::cout``` that will dump everything output to the console to a log file automagically for you
- ```math```, containers for geometric shapes
- ```memory```, smart pointer stuff
- ```string```, a replacement for ```std::string```
- ```utility```, a replacement for the ```utility``` header
- ```vector```, a replacement for ```std::vector```

Including ```brisk.h``` will, for namesakes, include all of these. However, just append ```.hpp``` to the library name to just get the library you want.

## Requirements
#### Linux
- Compiler (with C++20 support)
- git
- make (only if compiling test program)

#### Windows
- Cygwin or MinGW (in PATH)
- Git
- Compiler (with C++20 support)

## Build (Linux)
1. Install the ```git``` package.
2. Clone repository using ```git clone https://github.com/akachronix/brisk.git```
3. Run makefile to build examples or use headers in your own project.

## Build (Windows)
1. Install Git for Windows.
2. Clone repository using ```git clone https://github.com/akachronix/brisk.git```
3. Open MinGW environment to repository directory.
4. Run makefile to build examples or use headers in your own project.

## Help
If you think you've found a bug, leave an issue. If you have some changes to suggest, make a pull request or put ```[REQUEST]``` before an issue.
