#include "gtest/gtest.h"
#include "test_utils.h"
#include "file_tape.h"


TEST(TapeTest, ReadTest)
{
    std::vector<int> temp_tape_elements = {1};

    auto tape_path = create_temp_tape(temp_tape_elements);
    {
        file_tape tape(tape_path);

        auto value = tape.read();

        ASSERT_EQ(value, 1);
    }
    std::filesystem::remove(tape_path);
}

TEST(TapeTest, WriteTest)
{
    std::vector<int> temp_tape_elements = {1, 2, 3};

    auto tape_path = create_temp_tape(temp_tape_elements);
    {
        file_tape tape(tape_path);

        int value = 10;

        tape.write(value);

        ASSERT_EQ(tape.read(), value);
    }
    std::filesystem::remove(tape_path);
}

TEST(TapeTest, Move_RightTest)
{
    std::vector<int> temp_tape_elements = {1, 2, 3};

    auto tape_path = create_temp_tape(temp_tape_elements);
    {
        file_tape tape(tape_path);

        tape.move_right();

        ASSERT_EQ(tape.read(), 2);
    }
    std::filesystem::remove(tape_path);
}

TEST(TapeTest, Move_RightBoundaryTest)
{
    std::vector<int> temp_tape_elements = {1, 2, 3};

    auto tape_path = create_temp_tape(temp_tape_elements);
    {
        file_tape tape(tape_path);

        tape.move_right();

        tape.move_right();

        ASSERT_FALSE(tape.move_right());
    }
    std::filesystem::remove(tape_path);
}

TEST(TapeTest, Move_LeftTest)
{
    std::vector<int> temp_tape_elements = {1, 2, 3};

    auto tape_path = create_temp_tape(temp_tape_elements);
    {
        file_tape tape(tape_path);

        tape.move_right();

        tape.move_left();

        ASSERT_EQ(tape.read(), 1);
    }
    std::filesystem::remove(tape_path);
}

TEST(TapeTest, Move_LeftBoundaryTest)
{
    std::vector<int> temp_tape_elements = {1, 2, 3};

    auto tape_path = create_temp_tape(temp_tape_elements);
    {
        file_tape tape(tape_path);

        ASSERT_FALSE(tape.move_left());
    }
    std::filesystem::remove(tape_path);
}

TEST(TapeTest, Move_RewindTest)
{
    std::vector<int> temp_tape_elements = {1, 2, 3};

    auto tape_path = create_temp_tape(temp_tape_elements);
    {
        file_tape tape(tape_path);

        tape.move_right();

        tape.move_right();

        tape.rewind();

        ASSERT_EQ(tape.read(), 1);
    }
    std::filesystem::remove(tape_path);
}

TEST(TapeTest, get_str_reprTest)
{
    std::vector<int> temp_tape_elements = {1, 2, 3};

    auto tape_path = create_temp_tape(temp_tape_elements);
    {
        file_tape tape(tape_path);

        ASSERT_EQ(tape.get_str_repr(), "          1          2          3");
    }
    std::filesystem::remove(tape_path);
}

TEST(TapeTest, EmptyTapeTest)
{
    std::vector<int> temp_tape_elements = {};

    auto tape_path = create_temp_tape(temp_tape_elements);
    ASSERT_THROW(
        {
            file_tape tape(tape_path);
        },
        std::runtime_error);
    std::filesystem::remove(tape_path);
}
