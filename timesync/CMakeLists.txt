include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

set(TimeSync_SRCS xmlstorage.cpp
    istorage.cpp
    object.cpp
    binding.cpp
)

add_library(TimeSync SHARED ${TimeSync_SRCS})
target_link_libraries(TimeSync ${QT_QTCORE_LIBRARY})

install(TARGETS TimeSync RUNTIME DESTINATION bin LIBRARY DESTINATION lib)

