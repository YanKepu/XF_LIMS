cmake_minimum_required(VERSION 3.10)    # 版本需大于3.10
project(lims_server)        # 项目名称

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall -O2")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)     # 可执行文件输出目录

# 查找依赖库
find_package(PkgConfig REQUIRED)
find_package(Poco REQUIRED COMPONENTS Foundation Net Util)

# 改为直接查找libpqxx库文件
find_library(PQXX_LIBRARY pqxx)  # 查找pqxx库
find_library(PQ_LIBRARY pq)      # pqxx依赖PostgreSQL的C库libpq，必须链接
if(NOT PQXX_LIBRARY OR NOT PQ_LIBRARY)
    message(FATAL_ERROR "libpqxx or libpq not found! Please install libpqxx-dev and libpq-dev.")
endif()

find_package(nlohmann_json REQUIRED)


# 检查是否能找到libjwt
find_path(LIBJWT_INCLUDE_DIR NAMES jwt.h)
find_library(LIBJWT_LIBRARY NAMES jwt)
find_package(OpenSSL REQUIRED)
# 检查是否找到（保留，确保编译前报错）
if(NOT LIBJWT_INCLUDE_DIR OR NOT LIBJWT_LIBRARY)
    message(FATAL_ERROR "libjwt not found! Please install libjwt-dev or check path.")
endif()

# 打印调试信息（可选，保留）
message(STATUS "Found libjwt headers at: ${LIBJWT_INCLUDE_DIR}")
message(STATUS "Found libjwt library at: ${LIBJWT_LIBRARY}")

# 包含头文件目录
include_directories(include)
include_directories(/usr/include)


# 收集源文件
file(GLOB_RECURSE  SRC_FILES
    "src/*.cpp" 
    "src/network/*.cpp" 
    "src/service/*.cpp" 
    "src/dao/*.cpp" 
    "src/common/*.cpp"
    "src/business/*.cpp"
    "src/business/abstract/*.cpp"
    "src/business/handler/*.cpp"
    )

# 生成可执行文件
add_executable(lims_server main.cpp ${SRC_FILES})

target_include_directories(lims_server PRIVATE ${libpqxx_INCLUDE_DIRS})
target_include_directories(lims_server PRIVATE 
    ${Poco_INCLUDE_DIRS}
    ${LIBJWT_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include)


# 链接依赖库
target_link_libraries(lims_server 
    Poco::Foundation
    Poco::Net 
    Poco::Util 
    ${PQXX_LIBRARY}  # 链接pqxx库
    ${PQ_LIBRARY}    # 链接依赖的libpq库
    nlohmann_json::nlohmann_json
    ${LIBJWT_LIBRARY}      # 链接 JWT 库
    pthread  # 线程库
    OpenSSL::SSL
    OpenSSL::Crypto
)

target_include_directories(lims_server PRIVATE ${LIBJWT_INCLUDE_DIR})
# 开启Debug 模式（可选）
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# 安装路径（可选）
install(TARGETS lims_server DESTINATION /usr/local/bin)
install(DIRECTORY config/ DESTINATION /etc/lims_server)
install(DIRECTORY sql/ DESTINATION /usr/local/share/lims_server/sql)