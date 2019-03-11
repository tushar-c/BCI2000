////////////////////////////////////////////////////////////////////////////////
// $Id: BitmapImage.h 4214 2012-06-30 14:15:25Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A bitmap image that reads/writes itself to/from a stream in
//   run-length encoding.
//   BitmapImages support subtraction and addition to allow for efficient
//   transmission of differences between subsequent frames.
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
#ifndef BITMAP_IMAGE_H
#define BITMAP_IMAGE_H

#include <iostream>
#include <cstring>
#include "Color.h"
#include "defines.h"

class BitmapImage
{
 public:
  class PixelRef;
  friend class PixelRef;

  class PixelRef
  {
   public:
    PixelRef( BitmapImage& b, int x, int y )
      : mpData( b.mpData + b.mWidth * y + x )
      {}
    bool IsBlack() const
      {
        return *mpData == 0;
      }
    operator RGBColor() const
      {
        if( *mpData & 0x8000 )
          return RGBColor::NullColor;

        int r = *mpData >> 4 & 0xf0,
            g = *mpData      & 0xf0,
            b = *mpData << 4 & 0xf0;
        return RGBColor( r | r >> 4, g | g >> 4, b | b >> 4 );
      }
    PixelRef& operator=( const RGBColor& c )
      {
        if( c == RGBColor( RGBColor::NullColor ) )
          *mpData = uint16_t( -1 );
        else
        {
          int r = c >> 20 & 0xf,
              g = c >> 12 & 0xf,
              b = c >>  4 & 0xf;
          *mpData = r << 8 | g << 4 | b;
        }
        return *this;
      }

   private:
    uint16_t* mpData;
  };

 public:
  BitmapImage( int inWidth = 0, int inHeight = 0 );
  BitmapImage( const BitmapImage& );
  BitmapImage( int inWidth, int inHeight, uint16_t* inpData );
  ~BitmapImage()
    { delete[] mpData; }
  BitmapImage& operator=( const BitmapImage& );

  bool Empty() const
    {
      return ( mWidth == 0 ) || ( mHeight == 0 );
    }

  int Width() const
    {
      return mWidth;
    }

  int Height() const
    {
      return mHeight;
    }

  const uint16_t* RawData() const
    {
      return mpData;
    }

  BitmapImage& SetBlack()
    {
      std::memset( mpData, 0, mWidth * mHeight * sizeof( *mpData ) );
      return *this;
    }

  BitmapImage& SetTransparent()
    {
      std::memset( mpData, uint16_t( -1 ), mWidth * mHeight * sizeof( *mpData ) );
      return *this;
    }

  const PixelRef operator()( int x, int y ) const
    {
      return PixelRef( *const_cast<BitmapImage*>( this ), x, y );
    }
  PixelRef operator()( int x, int y )
    {
      return PixelRef( *this, x, y );
    }

  BitmapImage& operator+=( const BitmapImage& );
  BitmapImage& operator-=( const BitmapImage& );

  BitmapImage operator-( const BitmapImage& b ) const
    {
      return BitmapImage( *this ) -= b;
    }
  BitmapImage operator+( const BitmapImage& b ) const
    {
      return BitmapImage( *this ) += b;
    }
  // For this function, negative values are interpreted as an inverted clipping
  // region. Input pixels will only replace existing pixels with negative values,
  // i.e. pixels outside the clipping region.
  BitmapImage& SetBackground( const BitmapImage& );


  std::ostream& WriteBinary( std::ostream& ) const;
  std::istream& ReadBinary( std::istream& );

 private:
  void DimensionCheck( const BitmapImage& ) const;

  int     mWidth,
          mHeight;
  uint16_t* mpData;
};

#endif // BITMAP_IMAGE_H

