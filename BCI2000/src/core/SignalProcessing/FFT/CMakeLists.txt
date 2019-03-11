###########################################################################
## $Id: CMakeLists.txt 4194 2012-06-27 19:03:26Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for FFTSignalProcessing in Qt
 
# Set the executable name
SET( EXECUTABLE_NAME FFTSignalProcessing )

# Set the project specific sources
SET( SRC_PROJECT
  PipeDefinition.cpp
  ${BCI2000_SRC_DIR}/shared/modules/signalprocessing/FFTFilter.cpp
)
SET( HDR_PROJECT
  ${BCI2000_SRC_DIR}/shared/modules/signalprocessing/FFTFilter.h
)

BCI2000_INCLUDE( "FFT" )

# Create the signal processing module
BCI2000_ADD_SIGNAL_PROCESSING_MODULE( 
  "${EXECUTABLE_NAME}" 
  "${SRC_PROJECT}" "${HDR_PROJECT}" 
)
