//
// NESkvik - Fredrik Ofstad
//
#include "SDL.h"
#undef main

#include "app.h"

int main(int argc, char* argv[]) {
    printf("starting NESkvik...\n");
    App app;
    printf("app created\n");
    app.run();
    printf("app finished\n");
    return 0;
}
