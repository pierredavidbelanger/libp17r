#ifndef P17R_PROFILE_H
#define P17R_PROFILE_H

#ifdef TRACY_ENABLE
  #include <tracy/TracyC.h>
#else
  #define TracyCFrameMark
  #define TracyCZone(ctx, active)
  #define TracyCZoneEnd(ctx)
  #define TracyCAlloc(ptr, size)
  #define TracyCFree(ptr)
#endif

#endif //P17R_PROFILE_H
