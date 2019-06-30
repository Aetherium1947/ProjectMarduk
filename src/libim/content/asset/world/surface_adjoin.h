#ifndef LIBIM_SURFACE_ADJOIN_H
#define LIBIM_SURFACE_ADJOIN_H
#include <cstdint>
#include <string>
#include <optional>

namespace libim::content::asset {

    struct SurfaceAdjoin final
    {
        enum  Flag
        {
            Visible            = 0x1,
            AllowMovement      = 0x2,
            AllowSound         = 0x4,
            AllowPlayerOnly    = 0x8,
            AllowAiOnly        = 0x10
        };

        Flag flags;
        std::optional<std::size_t> mirrorIdx; // mirror surface
        std::optional<std::size_t> surfaceIdx;
        float distance;
    };


    /* Jones3D engine's adjoin struct
        struct SurfaceAdjoin
        {
          int flags;
          int unknown1;
          Surface *pSurface;
          SurfaceAdjoin *pMirror;
          int unknown4;
          int unknown5;
          float distance;
        };
    */
}

#endif // LIBIM_SURFACE_ADJOIN_H