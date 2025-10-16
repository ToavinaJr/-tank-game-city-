# --- Version minimale de CMake ---
cmake_minimum_required(VERSION 3.16)

# --- Nom du projet ---
project(TankBattleGame VERSION 1.0 LANGUAGES CXX)

# --- Version du standard C++ ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)       # Génération automatique des fichiers MOC (Q_OBJECT)
set(CMAKE_AUTORCC ON)       # Compilation automatique des fichiers .qrc
set(CMAKE_AUTOUIC ON)       # Compilation automatique des fichiers .ui

# --- Gestion du chemin Qt si variable d'environnement définie ---
if(NOT Qt6_DIR)
    if(DEFINED ENV{QT_DIR})
        set(Qt6_DIR "$ENV{QT_DIR}/lib/cmake/Qt6")
    endif()
endif()

# --- Trouver les modules Qt nécessaires ---
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Multimedia)

# --- Dossiers sources et includes ---
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(HEADERS
    ${PROJECT_SOURCE_DIR}/include/Constants.hpp
    ${PROJECT_SOURCE_DIR}/include/GameConfig.hpp
    ${PROJECT_SOURCE_DIR}/include/Entity.hpp
    ${PROJECT_SOURCE_DIR}/include/Tank.hpp
    ${PROJECT_SOURCE_DIR}/include/Block.hpp
    ${PROJECT_SOURCE_DIR}/include/Bullet.hpp
    ${PROJECT_SOURCE_DIR}/include/PowerUp.hpp
    ${PROJECT_SOURCE_DIR}/include/Enemy.hpp
    ${PROJECT_SOURCE_DIR}/include/GameScene.hpp
    ${PROJECT_SOURCE_DIR}/include/GameEngine.hpp
    ${PROJECT_SOURCE_DIR}/include/MainWindow.hpp
    ${PROJECT_SOURCE_DIR}/include/MenuWidget.hpp
    ${PROJECT_SOURCE_DIR}/include/SettingsWidget.hpp
    ${PROJECT_SOURCE_DIR}/include/GameWidget.hpp
    ${PROJECT_SOURCE_DIR}/include/HUDWidget.hpp
    ${PROJECT_SOURCE_DIR}/include/SoundManager.hpp
    ${PROJECT_SOURCE_DIR}/include/SaveManager.hpp
)

set(SOURCES
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/Entity.cpp
    ${PROJECT_SOURCE_DIR}/src/Tank.cpp
    ${PROJECT_SOURCE_DIR}/src/Block.cpp
    ${PROJECT_SOURCE_DIR}/src/Bullet.cpp
    ${PROJECT_SOURCE_DIR}/src/PowerUp.cpp
    ${PROJECT_SOURCE_DIR}/src/Enemy.cpp
    ${PROJECT_SOURCE_DIR}/src/GameScene.cpp
    ${PROJECT_SOURCE_DIR}/src/GameEngine.cpp
    ${PROJECT_SOURCE_DIR}/src/MainWindow.cpp
    ${PROJECT_SOURCE_DIR}/src/MenuWidget.cpp
    ${PROJECT_SOURCE_DIR}/src/SettingsWidget.cpp
    ${PROJECT_SOURCE_DIR}/src/GameWidget.cpp
    ${PROJECT_SOURCE_DIR}/src/HUDWidget.cpp
    ${PROJECT_SOURCE_DIR}/src/SoundManager.cpp
    ${PROJECT_SOURCE_DIR}/src/SaveManager.cpp
)

set(RESOURCES
    ${PROJECT_SOURCE_DIR}/resources/resources.qrc
)

# --- Création de l’exécutable ---
qt_add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${RESOURCES}
)

# --- Inclure les fichiers d'en-tête ---
target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

# --- Lier les modules Qt ---
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Multimedia
)

# --- Propriétés spécifiques Windows ---
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

# --- Installation (optionnelle) ---
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# --- Option de debug ---
message(STATUS "CMake automoc: ${CMAKE_AUTOMOC}")
message(STATUS "Qt6 version: ${Qt6Core_VERSION}")
message(STATUS "Qt6_DIR: ${Qt6_DIR}")
