include(AnalysisTree)

if(Boost_FOUND)
    include_directories(${PROJECT_INCLUDE_DIRECTORIES} ${CMAKE_CURRENT_SOURCE_DIR} ${Boost_INCLUDE_DIR} ${AnalysisTree_BINARY_DIR}/include)
    link_directories(${PROJECT_LINK_DIRECTORIES} ${Boost_LIBRARY_DIRS})
else()
    include_directories(${PROJECT_INCLUDE_DIRECTORIES} ${CMAKE_CURRENT_SOURCE_DIR} ${AnalysisTree_BINARY_DIR}/include)
    link_directories(${PROJECT_LINK_DIRECTORIES})
endif()

if(PROJECT_LINK_DIRECTORIES)
    include_directories(${PROJECT_INCLUDE_DIRECTORIES} ${AnalysisTree_BINARY_DIR}/include)
    link_directories(${PROJECT_LINK_DIRECTORIES})
endif()

add_executable(example example.cpp)
add_executable(pid_gauss pid_gauss.cpp)
add_executable(onnx_val example1.cpp)
add_executable(b_eng_thesis b_eng_thesis.cpp)
add_executable(rich_plots rich_plots.cpp)

add_dependencies(example AnalysisTreeQA)
add_dependencies(onnx_val AnalysisTreeQA)
add_dependencies(b_eng_thesis AnalysisTreeQA)
add_dependencies(rich_plots AnalysisTreeQA)

target_link_libraries(example ${ROOT_LIBRARIES} AnalysisTreeQA AnalysisTreeBase AnalysisTreeInfra)
target_link_libraries(onnx_val ${ROOT_LIBRARIES} AnalysisTreeQA AnalysisTreeBase AnalysisTreeInfra)
target_link_libraries(pid_gauss ${ROOT_LIBRARIES} AnalysisTreeQA AnalysisTreeBase AnalysisTreeInfra)
target_link_libraries(b_eng_thesis ${ROOT_LIBRARIES} AnalysisTreeQA AnalysisTreeBase AnalysisTreeInfra)
target_link_libraries(rich_plots ${ROOT_LIBRARIES} AnalysisTreeQA AnalysisTreeBase AnalysisTreeInfra)

install (TARGETS example onnx_val pid_gauss rich_plots b_eng_thesis RUNTIME DESTINATION bin)
