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
  
  # Scenes
  ${INCDIR}/Scenes/LevelScene.hpp
  ${SRCDIR}/Scenes/LevelScene.cpp

  ${SRCDIR}/main.cpp
)

add_executable(Jam ${SRC})
#add_dependencies(Jam SFML GLM JSON CURL)
add_dependencies(Jam SFML GLM JSON)

#add_definitions(-DSFML_STATIC -DCURL_STATICLIB)
add_definitions(-DSFML_STATIC)

if (MSVC)
  ExternalProject_Get_Property(SFML BINARY_DIR)
  target_link_libraries(Jam
    # SFML libs
    debug ${BINARY_DIR}/lib/${CMAKE_CFG_INTDIR}/sfml-audio-s-d.lib
    debug ${BINARY_DIR}/lib/${CMAKE_CFG_INTDIR}/sfml-graphics-s-d.lib
    debug ${BINARY_DIR}/lib/${CMAKE_CFG_INTDIR}/sfml-network-s-d.lib
    debug ${BINARY_DIR}/lib/${CMAKE_CFG_INTDIR}/sfml-main-d.lib
    debug ${BINARY_DIR}/lib/${CMAKE_CFG_INTDIR}/sfml-system-s-d.lib
    debug ${BINARY_DIR}/lib/${CMAKE_CFG_INTDIR}/sfml-window-s-d.lib
  )
  target_link_libraries(Jam
    # SFML libs
    optimized ${BINARY_DIR}/lib/${CMAKE_CFG_INTDIR}/sfml-audio-s.lib
    optimized ${BINARY_DIR}/lib/${CMAKE_CFG_INTDIR}/sfml-graphics-s.lib
    optimized ${BINARY_DIR}/lib/${CMAKE_CFG_INTDIR}/sfml-network-s.lib
    optimized ${BINARY_DIR}/lib/${CMAKE_CFG_INTDIR}/sfml-main.lib
    optimized ${BINARY_DIR}/lib/${CMAKE_CFG_INTDIR}/sfml-system-s.lib
    optimized ${BINARY_DIR}/lib/${CMAKE_CFG_INTDIR}/sfml-window-s.lib
  )

  target_link_libraries(Jam
    # SFML dependencies
    ${SOURCE_DIR}/extlibs/libs-msvc-universal/x86/flac.lib
    ${SOURCE_DIR}/extlibs/libs-msvc-universal/x86/freetype.lib
    ${SOURCE_DIR}/extlibs/libs-msvc-universal/x86/jpeg.lib
    ${SOURCE_DIR}/extlibs/libs-msvc-universal/x86/ogg.lib
    ${SOURCE_DIR}/extlibs/libs-msvc-universal/x86/openal32.lib
    ${SOURCE_DIR}/extlibs/libs-msvc-universal/x86/vorbis.lib
    ${SOURCE_DIR}/extlibs/libs-msvc-universal/x86/vorbisenc.lib
    ${SOURCE_DIR}/extlibs/libs-msvc-universal/x86/vorbisfile.lib
    ${SOURCE_DIR}/extlibs/libs-msvc-universal/x86/flac.lib

    opengl32.lib
    winmm.lib
    gdi32.lib
    ws2_32.lib
    Wldap32.lib
  )

  set_target_properties(Jam PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
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
