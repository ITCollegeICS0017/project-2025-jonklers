// Lightweight key-detection implementation. This file provides a simple
// cross-platform API that other parts of the program can include.

#include "utils.h"

#include <iostream>
#include <limits>

// Platform-specific headers
#ifdef _WIN32
#include <conio.h> // _getch()
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif

void clearConsole() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

Key detectKey() {
#ifdef _WIN32
    int ch = _getch();
    if (ch == 0 || ch == 224) { // extended key
        int ext = _getch();
        switch (ext) {
            case 72: return Key::Up;
            case 80: return Key::Down;
            case 75: return Key::Left;
            case 77: return Key::Right;
            default: return Key::None;
        }
    }
    if (ch == ' ') return Key::Space;
    if (ch == '\r' || ch == '\n') return Key::Enter;
    return Key::None;
#else
    // Save current terminal state
    struct termios oldt;
    if (tcgetattr(STDIN_FILENO, &oldt) == -1) return Key::None;

    // Configure new terminal state: non-canonical, no echo, blocking read
    // VMIN=1 makes read() block until at least one byte is available.
    struct termios newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    newt.c_cc[VMIN] = 1;  // wait for at least 1 byte
    newt.c_cc[VTIME] = 0; // no timeout
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    unsigned char b;
    ssize_t n = read(STDIN_FILENO, &b, 1);
    if (n <= 0) {
        // restore terminal
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return Key::None;
    }

    // If we read ESC, blockingly read following bytes to form an escape
    // sequence. Because we removed timeouts, this will block until bytes
    // arrive (per your request).
    if (b == 27) {
        unsigned char seq1 = 0, seq2 = 0;
        ssize_t n1 = read(STDIN_FILENO, &seq1, 1);
        if (n1 <= 0) {
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            return Key::None;
        }
        if (seq1 == '[') {
            ssize_t n2 = read(STDIN_FILENO, &seq2, 1);
            if (n2 <= 0) {
                tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                return Key::None;
            }
            Key result = Key::None;
            switch (seq2) {
                case 'A': result = Key::Up; break;
                case 'B': result = Key::Down; break;
                case 'C': result = Key::Right; break;
                case 'D': result = Key::Left; break;
                default: result = Key::None; break;
            }
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            return result;
        }

        // Not an arrow sequence — restore and return None.
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return Key::None;
    }

    // Single-character keys
    Key out = Key::None;
    if (b == ' ') out = Key::Space;
    else if (b == '\n' || b == '\r') out = Key::Enter;

    // restore terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return out;
#endif
}

void wait() {
    std::cout << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
