// Lightweight key-detection API for terminal input
#pragma once

#include <iosfwd>
#include <string>
#include "../base/User.h"
#include "../base/Listing.h"

inline std::string enumToStrCategory(Category e) {
    switch (e) {
        case Category::BOOKS:
            return "BOOKS";
        case Category::ELECTRONICS:
            return "ELECTRONICS";
        case Category::FASHION:
            return "FASHION";
        case Category::GARDEN:
            return "GARDEN";
        case Category::HOME:
            return "HOME";
        default:
            return "NONE";
    }
}

// Keys that detectArrowKey can return
enum class Key {
    None,
    Up,
    Down,
    Left,
    Right,
    Enter,
    Space,
    Quit
};

void clearConsole();

// Read one keystroke (non-blocking). This function will temporarily
// configure the terminal to non-canonical/non-blocking mode, attempt to
// read one key (or an arrow escape sequence), then restore the terminal
// to its previous state before returning.
//
// Returns a Key enum describing the pressed key. Returns Key::None
// if no recognized key was pressed or if there was no input available.
Key detectKey();
void wait();
std::string getPassword();
