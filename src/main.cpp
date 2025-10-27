#include <iostream>
#include <string>
#include "ui/Menu.h"
#include "ui/MenuItem.h"

void lol() {}

int main() {
    Menu menu({
        MenuItem("File", {
            MenuItem("New", lol),
            MenuItem("Open", lol),
            MenuItem("Save", lol),
            MenuItem("Exit", lol),
        }),
        MenuItem("Edit", lol),
        MenuItem("Selection", lol),
        MenuItem("View", lol),
    });

    menu.run();

    return 0;
}