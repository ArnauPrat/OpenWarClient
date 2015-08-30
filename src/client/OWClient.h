

#ifndef OWCLIENT_H
#define OWCLIENT_H

#include "common/macros.h"
#include "io/MYPReader.h"
#include "io/Properties.h"
#include <irrlicht.h>
#include <string>

using namespace irr;

namespace owc {

  class OWClient {
    OWC_NON_INSTANTIABLE(OWClient);
    OWC_NON_COPYABLE(OWClient);

    public:
    enum Status {
      E_DISCONNECTED,
      E_LOGGED_IN,
    };

    public:

    static int init( const c8* config_file_name );
    static int free();
    static int run();

    private:


    static std::string   user_name_;
    static std::string   token_;
    static Status  client_status_;

    static IrrlichtDevice*        device_;
    static video::IVideoDriver*   driver_;
    static scene::ISceneManager*  smgr_;
    static gui::IGUIEnvironment*  guienv_;
    static io::IFileSystem*       fsystem_;
    
    static Properties*            properties_;
    static MYPArchiveLoader*      myp_archive_loader_;

  };
}

#endif
