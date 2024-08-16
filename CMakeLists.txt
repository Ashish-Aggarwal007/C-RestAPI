cmake_minimum_required(VERSION 3.10)
project(EcommerceAPI)

set(CMAKE_CXX_STANDARD 14)

add_executable(EcommerceAPI main.cpp)

# Add the Boost and SSL libraries
find_package(Boost REQUIRED)
find_package(OpenSSL REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(EcommerceAPI ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES})

