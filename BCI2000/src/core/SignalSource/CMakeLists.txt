###########################################################################
## $Id: CMakeLists.txt 3900 2012-03-30 21:30:33Z gmilsap $
## Authors: griffin.milsap@gmail.com
## Description: Build information for BCI2000 and CMake

# Set this directory name
SET( DIR_NAME "${DIR_NAME}SignalSource/" )

# Recurse down into all project subdirectories
ADD_SUBDIRECTORY( SignalGenerator )
ADD_SUBDIRECTORY( g.USBamp )
ADD_SUBDIRECTORY( g.MOBIlab )
ADD_SUBDIRECTORY( g.MOBIlabPlus )
