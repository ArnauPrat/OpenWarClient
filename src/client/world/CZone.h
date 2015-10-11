


#ifndef __CZONE_H_INCLUDED__
#define __CZONE_H_INCLUDED__

#include <irrlicht.h>
#include <CProperties.h>

using namespace irr;

namespace owc {
  namespace world {
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
