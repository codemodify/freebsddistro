
TEMPLATE    = subdirs
CONFIG     += ordered



# blocks
#################################################################################
CustomDockWidget          = yes
Jsonifier                 = yes
FileSystemOperations      = yes
FileSystemOperationsUi    = yes
IconProvider              = yes
FileListView              = yes
FileTreeView              = yes
Settings                  = yes
Styling                   = yes
FileSearch                = yes
FileSearchUi              = yes
FileEditor                = yes
FileLauncher              = yes
Deployer                  = yes
Skinning                  = yes
Pluginer                  = yes
PluginerUi                = yes
Configurator              = yes
ConfiguratorReader        = yes
_blocks_tests_            = yes

# applications
#################################################################################
#Architector                                   = yes
#Architecture.Renderer.2DRenderer              = yes
Commander                                     = yes
Console                                       = yes
TextEditor                                    = yes
Developer                                     = yes
#PictureViewer                                 = yes
ScreenRecorder                                = no
SchemaDrawer                                  = no
CommunicationCenter                           = no

# server
#################################################################################
InternetServerPlatformLibrary   = yes
InternetServerPlatform          = yes
WebServer                       = yes
JerryMouse                      = yes
CommunicationCenterServer       = no





# building for 'blocks'
#################################################################################
message( "// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----"  )
message( "// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----"  )
message( "// The following 'blocks' to be built"                                )
message( "// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----"  )
message( "// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----"  )
contains( CustomDockWidget,         yes ){ message( "CustomDockWidget"          )   SUBDIRS += ../blocks/CustomDockWidget                   }
contains( Jsonifier,                yes ){ message( "Jsonifier"                 )   SUBDIRS += ../blocks/Jsonifier                          }
contains( FileSystemOperations,     yes ){ message( "FileSystemOperations"      )   SUBDIRS += ../blocks/FileSystemOperations               }
contains( FileSystemOperationsUi,   yes ){ message( "FileSystemOperationsUi"    )   SUBDIRS += ../blocks/FileSystemOperationsUi             }
contains( IconProvider,             yes ){ message( "IconProvider"              )   SUBDIRS += ../blocks/IconProvider                       }
contains( FileListView,             yes ){ message( "FileListView"              )   SUBDIRS += ../blocks/FileListView                       }
contains( FileTreeView,             yes ){ message( "FileTreeView"              )   SUBDIRS += ../blocks/FileTreeView                       }
contains( Settings,                 yes ){ message( "Settings"                  )   SUBDIRS += ../blocks/Settings                           }
contains( Styling,                  yes ){ message( "Styling"                   )   SUBDIRS += ../blocks/Styling                            }
contains( FileSearch,               yes ){ message( "FileSearch"                )   SUBDIRS += ../blocks/FileSearch                         }
contains( FileSearchUi,             yes ){ message( "FileSearchUi"              )   SUBDIRS += ../blocks/FileSearchUi                       }
contains( FileEditor,               yes ){ message( "FileEditor"                )   SUBDIRS += ../blocks/FileEditor                         }
contains( FileLauncher,             yes ){ message( "FileLauncher"              )   SUBDIRS += ../blocks/FileLauncher                       }
contains( Deployer,                 yes ){ message( "Deployer"                  )   SUBDIRS += ../blocks/Deployer                           }
contains( Skinning,                 yes ){ message( "Skinning"                  )   SUBDIRS += ../blocks/Skinning                           }
contains( Pluginer,                 yes ){ message( "Pluginer"                  )   SUBDIRS += ../blocks/Pluginer                           }
contains( PluginerUi,               yes ){ message( "PluginerUi"                )   SUBDIRS += ../blocks/PluginerUi                         }
contains( Configurator,             yes ){ message( "Configurator"              )   SUBDIRS += ../blocks/Configurator                       }
contains( ConfiguratorReader,       yes ){ message( "ConfiguratorReader"        )   SUBDIRS += ../blocks/ConfiguratorReader                 }
contains( _blocks_tests_,           yes ){
        message( "_blocks_tests_" )
        SUBDIRS += ../blocks/!_tests_!/FileSystemOperationsUi
        SUBDIRS += ../blocks/!_tests_!/FileSearchUi
        SUBDIRS += ../blocks/!_tests_!/PluginerUi
        SUBDIRS += ../blocks/!_tests_!/PluginerUi.TestPluginOneInterfaceImplemented
        SUBDIRS += ../blocks/!_tests_!/Configurator
        SUBDIRS += ../blocks/!_tests_!/Jsonifier
}

# building for 'applications'
#################################################################################
message( "" )
message( ""  )
message( "// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----"  )
message( "// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----"  )
message( "// The following 'applications' to be built"                                                                  )
message( "// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----"  )
message( "// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----"  )
contains( Architector,                                 yes ){ message( "Architector"                                    )   SUBDIRS += ../applications/Architector                                   }
contains( Architecture.Renderer.2DRenderer,            yes ){ message( "Architector.Plugins.Renderer.TwoDRenderer"      )   SUBDIRS += ../applications/Architecture.Renderer.2DRenderer              }
contains( Commander,                                   yes ){ message( "Commander"                                      )   SUBDIRS += ../applications/Commander                                     }
contains( Console,                                     yes ){ message( "Console"                                        )   SUBDIRS += ../applications/Console                                       }
contains( TextEditor,                                  yes ){ message( "TextEditor"                                     )   SUBDIRS += ../applications/TextEditor                                    }
contains( Developer,                                   yes ){ message( "Developer"                                      )   SUBDIRS += ../applications/Developer                                     }
contains( PictureViewer,                               yes ){ message( "PictureViewer"                                  )   SUBDIRS += ../applications/PictureViewer                                 }
contains( ScreenRecorder,                              yes ){ message( "ScreenRecorder"                                 )   SUBDIRS += ../applications/ScreenRecorder                                }
contains( SchemaDrawer,                                yes ){ message( "SchemaDrawer"                                   )   SUBDIRS += ../applications/SchemaDrawer                                  }
contains( CommunicationCenter,                         yes ){ message( "CommunicationCenter"                            )   SUBDIRS += ../applications/CommunicationCenter                           }

# building for 'server'
#################################################################################
message( "// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----"  )
message( "// The following 'server' to be built"                                )
message( "// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----"  )
contains( InternetServerPlatformLibrary,   yes ){ message( "InternetServerPlatformLibrary"   )   SUBDIRS += ../server/InternetServerPlatformLibrary   }
contains( InternetServerPlatform,          yes ){ message( "InternetServerPlatform"          )   SUBDIRS += ../server/InternetServerPlatform          }
contains( WebServer,                       yes ){ message( "WebServer"                       )   SUBDIRS += ../server/WebServer                       }
contains( JerryMouse,                      yes ){ message( "JerryMouse"                      )   SUBDIRS += ../server/JerryMouse                      }
contains( CommunicationCenterServer,       yes ){ message( "CommunicationCenterServer"       )   SUBDIRS += ../server/CommunicationCenterServer       }

