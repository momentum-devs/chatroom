#include "loguru.hpp"

int main(int argc, char* argv[])
{
    loguru::g_preamble_date = false;

    loguru::init(argc, argv);

    return 0;
}