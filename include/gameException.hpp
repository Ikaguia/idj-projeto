#ifndef GAMEEXCEPTIONHPP
#define GAMEEXCEPTIONHPP

#include <exception>
#include <string>


class GameException : public std::exception {
    string msg;
public:

    GameException(string msg);


    virtual const char* what() const noexcept {
        return msg.c_str();
    };
};


#endif
