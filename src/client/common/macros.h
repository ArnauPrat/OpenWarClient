
#ifndef OWC_MACROS_HPP
#define OWC_MACROS_HPP

#define OWC_NON_COPYABLE(className)\
      private:\
        className( const className& c ); \
        className& operator= ( const className& c );

#define OWC_NON_INSTANTIABLE(className)\
      private:\
        className();

#endif
