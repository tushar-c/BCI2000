###########################################################################
## $Id: CMakeLists.txt 4194 2012-06-27 19:03:26Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for ARSignalProcessing in Qt

# Set the executable name
SET( EXECUTABLE_NAME ARSignalProcessing )

# Set the project specific sources
SET( SRC_PROJECT
  PipeDefinition.cpp 
)
SET( HDR_PROJECT
)

# Create the signal processing module
BCI2000_ADD_SIGNAL_PROCESSING_MODULE( 
  "${EXECUTABLE_NAME}" 
  "${SRC_PROJECT}" "${HDR_PROJECT}" 
)


