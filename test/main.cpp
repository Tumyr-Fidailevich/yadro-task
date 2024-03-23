#include "tape_config_test.h"
#include "file_tape_test.h"
#include "tape_sorting_test.h"

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "RU");
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}