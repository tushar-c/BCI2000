////////////////////////////////////////////////////////////////////////////////
// $Id: MessageTypes.h 3997 2012-05-10 16:37:42Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: Message-related object types for the script interpreter.
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
#ifndef MESSAGE_TYPES_H
#define MESSAGE_TYPES_H

#include "ObjectType.h"

namespace Interpreter {

class MessageType : public ObjectType
{
 protected:
  virtual const char* Name() const { return "Message"; }
  virtual const MethodEntry* MethodTable() const { return sMethodTable; }

 public:
  static bool Log( CommandInterpreter& );

 private:
  static const MethodEntry sMethodTable[];
  static MessageType sInstance;
};

class MessagesType : public ObjectType
{
 protected:
  virtual const char* Name() const { return "Messages"; }
  virtual const MethodEntry* MethodTable() const { return sMethodTable; }

 public:
  static bool Capture( CommandInterpreter& );
  static bool Flush( CommandInterpreter& );

 private:
  static const MethodEntry sMethodTable[];
  static MessagesType sInstance;
};

class WarningType : public ObjectType
{
 protected:
  virtual const char* Name() const { return "Warning"; }
  virtual const MethodEntry* MethodTable() const { return sMethodTable; }

 public:
  static bool Issue( CommandInterpreter& );

 private:
  static const MethodEntry sMethodTable[];
  static WarningType sInstance;
};

class ErrorType : public ObjectType
{
 protected:
  virtual const char* Name() const { return "Error"; }
  virtual const MethodEntry* MethodTable() const { return sMethodTable; }

 public:
  static bool Report( CommandInterpreter& );

 private:
  static const MethodEntry sMethodTable[];
  static ErrorType sInstance;
};

} // namespace

#endif // MESSAGE_TYPES_H
