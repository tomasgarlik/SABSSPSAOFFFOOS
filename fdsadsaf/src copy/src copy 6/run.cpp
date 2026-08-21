#include "includes.hpp"
#include <thread>
#include <atomic>
#include <regex>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <cstdio>
    #include <cstdlib>
#endif
#include <vector>
#include <sstream>

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

// 1. FUNKCE: Zjistí, zda jsou k dispozici nové aktualizace (vrací true/false)
bool CheckForUpdates(std::string gitPath = "git", std::string branch = "main") {
    // Provedeme fetch na pozadí, abychom zjišťovali stav proti vzdálenému repozitáři
#ifdef _WIN32
    // Přidán parametr --dry-run
    std::string fetchCmd = "\"" + gitPath + "\" fetch --dry-run origin " + branch + " 2>&1";
    std::string logCmd   = "\"" + gitPath + "\" log HEAD..origin/" + branch + " --oneline 2>&1";
#else
    std::string fetchCmd = gitPath + " fetch --dry-run origin " + branch + " 2>&1";
    std::string logCmd   = gitPath + " log HEAD..origin/" + branch + " --oneline 2>&1";
#endif

    ExecCmdSimple(fetchCmd); // Stáhne nejnovější stav ze serveru
    std::string output = ExecCmdSimple(logCmd);

    // Pokud výstup logu není prázdný, existují commity, které v lokální verzi chybí
    return !output.empty();
}

// 2. FUNKCE: Vrátí seznam (vector) všech nových commit zpráv
std::vector<std::string> GetCommitNotes(std::string gitPath = "git", std::string branch = "main") {
    std::vector<std::string> notes;

    // Příkaz vytáhne pouze zprávy nových commitů (bez hashů)
#ifdef _WIN32
    std::string cmd = "\"" + gitPath + "\" log HEAD..origin/" + branch + " --format=%s 2>&1";
#else
    std::string cmd = gitPath + " log HEAD..origin/" + branch + " --format=%s 2>&1";
#endif

    std::string output = ExecCmdSimple(cmd);
    if (output.empty()) return notes;

    // Rozsekání výstupu po jednotlivých řádcích
    std::stringstream ss(output);
    std::string line;
    while (std::getline(ss, line)) {
        // Oříznutí přebytečných znaků odřádkování (\r z Windows)
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (!line.empty()) {
            notes.push_back(line);
        }
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
static void InternalGitPullWorker(std::string gitPath="git") {
    g_Progress = 0;
    g_IsFinished = false;
    g_IsSuccess = false;

    std::regex percentRegex(R"((\d+)%)");
    std::smatch match;

#ifdef _WIN32
    // --- WINDOWS IMPLEMENTACE ---
    std::string cmd = "\"" + gitPath + "\" pull --progress origin main 2>&1";

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
    if (!CreateProcessA(NULL, (LPSTR)cmd.c_str(), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
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
                g_Progress = std::stoi(match[1].str());
            }
        }
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD exitCode;
    GetExitCodeProcess(pi.hProcess, &exitCode);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hRead);

    g_IsSuccess = (exitCode == 0);

#else
    // --- MAC / LINUX IMPLEMENTACE ---
    std::string cmd = gitPath + " pull --progress origin main 2>&1";
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        g_IsFinished = true;
        return;
    }

    char buffer[256];
    std::string currentLine = "";

    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        currentLine = buffer;
        if (std::regex_search(currentLine, match, percentRegex)) {
            g_Progress = std::stoi(match[1].str());
        }
    }

    int returnCode = pclose(pipe);
    g_IsSuccess = (returnCode == 0);
#endif

    // Nastavení 100 % a příznaku dokončení
    if (g_IsSuccess) g_Progress = 100;
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
int pull_state=0;
#ifdef _WIN32
    std::string gitPath = ".\\git\\cmd\\git.exe";
#else
    std::string gitPath = "git";
#endif
int main(int argc, char* argv[]) {
    #include "init.cpp"
    printf("init done\n");
    if (CheckForUpdates(gitPath)) {
        printf("Updates available!\n");
        // Jsou dostupné nové změny!
        std::vector<std::string> changes = GetCommitNotes(gitPath);

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
                            StartGitPull();
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
                p.scrollable_elements.push_back({
                    .type=ELEMENT_LABEL,
                    .label={
                        .xpos=width/2,
                        .ypos=0,
                        .text=std::to_string(progress)+"%"+" dokončeno",
                        .textsize=STANDARTPICEHEIGHT,
                        .origin_left=false
                    }
                });
                bool success;
                if (IsGitPullFinished(success)){
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
            }

            updatePanel(renderer, &p);
            if (pull_state==1){
                SDL_Rect rect = { 0, height-STANDARTPICEHEIGHT, width, STANDARTPICEHEIGHT };
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &rect);
                rect = { 0, height-STANDARTPICEHEIGHT+STANDARTBORDER, (int)((float)(width)*(float)(progress)/100.0f), STANDARTPICEHEIGHT-STANDARTBORDER*2 };
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
    return 0;
}