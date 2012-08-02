#include <fstream>

#include "TApplication.h"
#include "TVirtualX.h"
#include "testframe.h"

int main(int argc, char ** argv)
{
   using namespace ROOT::CocoaTest;

   TApplication app("test_app", &argc, argv);
   
   TestFrame *mainFrame = new TestFrame(0, 500, 500, kMainFrame, 0xff0000);
   TestFrame *childFrame = new TestFrame(mainFrame, 300, 300, kChildFrame, 0xff6600);
   gVirtualX->MoveWindow(childFrame->GetId(), 100, 100);
   TestFrame *childChildFrame = new TestFrame(childFrame, 100, 100, kChildFrame, 0xff9900);
   gVirtualX->MoveWindow(childChildFrame->GetId(), 100, 100);
   gVirtualX->MapSubwindows(childFrame->GetId());
   gVirtualX->MapSubwindows(mainFrame->GetId());
   gVirtualX->MapRaised(mainFrame->GetId());

#ifdef TEST1
   mainFrame->AddInput(kEnterWindowMask | kLeaveWindowMask);
   childFrame->AddInput(kButtonPressMask | kButtonReleaseMask | kLeaveWindowMask | kEnterWindowMask);
   childChildFrame->AddInput(kEnterWindowMask | kLeaveWindowMask);
#elif defined(TEST2)
   mainFrame->AddInput(kEnterWindowMask | kLeaveWindowMask);
   childChildFrame->AddInput(kEnterWindowMask | kLeaveWindowMask);
//   childFrame->AddInput(kEnterWindowMask | kLeaveWindowMask);
   gVirtualX->GrabButton(childFrame->GetId(), kButton1, kAnyModifier, kEnterWindowMask | kLeaveWindowMask, kNone, kNone, kTRUE);
   //Passive grab, owner_events == true. even if grab was activated, all windows will continue receiving events correctly.
#endif

   app.Run();
}
