////////////////////////////////////////////////////////////////////////////////
// $Id: EDFOutputFormat.cpp 3798 2012-02-01 18:07:06Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Output into an EDF data file.
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

#include "EDFOutputFormat.h"

#include "GDF.h"
#include "BCIError.h"

using namespace std;
using namespace GDF;

EDFOutputFormat::EDFOutputFormat()
{
}


EDFOutputFormat::~EDFOutputFormat()
{
}

void
EDFOutputFormat::Publish() const
{
  EDFOutputBase::Publish();

  BEGIN_PARAMETER_DEFINITIONS
    "Storage string LabID= % % % % "
      "// laboratory identification",
    "Storage string TechnicianID= % % % % "
      "// technician identification",
  END_PARAMETER_DEFINITIONS
}


void
EDFOutputFormat::Preflight( const SignalProperties& inProperties,
                            const StateVector& inStatevector ) const
{
  EDFOutputBase::Preflight( inProperties, inStatevector );
  if( inProperties.Type() != SignalType::int16 )
    bcierr << "Signal data type must be int16 for EDF files"
           << endl;

  Parameter( "SubjectYearOfBirth" );
  Parameter( "EquipmentID" );
  Parameter( "TechnicianID" );
  Parameter( "LabID" );
  Parameter( "SubjectName" );
}


void
EDFOutputFormat::Initialize( const SignalProperties& inProperties,
                             const StateVector& inStatevector )
{
  EDFOutputBase::Initialize( inProperties, inStatevector );
  // Adapt marker channels to EDF conventions.
  for( size_t i = inProperties.Channels(); i < Channels().size(); ++i )
  {
    Channels()[ i ].DigitalMinimum = 0;
    Channels()[ i ].DigitalMaximum = static_cast<int>( Channels()[ i ].DigitalMaximum - 1 ) & 0x7fff;
    Channels()[ i ].PhysicalMinimum = Channels()[ i ].DigitalMinimum;
    Channels()[ i ].PhysicalMaximum = Channels()[ i ].DigitalMaximum;
  }
}


void
EDFOutputFormat::StartRun( ostream& os, const string& inFileName )
{
  EDFOutputBase::StartRun( os, inFileName );

  time_t now = ::time( NULL );
  struct tm* time = ::localtime( &now );
  ostringstream year;
  year   << setw( 4 ) << setfill( '0' ) << time->tm_year + 1900;
  ostringstream startdate;
  startdate << setw( 2 ) << setfill( '0' ) << time->tm_mday << '.'
            << setw( 2 ) << setfill( '0' ) << time->tm_mon + 1 << '.'
            << year.str().substr( 2 )
            << setw( 2 ) << setfill( '0' ) << time->tm_hour << '.'
            << setw( 2 ) << setfill( '0' ) << time->tm_min << '.'
            << setw( 2 ) << setfill( '0' ) << time->tm_sec;

  ostringstream patient;
  patient << GDF::EncodedString( Parameter( "SubjectName" ) )
          << " X ";
  switch( int( Parameter( "SubjectSex" ) ) )
  {
    case GDF::unspecified:
      patient << "X";
      break;
    case GDF::male:
      patient << "M";
      break;
    case GDF::female:
      patient << "F";
      break;
  }
  patient << " XX-XXX-" << GDF::EncodedString( Parameter( "SubjectYearOfBirth" ) );

  ostringstream recording;
  recording << "Startdate "
            << GDF::DateTimeToString( now )
            << ' '
            << GDF::EncodedString( Parameter( "LabID" ) )
            << ' '
            << GDF::EncodedString( Parameter( "TechnicianID" ) )
            << ' '
            << GDF::EncodedString( Parameter( "EquipmentID" ) );

  PutField< Str<8>  >( os, "0" );
  PutField< Str<80> >( os, patient.str() );
  PutField< Str<80> >( os, recording.str() );
  PutField< Str<16> >( os, startdate.str() );
  PutField< Str<8>  >( os, static_cast<int>( 256 * ( Channels().size() + 1 ) ) );
  PutField< Str<44> >( os );
  PutField< Str<8>  >( os, -1 );
  PutField< Str<8>  >( os, Parameter( "SampleBlockSize" ) / Parameter( "SamplingRate" ) );
  PutField< Str<4>  >( os, Channels().size() );

  PutArray< Str<16> >( os, Channels(), &ChannelInfo::Label );
  PutArray< Str<80> >( os, Channels(), &ChannelInfo::TransducerType );
  PutArray< Str<8>  >( os, Channels(), &ChannelInfo::PhysicalDimension );
  PutArray< Str<8>  >( os, Channels(), &ChannelInfo::PhysicalMinimum );
  PutArray< Str<8>  >( os, Channels(), &ChannelInfo::PhysicalMaximum );
  PutArray< Str<8>  >( os, Channels(), &ChannelInfo::DigitalMinimum );
  PutArray< Str<8>  >( os, Channels(), &ChannelInfo::DigitalMaximum );
  PutArray< Str<80> >( os, Channels(), &ChannelInfo::Filtering );
  PutArray< Str<8>  >( os, Channels(), &ChannelInfo::SamplesPerRecord );
  PutArray< Str<32> >( os, Channels() );
}

void
EDFOutputFormat::StopRun( ostream& os )
{
  const int NumRecFieldPos = 236;
  if( os.seekp( NumRecFieldPos ) )
    PutField< Str<8> >( os, NumRecords() );
  EDFOutputBase::StopRun( os );
}


void
EDFOutputFormat::Write( ostream& os,
                        const GenericSignal& inSignal,
                        const StateVector& inStatevector )
{
  EDFOutputBase::Write( os, inSignal, inStatevector );
}


