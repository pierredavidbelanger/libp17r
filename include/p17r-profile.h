#ifndef P17R_PROFILE_H
#define P17R_PROFILE_H

#ifdef TRACY_ENABLE
  #include <tracy/TracyC.h>
#else
    #define TracyCZone(ctx, active)
    #define TracyCZoneN(ctx, name, active)
    #define TracyCZoneNC(ctx, name, color, active)
    #define TracyCFrameMark
    #define TracyCZoneEnd(ctx)
    #define TracyCAlloc(ptr, size)
    #define TracyCFree(ptr)
#endif

#endif //P17R_PROFILE_H
