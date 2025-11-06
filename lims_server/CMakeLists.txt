cmake_minimum_required(VERSION 3.10)    # 版本需大于3.10
project(lims_server)        # 项目名称

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -O2")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)     # 可执行文件输出目录

# 查找依赖库
find_package(PkgConfig REQUIRED)
find_package(Poco REQUIRED COMPONENTS Net Util)
pkg_check_modules(libpqxx REQUIRED)
find_package(nlohmann_json REQUIRED)

# 包含头文件目录
include_directories(include)

# 收集源文件
file(GLOB_RECURSE  SRC_FILES
    "src/*.cpp" 
    "src/network/*.cpp" 
    "src/service/*.cpp" 
    "src/dao/*.cpp" 
    "src/common/*.cpp")

# 生成可执行文件
add_executable(lims_server main.cpp ${SRC_FILES})

target_include_directories(lims_server PRIVATE ${libpqxx_INCLUDE_DIRS})

# 链接依赖库
target_link_libraries(lims_server 
    Poco::Net 
    Poco::Util 
    ${libpqxx_LIBRARIES} 
    nlohmann_json::nlohmann_json
    pthread  # 线程库
)

# 安装路径（可选）
install(TARGETS lims_server DESTINATION /usr/local/bin)
install(DIRECTORY config/ DESTINATION /etc/lims_server)
install(DIRECTORY sql/ DESTINATION /usr/local/share/lims_server/sql)