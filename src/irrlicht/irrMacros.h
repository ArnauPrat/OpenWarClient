
#ifndef __IRR_MACROS_H_INCLUDED__
#define __IRR_MACROS_H_INCLUDED__

#define IRR_NON_COPYABLE(className)\
      private:\
        className( const className& c ); \
        className& operator= ( const className& c );

#define IRR_NON_INSTANTIABLE(className)\
      private:\
        className();

#define IRR_BASECLASS_NON_INSTANTIABLE(className)\
      protected:\
        className();

#endif
