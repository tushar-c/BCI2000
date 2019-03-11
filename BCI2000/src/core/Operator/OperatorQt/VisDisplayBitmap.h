////////////////////////////////////////////////////////////////////////////////
// $Id: VisDisplayBitmap.h 3798 2012-02-01 18:07:06Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class for bitmap type visualization windows.
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
#ifndef VIS_DISPLAY_BITMAP_H
#define VIS_DISPLAY_BITMAP_H

#include "VisDisplayLayer.h"
#include "BitmapImage.h"

class VisDisplayBitmap : public VisDisplayLayer
{
  Q_OBJECT

 public:
  VisDisplayBitmap( const std::string& visID );
  virtual ~VisDisplayBitmap();

 public slots:
  void HandleBitmap( const BitmapImage& );

 protected:
  virtual void SetConfig( VisDisplayBase::ConfigSettings& );
  virtual void Restore();
  virtual void Save() const;

 protected:
  void paintEvent( QPaintEvent* );
  void mousePressEvent( QMouseEvent* );

 private:
  bool mWaitForReferenceFrame;
  BitmapImage mImageBuffer;
};

#endif // VIS_DISPLAY_BITMAP_H
