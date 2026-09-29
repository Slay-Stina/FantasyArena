cmake_minimum_required(VERSION 3.26)
project(FantasyArena LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()

set(LIB_NAME ${PROJECT_NAME}_game)

# ---------------------------------------------------------------------------
# FMOD
# ---------------------------------------------------------------------------
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(FMOD_ARCH x64)
else()
    set(FMOD_ARCH x86)
endif()

if(WIN32)
    set(FMOD_ROOT "$ENV{FMOD_ROOT}" CACHE PATH "FMOD Engine root directory")
    if(NOT FMOD_ROOT)
        set(FMOD_ROOT "C:/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows")
    endif()
    set(FMOD_INC ${FMOD_ROOT}/api/core/inc)
    set(FMOD_LIB_DIR ${FMOD_ROOT}/api/core/lib/${FMOD_ARCH})
    if(MSVC)
        set(FMOD_LIB ${FMOD_LIB_DIR}/fmod_vc.lib)
    else()
        # MinGW kan länka direkt mot DLL:en om ingen import-bibliotek finns
        set(FMOD_LIB ${FMOD_LIB_DIR}/fmod.dll)
    endif()
    set(FMOD_RUNTIME ${FMOD_LIB_DIR}/fmod.dll)
else()
    set(FMOD_ROOT "/opt/fmodengine" CACHE PATH "FMOD Engine root directory")
    set(FMOD_INC ${FMOD_ROOT}/api/core/inc)
    set(FMOD_LIB_DIR ${FMOD_ROOT}/api/core/lib/x86_64)
    set(FMOD_LIB fmod)
    file(GLOB FMOD_RUNTIME ${FMOD_LIB_DIR}/libfmod.so*)
endif()

# ---------------------------------------------------------------------------
# SDL3 (external)
# ---------------------------------------------------------------------------
find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_ttf REQUIRED)

# ---------------------------------------------------------------------------
# Third-party (vendored)
# ---------------------------------------------------------------------------
add_library(jsoncpp STATIC
        third_party/jsoncpp/src/lib_json/json_reader.cpp
        third_party/jsoncpp/src/lib_json/json_value.cpp
        third_party/jsoncpp/src/lib_json/json_writer.cpp)
target_include_directories(jsoncpp PUBLIC ${CMAKE_SOURCE_DIR}/third_party/jsoncpp/include)
set_target_properties(jsoncpp PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_library(imgui STATIC
        third_party/imgui/imgui.cpp
        third_party/imgui/imgui_draw.cpp
        third_party/imgui/imgui_tables.cpp
        third_party/imgui/imgui_widgets.cpp
        third_party/imgui/imgui_impl_sdl3.cpp
        third_party/imgui/imgui_impl_sdlrenderer3.cpp)
target_include_directories(imgui PUBLIC ${CMAKE_SOURCE_DIR}/third_party/imgui)
target_link_libraries(imgui PUBLIC SDL3::SDL3)
set_target_properties(imgui PROPERTIES POSITION_INDEPENDENT_CODE ON)

# ---------------------------------------------------------------------------
# Assets
# ---------------------------------------------------------------------------
set(DIR_ASSETS_ORIGIN ${CMAKE_SOURCE_DIR}/assets)
set(DIR_ASSETS_DESTINATION $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets)

# Flag executable specific files
set(EXE_EXCLUSIVE ${CMAKE_SOURCE_DIR}/src/host/main.cpp)

# Flag files that both our executable and shared library need to know about
set(SHARED_SOURCES
        ${CMAKE_SOURCE_DIR}/src/engine/memory/arena.cpp
        ${CMAKE_SOURCE_DIR}/src/engine/input/input.cpp
        ${CMAKE_SOURCE_DIR}/src/game/gameplay/levels.cpp)

# SHARED STATIC LIBRARY
set(SHARED_LIB_NAME ${PROJECT_NAME}_common)
add_library(${SHARED_LIB_NAME} STATIC ${SHARED_SOURCES})
target_include_directories(${SHARED_LIB_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(${SHARED_LIB_NAME} PRIVATE SDL3::SDL3 SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf jsoncpp)
set_target_properties(${SHARED_LIB_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

# Alla .cpp-filer i src/
file(GLOB_RECURSE LIB_EXCLUSIVE "src/*.cpp")
# Ta bort exe- och delade-filerna från biblioteksfilerna
list(REMOVE_ITEM LIB_EXCLUSIVE ${EXE_EXCLUSIVE} ${SHARED_SOURCES})

# EXEKVERBAR
add_executable(${PROJECT_NAME} ${EXE_EXCLUSIVE})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src ${FMOD_INC})
target_link_directories(${PROJECT_NAME} PRIVATE ${FMOD_LIB_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${FMOD_LIB} SDL3::SDL3 ${SHARED_LIB_NAME})
if(UNIX)
    target_link_options(${PROJECT_NAME} PRIVATE "LINKER:--no-as-needed")
    set_target_properties(${PROJECT_NAME} PROPERTIES
            BUILD_RPATH ${FMOD_LIB_DIR}
            INSTALL_RPATH "$ORIGIN")
endif()

# DELAT BIBLIOTEK (.so / .dll)
add_library(${LIB_NAME} SHARED ${LIB_EXCLUSIVE})
target_include_directories(${LIB_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src ${FMOD_INC})
target_link_directories(${LIB_NAME} PRIVATE ${FMOD_LIB_DIR})
target_link_libraries(${LIB_NAME} PRIVATE ${FMOD_LIB} ${SHARED_LIB_NAME} SDL3::SDL3 jsoncpp imgui)
if(UNIX)
    set_target_properties(${LIB_NAME} PROPERTIES
            BUILD_RPATH ${FMOD_LIB_DIR}
            INSTALL_RPATH "$ORIGIN")
endif()
if(WIN32)
    set_target_properties(${LIB_NAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# Kopiera assets bredvid exekverbara filen (körs varje bygge så kartor/assets synkas)
add_custom_target(copy_assets ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${DIR_ASSETS_ORIGIN} ${DIR_ASSETS_DESTINATION}
        DEPENDS ${PROJECT_NAME}
        VERBATIM
)

if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${FMOD_RUNTIME}
                    $<TARGET_FILE:SDL3::SDL3>
                    $<TARGET_FILE:SDL3_image::SDL3_image>
                    $<TARGET_FILE:SDL3_ttf::SDL3_ttf>
                    $<TARGET_FILE_DIR:${PROJECT_NAME}>
            VERBATIM
    )
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION .)
install(TARGETS ${LIB_NAME} LIBRARY DESTINATION . RUNTIME DESTINATION .)
install(FILES ${FMOD_RUNTIME} DESTINATION .)
install(DIRECTORY ${CMAKE_BINARY_DIR}/assets DESTINATION .)
