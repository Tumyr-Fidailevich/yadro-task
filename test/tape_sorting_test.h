#include "gtest/gtest.h"
#include "test_utils.h"
#include "file_tape.h"
#include "tape_sorting.h"

TEST(TapeSortingTest, AlreadySortedTapeTest)
{
    std::vector<int> elements = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto input_tape_path = create_temp_tape(elements);

    auto output_tape_path = create_temp_tape({});

    {

        const auto source_tape = file_tape(input_tape_path);

        auto output_tape = file_tape(output_tape_path, source_tape.size());

        const auto factory = file_tape_factory({});

        tape_sorting(source_tape, output_tape, factory, 40);

        ASSERT_EQ(output_tape.get_str_repr(), "          1          2          3          4          5          6          7          8          9         10");
    }

    remove_directory_recursively(TEST_TEMP_PATH);
}


TEST(TapeSortingTest, RegularyTapeSortTest)
{
    std::vector<int> elements = generate_random_vector(12, 10);

    auto input_tape_path = create_temp_tape(elements);

    auto output_tape_path = create_temp_tape({});

    {

        const auto source_tape = file_tape(input_tape_path);

        auto output_tape = file_tape(output_tape_path, source_tape.size());

        const auto factory = file_tape_factory({});

        tape_sorting(source_tape, output_tape, factory, 40);

        std::sort(elements.begin(), elements.end());

        std::stringstream ss;
        for(const auto& elem :elements)
        {
            ss << std::setw(11) << std::setfill(' ') << elem;
        }

        ASSERT_EQ(output_tape.get_str_repr(), ss.str());
    }

    remove_directory_recursively(TEST_TEMP_PATH);
}
