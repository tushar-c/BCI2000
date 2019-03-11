////////////////////////////////////////////////////////////////////////////////
// $Id: IIRBandpass.cpp 4112 2012-06-14 09:28:00Z mellinger $
// Authors: Jeremy Hill <jezhill@gmail.com>, juergen.mellinger@uni-tuebingen.de
// Description: A generic IIR bandpass filter that also provides a notch.
//
// $BEGIN_BCI2000_LICENSE$
//
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
//
// BCI2000 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// BCI2000 is distributed in the hope that it will be useful, but
//                         WITHOUT ANY WARRANTY
// - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
//
// $END_BCI2000_LICENSE$
////////////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop

#include "IIRBandpass.h"
#include "FilterDesign.h"

using namespace std;

RegisterFilter( IIRBandpass, 2.C1 );

void
IIRBandpass::Publish()
{
 BEGIN_PARAMETER_DEFINITIONS
  "Filtering:IIR%20Bandpass float HighPassCorner=  0Hz    0Hz  % % // High pass corner frequency",
  "Filtering:IIR%20Bandpass int   HighPassOrder=   2      2    0 % // High pass order",
  "Filtering:IIR%20Bandpass float LowPassCorner=   0Hz    0Hz  % % // Low pass corner frequency",
  "Filtering:IIR%20Bandpass int   LowPassOrder=    4      4    0 % // Low pass order",
  "Filtering:IIR%20Bandpass float NotchCenter=     0Hz    60Hz % % // Notch center frequency",
  "Filtering:IIR%20Bandpass int   NotchOrder=      4      4    0 % // Notch order",
  "Filtering:IIR%20Bandpass float FilterGain=      1.0    1.0  0 % // Overall filter gain",
 END_PARAMETER_DEFINITIONS
}

void
IIRBandpass::DesignFilter( const SignalProperties& Input,
                           Real& outGain,
                           ComplexVector& outZeros,
                           ComplexVector& outPoles ) const
{
  typedef Ratpoly<FilterDesign::Complex> TransferFunction;
  TransferFunction tf( 1.0 );
  Real gain = 1.0;

  // High pass
	Real hpCorner = Parameter( "HighPassCorner" ).InHertz() / Input.SamplingRate();
  if( hpCorner > 0.5 )
    bcierr << "HighPassCorner parameter exceeds Nyquist frequency" << endl;
  if( hpCorner < 0 )
    bcierr << "HighPassCorner must be >= 0" << endl;
  int hpOrder = Parameter( "HighPassOrder" );
  if( hpCorner > 0 && hpCorner < 0.5 && hpOrder )
  {
    TransferFunction hp =
      FilterDesign::Butterworth()
      .Order( hpOrder )
      .Highpass( hpCorner )
      .TransferFunction();
    tf *= hp;
    gain *= abs( hp.Evaluate( -1.0 ) ); // HF gain
  }

  // Low pass
	Real lpCorner = Parameter( "LowPassCorner" ).InHertz() / Input.SamplingRate();
  if( lpCorner > 0.5 )
    bcierr << "LowPassCorner parameter exceeds Nyquist frequency" << endl;
  if( lpCorner < 0 )
    bcierr << "LowPassCorner must be >= 0" << endl;
  int lpOrder = Parameter( "LowPassOrder" );
  if( lpCorner > 0 && lpCorner < 0.5 && lpOrder )
  {
    TransferFunction lp =
      FilterDesign::Butterworth()
      .Order( lpOrder )
      .Lowpass( lpCorner )
      .TransferFunction();
    tf *= lp;
    gain *= abs( lp.Evaluate( 1.0 ) ); // LF gain
  }

  // Notch
	Real notchCenter = Parameter( "NotchCenter" ).InHertz() / Input.SamplingRate(),
       corner1 = 0.9 * notchCenter,
       corner2 = 1.1 * notchCenter;
  if( corner2 > 0.5 )
    bcierr << "NotchCenter parameter is close to or exceeds Nyquist frequency" << endl;
  if( corner1 < 0 )
    bcierr << "NotchCenter parameter is close to 0, or negative" << endl;
  int notchOrder = Parameter( "NotchOrder" );
  if( corner1 > 0 && corner2 < 0.5 && notchOrder )
  {
    TransferFunction notch =
      FilterDesign::Chebyshev()
      .Ripple_dB( -0.1 )
      .Order( notchOrder )
      .Bandstop( corner1, corner2 )
      .TransferFunction();
    tf *= notch;
    gain *= abs( notch.Evaluate( 1.0 ) ); // LF gain
  }

  // User gain
  outGain = Parameter( "FilterGain" ) / gain;
  outZeros = tf.Numerator().Roots();
  outPoles = tf.Denominator().Roots();
}
