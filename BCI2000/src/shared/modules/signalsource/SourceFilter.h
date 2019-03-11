////////////////////////////////////////////////////////////////////////////////
// $Id: SourceFilter.h 3798 2012-02-01 18:07:06Z mellinger $
// Description: A notch filter for removing power line noise, and a high pass
//   collected into a single filter.
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
#ifndef SOURCE_FILTER_H
#define SOURCE_FILTER_H

#include "IIRFilterBase.h"

class SourceFilter : public IIRFilterBase
{
 public:
  SourceFilter();
  ~SourceFilter() {}
  virtual bool AllowsVisualization() const { return false; }

 private:
  // Translate user settings into a filter definition given by
  // - overall gain,
  // - complex roots of the numerator polynomial ("zeros"),
  // - complex roots of the denominator polynomial ("poles").
  virtual void DesignFilter( const SignalProperties& input,
                             Real& gain,
                             ComplexVector& zeros,
                             ComplexVector& poles ) const;
};

#endif // SOURCE_FILTER_H
