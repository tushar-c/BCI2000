////////////////////////////////////////////////////////////////////////////////
// $Id: CoreModuleQT.cpp 4130 2012-06-15 20:24:43Z mellinger $
// Author: griffin.milsap@gmail.com
// Description: A class that implements the CoreModule GUI interface functions
//          for QT-based modules, and an appropriate main() function.
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

#include "CoreModuleQT.h"
#include "ThreadUtils.h"
#include "BCIAssert.h"
#include <QApplication>

// In some versions of Qt4, QApplication::hasPendingEvents() always returns true.
static const int cMaxPending = 100;

CoreModuleQT::CoreModuleQT()
: mCount( 0 )
{
  bciassert( qApp == NULL );
}

CoreModuleQT::~CoreModuleQT()
{
  delete qApp;
}

void
CoreModuleQT::OnInitialize( int& ioArgc, char** ioArgv )
{
  new QApplication( ioArgc, ioArgv );
  qApp->processEvents();
}

void
CoreModuleQT::OnProcessGUIMessages()
{
  if( qApp )
  {
    qApp->sendPostedEvents();
    qApp->processEvents();
  }
#ifdef __APPLE__
  ThreadUtils::SleepFor(2);  // prevents core modules from taking 100% CPU on OSX under Qt 4.7
#endif // __APPLE__
}

bool
CoreModuleQT::OnGUIMessagesPending()
{
  return qApp && qApp->hasPendingEvents() && ( ++mCount % cMaxPending );
}

