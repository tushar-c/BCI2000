////////////////////////////////////////////////////////////////////////////////
// $Id: CoreModuleVCL.cpp 4194 2012-06-27 19:03:26Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class that implements the CoreModule GUI interface functions
//          for VCL-based modules, and an appropriate WinMain function.
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

#include <vcl.h>
#include "CoreModuleVCL.h"
#include "FileUtils.h"

WINAPI
WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  try
  {
    Application->Initialize();
    Application->Title = ( "BCI2000 " + FileUtils::ApplicationTitle() ).c_str();
    CoreModuleVCL().Run( _argc, _argv );
  }
  catch (Exception &exception)
  {
    Application->ShowException(&exception);
  }
  return 0;
}