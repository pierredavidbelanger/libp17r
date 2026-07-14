#ifndef P17R_PROFILE_H
#define P17R_PROFILE_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef TRACY_ENABLE
    #include <tracy/TracyC.h>
    #define WaitForTracyC() while (!TracyCIsConnected) {}
#else
    #define TracyCIsConnected 0
    #define TracyCZone(ctx, active)
    #define TracyCZoneN(ctx, name, active)
    #define TracyCZoneNC(ctx, name, color, active)
    #define TracyCFrameMark
    #define TracyCZoneEnd(ctx)
    #define TracyCAlloc(ptr, size)
    #define TracyCFree(ptr)
    #define WaitForTracyC() (void)0
#endif

#ifdef __cplusplus
}
#endif

#endif //P17R_PROFILE_H
