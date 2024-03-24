#ifndef TAPE_SORTING_H
#define TAPE_SORTING_H

#include "pch.h"
#include "tape_interface.h"


void tape_sorting(const tape_interface& source_tape, tape_interface& dst_tape, std::size_t temp_bytes = 40, std::size_t temp_tapes = 3);


#endif //TAPE_SORTING_H