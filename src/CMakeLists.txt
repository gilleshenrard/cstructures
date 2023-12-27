add_library(screen screen/screen.c)
target_include_directories(screen PRIVATE ${PROJECT_SOURCE_DIR}/include/screen)

add_library(datasetTest test/dataset_test.c)
target_include_directories(datasetTest PRIVATE ${PROJECT_SOURCE_DIR}/include/test)

add_library(structuresCommon cstructurescommon.c)
target_include_directories(structuresCommon PRIVATE ${PROJECT_SOURCE_DIR}/include)

add_library(cstructures carrays.c cavl.c clists.c cqueues.c cstacks.c cstructures.c)
target_include_directories(cstructures PRIVATE ${PROJECT_SOURCE_DIR}/include)