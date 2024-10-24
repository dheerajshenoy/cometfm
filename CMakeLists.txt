# ensure cmake version is at least 3.16.0
cmake_minimum_required(VERSION 3.16.0)

# defines a project with a version
project(cometfm VERSION 1.0.0 LANGUAGES CXX)

# pick the C++ standard to use, in this case C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# tell CMake to run the Qt tools moc, rcc, and uic automatically
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# configure the Qt 6 modules core and widgets
find_package(Qt6 COMPONENTS Core REQUIRED)
find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_definitions( -DMAGICKCORE_QUANTUM_DEPTH=16 )
add_definitions( -DMAGICKCORE_HDRI_ENABLE=0 )

# find imagemagick
find_package(ImageMagick COMPONENTS Magick++ REQUIRED)

qt_standard_project_setup()
# define an executable built from a source file
qt_add_executable(${PROJECT_NAME}
    src/main.cpp
    src/Comet.cpp
    src/Panel.cpp
    src/PreviewPanel.cpp
    src/ListView.cpp
    src/Minibuffer.cpp
    src/Modeline.cpp
    src/FileSystemModel.hpp
)

# tell cmake to link the executable to the Qt 6 core and test modules
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets ${ImageMagick_LIBRARIES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${ImageMagick_INCLUDE_DIRS})

add_custom_target(run
    COMMAND cometfm
    DEPENDS cometfm
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)