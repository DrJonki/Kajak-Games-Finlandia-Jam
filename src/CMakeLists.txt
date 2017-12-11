set(INCDIR ${CMAKE_SOURCE_DIR}/include/Jam)
set(SRCDIR ${CMAKE_CURRENT_SOURCE_DIR})

# Sources here
set(SRC
  # Core files
  ${INCDIR}/Entity.hpp
  ${SRCDIR}/Entity.cpp
  ${INCDIR}/Instance.hpp
  ${SRCDIR}/Instance.cpp
  ${INCDIR}/ConfigManager.hpp
  ${SRCDIR}/ConfigManager.cpp
  ${INCDIR}/ResourceManager.hpp
  ${SRCDIR}/ResourceManager.cpp
  ${INCDIR}/AnimatedSprite.hpp
  ${SRCDIR}/AnimatedSprite.cpp
  ${INCDIR}/ParticleEmitter.hpp
  ${SRCDIR}/ParticleEmitter.cpp
  ${INCDIR}/Layer.hpp
  ${INCDIR}/Layer.inl
  ${SRCDIR}/Layer.cpp
  ${INCDIR}/Scene.hpp
  ${SRCDIR}/Scene.cpp
  ${INCDIR}/PostProcessor.hpp
  ${INCDIR}/PostProcessor.inl
  ${SRCDIR}/PostProcessor.cpp
  ${INCDIR}/Randomizer.hpp
  ${INCDIR}/Randomizer.inl
  ${SRCDIR}/Randomizer.cpp
  ${INCDIR}/PostEffect.hpp
  ${SRCDIR}/PostEffect.cpp
  
  # Entities
  ${INCDIR}/Entities/InterpolatesTransform.hpp
  ${SRCDIR}/Entities/InterpolatesTransform.cpp
  ${INCDIR}/Entities/ListensMessages.hpp
  ${SRCDIR}/Entities/ListensMessages.cpp
  ${INCDIR}/Entities/Obstacle.hpp
  ${SRCDIR}/Entities/Obstacle.cpp
  ${INCDIR}/Entities/Particle.hpp
  ${SRCDIR}/Entities/Particle.cpp
  ${INCDIR}/Entities/Player.hpp
  ${SRCDIR}/Entities/Player.cpp
  ${INCDIR}/Entities/GenericEntity.hpp
  ${INCDIR}/Entities/GenericEntity.inl
  
  # Scenes
  ${INCDIR}/Scenes/LevelScene.hpp
  ${SRCDIR}/Scenes/LevelScene.cpp
  ${INCDIR}/Scenes/TitleScene.hpp
  ${SRCDIR}/Scenes/TitleScene.cpp

  ${SRCDIR}/main.cpp
)

add_executable(Jam ${SRC})
add_dependencies(Jam SFML GLM JSON)
add_definitions(-DSFML_STATIC)

set(CMK_SLP ${CMAKE_STATIC_LIBRARY_PREFIX})
set(CMK_SLS ${CMAKE_STATIC_LIBRARY_SUFFIX})

ExternalProject_Get_Property(SFML BINARY_DIR)
target_link_libraries(Jam
  # SFML libs
  debug ${BINARY_DIR}/lib/${CMAKE_CFG_INTDIR}/${CMK_SLP}sfml-audio-s-d${CMK_SLS}
  debug ${BINARY_DIR}/lib/${CMAKE_CFG_INTDIR}/${CMK_SLP}sfml-graphics-s-d${CMK_SLS}
  debug ${BINARY_DIR}/lib/${CMAKE_CFG_INTDIR}/${CMK_SLP}sfml-network-s-d${CMK_SLS}
  debug ${BINARY_DIR}/lib/${CMAKE_CFG_INTDIR}/${CMK_SLP}sfml-window-s-d${CMK_SLS}
  debug ${BINARY_DIR}/lib/${CMAKE_CFG_INTDIR}/${CMK_SLP}sfml-system-s-d${CMK_SLS}
)
target_link_libraries(Jam
  # SFML libs
  optimized ${BINARY_DIR}/lib/${CMAKE_CFG_INTDIR}/${CMK_SLP}sfml-audio-s${CMK_SLS}
  optimized ${BINARY_DIR}/lib/${CMAKE_CFG_INTDIR}/${CMK_SLP}sfml-graphics-s${CMK_SLS}
  optimized ${BINARY_DIR}/lib/${CMAKE_CFG_INTDIR}/${CMK_SLP}sfml-network-s${CMK_SLS}
  optimized ${BINARY_DIR}/lib/${CMAKE_CFG_INTDIR}/${CMK_SLP}sfml-window-s${CMK_SLS}
  optimized ${BINARY_DIR}/lib/${CMAKE_CFG_INTDIR}/${CMK_SLP}sfml-system-s${CMK_SLS}
)

if (WIN32)
  target_link_libraries(Jam
    debug ${BINARY_DIR}/lib/${CMAKE_CFG_INTDIR}/${CMK_SLP}sfml-main-d${CMK_SLS}
  )
  target_link_libraries(Jam
    optimized ${BINARY_DIR}/lib/${CMAKE_CFG_INTDIR}/${CMK_SLP}sfml-main${CMK_SLS}
  )

  target_link_libraries(Jam
    # SFML dependencies
    ${SOURCE_DIR}/extlibs/libs-msvc-universal/x86/flac.lib
    ${SOURCE_DIR}/extlibs/libs-msvc-universal/x86/freetype.lib
    ${SOURCE_DIR}/extlibs/libs-msvc-universal/x86/ogg.lib
    ${SOURCE_DIR}/extlibs/libs-msvc-universal/x86/openal32.lib
    ${SOURCE_DIR}/extlibs/libs-msvc-universal/x86/vorbis.lib
    ${SOURCE_DIR}/extlibs/libs-msvc-universal/x86/vorbisenc.lib
    ${SOURCE_DIR}/extlibs/libs-msvc-universal/x86/vorbisfile.lib

    opengl32.lib
    winmm.lib
    gdi32.lib
    ws2_32.lib
    Wldap32.lib
  )

  set_target_properties(Jam PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    add_definitions(-D_DEBUG)
  endif()

  find_package(X11 REQUIRED)
  find_package(OpenGL REQUIRED)
  find_package(Freetype REQUIRED)
  find_package(OpenAL REQUIRED)
  find_package(Vorbis REQUIRED)
  find_package(FLAC REQUIRED)

  target_link_libraries(Jam
    libudev.so
    libpthread.so
    libXrandr.so
    ${X11_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${FREETYPE_LIBRARIES}
    ${OPENAL_LIBRARY}
    ${VORBIS_LIBRARIES}
    ${FLAC_LIBRARY}
  )
endif()

add_custom_target(jam_assets ALL DEPENDS asset_copy)

add_custom_command(OUTPUT asset_copy PRE_BUILD
  DEPENDS always_rebuild
  
  COMMAND ${CMAKE_COMMAND} -E copy
          ${SOURCE_DIR}/extlibs/bin/x86/openal32.dll
          ${CMAKE_CURRENT_BINARY_DIR}/openal32.dll

  COMMAND ${CMAKE_COMMAND} -E copy
          ${SOURCE_DIR}/extlibs/bin/x86/openal32.dll
          ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/openal32.dll

  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_SOURCE_DIR}/assets
          ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/assets

  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_SOURCE_DIR}/assets
          ${CMAKE_CURRENT_BINARY_DIR}/assets
)

add_custom_command(OUTPUT always_rebuild COMMAND cmake -E echo)
add_dependencies(Jam jam_assets)
