#include "tape_sorting.h"

const std::size_t MINIMUM_BYTES = 8;

using tape_ptr = std::unique_ptr<tape_interface>;

void file_tape_sorting(const tape_interface& source_tape, tape_interface& dst_tape, const tape_factory& factory, std::size_t temp_bytes, std::size_t temp_tapes_count)
{
    source_tape.rewind();
    dst_tape.rewind();

    auto block_size = temp_bytes / sizeof(int);

    std::vector<tape_ptr> temp_tapes;
    temp_tapes.reserve(temp_tapes_count);

    auto temp_tape_sizes = split_parts(source_tape.size(), temp_tapes_count);

    for(std::size_t i = 0; i < temp_tape_sizes.size(); i++)
    {
        temp_tapes.push_back(factory.create_tape(temp_tape_sizes[i]));
    }
    temp_tape_sizes.clear();
    
    for(auto& temp_tape : temp_tapes)
    {
        block_sort(source_tape, temp_tape, block_size);
        merge_blocks(temp_tape, factory, block_size);
    }
    merge_tapes(temp_tapes, dst_tape);
}

void block_sort(const tape_interface& source_tape, tape_ptr& temp_tape, std::size_t block_size)
{
    std::vector<int> temp_block(block_size);
    std::size_t temp_block_count = ceil_divide(temp_tape->size(), block_size);
    for(std::size_t i = 0; i < temp_block_count; i++)
    {
        std::size_t read = 0;
        do
        {
            temp_block[read++] = source_tape.read();
        } while (source_tape.move_right() && read < block_size && i * block_size + read < temp_tape->size());

        std::sort(temp_block.begin(), temp_block.begin() + read);

        for(int j = 0; j < read; j++)
        {
            temp_tape->write(temp_block[j]);
            temp_tape->move_right();
        }
    }
}

void merge_blocks(tape_ptr& tape, const tape_factory& factory, std::size_t block_size)
{
    if(tape->size() == block_size) return;

    std::size_t blocks_count = ceil_divide(tape->size(), block_size);
    std::vector<std::size_t> block_info(blocks_count, 0);

    auto sub_tape = factory.create_tape(tape->size());

    do
    {
        int min = INT_MAX;
        std::size_t min_index = 0;
        tape->rewind();
        for(int i = 0; i < blocks_count; i++)
        {
            if(block_info[i] == block_size || i * block_size + block_info[i] == tape->size()) continue;

            std::size_t count = 0;

            while(count++ < block_info[i] && tape->move_right());

            int value = tape->read();
            if(min < value)
            {
                min_index = i;
                min = value;
            }
            count = 0;
            while(count++ < block_size && tape->move_right());
        }
        block_info[min_index]++;
        sub_tape->write(min);
    } while (sub_tape->move_right());
    
    
    tape.swap(sub_tape);
    tape->rewind();
}

void merge_tapes(const std::vector<tape_ptr>& temp_tapes, tape_interface& dst_tape)
{
    do
    {
        int min = INT_MAX;
        int min_tape_pos = 0;
        for(std::size_t tape_pos = 0; tape_pos < temp_tapes.size(); tape_pos++)
        {
            if(temp_tapes[tape_pos]->pos() + 1 == temp_tapes[tape_pos]->size()) continue;
            int value = temp_tapes[tape_pos]->read();
            if(min < value)
            {
                min_tape_pos = tape_pos;
                min = value;
            }
        }
        temp_tapes[min_tape_pos]->move_right();
        dst_tape.write(min);
    } while (dst_tape.move_right());
}

std::vector<std::size_t> split_parts(std::size_t n, std::size_t m)
{
    std::vector<std::size_t> parts(m, n / m);
    int remainder = n % m;

    for(int i = 0; i < remainder; i++)
    {
        parts[i]++;
    }
    
    return parts;
}

std::size_t ceil_divide(std::size_t dividend, std::size_t divisor)
{
    return (dividend % divisor == 0) ? dividend / divisor : dividend / divisor + 1;
}

void remove_directory_recursively(const std::filesystem::path& path) 
{
    for (const auto& entry : std::filesystem::directory_iterator(path)) 
    {
        if (std::filesystem::is_directory(entry.path())) {
            remove_directory_recursively(entry.path());
        } else {
            std::filesystem::remove(entry.path());
        }
    }
    std::filesystem::remove(path);
}


void tape_sorting(const tape_interface& source_tape, tape_interface& dst_tape, const tape_factory& factory, std::size_t temp_bytes, std::size_t temp_tapes_count)
{
    if(typeid(source_tape) != typeid(dst_tape)) throw std::invalid_argument("The source and destination tapes have different types");

    if(dst_tape.size() < source_tape.size()) throw std::runtime_error("Insufficient output tape size");
    
    if(temp_bytes < MINIMUM_BYTES) throw std::runtime_error("Not enough memory to sort");

    if(const file_tape* file_tape_ptr = dynamic_cast<const file_tape*>(&source_tape))
    {
        file_tape_sorting(source_tape, dst_tape, factory, temp_bytes, temp_tapes_count);

        remove_directory_recursively(TEMP_PATH);
    }
}

void tape_sorting(const tape_interface& source_tape, tape_interface& dst_tape, const tape_factory& factory, std::size_t temp_bytes)
{
    auto temp_tapes_count = source_tape.size() / (temp_bytes / sizeof(int));
    tape_sorting(source_tape, dst_tape, factory, temp_bytes, temp_tapes_count);
}