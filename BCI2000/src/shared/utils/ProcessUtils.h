////////////////////////////////////////////////////////////////////////////////
// $Id: ProcessUtils.h 4222 2012-07-03 12:09:30Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: Utility functions for executing processes.
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
#ifndef PROCESS_UTILS_H
#define PROCESS_UTILS_H

#include <string>
#include <iostream>

namespace ProcessUtils {

bool ExecuteSynchronously( const std::string& executable, const std::string& arguments, std::ostream&, int& );
bool ExecuteAsynchronously( const std::string& executable, const std::string& arguments, int& );

void GoIdle(); // Satisfy parent processes using WaitForInputIdle().

bool AssertSingleInstance( int argc, char** argv, const std::string& id = "", int timeout = 0 );

} // namespace

#endif // PROCESS_UTILS_H