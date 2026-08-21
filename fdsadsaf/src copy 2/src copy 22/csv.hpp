// #include <iostream>
// #include <fstream>
// #include <sstream>
// #include <vector>
// #include <string>
// #include <algorithm>
// #include <cctype>
// #include "includes.hpp"
// // Brutálně imunní normalizace: lowercase + odstranění mezer + odstranění českých diakritických znamének
// std::string normalize(const std::string& input) {
//     std::string result = "";
//     for (size_t i = 0; i < input.length(); ++i) {
//         unsigned char c = input[i];
        
//         // Detekce a ořezání UTF-8 českých znaků na ASCII lowercase
//         if (c == 0xC3 || c == 0xC4 || c == 0xC5) {
//             if (i + 1 < input.length()) {
//                 unsigned char next = input[i + 1];
//                 i++; // Přeskočíme druhý bajt UTF-8 znaku
//                 if ((c == 0xC3 && next == 0xA1) || (c == 0xC3 && next == 0x81)) result += 'a'; // á / Á
//                 else if ((c == 0xC3 && next == 0xA9) || (c == 0xC3 && next == 0x89) || 
//                          (c == 0xC4 && next == 0x9B) || (c == 0xC4 && next == 0x9A)) result += 'e'; // é / É / ě / Ě
//                 else if ((c == 0xC3 && next == 0xAD) || (c == 0xC3 && next == 0x8D)) result += 'i'; // í / Í
//                 else if ((c == 0xC3 && next == 0xB3) || (c == 0xC3 && next == 0x93)) result += 'o'; // ó / Ó
//                 else if ((c == 0xC3 && next == 0xBA) || (c == 0xC3 && next == 0x9A) ||
//                          (c == 0xC5 && next == 0xAF) || (c == 0xC5 && next == 0xAE)) result += 'u'; // ú / Ú / ů / Ů
//                 else if ((c == 0xC3 && next == 0xBD) || (c == 0xC3 && next == 0x9D)) result += 'y'; // ý / Ý
//                 else if ((c == 0xC4 && next == 0x8D) || (c == 0xC4 && next == 0x8C)) result += 'c'; // č / Č
//                 else if ((c == 0xC4 && next == 0x8F) || (c == 0xC4 && next == 0x8E)) result += 'd'; // ď / Ď
//                 else if ((c == 0xC5 && next == 0x88) || (c == 0xC5 && next == 0x87)) result += 'n'; // ň / Ň
//                 else if ((c == 0xC5 && next == 0x99) || (c == 0xC5 && next == 0x98)) result += 'r'; // ř / Ř
//                 else if ((c == 0xC5 && next == 0xA1) || (c == 0xC5 && next == 0xA0)) result += 's'; // š / Š
//                 else if ((c == 0xC5 && next == 0xA5) || (c == 0xC5 && next == 0xA4)) result += 't'; // ť / Ť
//                 else if ((c == 0xC5 && next == 0xBE) || (c == 0xC5 && next == 0xBD)) result += 'z'; // ž / Ž
//             }
//         } else if (c >= 'A' && c <= 'Z') {
//             result += (c + 32); // Klasický ASCII lowercase
//         } else if (c > 32) { 
//             result += c; // Zachováme čísla a běžné znaky, zahodíme mezery a taby
//         }
//     }
//     return result;
// }

// // Parsování seznamu indexů (např. "1,2,5" nebo " 1 ; 2 ")
// std::vector<int> parse_csv_indices(const std::string& input) {
//     std::vector<int> res;
//     std::string current = "";
//     for (char c : input) {
//         if (std::isdigit(static_cast<unsigned char>(c))) {
//             current += c;
//         } else {
//             if (!current.empty()) {
//                 res.push_back(std::stoi(current));
//                 current = "";
//             }
//         }
//     }
//     if (!current.empty()) {
//         res.push_back(std::stoi(current));
//     }
//     return res;
// }

// // Bezpečné rozdělení řádku CSV s ohledem na případné uvozovky
// std::vector<std::string> split_csv_line(const std::string& line, char delim) {
//     std::vector<std::string> tokens;
//     std::string token = "";
//     bool in_quotes = false;
//     for (char c : line) {
//         if (c == '"') {
//             in_quotes = !in_quotes;
//         } else if (c == delim && !in_quotes) {
//             tokens.push_back(token);
//             token = "";
//         } else {
//             token += c;
//         }
//     }
//     tokens.push_back(token);
//     return tokens;
// }
// void nacist_studenty_z_csv(const std::string& csv_path) {
//     // 1. Reset celého projektu podle zadání
//     newfunc(); 

//     std::ifstream file(csv_path);
//     if (!file.is_open()) {
//         // Pokud nelze soubor vůbec otevřít, je to pasé
//         msgbox("Soubor se nepovedlo otevrit nebo neexistuje!");
//         return;
//     }

//     std::string header_line;
//     if (!std::getline(file, header_line)) {
//         msgbox("Soubor je pojebany! (Je prazdny)");
//         return;
//     }

//     // Automatická detekce oddělovače (Český excel miluje středníky)
//     char delim = ',';
//     if (std::count(header_line.begin(), header_line.end(), ';') > std::count(header_line.begin(), header_line.end(), ',')) {
//         delim = ';';
//     }

//     // Rozsekáme hlavičku na sloupce
//     std::vector<std::string> headers = split_csv_line(header_line, delim);
    
//     // Indexy jednotlivých sloupců v tabulce (-1 znamená, že nebyl nalezen)
//     int idx_id = -1, idx_name = -1, idx_height = -1, idx_gender = -1;
//     int idx_sit_diff = -1, idx_prefered = -1, idx_musnt = -1, idx_in_front = -1;

//     for (size_t i = 0; i < headers.size(); ++i) {
//         std::string h = normalize(headers[i]);
//         if (h == "id") idx_id = i;
//         else if (h == "jmeno") idx_name = i;
//         else if (h == "vyska") idx_height = i;
//         else if (h == "pohlavi") idx_gender = i;
//         else if (h == "muze sedet s zakem jineho pohlavi") idx_sit_diff = i;
//         else if (h == "chce sedet s") idx_prefered = i;
//         else if (h == "nesmi sedet s") idx_musnt = i;
//         else if (h == "potreba sedet ve predu") idx_in_front = i;
//     }

//     // Kontrola kritických sloupců, bez kterých program nemůže fungovat
//     if (idx_id == -1 || idx_name == -1 || idx_height == -1 || idx_gender == -1) {
//         msgbox("Soubor je pokažený! Chybi povinne sloupce (ID, Jméno, Výška nebo Pohlaví).");
//         return;
//     }

//     // Struktura pro dočasné podržení dat, než zjistíme max_id pro alokaci vektoru
//     struct TempStudent { int id; student s; };
//     std::vector<TempStudent> temp_pool;
//     int max_id = -1;

//     std::string line;
//     while (std::getline(file, line)) {
//         if (line.empty()) continue; // Přeskočit prázdné řádky

//         std::vector<std::string> row = split_csv_line(line, delim);
        
//         // Pokud řádek nemá dostatek sloupců, přeskočíme nebo nahlásíme chybu
//         if (row.size() <= static_cast<size_t>(std::max({idx_id, idx_name, idx_height, idx_gender}))) {
//             continue; 
//         }

//         try {
//             int current_id = std::stoi(row[idx_id]);
//             if (current_id < 0) throw std::runtime_error("Zaporne ID");

//             student s = default_student; // Vezmeme výchozí šablonu
//             s.name = row[idx_name];
//             s.height = std::stoi(row[idx_height]);
//             s.empty = false;

//             // --- POHLAVÍ ---
//             std::string csv_gender = normalize(row[idx_gender]);
//             s.gender = 0; // fallback default
//             for (size_t g = 0; g < genders.size(); ++g) {
//                 if (normalize(genders[g]) == csv_gender) {
//                     s.gender = static_cast<int>(g);
//                     break;
//                 }
//             }

//             // --- MŮŽE SEDĚT S JINÝM POHLAVÍM ---
//             if (idx_sit_diff != -1 && idx_sit_diff < static_cast<int>(row.size())) {
//                 s.sit_with_different_gender = (normalize(row[idx_sit_diff]) == "ano");
//             }

//             // --- POTŘEBA SEDĚT VPŘEDU ---
//             if (idx_in_front != -1 && idx_in_front < static_cast<int>(row.size())) {
//                 std::string inf_str = normalize(row[idx_in_front]);
//                 if (inf_str == "ano") s.in_front = 1.0f;
//                 else if (inf_str == "ne") s.in_front = 0.0f;
//                 else {
//                     try { s.in_front = std::stof(row[idx_in_front]); } 
//                     catch (...) { s.in_front = 0.0f; }
//                 }
//             }
//             s.old_in_front = s.in_front;

//             // --- CHCE / NESMÍ SEDĚT S (INDEXY) ---
//             if (idx_prefered != -1 && idx_prefered < static_cast<int>(row.size())) {
//                 s.prefered = parse_csv_indices(row[idx_prefered]);
//             }
//             if (idx_musnt != -1 && idx_musnt < static_cast<int>(row.size())) {
//                 s.musnt = parse_csv_indices(row[idx_musnt]);
//             }

//             if (current_id > max_id) {
//                 max_id = current_id;
//             }

//             temp_pool.push_back({current_id, s});

//         } catch (...) {
//             msgbox("Soubor je pojebany! Nektery radek obsahuje neplatna ciselna data.");
//             return;
//         }
//     }
//     file.close();

//     // 3. Nalití do finálního vektoru project.students podle přesného ID
//     // Jelikož ID odpovídá indexu ve vectoru, musíme pole nafouknout na max_id + 1
//     if (max_id >= 0) {
//         project.students.assign(max_id + 1, default_student);
//         for (const auto& item : temp_pool) {
//             project.students[item.id] = item.s;
//         }
//     } else {
//         msgbox("Soubor je pojebany! Nenacteni zadni validni studenti.");
//     }
// }

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <map> // PŘIDÁNO pro mapování starých ID na nová indexová ID
#include "includes.hpp"

// Tvoje brutální normalizace (ponechána beze změny)
std::string normalize(const std::string& input) {
    std::string result = "";
    for (size_t i = 0; i < input.length(); ++i) {
        unsigned char c = input[i];
        if (c == 0xC3 || c == 0xC4 || c == 0xC5) {
            if (i + 1 < input.length()) {
                unsigned char next = input[i + 1];
                i++;
                if ((c == 0xC3 && next == 0xA1) || (c == 0xC3 && next == 0x81)) result += 'a';
                else if ((c == 0xC3 && next == 0xA9) || (c == 0xC3 && next == 0x89) || 
                         (c == 0xC4 && next == 0x9B) || (c == 0xC4 && next == 0x9A)) result += 'e';
                else if ((c == 0xC3 && next == 0xAD) || (c == 0xC3 && next == 0x8D)) result += 'i';
                else if ((c == 0xC3 && next == 0xB3) || (c == 0xC3 && next == 0x93)) result += 'o';
                else if ((c == 0xC3 && next == 0xBA) || (c == 0xC3 && next == 0x9A) ||
                         (c == 0xC5 && next == 0xAF) || (c == 0xC5 && next == 0xAE)) result += 'u';
                else if ((c == 0xC3 && next == 0xBD) || (c == 0xC3 && next == 0x9D)) result += 'y';
                else if ((c == 0xC4 && next == 0x8D) || (c == 0xC4 && next == 0x8C)) result += 'c';
                else if ((c == 0xC4 && next == 0x8F) || (c == 0xC4 && next == 0x8E)) result += 'd';
                else if ((c == 0xC5 && next == 0x88) || (c == 0xC5 && next == 0x87)) result += 'n';
                else if ((c == 0xC5 && next == 0x99) || (c == 0xC5 && next == 0x98)) result += 'r';
                else if ((c == 0xC5 && next == 0xA1) || (c == 0xC5 && next == 0xA0)) result += 's';
                else if ((c == 0xC5 && next == 0xA5) || (c == 0xC5 && next == 0xA4)) result += 't';
                else if ((c == 0xC5 && next == 0xBE) || (c == 0xC5 && next == 0xBD)) result += 'z';
            }
        } else if (c >= 'A' && c <= 'Z') {
            result += (c + 32);
        } else if (c > 32) { 
            result += c;
        }
    }
    return result;
}

// Parsování seznamu indexů (ponecháno beze změny)
std::vector<int> parse_csv_indices(const std::string& input) {
    std::vector<int> res;
    std::string current = "";
    for (char c : input) {
        if (std::isdigit(static_cast<unsigned char>(c))) {
            current += c;
        } else {
            if (!current.empty()) {
                res.push_back(std::stoi(current));
                current = "";
            }
        }
    }
    if (!current.empty()) {
        res.push_back(std::stoi(current));
    }
    return res;
}

// Bezpečné rozdělení řádku CSV (ponecháno beze změny)
std::vector<std::string> split_csv_line(const std::string& line, char delim) {
    std::vector<std::string> tokens;
    std::string token = "";
    bool in_quotes = false;
    for (char c : line) {
        if (c == '"') {
            in_quotes = !in_quotes;
        } else if (c == delim && !in_quotes) {
            tokens.push_back(token);
            token = "";
        } else {
            token += c;
        }
    }
    tokens.push_back(token);
    return tokens;
}

void nacist_studenty_z_csv(const std::string& csv_path) {
    newfunc(); 

    std::ifstream file(csv_path);
    if (!file.is_open()) {
        msgbox("Soubor se nepovedlo otevrit nebo neexistuje!");
        return;
    }

    std::string header_line;
    if (!std::getline(file, header_line)) {
        msgbox("Soubor je pojebany! (Je prazdny)");
        return;
    }

    char delim = ',';
    if (std::count(header_line.begin(), header_line.end(), ';') > std::count(header_line.begin(), header_line.end(), ',')) {
        delim = ';';
    }

    std::vector<std::string> headers = split_csv_line(header_line, delim);
    
    int idx_id = -1, idx_name = -1, idx_height = -1, idx_gender = -1;
    int idx_sit_diff = -1, idx_prefered = -1, idx_musnt = -1, idx_in_front = -1;

    for (size_t i = 0; i < headers.size(); ++i) {
        std::string h = normalize(headers[i]);
        if (h == "id") idx_id = i;
        else if (h == "jmeno") idx_name = i;
        else if (h == "vyska") idx_height = i;
        else if (h == "pohlavi") idx_gender = i;
        // Ošetření mezer a diakritiky z předchozího kroku
        else if (h == "muzesedetszakemjinehopohlavi") idx_sit_diff = i;
        else if (h == "chcesedets") idx_prefered = i;
        else if (h == "nesmisedets") idx_musnt = i;
        else if (h == "potrebasedetvepredu" || h == "potrebasedetvpredu") idx_in_front = i;
    }

    if (idx_id == -1 || idx_name == -1 || idx_height == -1 || idx_gender == -1) {
        msgbox("Soubor je pokažený! Chybi povinne sloupce (ID, Jméno, Výška nebo Pohlaví).");
        return;
    }

    // Pomocná struktura pro držení surových dat z CSV řádku
    struct RawStudent {
        int original_id;
        student s;
        std::vector<int> raw_prefered;
        std::vector<int> raw_musnt;
    };
    
    std::vector<RawStudent> raw_pool;
    std::map<int, int> old_to_new_id; // Mapa pro převod: [Staré ID z Excelu] -> [Nový index od 0]
    int next_free_index = 0;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::vector<std::string> row = split_csv_line(line, delim);
        if (row.size() <= static_cast<size_t>(std::max({idx_id, idx_name, idx_height, idx_gender}))) {
            continue; 
        }

        try {
            int current_id = std::stoi(row[idx_id]);
            if (current_id < 0) throw std::runtime_error("Zaporne ID");

            student s = default_student;
            s.name = row[idx_name];
            s.height = std::stoi(row[idx_height]);
            s.empty = false;

            // --- POHLAVÍ ---
            std::string csv_gender = normalize(row[idx_gender]);
            s.gender = 0;
            for (size_t g = 0; g < genders.size(); ++g) {
                if (normalize(genders[g]) == csv_gender) {
                    s.gender = static_cast<int>(g);
                    break;
                }
            }

            // --- MŮŽE SEDĚT S JINÝM POHLAVÍM ---
            if (idx_sit_diff != -1 && idx_sit_diff < static_cast<int>(row.size())) {
                s.sit_with_different_gender = (normalize(row[idx_sit_diff]) == "ano");
            }

            // --- POTŘEBA SEDĚT VPŘEDU ---
            if (idx_in_front != -1 && idx_in_front < static_cast<int>(row.size())) {
                std::string inf_str = normalize(row[idx_in_front]);
                if (inf_str == "ano") s.in_front = 1.0f;
                else if (inf_str == "ne") s.in_front = 0.0f;
                else {
                    try { s.in_front = std::stof(row[idx_in_front]); } 
                    catch (...) { s.in_front = 0.0f; }
                }
            }
            s.old_in_front = s.in_front;

            // Dočasně uložíme surové seznamy starých ID, převedeme je až na konci
            std::vector<int> pref, mus;
            if (idx_prefered != -1 && idx_prefered < static_cast<int>(row.size())) {
                pref = parse_csv_indices(row[idx_prefered]);
            }
            if (idx_musnt != -1 && idx_musnt < static_cast<int>(row.size())) {
                mus = parse_csv_indices(row[idx_musnt]);
            }

            // Pokud toto staré ID ještě nemáme v mapě, přiřadíme mu nový čistý index (0, 1, 2...)
            if (old_to_new_id.find(current_id) == old_to_new_id.end()) {
                old_to_new_id[current_id] = next_free_index++;
            }

            raw_pool.push_back({current_id, s, pref, mus});

        } catch (...) {
            msgbox("Soubor je pojebany! Nektery radek obsahuje neplatna ciselna data.");
            return;
        }
    }
    file.close();

    if (raw_pool.empty()) {
        msgbox("Soubor je pojebany! Nenacteni zadni validni studenti.");
        return;
    }

    // 3. Rekódování vazeb (prefered/musnt) a finální nalití do vektoru
    // Pole nafoukneme přesně podle počtu reálně načtených žáků
    project.students.assign(next_free_index, default_student);

    for (auto& item : raw_pool) {
        int novy_id = old_to_new_id[item.original_id];

        // Překlad preferovaných ID
        for (int stare_id : item.raw_prefered) {
            if (old_to_new_id.find(stare_id) != old_to_new_id.end()) {
                item.s.prefered.push_back(old_to_new_id[stare_id]);
            }
        }

        // Překlad zakázaných ID
        for (int stare_id : item.raw_musnt) {
            if (old_to_new_id.find(stare_id) != old_to_new_id.end()) {
                item.s.musnt.push_back(old_to_new_id[stare_id]);
            }
        }

        // Uložíme na správnou novou pozici v poli
        project.students[novy_id] = item.s;
    }
}