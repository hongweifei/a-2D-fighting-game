
#include "window.h"
#include "game.h"

int main()
{
    init_game(init_window());
    start_game();
    quit_game();

    return 0;
}
