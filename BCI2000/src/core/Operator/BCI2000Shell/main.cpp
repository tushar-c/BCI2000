////////////////////////////////////////////////////////////////////
// $Id: main.cpp 4157 2012-06-18 18:49:35Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A script shell that may be used to start up and
//   control BCI2000.
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
////////////////////////////////////////////////////////////////////
#include <iostream>

#include "BCI2000Connection.h"
#include "FileUtils.h"
#include "EnvVariable.h"
#include "VersionInfo.h"
#include <cstring>

using namespace std;

static const string sShebang = "#!";
#if _WIN32
static const char* sOperatorName = "Operator.exe";
#else
static const char* sOperatorName = "Operator";
#endif
static const string sShellName = FileUtils::ExtractBase( FileUtils::ExecutablePath() );
static const string sPrompt = sShellName + "> ";

int main( int argc, char** argv )
{
  enum { execute, command, interactive, help, version } mode = execute;
  int idx = 1;
  while( idx < argc && *argv[idx] == '-' )
  {
    if( !::strcmp( argv[idx], "-c" ) )
      mode = command;
    else if( !::strcmp( argv[idx], "-i" ) )
      mode = interactive;
    else if( !::strcmp( argv[idx], "-h" ) || !::stricmp( argv[idx], "--help" ) )
      mode = help;
    else if( !::strcmp( argv[idx], "-v" ) || !::stricmp( argv[idx], "--version" ) )
      mode = version;
    ++idx;
  }
  if( mode == execute && idx >= argc )
    mode = interactive;
  string script = "";
  if( mode == execute )
    script = FileUtils::AbsolutePath( argv[idx++] );

  string additionalArgs,
         telnetAddress;
  EnvVariable::Get( "BCI2000TelnetAddress", telnetAddress );
  while( idx < argc && argv[idx] != sShebang )
  {
    if( !::stricmp( argv[idx], "--Telnet" ) && idx++ < argc )
    {
      telnetAddress = argv[idx];
    }
    else
    {
      string arg = argv[idx++];
      if( mode != command && arg.find( '\"' ) == string::npos )
        arg = "\"" + arg + "\"";
      additionalArgs += " ";
      additionalArgs += arg;
    }
  }
  struct : BCI2000Connection
  {
    bool OnInput( string& s )
    { return getline( cin, s ); }
    bool OnOutput( const string& s )
    { return cout << s << endl; }
  } bci;
  bci.OperatorPath( "" );
  bci.TelnetAddress( telnetAddress );
  if( !bci.Connect() )
  {
    if( !bci.Run( FileUtils::InstallationDirectory() + sOperatorName, additionalArgs )
        || !bci.Connect() )
    {
      cerr << bci.Result() << endl;
      return -1;
    }
  }
  bci.Execute( "cd \"" + FileUtils::WorkingDirectory() + "\"" );
  int exitCode = 0;
  if( mode == interactive )
  {
    cout << sPrompt << flush;
    string line;
    while( bci.Connected() && getline( cin, line ) )
    {
      if( bci.Connected() )
      {
        exitCode = bci.Execute( line );
        if( !bci.Result().empty() )
          cerr << bci.Result() << '\n';
      }
      else
      {
        cout << "[Lost connection to Operator module]" << endl;
      }
      if( bci.Connected() )
        cout << sPrompt << flush;
    }
  }
  else
  {
    switch( mode )
    {
      case help:
        script = "help";
        break;
      case version:
        cout << sShellName << " version:\n";
        VersionInfo::Current.WriteToStream( cout, true );
        cout << "Connected to:\n";
        script = "get system version";
        break;
      case command:
        script = additionalArgs;
        break;
      case execute:
        script = "execute script \"" + script + "\"" + additionalArgs;
        break;
    }
    exitCode = bci.Execute( script );
    if( !bci.Result().empty() )
      cerr << bci.Result() << '\n';
  }
  return exitCode;
}
