# cuda-debugger-optimizer

## Overview

Main repository for 15418 final project, CUDA Debugger and Optimizer.

## Requirements

* gtkmm: On Mac, install with `brew install gtkmm3`. On Ubuntu, install with `sudo apt-get install libgtkmm-3.0-dev`.
* glade: On Mac, install with `brew install glade`.

## GUI

The GUI is built with C++, gtkmm3, and glade3. To compile, run `make`. To run, run `./gui`. To make changes to the glade file, edit and save `src/gui.glade` using the glade editor (open the editor by running `glade` in the Terminal).
