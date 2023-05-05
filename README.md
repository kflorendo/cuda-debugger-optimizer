# cuda-debugger-optimizer

## Overview

Main repository for 15418 final project, CUDA Debugger and Optimizer.

## Requirements

* `gtkmm3`: On Mac, install with `brew install gtkmm3`. On Ubuntu, install with `sudo apt-get install libgtkmm-3.0-dev`.
* `glade`: On Mac, install with `brew install glade`.
* `nvidia-cuda-toolkit`
* `python3`
* `matplotlib`
* `bc`

## Running and Editing the GUI

To compile, run `make`. To run, from the main folder, run `./gui`. To make changes to the glade file, edit and save `src/gui.glade` using the glade editor (open the editor by running `glade` in the Terminal).

## GUI Overview

The GUI was developed using GTK (gtkmm), a library for creating graphical user interfaces, and C++. The layout for the GUI was designed and created using glade. The layout is formatted as an XML hierarchy of the application, its components (like buttons and tree displays), and various properties (like position and spacing). The GUI program reads in the glade file, renders and obtains references to the different components, and specifies the behavior when the user interacts with the components, like click and text change signals. Custom functions were written to display data in different formats (tree and list views, tables, etc.) and simplify the process of taking in user input (for instance, the user can directly interact with the code file to set breakpoints, line numbers, and specify variable names).

The GUI consists of 3 pages: Config, Debug, and Optimize. In Config, the user specifies the directory containing the Makefile for their CUDA program, the command to run the executable, and the path to their .cu file. These allow us to modify the user’s CUDA file by inserting lines of code, recompiling, and extracting/analyzing from the output. These settings get saved in a text file. The Debug and Optimize pages let users interact with all the scripts and features we implemented.
