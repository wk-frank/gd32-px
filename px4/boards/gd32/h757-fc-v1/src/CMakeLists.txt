############################################################################
# boards/gd32/h757-fc-v1/src/CMakeLists.txt
# PX4 v1.17.0 Board Support Library for gd32_h757-fc-v1
############################################################################

px4_add_library(drivers_board
	timer_config.c
)

target_include_directories(drivers_board
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(drivers_board
	PRIVATE
		arch_io_pins
		arch_spi
		drivers__led
)
