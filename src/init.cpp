if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_ShowSimpleMessageBox(
        SDL_MESSAGEBOX_ERROR,
        "Somthing went wrong during the initalization of SDL2.",
        SDL_GetError(),
        nullptr // žádné okno zatím nemáme
    );
    return 1;
}
if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) == 0) {
    SDL_ShowSimpleMessageBox(
        SDL_MESSAGEBOX_ERROR,
        "SDL2_image initialization failed",
        IMG_GetError(),
        nullptr
    );
    SDL_Log("IMG_Init failed: %s", IMG_GetError());
    return 1;
}
std::ifstream inFile("gui_type.txt"); // Otevře soubor pro čtení
    
    if (inFile.is_open()) {
        inFile >> gui_type; // Načte hodnotu ze souboru do proměnné
        inFile.close();  // Zavře soubor
    } else {
        gui_type = 0; // Výchozí hodnota, pokud se soubor nepodaří otevřít
    }
SDL_Rect rect;
TTF_Init();
if (gui_type!=3){
    lfont = TTF_OpenFont("fonts/font3.ttf", lfont_size);
} else {
    lfont = TTF_OpenFont("fonts/font1.ttf", lfont_size);
}
hfont = TTF_OpenFont("fonts/font3.ttf", 300);
font=lfont;
fontsize=lfont_size;
SDL_Window* splashscreen = SDL_CreateWindow("Splash Screen",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    600, 275, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
if (!splashscreen) {
    SDL_Log("Window creation failed: %s", SDL_GetError());
    return 1;
}
SDL_Renderer* rendere = SDL_CreateRenderer(splashscreen, -1, SDL_RENDERER_ACCELERATED);
if (!rendere) {
    SDL_Log("Renderer creation failed: %s", SDL_GetError());
    SDL_DestroyWindow(splashscreen);
    return 1;
}
SDL_Texture* texture = loadTexture("icons/splashscreen.png", rendere);
SDL_RenderClear(rendere);
SDL_RenderCopy(rendere, texture, NULL, NULL);
SDL_RenderPresent(rendere);
Uint32 startTime = SDL_GetTicks();
init_cursors();
running=true;
while (running) {
    SDL_Event event0;
    while (SDL_PollEvent(&event0)) {
        if (event0.type == SDL_QUIT) {
            running = false;
        }
    }
    if (SDL_GetTicks() - startTime > SPLASHSCREENTIME) {
        running = false;
    }
}
SDL_DestroyTexture(texture);
SDL_DestroyRenderer(rendere);
SDL_DestroyWindow(splashscreen);
#ifdef RETINA_DISPLAY
Uint32 window_flags = SDL_WINDOW_SHOWN | 
                    SDL_WINDOW_RESIZABLE | 
                    SDL_WINDOW_MAXIMIZED | 
                    SDL_WINDOW_ALLOW_HIGHDPI;
retina=true;
#else 
Uint32 window_flags = SDL_WINDOW_SHOWN | 
                    SDL_WINDOW_RESIZABLE | 
                    SDL_WINDOW_MAXIMIZED;
retina=false;
#endif
if (retina){
    scale=2.0;
} else {
    scale=1.0;
}
window = SDL_CreateWindow(
    "SABSSPSAOFFFOOS Beta",
    SDL_WINDOWPOS_CENTERED, 
    SDL_WINDOWPOS_CENTERED,
    800, 600,  // při MAXIMIZED se ignoruje
    window_flags
);
if (!window) {
    SDL_ShowSimpleMessageBox(
        SDL_MESSAGEBOX_ERROR,
        "Something fucked up while creating a window.",
        SDL_GetError(),
        nullptr
    );
    SDL_Quit();
    return 1;
}
renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

if (!renderer) {
    SDL_ShowSimpleMessageBox(
        SDL_MESSAGEBOX_ERROR,
        "Something fucked up while trying to show something in the window.",
        SDL_GetError(),
        nullptr
    );
    SDL_Quit();
    return 1;
}
dropmenu filemenu = {
    .functions={
        newfunc,
        openfunc,
        saveasfunc,
        savefunc,
        [](){export_opened=true;init_export_panel();},
        quitfunc
    },
    .textures={
        createTextTexture(renderer,"Nový"),
        createTextTexture(renderer,"Otevřít ..."),
        createTextTexture(renderer,"Uložit jako..."),
        createTextTexture(renderer,"Uložit"),
        createTextTexture(renderer,"Exportovat do PDF..."),
        createTextTexture(renderer,"Ukončit")
    },
    .lenght=6,
    .title=createTextTexture(renderer, "Soubor"),
    .titletype="defined",
    .xpos=0,
    .ypos=0,
    .width=getTextureWidth(createTextTexture(renderer,"Exportovat do PDF... "), STANDARTPICEHEIGHT-STANDARTBORDER*2),
    .topwidth=80
};
// textbox testbox = {
//     .xpos=300,
//     .ypos=200,
//     .text="Hello guis today we have a list of",
//     .active=false,
//     .textsize=30
// };
SDL_Color cc={255,100,20,255};
running=true;
SDL_StartTextInput();
SDL_SetWindowIcon(window, IMG_Load("icons/icon.png"));
SDL_GetWindowSize(window, &width, &height);
