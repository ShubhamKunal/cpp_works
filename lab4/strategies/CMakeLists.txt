project(strategies)
set(SOURCES
		human.cpp
        computer_first_strategy.cpp
		computer_second_strategy.cpp
		computer_interface.cpp
		)
set(HEADERS
		human.h
        computer_first_strategy.h
		computer_second_strategy.h
		interface.h
		computer_interface.h)

include_directories(../)

add_library(strategies ${SOURCES} ${HEADERS})