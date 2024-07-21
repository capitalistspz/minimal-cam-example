#include "altered.h"
#include <camera/camera.h>
#include <proc_ui/procui.h>
#include <coreinit/foreground.h>

#include <cstdlib>

void* AllocSafe(uint32_t alignment, uint32_t size)
{
   while (true)
   {
      auto* mem = std::aligned_alloc(alignment, size);
      if (CAMCheckMemSegmentation(mem, size) == CAMERA_ERROR_OK)
         return mem;
      free(mem);
   }
}

void WaitForExit()
{
   while (true)
   {
      const auto procStatus = ProcUIProcessMessages(TRUE);
      if (procStatus == PROCUI_STATUS_EXITING)
         break;
      if (procStatus == PROCUI_STATUS_RELEASE_FOREGROUND)
         ProcUIDrawDoneRelease();
   }
}

void CameraEventHandler(CAMEventData*) {}

int main()
{
   ProcUIInit(OSSavesDone_ReadyToRelease);
   altered::CAMSetupInfo setupInfo{};

   auto& streamInfo = setupInfo.streamInfo;
   streamInfo.type = CAMERA_STREAM_TYPE_1;
   streamInfo.width = CAMERA_WIDTH;
   streamInfo.height = CAMERA_HEIGHT;

   int32_t memReq = CAMGetMemReq(&streamInfo);
   setupInfo.workMem.pMem = AllocSafe(0x100, memReq);
   setupInfo.workMem.size = memReq;

   setupInfo.mode.fps = CAMERA_FPS_30;

   //! Display directly on drc
   setupInfo.mode.forceDrc = TRUE;

   //! Event handler may not be null
   setupInfo.eventHandler = CameraEventHandler;

   CamError error;
   auto handle = altered::CAMInit(0, &setupInfo, &error);
   if (handle >= 0)
   {
      bool shouldOpen = true;
      while (true)
      {
         ProcUIStatus procStatus = ProcUIProcessMessages(TRUE);
         if (procStatus == PROCUI_STATUS_EXITING)
            break;
         else if (procStatus == PROCUI_STATUS_RELEASE_FOREGROUND)
         {
            ProcUIDrawDoneRelease();
            //! Releasing foreground closes the camera
            shouldOpen = true;
         }
         else if (procStatus == PROCUI_STATUS_IN_FOREGROUND && shouldOpen)
         {
            CAMOpen(handle);
            shouldOpen = false;
         }
      }
      CAMExit(handle);
   }
   free(setupInfo.workMem.pMem);
   ProcUIShutdown();
   return 0;
}
