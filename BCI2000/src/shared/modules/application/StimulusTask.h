////////////////////////////////////////////////////////////////////////////////
// $Id: StimulusTask.h 3798 2012-02-01 18:07:06Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A base class for application modules that present a sequence
//   of stimuli.
//   Inheriting from ApplicationBase, descendants of StimulusTask have access
//   to the AppLog, AppLog.File and AppLog.Screen streams declared in
//   ApplicationBase.
//
//   This class performs sequencing, and dispatches GenericFilter::Process()
//   calls to its virtual member functions, depending on its current state.
//
//   Child classes (descendants) of StimulusTask implement event
//   handlers by overriding its virtual functions, and populate the stimulus
//   map associating stimulus codes with sets of stimuli.
//
//   Sequence of events         Typical application behavior
//
//   OnPreflight
//   OnInitialize
//   OnStartRun                 display initial message
//   DoPreRun*
//   Loop {
//    OnNextStimulusCode        provide a stimulus code, or 0 to finish run
//    OnPreSequence             determine attended target
//    DoPreSequence*
//    OnSequenceBegin
//    Loop {
//      OnStimulusBegin         present stimulus
//      DoStimulus*
//      OnStimulusEnd           hide visual stimulus
//      OnNextStimulusCode      provide a stimulus code, or 0 to finish sequence
//      DoISI*
//      [OnClassInput]*         store classification input (copy/free modes only)
//                              -- this event may occur any time during the
//                              sequence, and during the post sequence phase
//    }
//    OnSequenceEnd
//    DoPostSequence*
//    [OnClassResult]           determine selected target (copy/free modes only)
//   }
//   OnPostRun
//   DoPostRun*
//   OnStopRun                  display final message
//   OnHalt
//
//   Events marked with * may occur multiple times in a row.
//   Progress from one state to the next will occur according to the sequencing
//   parameters, or if requested by a handler via its doProgress output argument.
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
#ifndef STIMULUS_TASK_H
#define STIMULUS_TASK_H

#include "ApplicationBase.h"
#include "ApplicationWindow.h"
#include "Association.h"
#include "TextField.h"

// Values of the PhaseInSequence state:
namespace PhaseInSequence
{
  enum
  {
    None = 0,
    PreSequence = 1,
    Sequence = 2,
    PostSequence = 3,
  };
}

namespace InterpretModes
{
  enum
  {
    None,
    Free,
    Copy,
  };
}

class StimulusTask : public ApplicationBase
{
 private:
  enum TaskPhases
  {
    none,
    preRun,
    preSequence,
    stimulus,
    ISI,
    postSequence,
    postRun,
  };

 protected:
  // Events to be handled by StimulusTask descendants.
  //  Events triggered by the GenericFilter interface
  virtual void OnPreflight( const SignalProperties& Input ) const {}
  virtual void OnInitialize( const SignalProperties& Input ) {}
  virtual void OnStartRun() {}
  virtual void OnStopRun() {}
  virtual void OnHalt() {}
  //  Events triggered during the course of a sequence
  virtual void OnPreSequence() {}
  virtual void OnSequenceBegin() {}
  virtual void OnSequenceEnd() {}
  virtual void OnStimulusBegin( int stimulusCode );
  virtual void OnStimulusEnd( int stimulusCode );
  virtual void OnPostRun() {}
  virtual void OnClassInput( int stimulusCode, const GenericSignal& ) {}
  virtual Target* OnClassResult( const ClassResult& );
  // Events
  //  Dispatching of the input signal.
  //  Each call to GenericSignal::Process() is dispatched to one of these
  //  events, depending on the phase in the sequence.
  //  There, each handler function corresponds to a phase.
  //  If a handler sets the "progress" argument to true, the application's
  //  state will switch to the next phase independently of the phases' pre-set
  //  durations.
  virtual void DoPreRun(       const GenericSignal&, bool& doProgress ) {}
  virtual void DoPreSequence(  const GenericSignal&, bool& doProgress ) {}
  virtual void DoStimulus(     const GenericSignal&, bool& doProgress ) {}
  virtual void DoISI(          const GenericSignal&, bool& doProgress ) {}
  virtual void DoPostSequence( const GenericSignal&, bool& doProgress ) {}
  virtual void DoPostRun(      const GenericSignal&, bool& doProgress ) {}
  // A virtual interface for sequence access.
  // An implementation of OnNextStimulusCode() should return the next element of
  // the current sequence, or zero to indicate the end of the sequence.
  // A null sequence indicates the end of a run, i.e. the current run will end when
  // OnNextStimulusCode() returns two zeros in sequence.
  virtual int OnNextStimulusCode() = 0;

 protected:
  // Access to the Associations property.
  //  An association map maps stimulus codes to sets of stimuli and sets of
  //  targets. Stimuli are used for presentation, while targets are used for
  //  classification, and selected according to the classification result.
  AssociationMap& Associations()
    { return mAssociations; }
  const AssociationMap& Associations() const
    { return mAssociations; }

  // Access to the AttendedTarget property.
  //  The StimulusType state indicates whether the current stimulus is associated
  //  with the target given by the AttendedTarget property.
  StimulusTask& SetAttendedTarget( Target* inTarget )
    { mpAttendedTarget = inTarget; return *this; }
  Target* AttendedTarget() const
    { return mpAttendedTarget; }

  // Access to the Display property.
  GUI::GraphDisplay& Display()
    { return mrDisplay; }
  const GUI::GraphDisplay& Display() const
    { return mrDisplay; }

  // User message display.
  void DisplayMessage( const std::string& );

 protected:
  StimulusTask();

 public:
  virtual ~StimulusTask();

  // Implementation of the GenericFilter interface.
  // Normally, there should be no need to override these functions from
  // a descendant class.
  virtual void Preflight( const SignalProperties&, SignalProperties& ) const;
  virtual void Initialize( const SignalProperties&, const SignalProperties& );
  virtual void Process( const GenericSignal&, GenericSignal& );
  virtual void StartRun();
  virtual void StopRun();
  virtual void Halt();

 private:
  int mPhase,
      mBlocksInPhase,
      mBlocksSinceStimulus,
      mISIDuration,
      mStimulusCode;

  int mPreRunDuration,
      mPostRunDuration,
      mPreSequenceDuration,
      mPostSequenceDuration,
      mInterpretMode,
      mStimulusDuration,
      mISIMinDuration,
      mISIMaxDuration,
      mStimToClassDuration;

  // Display elements.
  ApplicationWindow& mrDisplay;
  TextField*         mpMessageField;

  // Stimulus/Target configuration
  AssociationMap mAssociations;

  ClassResult    mClassResult;
  std::set<int>  mCodesPresented;
  Target*        mpAttendedTarget;
};

#endif // STIMULUS_TASK_H
