// Copyright(c) 2023 Klimova Mary
#include "App.hpp"

int App::Run(int argc, char **argv) {
    const int neededArgsNum = 6;
    if (argc != neededArgsNum) {
        std::cerr << "Wrong number of input arguments" << std::endl;
        return 1;
    }
    std::map<std::string, std::string> arguments = {{"-nbasics", ""},
                                                    {"-tcrew", ""},
                                                    {"-tbasics", ""},
                                                    {"-takas", ""},
                                                    {"-dir", ""}};
    if (parseCmd(&arguments, argc, argv)) {
        return 2;
    }
    std::unique_ptr<directorInfo> dirInfo = std::make_unique<directorInfo>();
    std::cout << "Searching for the director..." << std::endl;
    if (App::directorsInfoCheck(arguments["-nbasics"], arguments["-dir"],
                                dirInfo)) {
        return 3;
    }
    std::cout << "Director found! Checking titles..." << std::endl;

    checkIfIsDirector(arguments["-tcrew"], dirInfo);

    if (dirInfo->knownForTitles.size() == 0) {
        std::cout << "No known titles for this director "
                     "in IMDB"
                  << std::endl;
        return 4;
    }
    std::cout << "This director has " << dirInfo->knownForTitles.size()
              << " titles. Checking..." << std::endl;

    std::cout << "Filtering adult movies and other types of media..."
              << std::endl;
    checkIfIsAdult(arguments["-tbasics"], dirInfo);
    std::cout << "Found " << dirInfo->knownForTitles.size() << " inique titles."
              << std::endl;

    std::cout << "Checking if there are russian movies..." << std::endl;

    checkRussianRegion(arguments["-takas"], dirInfo);
    std::cout << std::endl;
    return 0;
}

const int App::directorsInfoCheck(
    std::string nbasname, std::string director,
    std::unique_ptr<directorInfo> const &dirInfo) {
    std::ifstream imdbnbas;
    imdbnbas.open(nbasname);
    if (!imdbnbas.is_open()) {
        std::cout << "Names database is not opened." << std::endl;
        return 1;
    }
    std::string line = "";
    getline(imdbnbas, line);
    dirInfo->nameDirector = "";
    while (director != dirInfo->nameDirector && !imdbnbas.eof()) {
        getline(imdbnbas, dirInfo->idDirector, '\t');
        getline(imdbnbas, dirInfo->nameDirector, '\t');
        getline(imdbnbas, line);
    }
    imdbnbas.close();
    if (dirInfo->nameDirector != director) {
        std::cout << "Person not found in IMDB database" << std::endl;
        return 1;
    }
    return 0;
}

int App::checkIfIsDirector(std::string tcrewname,
                           std::unique_ptr<directorInfo> const &dirInfo) {
    std::ifstream imdbtcrew;
    imdbtcrew.open(tcrewname);
    if (!imdbtcrew.is_open()) {
        std::cout << "File with crew is not opened" << std::endl;
        return 1;
    }
    std::string tline = "", dline = "";
    getline(imdbtcrew, tline);
    while (!imdbtcrew.eof()) {
        getline(imdbtcrew, tline, '\t');
        getline(imdbtcrew, dline, '\t');
        if (dline == dirInfo->idDirector) {
            dirInfo->knownForTitles.push_back(tline);
        }
        getline(imdbtcrew, tline);
    }
    imdbtcrew.close();
    return 0;
}

int App::checkIfIsAdult(std::string tbasname,
                        std::unique_ptr<directorInfo> const &df) {
    std::ifstream imdbtbas;
    imdbtbas.open(tbasname);
    if (!imdbtbas.is_open()) {
        std::cout << "File with titles database is not opened" << std::endl;
        return 1;
    }
    std::string line = "";
    int i = 0;
    getline(imdbtbas, line);
    while (i < df->knownForTitles.size() && !imdbtbas.eof()) {
        getline(imdbtbas, line, '\t');
        auto it = std::find(df->knownForTitles.begin(),
                            df->knownForTitles.end(), line);
        if (it != df->knownForTitles.end()) {
            std::string titleType = "";
            bool isAdult = false;
            getline(imdbtbas, titleType, '\t');
            getline(imdbtbas, line, '\t');
            getline(imdbtbas, line, '\t');
            imdbtbas >> isAdult;
            if (titleType != "movie" || isAdult) {
                df->knownForTitles.erase(it);
                getline(imdbtbas, line);
                continue;
            }
            i++;
        }
        getline(imdbtbas, line);
    }
    imdbtbas.close();
    return 0;
}

int App::checkRussianRegion(std::string takasname,
                            std::unique_ptr<directorInfo> const &df) {
    std::ifstream imdbtakas;
    imdbtakas.open(takasname);
    if (!imdbtakas.is_open()) {
        std::cout << "File with regions database is not opened" << std::endl;
        return 1;
    }
    int i = 0;
    std::string line = "";
    getline(imdbtakas, line);
    getline(imdbtakas, line, '\t');
    while (i < df->knownForTitles.size() && !imdbtakas.eof()) {
        auto it = std::find(df->knownForTitles.begin(),
                            df->knownForTitles.end(), line);
        if (it != df->knownForTitles.end()) {
            while (line == *it) {
                std::string movieTitle;
                getline(imdbtakas, line, '\t');
                getline(imdbtakas, movieTitle, '\t');
                getline(imdbtakas, line, '\t');
                if (line == "RU" || line == "SUHH") {
                    std::cout << movieTitle << std::endl;
                }
                getline(imdbtakas, line);
                getline(imdbtakas, line, '\t');
            }
            i++;
            continue;
        }
        getline(imdbtakas, line);
        getline(imdbtakas, line, '\t');
    }
    imdbtakas.close();
    return 0;
}

int App::parseCmd(std::map<std::string, std::string> *arguments, int argc,
                  char **argv) {
    auto args = std::span(argv, static_cast<size_t>(argc));
    std::string prefix = "";
    std::string value = "";
    for (int i = 1; i < argc; i++) {
        std::stringstream arg(args[i]);
        getline(arg, prefix, '=');
        if (auto search = arguments->find(prefix); search != arguments->end()) {
            getline(arg, value);
            (*arguments)[prefix] = value;
        } else {
            std::cout << "Wrong input "
                         "arguments"
                      << std::endl;
            return 1;
        }
    }
    auto i = (*arguments)["-dir"].find("_");
    while (i != std::string::npos) {
        (*arguments)["-dir"].replace(i, 1, " ");
        i = (*arguments)["-dir"].find("_");
    }
    return 0;
}
