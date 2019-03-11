////////////////////////////////////////////////////////////////////////////////
// $Id: TelnetServer.h 4142 2012-06-16 18:46:48Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A simple telnet server that runs inside a BCI2000
//   operator module.
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
#ifndef TELNET_SERVER_H
#define TELNET_SERVER_H

#include <string>
#include "ScriptInterpreter.h"
#include "OSThread.h"
#include "OSMutex.h"
#include "SockStream.h"

class TelnetServer : private ScriptInterpreter, private OSThread
{
 public:
  TelnetServer( class StateMachine&, const std::string& address );
  ~TelnetServer();

 private:
  TelnetServer( TelnetServer* parent );
  void Initialize();

  // OSThread interface
  int OnExecute();
  void OnFinished();
  // ScriptInterpreter interface
  void OnScriptError( const std::string& );
  // CommandInterpreter interface
  static bool OnWriteLine( void*, const std::string& );
  static bool OnReadLine( void*, std::string& );

  // Input
  TelnetServer& ReadCharacter();
  // Output
  TelnetServer& WriteHello();
  TelnetServer& Write( const std::string& );
  TelnetServer& WriteNewline();
  TelnetServer& WritePrompt();

 private:
  OSMutex       mMutex;
  TelnetServer* mpParent,
              * mpChild;
  std::string mAddress,
              mLineBuffer;
  SharedPointer<server_tcpsocket> mpListeningSocket;
  tcpsocket mSocket;
  sockstream mStream;
};

#endif // TELNET_SERVER_H
