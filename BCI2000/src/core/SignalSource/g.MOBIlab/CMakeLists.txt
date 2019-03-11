###########################################################################
## $Id: CMakeLists.txt 4194 2012-06-27 19:03:26Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for gMOBIlab

# Set the executable name
SET( EXECUTABLE_NAME gMOBIlab )

# Set the project specific sources
SET( SRC_PROJECT
  gMOBIlabADC.cpp 
)
SET( HDR_PROJECT
  gMOBIlabADC.h
)

# Tell the macro to link against the gMOBIlab libraries in extlib
BCI2000_INCLUDE( "GMOBILAB" )
BCI2000_INCLUDE( "SOURCEFILTER" )

# Create the signal source module
BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
  "${EXECUTABLE_NAME}" 
  "${SRC_PROJECT}" "${HDR_PROJECT}" 
)

