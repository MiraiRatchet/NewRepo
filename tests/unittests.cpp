// Copyright(c) 2023 Klimova Mary
#include <gtest/gtest.h>
#include "App.hpp"

TEST(parseCmdTest, CheckArgs) {
    std::map<std::string, std::string> arguments = {{"-nbasics", ""},
                                                    {"-tcrew", ""},
                                                    {"-tbasics", ""},
                                                    {"-takas", ""},
                                                    {"-dir", ""}};
    std::map<std::string, std::string> expected = {{"-nbasics", "ndsvnenu"},
                                                   {"-tcrew", "ljdkejk"},
                                                   {"-tbasics", "llldoo"},
                                                   {"-takas", "lcewilfml"},
                                                   {"-dir", "lll doo j"}};
    const int argc = 6;
    char **argv = new char *[argc];
    argv[0] = "ejejd";
    argv[5] = "-nbasics=ndsvnenu";
    argv[1] = "-dir=lll_doo_j";
    argv[2] = "-tak=lcewilfml";
    argv[3] = "-tcrew=ljdkejk";
    argv[4] = "-tbasics=llldoo";
    // App::parseCmd(&arguments, argc, argv);
    // EXPECT_EQ(arguments, expected);
    EXPECT_EQ(App::parseCmd(&arguments, argc, argv), 1);
}
