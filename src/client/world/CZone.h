


#ifndef __CZONE_H_INCLUDED__
#define __CZONE_H_INCLUDED__

#include <irrlicht.h>
#include <CProperties.h>

using namespace irr;

namespace owc {
  namespace world {

    struct SSectorData {

      SSectorData( s32 sizex, s32 sizey, s32 baseFactor, s32 offsetFactor ) 
        : SizeX(sizex), 
          SizeY(sizey), 
          BaseFactor(baseFactor), 
          OffsetFactor(offsetFactor) {}

      s32 SizeX;
      s32 SizeY;
      s32 BaseFactor;
      s32 OffsetFactor;
    };

    class CZone
    {
      public:
        CZone ( scene::ISceneManager* SceneManager );
        virtual ~CZone ();

        u32 loadZone( u32 zoneId );

        u32 getZoneId();

      private:
        u32                     ZoneId;
        io::CProperties         Properties;
        scene::ISceneManager*   SceneManager;

    };
  }
}

#endif
