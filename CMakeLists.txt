# Utiliser une version de CMake supportant Qt 6 (>= 3.16)
cmake_minimum_required(VERSION 3.16)
project(TankBattleGame VERSION 1.0.0 LANGUAGES CXX)

# --- Configuration C++ Standard ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Activation des outils Qt (MOC, RCC, UIC) ---
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# --- Trouver le paquet Qt 6 ---
# L'inclusion des composants charge les macros nécessaires comme qt_add_executable
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Multimedia)

# --- Définition des chemins absolus (Corrigé) ---
# NOTE: CMAKE_CURRENT_SOURCE_DIR est la variable native de CMake, pas besoin de la redéfinir.
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(HEADERS
    ${PROJECT_SOURCE_DIR}/include/Constants.h
    ${PROJECT_SOURCE_DIR}/include/GameConfig.h
    ${PROJECT_SOURCE_DIR}/include/Entity.h
    ${PROJECT_SOURCE_DIR}/include/Tank.h
    ${PROJECT_SOURCE_DIR}/include/Block.h
    ${PROJECT_SOURCE_DIR}/include/Bullet.h
    ${PROJECT_SOURCE_DIR}/include/PowerUp.h
    ${PROJECT_SOURCE_DIR}/include/Enemy.h
    ${PROJECT_SOURCE_DIR}/include/GameScene.h
    ${PROJECT_SOURCE_DIR}/include/GameEngine.h
    ${PROJECT_SOURCE_DIR}/include/MainWindow.h
    ${PROJECT_SOURCE_DIR}/include/MenuWidget.h
    ${PROJECT_SOURCE_DIR}/include/SettingsWidget.h
    ${PROJECT_SOURCE_DIR}/include/GameWidget.h
    ${PROJECT_SOURCE_DIR}/include/HUDWidget.h
    ${PROJECT_SOURCE_DIR}/include/SoundManager.h
    ${PROJECT_SOURCE_DIR}/include/SaveManager.h
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

# --- Exécutable ---
# Utiliser qt_add_executable (la macro est maintenant définie grâce à find_package)
# Ne pas inclure ${HEADERS} ici.
qt_add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${RESOURCES}
)

# --- Dossiers d'inclusion ---
target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

# --- Liaison des bibliothèques (Targets Qt 6) ---
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Multimedia
)

# --- Propriétés de l'exécutable et Installation ---
set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
)

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)