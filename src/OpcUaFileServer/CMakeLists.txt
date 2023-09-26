# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
#
# prebuild
#
# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
configure_file(
    "${PROJECT_SOURCE_DIR}/OpcUaFileServer/Build/BuildConfig.h.in"
    "${PROJECT_BINARY_DIR}/BuildConfig.h"
)

configure_file(
    "${PROJECT_SOURCE_DIR}/OpcUaFileServer/linux/etc/init/OpcUaFileServer.conf.in"
    "${PROJECT_BINARY_DIR}/OpcUaFileServer.conf"
)

configure_file(
    "${PROJECT_SOURCE_DIR}/OpcUaFileServer/linux/etc/init.d/OpcUaFileServer.in"
    "${PROJECT_BINARY_DIR}/OpcUaFileServer"
)

configure_file(
    "${PROJECT_SOURCE_DIR}/OpcUaFileServer/linux/usr/lib/systemd/system/OpcUaFileServer.service.in"
    "${PROJECT_BINARY_DIR}/OpcUaFileServer.service"
)


# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
#
# build 
# 
# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
include_directories(
    ${OPCUASTACK_INSTALL_PREFIX}/usr/include/OpcUaStack${OPCUASTACK_VERSION_MAJOR}
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_BINARY_DIR}
    ${Boost_INCLUDE_DIR}
    ${OPENSSL_INCLUDE_DIR}
)

file(
    GLOB 
    OpcUaFileServer_SRC 
    ${PROJECT_SOURCE_DIR}/OpcUaFileServer/*/*.cpp
)

add_library(
    OpcUaFileServer SHARED
    ${OpcUaFileServer_SRC}
)

target_link_libraries(
    OpcUaFileServer
    ${pthread}
    ${OPCUASTACK_LIBRARY_CORE}
    ${OPCUASTACK_LIBRARY_CLIENT}
    ${OPCUASTACK_LIBRARY_SERVER}
    ${OPCUASTACK_LIBRARY_PUBSUB}
    ${Boost_LIBRARIES}
)

if (WIN32)
    target_link_libraries(OpcUaFileServer wsock32 ws2_32)

    if (MINGW)
        set(CMAKE_SHARED_LIBRARY_PREFIX "")
    endif()
endif()

# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
#
# install
# 
# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------

file (GLOB CONFIG_FILES ${PROJECT_SOURCE_DIR}/OpcUaFileServer/Config/*.xml)
install(TARGETS OpcUaFileServer DESTINATION ${INSTALL_PREFIX}/lib)

install(DIRECTORY DESTINATION ${INSTALL_PREFIX}/../var/OpcUaFileServer)
install(DIRECTORY DESTINATION ${INSTALL_PREFIX}/../var/OpcUaFileServer/FileSystem1)
install(DIRECTORY DESTINATION ${INSTALL_PREFIX}/../var/OpcUaFileServer/FileSystem2)

if (WIN32)
    install(FILES ${CONFIG_FILES} DESTINATION ${INSTALL_PREFIX}/../etc/OpcUaStack/OpcUaFileServer) 
    install(FILES ${PROJECT_SOURCE_DIR}/OpcUaFileServer/linux/log/README DESTINATION ${INSTALL_PREFIX}/../var/log/OpcUaStack/OpcUaFileServer)
    install(DIRECTORY DESTINATION ${INSTALL_PREFIX}/../var/log/OpcUaStack/OpcUaFileServer)
else()    
    install(FILES ${CONFIG_FILES} DESTINATION ${INSTALL_PREFIX}/../etc/OpcUaStack/OpcUaFileServer) 
    install(FILES ${PROJECT_SOURCE_DIR}/OpcUaFileServer/linux/log/README DESTINATION /var/log/OpcUaStack/OpcUaFileServer)
    install(DIRECTORY DESTINATION /var/log/OpcUaStack/OpcUaFileServer)

    install(PROGRAMS ${PROJECT_BINARY_DIR}/OpcUaFileServer DESTINATION /etc/init.d)
    install(FILES ${PROJECT_BINARY_DIR}/OpcUaFileServer.conf DESTINATION /etc/init)
    install(FILES ${PROJECT_BINARY_DIR}/OpcUaFileServer.service DESTINATION /usr/lib/systemd/system)
endif()

if (CPACK_BINARY_DEB)
    set( CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/OpcUaFileServer/linux/script/postinst"
                                            "${CMAKE_CURRENT_SOURCE_DIR}/OpcUaFileServer/linux/script/preinst"
                                            "${CMAKE_CURRENT_SOURCE_DIR}/OpcUaFileServer/linux/script/prerm"
                                            "${CMAKE_CURRENT_SOURCE_DIR}/OpcUaFileServer/linux/script/postrm")
endif()

if (CPACK_BINARY_RPM)
    set(CPACK_RPM_POST_INSTALL_SCRIPT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/OpcUaFileServer/linux/script/postinst)
    set(CPACK_RPM_PRE_UNINSTALL_SCRIPT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/OpcUaFileServer/linux/script/prerm)
endif()
