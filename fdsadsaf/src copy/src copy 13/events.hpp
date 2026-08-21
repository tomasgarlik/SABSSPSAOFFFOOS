#include "includes.hpp"
void handle_events(){
    clic=false;
    typedChar="";
    specialkey=0;
    modkey=KMOD_NONE;
    clickedOnSomething=false;
    now_wheel_x=0;
    now_wheel_y=0;
    while (SDL_PollEvent(&event)) {
        switch (event.type){
            case SDL_QUIT:
                SDL_Quit();
                running=false;
                SDL_DestroyWindow(window);
                SDL_DestroyRenderer(renderer);
                exit(0);
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event==SDL_WINDOWEVENT_CLOSE){
                    SDL_Quit();
                    running=false;
                    SDL_DestroyWindow(window);
                    SDL_DestroyRenderer(renderer);
                    exit(0);
                }
                break;
            case SDL_MOUSEMOTION:
                if (retina){
                    mousex = event.motion.x*2;
                    mousey = event.motion.y*2;
                } else {
                    mousex = event.motion.x;
                    mousey = event.motion.y;
                }
                if (is_panning && active) {
                    // e.motion.xrel a yrel v sobě mají přesný počet pixelů, 
                    // o kolik se myš posunula od posledního framu
                    grid_sx += event.motion.xrel;
                    grid_sy += event.motion.yrel;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    clicked=true;
                    clic=true;
                }
                if (event.button.button == SDL_BUTTON_RIGHT && active) {
                    is_panning = true;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    is_panning = false;
                } else {
                    clickup=true;
                    clicked=false;
                }
                break;
            case SDL_MOUSEWHEEL:
                {
                    now_wheel_x=event.wheel.x;
                    now_wheel_y=event.wheel.y;
                    break;
                }
            case SDL_TEXTINPUT:
                typedChar=event.text.text;
                break;
            case SDL_KEYDOWN:
                modkey = SDL_GetModState();
                if (modkey & KMOD_COMMAND) {
                    // switch (event.key.keysym.sym){
                    //     case SDLK_EQUALS:
                    //         scale+=0.2;
                    //         break;
                    //     case SDLK_MINUS:
                    //         scale-=0.2;
                    //         if (scale<=0.0){scale=0.2;}
                    //         break;
                    //     }
                    if (event.key.keysym.sym == SDLK_g) {
                        gui_type++;
                        if (gui_type > 3) {
                            gui_type = 0;
                        }
                        std::ofstream outFile("gui_type.txt"); // Otevře soubor pro zápis
    
if (outFile.is_open()) {
    outFile << gui_type; // Zapíše int do souboru
    outFile.close();  // Soubor je důležité zavřít
} else {
    std::cerr << "Chyba: Nepodarilo se otevrit soubor pro zapis!\n";
}

                    }
                } else {
                    switch (event.key.keysym.sym){
                        case SDLK_LEFT:
                            specialkey='l';
                            break;
                        case SDLK_RIGHT:
                            specialkey='r';
                            break;
                        case SDLK_BACKSPACE:
                            specialkey='b';
                            break;
                        }
                }
                break;
            case SDL_WINDOWEVENT_RESIZED:
                SDL_GetWindowSize(window, &width, &height);
                break;
            
        }
    }
    if (!swapping && !jetzt_normal_cursor){
        SDL_SetCursor(cursor_default);
    }
    SDL_GetWindowSize(window, &width, &height);
}