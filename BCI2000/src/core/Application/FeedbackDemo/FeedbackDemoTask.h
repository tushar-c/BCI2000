////////////////////////////////////////////////////////////////////////////////
// $Id: FeedbackDemoTask.h 3798 2012-02-01 18:07:06Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A demo feedback task.
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
#ifndef FEEDBACK_DEMO_TASK_H
#define FEEDBACK_DEMO_TASK_H

#include "FeedbackTask.h"
#include "TrialStatistics.h"

class FeedbackDemoTask : public FeedbackTask
{
 public:
  FeedbackDemoTask();
  virtual ~FeedbackDemoTask();

 private:
  // Events to be handled by FeedbackTask descendants.
  //  Events triggered by the GenericFilter interface
  virtual void OnPreflight( const SignalProperties& Input ) const;
  virtual void OnInitialize( const SignalProperties& Input );
  virtual void OnStartRun();
  virtual void OnStopRun();
  virtual void OnHalt()                                           {}
  //  Events triggered during the course of a trial
  virtual void OnTrialBegin();
  virtual void OnTrialEnd();
  virtual void OnFeedbackBegin();
  virtual void OnFeedbackEnd();
  //  Dispatching of the input signal.
  //  Each call to GenericSignal::Process() is dispatched to one of these
  //  events, depending on the phase in the sequence.
  //  There, each handler function corresponds to a phase.
  //  If a handler sets the "progress" argument to true, the application's
  //  state will switch to the next phase.
  virtual void DoPreRun(       const GenericSignal&, bool& doProgress );
  virtual void DoPreFeedback(  const GenericSignal&, bool& doProgress );
  virtual void DoFeedback(     const GenericSignal&, bool& doProgress );
  virtual void DoPostFeedback( const GenericSignal&, bool& doProgress );
  virtual void DoITI(          const GenericSignal&, bool& doProgress );

#ifndef __BORLANDC__
  void SetLabel( const char* text, RGBColor &color );
#endif // __BORLANDC__

 private:
  int   mRunCount,
        mTrialCount;
  float mCursorPosX,
        mCursorPosY,
        mCursorSpeedX,
        mCursorSpeedY;

  TrialStatistics mTrialStatistics;

#ifdef __BORLANDC__
  class TForm*  mpForm;
  class TLabel* mpLabel;
  class TShape* mpCursor,
              * mpTarget;
#else // __BORLANDC__
  class QWidget* mpForm;
  class QGraphicsScene* mpScene;
  class QGraphicsView* mpSceneView;
  class QGraphicsSimpleTextItem* mpLabel;
  class QGraphicsRectItem* mpTarget;
  class QGraphicsEllipseItem* mpCursor;
#endif // __BORLANDC__
};

#endif // FEEDBACK_DEMO_TASK_H
