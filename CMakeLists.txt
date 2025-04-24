cmake_minimum_required(VERSION 3.10)
project(ClientBinary)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS system thread)

add_subdirectory(libs)

# Указываем, где искать заголовочные файлы
include_directories(${CMAKE_SOURCE_DIR}/include)

# Собираем все подкаталоги include
###
file(GLOB_RECURSE PROJECT_INCLUDE_DIRS LIST_DIRECTORIES true "${CMAKE_CURRENT_SOURCE_DIR}/include/*")

# Фильтруем только директории, удаляя файлы
set(DIR_LIST "")
foreach(dir ${PROJECT_INCLUDE_DIRS})
    if (IS_DIRECTORY ${dir})
        list(APPEND DIR_LIST ${dir})
    endif()
endforeach()

include_directories(${DIR_LIST})
###

add_executable(client main.cpp src/Client.cpp)

target_link_libraries(client BinaryProtocol Boost::system Boost::thread)
target_link_libraries(client libClasses)