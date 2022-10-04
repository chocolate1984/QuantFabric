
PROJECT(test1)
# CMake 最低版本号要求
cmake_minimum_required(VERSION 3.0)

# 把指定路径下的源文件全部放入变量SRC_LIST
AUX_SOURCE_DIRECTORY(./ SRC_LIST)

# 配置可执行文件输出存放路径
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

# 配置DEBUG模式 编译参数
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g")
ADD_DEFINITIONS(-g -O0 -W -Wall -D_REENTRANT -D_FILE_OFFSET_BITS=64 -DAC_HAS_INFO -DAC_HAS_WARNING -DAC_HAS_ERROR -DAC_HAS_CRITICAL -DTIXML_USE_STL -DHAVE_CXX_STDHEADERS -Wno-deprecated ${CMAKE_CXX_FLAGS})

# 头文件搜索路径
INCLUDE_DIRECTORIES(
    ./
    ./include
)

# 库文件搜索路径
LINK_DIRECTORIES(
    ./
    ./lib
)



# 添加编译参数 -D是宏
# add_compile_options(-std=c++11 -O2 -DIPC -DCONSUMER)

ADD_EXECUTABLE(test1 ${SRC_LIST})

# 指定可执行文件链接时需要依赖的库文件
target_link_libraries(test1 pthread)


# 进程的编译就是四次了，进程之间只能共享内存通信
#  test RingBuffer by Heap Memory in one Process
#  g++ --std=c++11 -O2 -DHEAP -DPRODUCER -DCONSUMER RingBufferTest.cpp -o test -pthread
#  g++ --std=c++11 -O2 -DIPC -DPRODUCER -DCONSUMER RingBufferTest.cpp -o test -pthread
#  test RingBuffer by Share Memory in different Process
#  g++ --std=c++11 -O2 -DIPC -DPRODUCER RingBufferTest.cpp -o producer -pthread
#  g++ --std=c++11 -O2 -DIPC -DCONSUMER RingBufferTest.cpp -o consumer -pthread
