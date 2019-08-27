find_package(GMP REQUIRED)
find_package(GMPXX REQUIRED)

set(
	EXPRESS_SRC
	Expr.cpp
	AInt.cpp
)

add_library(express ${EXPRESS_HEADERS} ${EXPRESS_SRC})

target_include_directories(express PRIVATE ${GMPXX_INCLUDE_DIR})

target_link_libraries(express fmt ${GMP_LIBRARIES} ${GMPXX_LIBRARIES})
