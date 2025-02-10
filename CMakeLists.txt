
# here you can set the executable name
project(rain.exe)

#add_executable(rain.exe Main.cpp PlugIn_AddDiscreteElement.cpp)

cmake_minimum_required(VERSION 3.5)
set(TPL_NAME lib/cmake/GranOO3/CMakeLists-Make-App.txt)

find_package(MPI REQUIRED)
set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} ${MPI_LINK_FLAGS})
include_directories(${MPI_INCLUDE_PATH})




message(STATUS "Run: ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} ${MPIEXEC_MAX_NUMPROCS} ${MPIEXEC_PREFLAGS} EXECUTABLE ${MPIEXEC_POSTFLAGS} ARGS")
# target_link_libraries(MyTarget PUBLIC MPI::MPI_CXX)
find_path(TPL_PATH ${TPL_NAME})
include(${TPL_PATH}/${TPL_NAME})
target_link_libraries(rain.exe ${MPI_LIBRARIES})