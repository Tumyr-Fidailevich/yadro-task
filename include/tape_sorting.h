#ifndef TAPE_SORTING_H
#define TAPE_SORTING_H

#include "pch.h"
#include "tape_interface.h"
#include "tape_factory.h"

using tape_ptr = std::unique_ptr<tape_interface>;

void file_tape_sorting(const tape_interface& source_tape, tape_interface& dst_tape, const tape_factory& factory, std::size_t temp_bytes, std::size_t temp_tapes_count);

void block_sort(const tape_interface& source_tape, tape_ptr& temp_tape, std::size_t block_size);

void merge_blocks(tape_ptr& tape, const tape_factory& factory, std::size_t block_size);

void merge_tapes(const std::vector<tape_ptr>& temp_tapes, tape_interface& dst_tape);

std::vector<std::size_t> split_parts(std::size_t n, std::size_t m);

std::size_t ceil_divide(std::size_t dividend, std::size_t divisor);

void remove_directory_recursively(const std::filesystem::path& path);

void tape_sorting(const tape_interface& source_tape, tape_interface& dst_tape, const tape_factory& factory, std::size_t temp_bytes, std::size_t temp_tapes_count);

void tape_sorting(const tape_interface& source_tape, tape_interface& dst_tape, const tape_factory& factory, std::size_t temp_bytes);


#endif //TAPE_SORTING_H