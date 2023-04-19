# cuda-debugger-optimizer

## Overview

Main repository for 15418 final project, CUDA Debugger and Optimizer.

## Requirements

* gtkmm: On Mac, install with `brew install gtkmm4`.
* glade: On Mac, install with `brew install glade`.

## GUI

The GUI is built with C++, gtkmm4, and glade3. To compile, run `make`. To run, run `./gui`. To make changes to the glade file:
1. Edit and save `src/gui.glade` using the glade editor (open the editor by running `glade` in the Terminal).
2. Run `gtk4-builder-tool simplify --3to4 src/gui.glade > src/gui4.glade`
3. Open `src/gui4.glade` and delete any lines that specify the `position` property for the GtkStack (these are not supported in gtk4).
