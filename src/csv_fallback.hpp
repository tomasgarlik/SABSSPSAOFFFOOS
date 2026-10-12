#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <SDL2/SDL.h>
bool end_dotaz=false;
bool yes_no_answer=false;
// Pomocná funkce pro zobrazení SDL dotazu s tlačítky Ano / Ne
bool ask_yes_no(const std::string& title, const std::string& message, const std::string& message2) {
    panel p;
    p.x=width/2-300;
    p.y=height/2-SMALL_PANEL_HEIGHT/2;
    p.w=600;
    p.h=SMALL_PANEL_HEIGHT;
    end_dotaz=false;
    yes_no_answer=false;
    p.top_elements.push_back({
        .type=ELEMENT_LABEL,
        .label={
            .xpos=20,
            .ypos=0,
            .text=title,
            .textsize=STANDARTPICEHEIGHT,
            .origin_left=true
        }
    });
    p.scrollable_elements.push_back({
        .type=ELEMENT_LABEL,
        .label={
            .xpos=0,
            .ypos=0,
            .text=message,
            .textsize=STANDARTPICEHEIGHT,
            .origin_left=true
        }
    });
    p.scrollable_elements.push_back({
        .type=ELEMENT_LABEL,
        .label={
            .xpos=0,
            .ypos=STANDARTPICEHEIGHT,
            .text=message2,
            .textsize=STANDARTPICEHEIGHT,
            .origin_left=true
        }
    });
    p.bottom_elements.push_back({
        .type = ELEMENT_BUTTON,
        .button = {
            .texture = createTextTexture(renderer,"Ano"),
            .xpos = p.w-250,
            .ypos = 0,
            .function = [](){
                end_dotaz=true;
                yes_no_answer=true;
            }
        }
    });
    p.bottom_elements.push_back({
        .type = ELEMENT_BUTTON,
        .button = {
            .texture = createTextTexture(renderer,"Ne"),
            .xpos = p.w-125,
            .ypos = 0,
            .function = [](){
                end_dotaz=true;
                yes_no_answer=false;
            }
        }
    });
    updatePanel(renderer, &p);
    SDL_RenderPresent(renderer);

    // Čekání na odpověď uživatele
    while (!end_dotaz) {
        SDL_RenderClear(renderer);
        p.x=(width/2)-300;
        p.y=(height/2)-(SMALL_PANEL_HEIGHT/2);
        if (clickup){clickup=false;}
        handle_events();
        render_and_manage_grid();
        updatePanel(renderer, &p);
        click_in_meziprostor=false;
        if (clic && !clickedOnSomething){click_in_meziprostor=true;}
        SDL_RenderPresent(renderer);
        SDL_Delay(10); // Malé zpoždění pro snížení zátěže CPU
    }

    clickedOnSomething=false;
    SDL_Delay(200);
    return yes_no_answer;
}

bool nacist_studenty_z_csv_bak(const std::string& csv_path) {
    newfunc();

    std::ifstream file(csv_path);
    if (!file.is_open()) {
        msgbox("Soubor se nepovedlo otevřít nebo neexistuje!");
        return false;
    }

    // Samostatné seznamy klíčových slov pro záhlaví a zápatí
    std::vector<std::string> header_keywords = {
        "jmeno", "name", "student", "id", "vyska", "pohlavi", "seznam", "trida", "prijmeni", "Seznam", "seznam", "žáků", "žáci", "žák", "žákyně"
    };
    std::vector<std::string> footer_keywords = {
        "celkem", "pocet", "total", "strana", "konec", "poznamka", "sum", "soucet", "1/1", "1/2", "2/2", "1/3", "2/3", "3/3"
    };

    std::vector<std::string> lines;
    std::string raw_line;

    // 1. Načtení řádků a vyčištění koncových znaků (středníky, mezerové znaky, \r)
    while (std::getline(file, raw_line)) {
        if (!raw_line.empty() && raw_line.back() == '\r') {
            raw_line.pop_back();
        }

        // Odstranění koncových středníků a čárek
        while (!raw_line.empty() && (raw_line.back() == ';' || raw_line.back() == ',')) {
            raw_line.pop_back();
        }

        // Oříznutí mezer na začátku a konci
        size_t start = raw_line.find_first_not_of(" \t");
        size_t end = raw_line.find_last_not_of(" \t");
        
        if (start != std::string::npos && end != std::string::npos) {
            raw_line = raw_line.substr(start, end - start + 1);
        } else {
            continue; // Prázdný řádek přeskočíme
        }

        lines.push_back(raw_line);
    }
    file.close();

    if (lines.empty()) {
        msgbox("Soubor je prázdný nebo neobsahuje žádná data!");
        return false;
    }

    // 2. Kontrola prvních X položek (záhlaví)
    size_t check_header_count = std::min<size_t>(lines.size(), 3);
    size_t invalid_header_rows = 0;

    for (size_t i = 0; i < check_header_count; ++i) {
        std::string norm = normalize(lines[i]);
        for (const auto& kw : header_keywords) {
            if (norm.find(kw) != std::string::npos) {
                invalid_header_rows = i + 1; // Označíme řádky od začátku až po tento
                break;
            }
        }
    }

    if (invalid_header_rows > 0) {
        std::string msg;
        std::string msg2;
        if (invalid_header_rows==1){
            msg = "První řádek nevypadá jako jméno.";
            msg2="Chcete jej odstranit?";

        } else if (invalid_header_rows==2 || invalid_header_rows==3 || invalid_header_rows==4){
            msg = "První " + std::to_string(invalid_header_rows) + 
                          " řádky nevypadají jako jméno.";
            msg2="Chcete je odstranit?";
        } else {
            msg = "Prvních " + std::to_string(invalid_header_rows) + 
                          " řádků nevypadá jako jméno.";
            msg2="Chcete je odstranit?";
        }
        if (ask_yes_no("Detekce záhlaví", msg, msg2)) {
            lines.erase(lines.begin(), lines.begin() + invalid_header_rows);
        }
    }

    if (lines.empty()) {
        msgbox("Po odstranění záhlaví nezůstala žádná jména!");
        return false;
    }

    // 3. Kontrola posledních Y položek (zápatí)
    size_t check_footer_count = std::min<size_t>(lines.size(), 3);
    size_t remove_footer_from = lines.size();

    for (size_t i = 0; i < check_footer_count; ++i) {
        size_t idx = lines.size() - 1 - i;
        std::string norm = normalize(lines[idx]);
        for (const auto& kw : footer_keywords) {
            if (norm.find(kw) != std::string::npos) {
                remove_footer_from = idx; // Označíme od tohoto řádku do konce
                break;
            }
        }
    }

    if (remove_footer_from < lines.size()) {
        size_t count = lines.size() - remove_footer_from;
        std::string msg = "Poslední(ch) " + std::to_string(count) + 
                          " řádek/řádky nevypadá jako jméno.";
        if (ask_yes_no("Detekce zápatí", msg, "Chcete je odstranit?")) {
            lines.erase(lines.begin() + remove_footer_from, lines.end());
        }
    }

    if (lines.empty()) {
        msgbox("Po odstranění zápatí nezůstala žádná jména!");
        return false;
    }

    // 4. Vložení žáků s výchozími hodnotami do projektového pole
    project.students.clear();
    project.students.reserve(lines.size());

    for (const auto& name : lines) {
        student s = default_student;
        s.name = name;
        s.empty = false;
        
        // Všechny ostatní vlastnosti (height, gender, prefered, musnt, atd.) 
        // automaticky přeberou výchozí stav z default_student.

        project.students.push_back(s);
    }

    return true;
}