#include <fstream>

#include "TApplication.h"
#include "TVirtualX.h"
#include "testframe.h"

#define TEST5

int main(int argc, char ** argv)
{
   using namespace ROOT::CocoaTest;

   TApplication app("test_app", &argc, argv);
   
   TestFrame *mainFrame = new TestFrame(0, 500, 500, kMainFrame, 0xff0000);
   TestFrame *childFrame = new TestFrame(mainFrame, 400, 300, kChildFrame, 0xff6600);
   gVirtualX->MoveWindow(childFrame->GetId(), 100, 100);
   TestFrame *childChildFrame = new TestFrame(childFrame, 300, 100, kChildFrame, 0xff9900);
   gVirtualX->MoveWindow(childChildFrame->GetId(), 100, 100);
   gVirtualX->MapSubwindows(childFrame->GetId());
   gVirtualX->MapSubwindows(mainFrame->GetId());
   gVirtualX->MapRaised(mainFrame->GetId());

   //Test case 1: "middle" window has a grab, and
#ifdef TEST1
   mainFrame->AddInput(kEnterWindowMask | kLeaveWindowMask);
   childFrame->AddInput(kEnterWindowMask | kLeaveWindowMask);
   childChildFrame->AddInput(kEnterWindowMask | kLeaveWindowMask);
#elif defined(TEST2)
   //Masks for the first window:
   mainFrame->AddInput(kEnterWindowMask | kLeaveWindowMask);
   childFrame->AddInput(kEnterWindowMask | kLeaveWindowMask);
   childChildFrame->AddInput(kEnterWindowMask | kLeaveWindowMask);


   //Create the second window:
   TestFrame *main2 = new TestFrame(0, 500, 500, kMainFrame, 0xffff00);
   TestFrame *child2 = new TestFrame(main2, 300, 300, kChildFrame, 0xff00);
   
   main2->AddInput(kEnterWindowMask | kLeaveWindowMask);
   child2->AddInput(kEnterWindowMask | kLeaveWindowMask);
   //Place top-level windows side-by-side.
   gVirtualX->MoveWindow(mainFrame->GetId(), 600, 600);
   gVirtualX->MoveWindow(main2->GetId(), 1100, 600);
   
   gVirtualX->MapSubwindows(main2->GetId());
   gVirtualX->MapRaised(main2->GetId());
#elif defined (TEST3)
   mainFrame->AddInput(kEnterWindowMask | kLeaveWindowMask);
   //childFrame->AddInput(kEnterWindowMask | kLeaveWindowMask | kButtonPressMask);
   childFrame->AddInput(kEnterWindowMask | kLeaveWindowMask | kButtonPressMask | kButtonReleaseMask);
   childChildFrame->AddInput(kEnterWindowMask | kLeaveWindowMask);
#elif defined (TEST4)
   mainFrame->AddInput(kEnterWindowMask | kLeaveWindowMask);
   childFrame->AddInput(kEnterWindowMask | kLeaveWindowMask | kButtonPressMask | kButtonReleaseMask);//this window can activate implicit grab.
   childChildFrame->AddInput(kEnterWindowMask | kLeaveWindowMask);


   //Create the second window:
   TestFrame *main2 = new TestFrame(0, 500, 500, kMainFrame, 0xffff00);
   TestFrame *child2 = new TestFrame(main2, 300, 300, kChildFrame, 0xff00);
   
   main2->AddInput(kEnterWindowMask | kLeaveWindowMask | kButtonPressMask | kButtonReleaseMask);//this window can activate implicit grab.
   child2->AddInput(kEnterWindowMask | kLeaveWindowMask);
   //Place top-level windows side-by-side.
   gVirtualX->MoveWindow(mainFrame->GetId(), 600, 600);
   gVirtualX->MoveWindow(main2->GetId(), 1100, 600);
   
   gVirtualX->MapSubwindows(main2->GetId());
   gVirtualX->MapRaised(main2->GetId());
#elif defined (TEST5)
   mainFrame->AddInput(kEnterWindowMask | kLeaveWindowMask);
   childFrame->AddInput(kEnterWindowMask | kLeaveWindowMask);
   childChildFrame->AddInput(kEnterWindowMask | kLeaveWindowMask);

   gVirtualX->GrabButton(childFrame->GetId(), kButton1, kAnyModifier, kButtonPressMask | kButtonReleaseMask | kEnterWindowMask | kLeaveWindowMask, kNone, kNone, kTRUE);
#elif defined (TEST6)
   mainFrame->AddInput(kEnterWindowMask | kLeaveWindowMask);
   childFrame->AddInput(kEnterWindowMask | kLeaveWindowMask);
   childChildFrame->AddInput(kEnterWindowMask | kLeaveWindowMask);

   gVirtualX->GrabButton(childFrame->GetId(), kButton1, kAnyModifier, kNone, kNone, kNone, kTRUE);
#endif

   app.Run();
}
