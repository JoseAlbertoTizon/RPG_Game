#include "game.hpp"

int main() {
    Game a;
    a.read_save_from("save_data.txt");
    a.loop();
}
