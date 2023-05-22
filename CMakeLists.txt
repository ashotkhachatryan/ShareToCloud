cmake_minimum_required(VERSION 3.1)

project(ShareToCloud)

find_package(OpenSSL REQUIRED)
if(OPENSSL_FOUND)
    set(HTTPLIB_IS_USING_OPENSSL TRUE)
endif()

set(CMAKE_CXX_STANDARD 20)
set(SRC_FILES src/main.cpp
              src/folder_tracker.cpp
              src/folder_tracker.h)

include(ExternalProject)
ExternalProject_Add(
        GoogleAuthenticator
  GIT_REPOSITORY    https://github.com/ashotkhachatryan/GoogleAuthenticator
  GIT_TAG           main
  SOURCE_DIR        "${CMAKE_BINARY_DIR}/GoogleAuthenticator"
  BINARY_DIR        "${CMAKE_BINARY_DIR}/GoogleAuthenticatorLib"
  CONFIGURE_COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_PREFIX=install ${CMAKE_BINARY_DIR}/GoogleAuthenticator
  BUILD_COMMAND     ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/GoogleAuthenticatorLib
)

add_executable(ShareToCloud ${SRC_FILES})
add_dependencies(ShareToCloud GoogleAuthenticator)
target_include_directories(ShareToCloud PRIVATE ${CMAKE_BINARY_DIR}/GoogleAuthenticator/include/)

if (WIN32)
    set(GA_LIB_NAME google_authenticator${CMAKE_STATIC_LIBRARY_SUFFIX})
else()
    set(GA_LIB_NAME libgoogle_authenticator${CMAKE_STATIC_LIBRARY_SUFFIX})
endif ()

if (APPLE)
    set(MACOS_LIBS "-framework CoreFoundation -framework Foundation -framework ApplicationServices")
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC
        ${CMAKE_BINARY_DIR}/GoogleAuthenticatorLib/install/lib/${GA_LIB_NAME}
        $<$<BOOL:${HTTPLIB_IS_USING_OPENSSL}>:OpenSSL::SSL>
        $<$<BOOL:${HTTPLIB_IS_USING_OPENSSL}>:OpenSSL::Crypto> ${MACOS_LIBS})

target_compile_definitions(${PROJECT_NAME} PUBLIC
        $<$<BOOL:${HTTPLIB_IS_USING_OPENSSL}>:CPPHTTPLIB_OPENSSL_SUPPORT>
        )
