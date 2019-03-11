////////////////////////////////////////////////////////////////////////////////
// $Id: LineType.h 4041 2012-05-24 16:33:44Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: I/O Line object type for the script interpreter.
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
#ifndef LINE_TYPE_H
#define LINE_TYPE_H

#include "ObjectType.h"

namespace Interpreter {

class LineType : public ObjectType
{
 protected:
  virtual const char* Name() const { return "Line"; }
  virtual const MethodEntry* MethodTable() const { return sMethodTable; }

 public:
  static bool Read( CommandInterpreter& );
  static bool Write( CommandInterpreter& );

 private:
  static const MethodEntry sMethodTable[];
  static LineType sInstance;
};

} // namespace

#endif // LINE_TYPE_H
