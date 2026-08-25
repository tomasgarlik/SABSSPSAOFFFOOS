#include "includes.hpp"
#include <thread>
#include <atomic>
#include <regex>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <cstdio>
    #include <cstdlib>
    #include <unistd.h>
#endif
// FUNKCE: Ověří připojení k GitHubu a v případě chyby vrátí popis v errorMessage

// Pomocná funkce pro spuštění příkazu a přečtení jeho celého výstupu do stringu
static std::string ExecCmdSimple(const std::string& cmd) {
    std::string result = "";
#ifdef _WIN32
    FILE* pipe = _popen(cmd.c_str(), "r");
    if (!pipe) return "";
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        result += buffer;
    }
    _pclose(pipe);
#else
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) return "";
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        result += buffer;
    }
    pclose(pipe);
#endif
    return result;
}
bool IsGitHubReachable(std::string& errorMessage, std::string gitPath = "git", std::string repoUrl = "origin") {
    errorMessage = "";

#ifdef _WIN32
    std::string cmd = "\"" + gitPath + "\" ls-remote --exit-code " + repoUrl + " HEAD 2>&1";
#else
    std::string cmd = gitPath + " ls-remote --exit-code " + repoUrl + " HEAD 2>&1";
#endif

    std::string output = ExecCmdSimple(cmd);

    // 1. Žádný výstup obvykle znamená selhání vykonání příkazu
    if (output.empty()) {
        errorMessage = "Chyba: Příkaz Git neodpovídá nebo nebyl nalezen.";
        return false;
    }

    // 2. Analýza chybových hlášek z výstupu Gitu
    if (output.find("Could not resolve host") != std::string::npos || 
        output.find("network is unreachable") != std::string::npos ||
        output.find("Failed to connect") != std::string::npos) {
        errorMessage = "Nelze se připojit k síti. Zkontrolujte připojení k internetu.";
        return false;
    }

    if (output.find("Repository not found") != std::string::npos) {
        errorMessage = "Vzdálený repozitář na GitHubu nebyl nalezen.";
        return false;
    }

    if (output.find("Permission denied") != std::string::npos || 
        output.find("Authentication failed") != std::string::npos) {
        errorMessage = "Chyba přístupových práv nebo autentizace k repozitáři.";
        return false;
    }

    if (output.find("fatal:") != std::string::npos || output.find("error:") != std::string::npos) {
        errorMessage = "Chyba Gitu: " + output;
        return false;
    }

    return true; // Připojení je v pořádku
}
// 1. FUNKCE: Zjistí, zda jsou k dispozici nové aktualizace (vrací true/false)
bool CheckForUpdates(std::string gitPath = "git", std::string branch = "main") {
    printf("checking for updates...\n");

    // 1. Zjistíme hash posledního LOKÁLNÍHO commitu (HEAD)
#ifdef _WIN32
    std::string localCmd  = "\"" + gitPath + "\" rev-parse HEAD 2>&1";
    std::string remoteCmd = "\"" + gitPath + "\" ls-remote origin " + branch + " 2>&1";
#else
    std::string localCmd  = gitPath + " rev-parse HEAD 2>&1";
    std::string remoteCmd = gitPath + " ls-remote origin " + branch + " 2>&1";
#endif

    std::string localHash = ExecCmdSimple(localCmd);
    std::string remoteOutput = ExecCmdSimple(remoteCmd);

    // Ořízneme případné odřádkování (\n)
    while (!localHash.empty() && (localHash.back() == '\n' || localHash.back() == '\r')) localHash.pop_back();

    // Vyextrahujeme samotný hash z výstupu ls-remote (vypadá jako: "85e5d4c... refs/heads/main")
    std::string remoteHash = "";
    if (!remoteOutput.empty()) {
        size_t spacePos = remoteOutput.find_first_of(" \t");
        if (spacePos != std::string::npos) {
            remoteHash = remoteOutput.substr(0, spacePos);
        }
    }

    // Pokud se lokální a vzdálený hash neshodují, je k dispozici aktualizace!
    if (!remoteHash.empty() && localHash != remoteHash) {
        printf("updates found!\n");
        return true;
    }

    printf("no updates found\n");
    return false;
}
// 2. FUNKCE: Vrátí seznam (vector) všech nových commit zpráv
// Vylepšený GetCommitNotes - stáhne JEN zprávy commitů bez 100MB dat
std::vector<std::string> GetCommitNotes(std::string gitPath, std::string branch="main") {
    std::vector<std::string> notes;

#ifdef _WIN32
    std::string fetchCmd = "\"" + gitPath + "\" fetch --no-tags --quiet origin " + branch + " 2>&1";
    std::string logCmd   = "\"" + gitPath + "\" log HEAD..origin/" + branch + " --format=%s 2>&1";
#else
    std::string fetchCmd = gitPath + " fetch --no-tags --quiet origin " + branch + " 2>&1";
    std::string logCmd   = gitPath + " log HEAD..origin/" + branch + " --format=%s 2>&1";
#endif

    std::string fetchOutput = ExecCmdSimple(fetchCmd);
    printf("fetch output: %s\n", fetchOutput.c_str()); // DEBUG

    std::string output = ExecCmdSimple(logCmd);
    printf("log output: %s\n", output.c_str()); // DEBUG

    if (output.empty()) return notes;

    std::stringstream ss(output);
    std::string line;
    while (std::getline(ss, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (!line.empty()) notes.push_back(line);
    }

    return notes;
}
// Interní atomické proměnné (bezpečné pro přístup z více vláken)
static std::atomic<int>  g_Progress{0};
static std::atomic<bool> g_IsFinished{false};
static std::atomic<bool> g_IsSuccess{false};

// -------------------------------------------------------------------
// INTERNÍ VLÁKNO (Nevoláš přímo, spouští ho StartGitPull)
// -------------------------------------------------------------------
static void InternalGitPullWorker(std::string gitPath = "git") {
    g_Progress = 0;
    g_IsFinished = false;
    g_IsSuccess = false;

    std::regex percentRegex(R"((\d+)%)");
    std::smatch match;

    // -------------------------------------------------------------------
    // 1. FÁZE: FETCH (Stahování dat ze sítě: 0 % -> 85 %)
    // -------------------------------------------------------------------
#ifdef _WIN32
    std::string fetchCmd = "\"" + gitPath + "\" fetch --progress origin main 2>&1";

    HANDLE hRead, hWrite;
    SECURITY_ATTRIBUTES saAttr = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
    if (!CreatePipe(&hRead, &hWrite, &saAttr, 0)) {
        g_IsFinished = true;
        return;
    }
    SetHandleInformation(hRead, HANDLE_FLAG_INHERIT, 0);

    STARTUPINFOA si = { sizeof(STARTUPINFOA) };
    si.cb = sizeof(STARTUPINFOA);
    si.hStdOutput = hWrite;
    si.hStdError = hWrite;
    si.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    PROCESS_INFORMATION pi;
    if (!CreateProcessA(NULL, (LPSTR)fetchCmd.c_str(), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        CloseHandle(hRead);
        CloseHandle(hWrite);
        g_IsFinished = true;
        return;
    }
    CloseHandle(hWrite);

    char buffer[256];
    DWORD bytesRead;
    std::string currentLine = "";

    while (ReadFile(hRead, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
        buffer[bytesRead] = '\0';
        currentLine += buffer;

        size_t pos = 0;
        while ((pos = currentLine.find_first_of("\r\n")) != std::string::npos) {
            std::string line = currentLine.substr(0, pos);
            currentLine.erase(0, pos + 1);

            if (std::regex_search(line, match, percentRegex)) {
                int rawPercent = std::stoi(match[1].str());
                g_Progress = static_cast<int>(rawPercent * 0.85); // 0-100% z fetche -> 0-85% celkově
            }
        }
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD exitCode;
    GetExitCodeProcess(pi.hProcess, &exitCode);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hRead);

    if (exitCode != 0) {
        g_IsSuccess = false;
        g_IsFinished = true;
        return;
    }

#else
    // --- MAC / LINUX IMPLEMENTACE ---
    std::string fetchCmd = gitPath + " fetch --progress origin main 2>&1";
    FILE* pipe = popen(fetchCmd.c_str(), "r");
    if (!pipe) {
        g_IsFinished = true;
        return;
    }

    char buffer[256];
    std::string currentLine = "";
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer) - 1, pipe)) > 0) {
        buffer[bytesRead] = '\0';
        currentLine += buffer;

        size_t pos = 0;
        while ((pos = currentLine.find_first_of("\r\n")) != std::string::npos) {
            std::string line = currentLine.substr(0, pos);
            currentLine.erase(0, pos + 1);

            if (std::regex_search(line, match, percentRegex)) {
                int rawPercent = std::stoi(match[1].str());
                g_Progress = static_cast<int>(rawPercent * 0.85); // 0-100% z fetche -> 0-85% celkově
            }
        }
    }

    int returnCode = pclose(pipe);
    if (returnCode != 0) {
        g_IsSuccess = false;
        g_IsFinished = true;
        return;
    }
#endif

    // -------------------------------------------------------------------
    // 2. FÁZE: MERGE (Aplikování změn na disk: 85 % -> 100 %)
    // -------------------------------------------------------------------
    g_Progress = 85;

#ifdef _WIN32
    std::string mergeCmd = "\"" + gitPath + "\" merge --ff-only origin/main 2>&1";
#else
    std::string mergeCmd = gitPath + " merge --ff-only origin/main 2>&1";
#endif

    // Pro samotný merge stačí použít tvou pomocnou funkci ExecCmdSimple
    std::string mergeOutput = ExecCmdSimple(mergeCmd);

    // Kontrola, zda merge nekončí chybou (např. konflikt)
    if (mergeOutput.find("fatal:") != std::string::npos || 
        mergeOutput.find("error:") != std::string::npos ||
        mergeOutput.find("CONFLICT") != std::string::npos) {
        g_IsSuccess = false;
        g_IsFinished = true;
        return;
    }

    // Vše proběhlo v pořádku
    g_Progress = 100;
    g_IsSuccess = true;
    g_IsFinished = true;
}

// ===================================================================
// TVOJE 2 POŽADOVANÉ FUNKCE + KONTROLA
// ===================================================================

// 1. FUNKCE: Odstartuje git pull na pozadí
void StartGitPull(std::string gitPath = "git") {
    std::thread t(InternalGitPullWorker, gitPath);
    t.detach(); // Odpojí vlákno, takže běží autonomně a neblokuje hlavní kód
}

// 2. FUNKCE: Vrací aktuální procenta (0 až 100)
int GetGitProgress() {
    return g_Progress.load();
}

// POMOCNÁ FUNKCE: Zjistí, zda je update hotový (a zda byl úspěšný)
bool IsGitPullFinished(bool &outSuccess) {
    outSuccess = g_IsSuccess.load();
    return g_IsFinished.load();
}
void init_cursors() {
    // Vytvoříme výchozí systémový kurzor (šipku)
    cursor_default = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    // Vytvoříme kurzor, který symbolizuje ruční prohazování/akci (např. ručičku)
    // Na některých OS to bude přesně ten prohazovací symbol, jinde jen jiná šipka/ručička
    cursor_swap = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND); 
}
void quitfunc(){
    return;
}
void newfunc(){}
static void LaunchDetached(const std::string& path) {
#ifdef _WIN32
    STARTUPINFOA si = { sizeof(STARTUPINFOA) };
    PROCESS_INFORMATION pi;

    CreateProcessA(
        NULL,
        (LPSTR)path.c_str(),
        NULL, NULL, FALSE,
        DETACHED_PROCESS | CREATE_NEW_PROCESS_GROUP,
        NULL, NULL,
        &si, &pi
    );
    if (pi.hProcess) CloseHandle(pi.hProcess);
    if (pi.hThread) CloseHandle(pi.hThread);
#else
    pid_t pid = fork();
    if (pid == 0) {
        // --- potomek ---
        setsid(); // odpojí se od terminálu/rodiče
        execl(path.c_str(), path.c_str(), (char*)NULL);
        _exit(1); // execl se vrátí jen při chybě
    }
    // --- rodič (updater) pokračuje dál, nečeká ---
#endif
}
int pull_state=0;
std::string gitPath = "";
int main(int argc, char* argv[]) {
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




















    std::string gitError;
    #ifdef _WIN32
    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);
    std::string exeDir = std::string(exePath).substr(0, std::string(exePath).find_last_of("\\/"));
    gitPath = exeDir + "\\git\\cmd\\git.exe";
    #else
        gitPath = "git";
    #endif
    printf("git path: %s\n", gitPath.c_str());
    // Předáme proměnnou gitError, do které se zapíše případná chyba
    printf("checking for git reachable...\n");
    if (!IsGitHubReachable(gitError, gitPath)) {
        printf("Chyba připojení: %s\n", gitError.c_str());
        pull_state=3;
    } else {
        printf("github is reachable\n");
    }
    if (CheckForUpdates(gitPath) || pull_state==3) {
        printf("continuing with making window\n");
        // Jsou dostupné nové změny!
        std::vector<std::string> changes = GetCommitNotes(gitPath);

SDL_DestroyTexture(texture);
SDL_DestroyRenderer(rendere);
SDL_DestroyWindow(splashscreen);
#ifdef RETINA_DISPLAY
Uint32 window_flags = SDL_WINDOW_SHOWN | 
                    SDL_WINDOW_RESIZABLE | 
                    SDL_WINDOW_ALLOW_HIGHDPI;
retina=true;
#else 
Uint32 window_flags = SDL_WINDOW_SHOWN | 
                    SDL_WINDOW_RESIZABLE;
retina=false;
#endif
if (retina){
    scale=2.0;
} else {
    scale=1.0;
}
window = SDL_CreateWindow(
    "Aktualizacovátor",
    SDL_WINDOWPOS_CENTERED, 
    SDL_WINDOWPOS_CENTERED,
    700, 500,  // při MAXIMIZED se ignoruje
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
        float progress_shift=0.0f;
        int overall_progress=0;
        #define MAX_PROGRESS_SHIFT 70.0f
        std::string endstring;
        #define ENDSTRING_PERIOD 500
        while (running){
            if (clickup){clickup=false;}
            SDL_GetRendererOutputSize(renderer, &width, &height);
            handle_events();

            SDL_RenderClear(renderer);
            SDL_Rect rect = { 0, 0, width, height };
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &rect);
            panel p={};
            if ((SDL_GetTicks()/ENDSTRING_PERIOD)%3==0){
                endstring=".";
            } else if ((SDL_GetTicks()/ENDSTRING_PERIOD)%3==1){
                endstring="..";
            } else {
                endstring="...";
            }
            p.x=0;
            p.y=0;
            p.w=width;
            p.h=height;
            int progress;
            if (pull_state==0){
                p.top_elements.push_back({
                    .type=ELEMENT_LABEL,
                    .label={
                        .xpos=width/2,
                        .ypos=0,
                        .text="Byla vyhledána aktualizace",
                        .textsize=STANDARTPICEHEIGHT,
                        .origin_left=false
                    }
                });
                p.bottom_elements.push_back({
                    .type=ELEMENT_BUTTON,
                    .button={
                        .texture=createTextTexture(renderer, "Zrušit"),
                        .xpos=width-300,
                        .ypos=0,
                        .function=[](){
                            running=false;
                        }
                    }
                });
                p.bottom_elements.push_back({
                    .type=ELEMENT_BUTTON,
                    .button={
                        .texture=createTextTexture(renderer, "Stáhnout aktualizaci"),
                        .xpos=width-225,
                        .ypos=0,
                        .function=[](){
                            StartGitPull(gitPath);
                            pull_state=1;
                        }
                    }
                });
                p.scrollable_elements.push_back({
                    .type=ELEMENT_LABEL,
                    .label={
                        .xpos=0,
                        .ypos=0,
                        .text="Změny:",
                        .textsize=STANDARTPICEHEIGHT,
                        .origin_left=true
                    }
                });
                int index=1;
                for (auto& c:changes){
                    p.scrollable_elements.push_back({
                        .type=ELEMENT_LABEL,
                        .label={
                            .xpos=10,
                            .ypos=index*STANDARTPICEHEIGHT,
                            .text=c,
                            .textsize=STANDARTPICEHEIGHT,
                            .origin_left=true
                        }
                    });
                    index++;
                }
            } else if (pull_state==1){
                p.top_elements.push_back({
                    .type=ELEMENT_LABEL,
                    .label={
                        .xpos=width/2,
                        .ypos=0,
                        .text="Stahování aktualizace"+endstring,
                        .textsize=STANDARTPICEHEIGHT,
                        .origin_left=false
                    }
                });
                progress=GetGitProgress();
                // overall_progress=progress>(int)progress_shift?progress:(int)progress_shift;
                if (progress>(int)progress_shift){
                    progress_shift+=1.0f;
                } else {
                    progress_shift+=(MAX_PROGRESS_SHIFT-progress_shift)/200.0f;
                }
                overall_progress=(int)progress_shift;
                // overall_progress=progress;
                p.scrollable_elements.push_back({
                    .type=ELEMENT_LABEL,
                    .label={
                        .xpos=width/2,
                        .ypos=0,
                        .text=std::to_string(overall_progress)+"%"+" dokončeno",
                        .textsize=STANDARTPICEHEIGHT,
                        .origin_left=false
                    }
                });
                bool success;
                if (IsGitPullFinished(success) && overall_progress>=100){
                    pull_state=2;
                }
            } else if (pull_state==2){
                p.top_elements.push_back({
                    .type=ELEMENT_LABEL,
                    .label={
                        .xpos=width/2,
                        .ypos=0,
                        .text="Aktualizace byla dokončena",
                        .textsize=STANDARTPICEHEIGHT,
                        .origin_left=false
                    }
                });
                p.bottom_elements.push_back({
                    .type=ELEMENT_BUTTON,
                    .button={
                        .texture=createTextTexture(renderer, "Zavřít"),
                        .xpos=width-75,
                        .ypos=0,
                        .function=[](){
                            running=false;
                        }
                    }
                });
            } else if (pull_state==3){
                p.top_elements.push_back({
                    .type=ELEMENT_LABEL,
                    .label={
                        .xpos=width/2,
                        .ypos=0,
                        .text="Nepodařilo se vyhledat aktualizace",
                        .textsize=STANDARTPICEHEIGHT,
                        .origin_left=false
                    }
                });
                p.bottom_elements.push_back({
                    .type=ELEMENT_BUTTON,
                    .button={
                        .texture=createTextTexture(renderer, "Zavřít"),
                        .xpos=width-75,
                        .ypos=0,
                        .function=[](){
                            running=false;
                        }
                    }
                });
                p.scrollable_elements.push_back({
                    .type=ELEMENT_LABEL,
                    .label={
                        .xpos=width/2,
                        .ypos=0,
                        .text=gitError,
                        .textsize=STANDARTPICEHEIGHT,
                        .origin_left=false
                    }
                });
            }

            updatePanel(renderer, &p);
            if (pull_state==1){
                SDL_Rect rect = { 0, height-STANDARTPICEHEIGHT, width, STANDARTPICEHEIGHT };
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &rect);
                rect = { 0, height-STANDARTPICEHEIGHT+STANDARTBORDER, (int)((float)(width)*(float)(overall_progress)/100.0f), STANDARTPICEHEIGHT-STANDARTBORDER*2 };
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                SDL_RenderFillRect(renderer, &rect);
            }




            click_in_meziprostor=false;
            
            if (clic && !clickedOnSomething){click_in_meziprostor=true;}
            SDL_RenderPresent(renderer);
            SDL_Delay(20); // makes memory leak slower
        }
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
    #ifdef _WIN32
        LaunchDetached("./program_win64.exe");
    #else
        LaunchDetached("./program");
    #endif
    return 0;
}