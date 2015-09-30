

#ifndef OWCLIENT_H
#define OWCLIENT_H

#include <irrTypes.h>
#include <irrMacros.h>
#include "io/Properties.h"
#include <irrlicht.h>
#include <CMYPReader.h>
#include <string>

using namespace irr;

namespace owc {

  class OWClient {
    IRR_NON_INSTANTIABLE(OWClient);
    IRR_NON_COPYABLE(OWClient);

    public:
    enum Status {
      E_DISCONNECTED,
      E_LOGGED_IN,
    };

    public:

    static u32 init( const c8* config_file_name );
    static u32 free();
    static u32 run();

    private:


    static std::string   user_name_;
    static std::string   token_;
    static Status  client_status_;

    static IrrlichtDevice*        device_;
    static video::IVideoDriver*   driver_;
    static scene::ISceneManager*  smgr_;
    static gui::IGUIEnvironment*  guienv_;
    static io::IFileSystem*       fsystem_;
    static ILogger*               logger_;
    
    static Properties*            properties_;

    static io::CMYPArchiveLoader*     myp_archive_loader_;


  };
}

#endif
