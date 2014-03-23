#ifndef __PluginDefinition_H__
#define __PluginDefinition_H__

// Qt headers
#include <QtPlugin>

#define DeclarePluginInterfaceCommonPartBegin( CustomInterface )                                \
        class CustomInterface                                                                   \
        {                                                                                       \
            public:                                                                             \
                virtual ~CustomInterface(){}

#define DeclarePluginInterfaceCommonPartEnd( CustomInterface )                                  \
        };                                                                                      \
        Q_DECLARE_INTERFACE( CustomInterface, #CustomInterface )

#define DeclarePluginInterface( CustomInterface, ... )                                          \
        DeclarePluginInterfaceCommonPartBegin( CustomInterface )                                \
                __VA_ARGS__                                                                     \
        DeclarePluginInterfaceCommonPartEnd( CustomInterface )

#define DeclarePluginInterface1( CustomInterface, method1 )                                     \
        DeclarePluginInterfaceCommonPartBegin( CustomInterface )                                \
                virtual method1 = 0;                                                            \
        DeclarePluginInterfaceCommonPartEnd( CustomInterface )

#define DeclarePluginInterface2( CustomInterface, method1, method2 )                            \
        DeclarePluginInterfaceCommonPartBegin( CustomInterface )                                \
                virtual method1 = 0;                                                            \
                virtual method2 = 0;                                                            \
        DeclarePluginInterfaceCommonPartEnd( CustomInterface )

#define DeclarePluginInterface3( CustomInterface, method1, method2, method3 )                   \
        DeclarePluginInterfaceCommonPartBegin( CustomInterface )                                \
                virtual method1 = 0;                                                            \
                virtual method2 = 0;                                                            \
                virtual method3 = 0;                                                            \
        DeclarePluginInterfaceCommonPartEnd( CustomInterface )

#define DeclarePluginInterface4( CustomInterface, method1, method2, method3, method4 )          \
        DeclarePluginInterfaceCommonPartBegin( CustomInterface )                                \
                virtual method1 = 0;                                                            \
                virtual method2 = 0;                                                            \
                virtual method3 = 0;                                                            \
                virtual method4 = 0;                                                            \
        DeclarePluginInterfaceCommonPartEnd( CustomInterface )

#define DeclarePluginInterface5( CustomInterface, method1, method2, method3, method4, method5 ) \
        DeclarePluginInterfaceCommonPartBegin( CustomInterface )                                \
                virtual method1 = 0;                                                            \
                virtual method2 = 0;                                                            \
                virtual method3 = 0;                                                            \
                virtual method4 = 0;                                                            \
                virtual method5 = 0;                                                            \
        DeclarePluginInterfaceCommonPartEnd( CustomInterface )

#define DeclarePluginInterface6( CustomInterface, method1, method2, method3, method4, method5, method6 ) \
        DeclarePluginInterfaceCommonPartBegin( CustomInterface )                                \
                virtual method1 = 0;                                                            \
                virtual method2 = 0;                                                            \
                virtual method3 = 0;                                                            \
                virtual method4 = 0;                                                            \
                virtual method5 = 0;                                                            \
                virtual method6 = 0;                                                            \
        DeclarePluginInterfaceCommonPartEnd( CustomInterface )

#define DeclarePluginInterface7( CustomInterface, method1, method2, method3, method4, method5, method6, method7 ) \
        DeclarePluginInterfaceCommonPartBegin( CustomInterface )                                \
                virtual method1 = 0;                                                            \
                virtual method2 = 0;                                                            \
                virtual method3 = 0;                                                            \
                virtual method4 = 0;                                                            \
                virtual method5 = 0;                                                            \
                virtual method6 = 0;                                                            \
                virtual method7 = 0;                                                            \
        DeclarePluginInterfaceCommonPartEnd( CustomInterface )

#define DeclarePluginInterface8( CustomInterface, method1, method2, method3, method4, method5, method6, method7, method8 ) \
        DeclarePluginInterfaceCommonPartBegin( CustomInterface )                                \
                virtual method1 = 0;                                                            \
                virtual method2 = 0;                                                            \
                virtual method3 = 0;                                                            \
                virtual method4 = 0;                                                            \
                virtual method5 = 0;                                                            \
                virtual method6 = 0;                                                            \
                virtual method7 = 0;                                                            \
                virtual method8 = 0;                                                            \
        DeclarePluginInterfaceCommonPartEnd( CustomInterface )

#define DeclarePluginInterface9( CustomInterface, method1, method2, method3, method4, method5, method6, method7, method8, method9 ) \
        DeclarePluginInterfaceCommonPartBegin( CustomInterface )                                \
                virtual method1 = 0;                                                            \
                virtual method2 = 0;                                                            \
                virtual method3 = 0;                                                            \
                virtual method4 = 0;                                                            \
                virtual method5 = 0;                                                            \
                virtual method6 = 0;                                                            \
                virtual method7 = 0;                                                            \
                virtual method8 = 0;                                                            \
                virtual method9 = 0;                                                            \
        DeclarePluginInterfaceCommonPartEnd( CustomInterface )

#define DeclarePluginInterface10( CustomInterface, method1, method2, method3, method4, method5, method6, method7, method8, method9, method10 ) \
        DeclarePluginInterfaceCommonPartBegin( CustomInterface )                                \
                virtual method1 = 0;                                                            \
                virtual method2 = 0;                                                            \
                virtual method3 = 0;                                                            \
                virtual method4 = 0;                                                            \
                virtual method5 = 0;                                                            \
                virtual method6 = 0;                                                            \
                virtual method7 = 0;                                                            \
                virtual method8 = 0;                                                            \
                virtual method9 = 0;                                                            \
                virtual method10 = 0;                                                           \
        DeclarePluginInterfaceCommonPartEnd( CustomInterface )

#endif
