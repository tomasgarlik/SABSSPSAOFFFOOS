#include "includes.hpp"

// --- REGISTRACE STRUKTUR PRO JSON (Musí být pod definicí tvých structů) ---
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(weights, height, in_front, prefered, close, close_dist, musnt, sit_with_another_gender, yap_groups)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(student, name, height, in_front, prefered, musnt, gender, sit_with_different_gender)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(seat, xpos, ypos, student)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(projectType, students, seats, yapp_groups, header, subheader, comment, filename, guesswork_itterations, weights, faktor_socialni_rovnosti, penalizace_za_nespokojenost)

// Předpokládám tvou globální proměnnou někde v kódu:
// projectType project;

// Deklarace dopředu, aby savefunc věděla o saveasfunc
void saveasfunc();

void savefunc() {
    // Pokud je filename prázdné, chová se to jako "Uložit jako..."
    if (project.filename.empty()) {
        saveasfunc();
        return;
    }

    // Jinak rovnou ukládáme na známou cestu
    std::ofstream soubor(project.filename);
    if (soubor.is_open()) {
        nlohmann::json j = project;
        soubor << j.dump(4); // Odsazení 4 mezery pro čitelnost
        soubor.close();
        std::cout << "Projekt byl rychle ulozen do: " << project.filename << std::endl;
    } else {
        tinyfd_messageBox("Chyba zapisu", "Nepodarilo se zapsat do souboru!", "ok", "error", 1);
    }
}

void saveasfunc() {
    const char* filtry[] = { "*.zsp" };
    const char* cesta = tinyfd_saveFileDialog(
        "Ulozit projekt jako...",
        "bez názvu.zsp",
        1,
        filtry,
        "zsp soubory (*.zsp)"
    );

    // Uživatel dal Storno
    if (!cesta) return;

    std::string filename = cesta;

    // pokud to nekončí na .zsp, přidej to
    if (filename.size() < 5 || filename.substr(filename.size() - 5) != ".zsp") {
        filename += ".zsp";
    }

    project.filename = filename;
    savefunc();
}

void openfunc() {
    const char* filtry[] = { "zsp" };
    const char* cesta = tinyfd_openFileDialog(
        "Otevrit projekt",
        "",
        0,
        NULL,
        "zsp soubory (*.zsp)",
        0
    );

    // Uživatel dal Storno
    if (!cesta) return;

    std::ifstream soubor(cesta);
    if (soubor.is_open()) {
        try {
            nlohmann::json j;
            soubor >> j;
            
            // Mapování JSONu zpět do našeho structu
            project = j.get<projectType>();
            project.filename = cesta; // Zapamatujeme si cestu pro budoucí savefunc()
            
            std::cout << "Projekt uspesne nacten z: " << cesta << std::endl;
        } catch (const nlohmann::json::exception& e) {
            // Bezpečné zachycení chyb, pokud se v tom někdo rýpal textovým editorem a udělal botu
            tinyfd_messageBox("Chyba formatu", "Soubor ma poskozenou strukturu JSON!", "ok", "error", 1);
            std::cerr << "Chyba nlohmann::json: " << e.what() << std::endl;
        }
        soubor.close();
    } else {
        tinyfd_messageBox("Chyba cteni", "Nepodarilo se otevrit soubor!", "ok", "error", 1);
    }
    for (auto& s:project.students){
        s.empty=false;
        s.old_in_front=s.in_front;
    }
    project.score=0.0f;
    //center the shift
    get_avgx();
    grid_sy=-(project.front_ypos*gridsize)+TOPPICEHEIGHT+STANDARTPICEHEIGHT*4;
    grid_sx=-(project.avgx*gridsize+gridsize)+width/2;
    
}
void import_csv(){
    const char* lFilterPatterns[1] = { "*.csv" };

// Otevření dialogu
const char* vybrana_cesta = tinyfd_openFileDialog(
    "Importovat tabulku se studenty (CSV)", // Titulek okna
    "",                                // Výchozí cesta (prázdná = aktuální adresář)
    1,                                 // Počet filtrů v poli
    lFilterPatterns,                   // Pole filtrů
    "CSV soubory (*.csv)",             // Popisek filtru pro uživatele
    0                                  // 0 = vybrat jen jeden soubor, 1 = vícenásobný výběr
);

if (vybrana_cesta != nullptr) {
    // Uživatel soubor vybral, předáme cestu naší funkci
    nacist_studenty_z_csv(vybrana_cesta);
} else {
    // Uživatel dialog zavřel nebo kliknul na Storno
    // Tady nemusíš dělat nic, nebo jen drobný log
}
}
void export_csv() {
    // 1. Definice filtru pro ukládací dialog
    const char* lSaveFilterPatterns[1] = { "*.csv" };

    // 2. Otevření nativního systémového okna pro uložení souboru
    const char* vybrana_cesta = tinyfd_saveFileDialog(
        "Exportovat seznam studentů jako CSV", // Titulek okna
        "bez_názvu.csv",                 // Výchozí název souboru
        1,                                  // Počet filtrů
        lSaveFilterPatterns,                // Pole filtrů
        "CSV soubory (*.csv)"               // Popisek filtru
    );

    // Pokud uživatel kliknul na Storno nebo zavřel okno, ukončíme funkci
    if (vybrana_cesta == nullptr) {
        return;
    }

    // 3. Otevření souboru pro zápis
    std::ofstream file(vybrana_cesta);
    if (!file.is_open()) {
        msgbox("Nepovedlo se vytvorit soubor pro export!");
        return;
    }

    // 4. Zápis hlavičky (kompatibilní s naším importérem)
    file << "ID;Jméno;Výška;Pohlaví;může sedět s zakem jiného pohlaví;chce sedět s;nesmí sedět s;potřeba sedět ve předu\n";

    // 5. Projdeme všechny studenty ve vektoru a zapíšeme je do řádků
    for (size_t i = 0; i < project.students.size(); ++i) {
        const student& s = project.students[i];

        // Pokud je pozice v poli prázdná, přeskočíme ji
        if (s.empty) continue;

        // Zápis základních dat (ID jako index ve vectoru)
        file << i << ";";          
        file << s.name << ";";
        file << s.height << ";";

        // --- POHLAVÍ ---
        if (s.gender >= 0 && s.gender < static_cast<int>(genders.size())) {
            file << genders[s.gender] << ";";
        } else {
            file << "Neznámé;";
        }

        // --- MŮŽE SEDĚT S JINÝM POHLAVÍM ---
        file << (s.sit_with_different_gender ? "ano" : "ne") << ";";

        // --- CHCE SEDĚT S (INDEXY SEZNAMU) ---
        for (size_t p = 0; p < s.prefered.size(); ++p) {
            file << s.prefered[p];
            if (p + 1 < s.prefered.size()) file << ",";
        }
        file << ";";

        // --- NESMÍ SEDĚT S (INDEXY SEZNAMU) ---
        for (size_t m = 0; m < s.musnt.size(); ++m) {
            file << s.musnt[m];
            if (m + 1 < s.musnt.size()) file << ",";
        }
        file << ";";

        // --- POTŘEBA SEDĚT VPŘEDU ---
        if (s.in_front == 1.0f) file << "ano";
        else if (s.in_front == 0.0f) file << "ne";
        else file << s.in_front;

        file << "\n"; // Konec řádku studenta
    }

    file.close();
    // msgbox("Export byl uspesne dokoncen!");
}