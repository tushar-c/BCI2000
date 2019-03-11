////////////////////////////////////////////////////////////////////////////////
// $Id: gUSBampADC.h 3798 2012-02-01 18:07:06Z mellinger $
// Author: schalk@wadsworth.org
// Description: BCI2000 Source Module for gUSBamp devices.
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
#ifndef GUSBAMP_ADC_H
#define GUSBAMP_ADC_H

#include "GenericADC.h"
#include "GenericVisualization.h"

// Number of buffers for g.USBamp acquisition
//#define NUM_BUFS        1

#include <windows.h>
#include <vector>
#include <string>
#include <algorithm>
#include "OSThread.h"
#include "PrecisionTime.h"
#include "Expression/Expression.h"

class gUSBampADC : public GenericADC
{
 public:
            gUSBampADC();
    virtual ~gUSBampADC();

    virtual void Preflight( const SignalProperties&, SignalProperties& ) const;
    virtual void Initialize( const SignalProperties&, const SignalProperties& );
    virtual void Process( const GenericSignal&, GenericSignal& );
    virtual void Halt();

 protected:

    std::vector<std::string> m_DeviceIDs;
    std::vector< HANDLE >     m_hdev;
    //std::vector<BYTE *>      m_pBuffer[20];

    std::vector<int>         m_buffersize;
    std::vector<int>         m_iBytesperScan;
    std::vector<int>         m_numchans;
    std::vector<float>       m_LSB; // how many microVolts is one A/D unit (=SourceChGain)
    int            DetectAutoMode() const;
    int            DetermineFilterNumber() const;
    int            DetermineNotchNumber() const;
    GenericVisualization mVis;
    int            m_numdevices;
    float          m_filterhighpass, m_filterlowpass, m_notchhighpass, m_notchlowpass;   // at the moment, only one filter setting for all channels and all devices
    int            m_filtermodelorder, m_filtertype, m_notchmodelorder, m_notchtype;
    std::string    mMasterDeviceID;  // device ID for the master device (exactly one device has to be master)
    int            m_timeoutms;
    int            mSampleBlockSize;
    bool           mFloatOutput;
    bool           m_digitalinput;
    bool           m_digitalOutput;
    int            m_acqmode;         // normal, calibrate, or impedance
    bool           m_digitalOut1;
    int mTotalChs;
    int NUM_BUFS, mBufferSize;
    int mThreadBlock, mProcBlock;
    Expression mDigExpression;



    class AcquireThread;
    HANDLE acquireEventRead;
    friend class AcquireThread;
    class AcquireThread : public OSThread
    {
      public:
        AcquireThread( gUSBampADC * parent )
        : amp( parent )
        {
            SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
        }
        float getNextValue();
        int getNextValueInt();
        private:
            int Execute();
            gUSBampADC *amp;
            int mBufferReadPos, mBufferWritePos;
            float *mData;
            int *mDataInt;
            std::vector< std::vector<OVERLAPPED> >     m_ov;
            std::vector< std::vector<HANDLE> >         m_hEvent;
    } *mpAcquireThread;

};

#endif // GUSBAMP_ADC_H




