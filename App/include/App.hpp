// Copyright(c) 2023 Klimova Mary
#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <map>
#include <memory>

struct directorInfo {
    std::string idDirector;
    std::string nameDirector;
    std::vector<std::string> knownForTitles;
};

class App {
 public:
    const static int directorsInfoCheck(std::string nbasname,
                                        std::string director,
                                        std::unique_ptr<directorInfo> &dirInfo);
    static int checkIfIsDirector(std::string tcrewname,
                                 std::unique_ptr<directorInfo> &dirInfo);
    static int checkIfIsAdult(std::string tbasname,
                              std::unique_ptr<directorInfo> &df);
    static int checkRussianRegion(std::string takasname,
                                  std::unique_ptr<directorInfo> &df);
    static int parseCmd(std::map<std::string, std::string> *arguments, int argc,
                        char **argv);
    static int Run(int argc, char **argv);
};
