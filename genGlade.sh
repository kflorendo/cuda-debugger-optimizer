#!/bin/bash

gtk4-builder-tool simplify --3to4 src/gui.glade > src/gui4.glade
sed -i '' '/position/d' src/gui4.glade
sed -i '' '/shadow-type/d' src/gui4.glade