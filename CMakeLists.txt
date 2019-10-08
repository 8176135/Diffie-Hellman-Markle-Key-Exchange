cmake_minimum_required(VERSION 3.14)
project(deffie_helman_markle C)

set(CMAKE_C_STANDARD 11)

add_executable(dhm_client DhmClient.c)
add_executable(dhm_server DhmServer.c)