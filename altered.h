#pragma once
#include <camera/camera.h>

/**
 * Things that are not quite correct in WUT
 */
namespace altered
{
   struct CAMMode
   {
      BOOL forceDrc;
      CamFps fps;
   };

   struct CAMSetupInfo
   {
      CAMStreamInfo streamInfo;
      CAMWorkMem workMem;
      CAMEventHandler eventHandler;
      altered::CAMMode mode;
      OS_THREAD_ATTRIB threadAttrib;
      WUT_PADDING_BYTES(0x10);
   };
   WUT_CHECK_OFFSET(CAMSetupInfo, 0x00, streamInfo);
   WUT_CHECK_OFFSET(CAMSetupInfo, 0x0C, workMem);
   WUT_CHECK_OFFSET(CAMSetupInfo, 0x14, eventHandler);
   WUT_CHECK_OFFSET(CAMSetupInfo, 0x18, mode);
   WUT_CHECK_OFFSET(CAMSetupInfo, 0x20, threadAttrib);
   WUT_CHECK_SIZE(CAMSetupInfo, 0x34);

   inline CAMHandle CAMInit(uint32_t instance, altered::CAMSetupInfo* setupInfo, CamError* err)
   {
      return ::CAMInit(instance, reinterpret_cast<::CAMSetupInfo*>(setupInfo), reinterpret_cast<CAMError*>(err));
   }
}