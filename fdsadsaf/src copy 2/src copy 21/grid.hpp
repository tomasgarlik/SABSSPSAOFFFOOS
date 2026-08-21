#include "includes.hpp"

// Pomocné stavové proměnné pro swapování (pamatují si stav mezi snímky)
static int first_x = -1;
static int first_y = -1;
// Pomocná funkce pro zajištění bezpečného prohození
void secure_seat_swap(size_t first_idx, size_t current_idx, int current_x, int current_y) {
    if (first_idx >= project.seats.size()) return;

    if (current_idx != -1 && current_idx < project.seats.size()) {
        // Scénář: Lavice <-> Lavice
        // Prohodíme jejich souřadnice
        int temp_x = project.seats[first_idx].xpos;
        int temp_y = project.seats[first_idx].ypos;
        
        project.seats[first_idx].xpos = project.seats[current_idx].xpos;
        project.seats[first_idx].ypos = project.seats[current_idx].ypos;
        
        project.seats[current_idx].xpos = temp_x;
        project.seats[current_idx].ypos = temp_y;
    } else {
        // Scénář: Lavice <-> Prázdno
        // Přesuneme první lavici na novou pozici
        project.seats[first_idx].xpos = current_x;
        project.seats[first_idx].ypos = current_y;
    }
}
// int pick_student(){
//     gend=-1;
//     rn=true;
//     label l={
//         .text="Vyberte žáka",
//         .textsize=STANDARTPICEHEIGHT,
//         .xpos=width/2,
//         .ypos=0,
//         .origin_left=false
//     };
//     panel p={};
//     p.top_elements.push_back({.type=ELEMENT_LABEL,.label=l});
//     p.bottom_elements.push_back({
//         .type = ELEMENT_BUTTON,
//         .button = {
//             .texture = createTextTexture(renderer,"Zrušit"),
//             .xpos =width-160,
//             .ypos=0,
//             .function = [](){rn=false;}
//         }
//     });



    
//     int index=0;
//     for (auto& s:project.students){
//         if (std::any_of(project.seats.begin(), project.seats.end(), 
//             [index](const seat& s) {
//             return s.student == index;
//             })){
//                 continue;
//             }
//         p.scrollable_elements.push_back({
//             .type = ELEMENT_BUTTON,
//             .button = {
//                 .texture = createTextTexture(renderer,s.name),
//                 .xpos = 0,
//                 .ypos=(int)(p.scrollable_elements.size())*(STANDARTPICEHEIGHT+STANDARTBORDER*2),
//                 .function = [index]() {gend=index;rn=false;}
//             }
//         });
//         index++;
//     }
//     p.w=width;
//     p.h=height;
//     p.x=0;
//     p.y=0;


//     while (rn && running){
//         SDL_RenderClear(renderer);
//         if (clickup){clickup=false;}
//         handle_events();
//         updatePanel(renderer, &p);
//         click_in_meziprostor=false;
//         if (clic && !clickedOnSomething){click_in_meziprostor=true;}
//         SDL_RenderPresent(renderer);
//     }
//     active=false;
//     return gend;
// }
int pick_student(){
    gend = -1;
    rn = true;
    
    label l = {
        .text = "Vyberte žáka",
        .textsize = STANDARTPICEHEIGHT,
        .xpos = width / 2,
        .ypos = 0,
        .origin_left = false
    };
    
    panel p = {};
    p.top_elements.push_back({.type = ELEMENT_LABEL, .label = l});
    p.bottom_elements.push_back({
        .type = ELEMENT_BUTTON,
        .button = {
            .texture = createTextTexture(renderer, "Zrušit"),
            .xpos = width - 160,
            .ypos = 0,
            .function = [](){ rn = false; }
        }
    });

    // OPRAVA LOGIKY: Použijeme klasický for cyklus s i, abychom měli jistotu, 
    // že ID studenta odpovídá jeho reálné pozici v project.students
    for (int i = 0; i < project.students.size(); i++) {
        // Kontrola, zda student s tímto ID (i) už někde nesedí
        if (std::any_of(project.seats.begin(), project.seats.end(), 
            [i](const seat& s) {
                return s.student == i;
            })) {
                continue; // Student už sedí, jdeme na dalšího, ale 'i' se regulérně zvětší
            }
            
        p.scrollable_elements.push_back({
            .type = ELEMENT_BUTTON,
            .button = {
                .texture = createTextTexture(renderer, project.students[i].name),
                .xpos = 0,
                // Výška se počítá podle aktuálního počtu přidaných tlačítek v panelu
                .ypos = (int)(p.scrollable_elements.size()) * (STANDARTPICEHEIGHT + STANDARTBORDER * 2),
                .function = [i]() { gend = i; rn = false; } // Bezpečně předáme reálné ID studenta
            }
        });
    }
    
    p.w = width;
    p.h = height;
    p.x = 0;
    p.y = 0;

    while (rn && running){
        SDL_RenderClear(renderer);
        if (clickup) { clickup = false; }
        handle_events();
        updatePanel(renderer, &p);
        click_in_meziprostor = false;
        if (clic && !clickedOnSomething) { click_in_meziprostor = true; }
        SDL_RenderPresent(renderer);
    }
    
    // ČIŠTĚNÍ TEXTUR (Ochrana před memory leakem)
    // Tady by bylo ideální projet p.scrollable_elements a p.bottom_elements 
    // a zavolat SDL_DestroyTexture na jejich .button.texture, pokud tvůj updatePanel 
    // nebo destruktor panelu neničí textury automaticky.

    active = false; // Vrátíme herní mřížce aktivitu
    return gend;
}
// void render_and_manage_grid() {
//     // Spočítáme výšku buňky na základě gridsize a grid_aspect
//     int grid_height = static_cast<int>(gridsize / grid_aspect);
//     if (grid_height <= 0) grid_height = 1; // Ochrana proti dělení nulou nebo nesmyslům

//     // 1. CHYTRÝ START: Spočítáme, kde přesně začít kreslit první viditelný chlívek.
//     int start_x = grid_sx % gridsize;
//     if (start_x > 0) start_x -= gridsize; 

//     int start_y = TOPPICEHEIGHT + ((grid_sy - TOPPICEHEIGHT) % grid_height);
//     if (start_y > TOPPICEHEIGHT) start_y -= grid_height; 

//     // Procházemies obrazovku v mřížce s plynulým počátkem
//     for (int y = start_y; y < height; y += grid_height) {
//         // Ochrana, abychom nekreslili v horní liště (přetečení kvůli start_y v záporu)
//         if (y + grid_height <= TOPPICEHEIGHT) continue; 

//         for (int x = start_x; x < width; x += gridsize) {
            
//             // 2. Opravený výpočet maticové pozice (xpos, ypos) pro aktuální chlívek
//             int current_xpos = static_cast<int>(std::floor(static_cast<double>(x - grid_sx) / gridsize));
//             int current_ypos = static_cast<int>(std::floor(static_cast<double>(y - grid_sy) / grid_height));

//             // Definujeme obdélník, který se ořeže o horní lištu, pokud do ní zasahuje
//             int draw_y = y;
//             int draw_height = grid_height;
//             if (draw_y < TOPPICEHEIGHT) {
//                 draw_height -= (TOPPICEHEIGHT - draw_y);
//                 draw_y = TOPPICEHEIGHT;
//             }

//             SDL_Rect cellRect = { x, draw_y, gridsize, draw_height };
//             SDL_Rect fullCellRect = { x, y, gridsize, grid_height }; 

//             // 3. Detekce, zda v tomto chlívku zrovna stojí myš
//             bool is_hovered = (mousex >= fullCellRect.x && mousex < fullCellRect.x + fullCellRect.w &&
//                                mousey >= fullCellRect.y && mousey < fullCellRect.y + fullCellRect.h && active && mousey > TOPPICEHEIGHT);

//             // 4. Podíváme se, jestli je tato pozice obsazená
//             auto it = std::find_if(project.seats.begin(), project.seats.end(), 
//                 [current_xpos, current_ypos](const seat& s) {
//                     return s.xpos == current_xpos && s.ypos == current_ypos;
//                 });

//             bool is_occupied = (it != project.seats.end());
            
//             int current_seat_index = -1;
//             if (is_occupied) {
//                 current_seat_index = static_cast<int>(std::distance(project.seats.begin(), it));
//             }

//             // 5. Správa kliknutí (LOGIKA UPDATE)
//             if (is_hovered && clickup && active) {
//                 clickup = false; 
                
//                 // POKUD SWAPUJEME
//                 if (swapping) {
//                     if (!is_first_selected) {
//                         // PRVNÍ KLIKNUTÍ: Zapamatujeme si pozici, ať je tu lavice nebo prázdno
//                         first_x = current_xpos;
//                         first_y = current_ypos;
//                         first_selected_seat_index = current_seat_index; // -1 pokud je tu prázdno
//                         is_first_selected = true;
//                     } else {
//                         // DRUHÉ KLIKNUTÍ (akce):
//                         bool first_was_occupied = (first_selected_seat_index != -1);
//                         bool second_is_occupied = is_occupied;

//                         if (first_was_occupied && second_is_occupied) {
//                             // SCÉNÁŘ A: Lavice <-> Lavice (prohodíme souřadnice)
//                             secure_seat_swap(static_cast<size_t>(first_selected_seat_index), current_seat_index, 0, 0);
//                         } else if (first_was_occupied && !second_is_occupied) {
//                             // SCÉNÁŘ B: Lavice <-> Prázdno (přesuneme první lavici)
//                             secure_seat_swap(static_cast<size_t>(first_selected_seat_index), -1, current_xpos, current_ypos);
//                         } else if (!first_was_occupied && second_is_occupied) {
//                             // SCÉNÁŘ C: Prázdno <-> Lavice (přesuneme druhou lavici na první pozici)
//                             secure_seat_swap(static_cast<size_t>(current_seat_index), -1, first_x, first_y);
//                         }
//                         // SCÉNÁŘ D: Prázdno <-> Prázdno: Nic se neděje, oba vybrali prázdno.

//                         // Úspěšně dokončeno, resetujeme stavy
//                         swapping = false;
//                         is_first_selected = false;
//                         first_selected_seat_index = -1;
//                         first_x = -1;
//                         first_y = -1;
                        
//                         // Zneplatníme aktuální data pro tento frame (pozice prvků se změnily)
//                         is_occupied = false; 
//                         current_seat_index = -1;
//                     }
//                 } 
//                 // KLASICKÉ PŘIDÁVÁNÍ / MAZÁNÍ (pokud není zapnutý swapping)
//                 else {
//                     if (is_occupied) {
//                         project.seats.erase(it);
//                         is_occupied = false;
//                         current_seat_index = -1;
//                     } else {
//                         seat new_seat;
//                         new_seat.xpos = current_xpos;
//                         new_seat.ypos = current_ypos;
//                         new_seat.student = -1;
//                         project.seats.push_back(new_seat);
                        
//                         is_occupied = true;
//                         it = project.seats.end() - 1;
//                         current_seat_index = static_cast<int>(project.seats.size() - 1);
//                     }
//                 }
//             }

//             // Pokud chlívek po oříznutí o TOPPICEHEIGHT nemá žádnou výšku, nemá cenu ho kreslit
//             if (draw_height <= 0) continue;

//             // 6. VYKRESLOVÁNÍ POZADÍ (i prázdné políčko zmodrá)
//             bool is_this_first_selected_pos = (is_first_selected && current_xpos == first_x && current_ypos == first_y);

//             if (is_this_first_selected_pos) {
//                 // První vybrané políčko (lavice i prázdno) zmodrá barvou COLOR_SELECTED
//                 SDL_SetRenderDrawColor(renderer, COLOR_SELECTED, 255);
//                 SDL_RenderFillRect(renderer, &cellRect);
//             } else if (is_hovered && clicked && active) {
//                 SDL_SetRenderDrawColor(renderer, COLOR_CLICKED, 255);
//                 SDL_RenderFillRect(renderer, &cellRect);
//             } else if (is_hovered) {
//                 SDL_SetRenderDrawColor(renderer, COLOR_HOVER, 255);
//                 SDL_RenderFillRect(renderer, &cellRect);
//             } else if (is_occupied) {
//                 SDL_SetRenderDrawColor(renderer, COLOR_BACKGROUND, 255);
//                 SDL_RenderFillRect(renderer, &cellRect);
//             }

//             // Pokud je chlívek obsazený a student v něm je platný, vykreslíme text a border
//             if (is_occupied && current_seat_index != -1 && it->student >= 0 && it->student < project.students.size()) {
//                 // SDL_Texture* textTex = createTextTexture(renderer, project.students[it->student].name);

// std::string fullname =
//     project.students[it->student].name;

// size_t space =
//     fullname.find(' ');

// SDL_Texture* textTex1 = nullptr;
// SDL_Texture* textTex2 = nullptr;

// bool two_lines = false;

// if(space != std::string::npos)
// {
//     two_lines = true;

//     std::string line1 =
//         fullname.substr(0, space);

//     std::string line2 =
//         fullname.substr(space + 1);

//     textTex1 =
//         createTextTexture(renderer, line1);

//     textTex2 =
//         createTextTexture(renderer, line2);
// }
// else
// {
//     textTex1 =
//         createTextTexture(renderer, fullname);
// }
//                 // if (textTex != nullptr) {
//                 //     int padding = STANDARTBORDER + 2; 
                    
//                 //     SDL_Rect textRect = {
//                 //         fullCellRect.x + padding,
//                 //         fullCellRect.y + padding,
//                 //         fullCellRect.w - (2 * padding),
//                 //         fullCellRect.h - (2 * padding)
//                 //     };

//                 //     if (textRect.w > 0 && textRect.h > 0) {
//                 //         SDL_RenderCopy(renderer, textTex, NULL, &textRect);
//                 //     }
//                 //     SDL_DestroyTexture(textTex);
//                 // }
// const float MAX_TEXT_SCALE = (maxtextscale/(float)fontsize)*(float)gridsize; // Maximální škálování textu, aby nepřesáhl velikost buňky

// if (textTex1 != nullptr)
// {
//     int padding = STANDARTBORDER + 2;

//     SDL_Rect availableRect =
//     {
//         fullCellRect.x + padding,
//         fullCellRect.y + padding,
//         fullCellRect.w - (2 * padding),
//         fullCellRect.h - (2 * padding)
//     };

//     if(two_lines)
//     {
//         int tex1_w, tex1_h;
//         int tex2_w, tex2_h;

//         SDL_QueryTexture(textTex1, NULL, NULL,
//                          &tex1_w, &tex1_h);

//         SDL_QueryTexture(textTex2, NULL, NULL,
//                          &tex2_w, &tex2_h);

//         SDL_Rect topHalf =
//         {
//             availableRect.x,
//             availableRect.y,
//             availableRect.w,
//             availableRect.h / 2
//         };

//         SDL_Rect bottomHalf =
//         {
//             availableRect.x,
//             availableRect.y + availableRect.h / 2,
//             availableRect.w,
//             availableRect.h / 2
//         };

//         float scale1 =
//             std::min(
//                 (float)topHalf.w / tex1_w,
//                 (float)topHalf.h / tex1_h);

//         float scale2 =
//             std::min(
//                 (float)bottomHalf.w / tex2_w,
//                 (float)bottomHalf.h / tex2_h);

// scale1 = std::min(scale1, MAX_TEXT_SCALE);
// scale2 = std::min(scale2, MAX_TEXT_SCALE);
//         SDL_Rect dst1;
//         dst1.w = tex1_w * scale1;
//         dst1.h = tex1_h * scale1;
//         dst1.x = topHalf.x +
//                  (topHalf.w - dst1.w) / 2;
//         dst1.y = topHalf.y +
//                  (topHalf.h - dst1.h) / 2;

//         SDL_Rect dst2;
//         dst2.w = tex2_w * scale2;
//         dst2.h = tex2_h * scale2;
//         dst2.x = bottomHalf.x +
//                  (bottomHalf.w - dst2.w) / 2;
//         dst2.y = bottomHalf.y +
//                  (bottomHalf.h - dst2.h) / 2;

//         SDL_RenderCopy(renderer,
//                        textTex1,
//                        NULL,
//                        &dst1);

//         SDL_RenderCopy(renderer,
//                        textTex2,
//                        NULL,
//                        &dst2);

//         SDL_DestroyTexture(textTex2);
//     }
//     else
//     {
//         int tex_w, tex_h;

//         SDL_QueryTexture(textTex1,
//                          NULL,
//                          NULL,
//                          &tex_w,
//                          &tex_h);

//         float scale =
//             std::min(
//                 (float)availableRect.w / tex_w,
//                 (float)availableRect.h / tex_h);

// scale = std::min(scale, MAX_TEXT_SCALE);
//         SDL_Rect dst;

//         dst.w = tex_w * scale;
//         dst.h = tex_h * scale;

//         dst.x = availableRect.x +
//                 (availableRect.w - dst.w) / 2;

//         dst.y = availableRect.y +
//                 (availableRect.h - dst.h) / 2;

//         SDL_RenderCopy(renderer,
//                        textTex1,
//                        NULL,
//                        &dst);
//     }

//     SDL_DestroyTexture(textTex1);
// }
//                 // Vykreslení borderu
//                 SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//                 for (int b = 0; b < STANDARTBORDER; ++b) {
//                     if (draw_height - (2 * b) > 0) {
//                         SDL_Rect borderRect = { x + b, draw_y + b, gridsize - (2 * b), draw_height - (2 * b) };
//                         SDL_RenderDrawRect(renderer, &borderRect);
//                     }
//                 }
//             } else {
//                 // Pro prázdné chlívky nebo neobsazené lavice
//                 if (!is_this_first_selected_pos) {
//                     SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
//                     SDL_RenderDrawRect(renderer, &cellRect);
//                 }
//             }
//         }
//     }
// }


void render_and_manage_grid() {
    // Spočítáme výšku buňky na základě gridsize a grid_aspect
    int grid_height = static_cast<int>(gridsize / grid_aspect);
    if (grid_height <= 0) grid_height = 1; // Ochrana proti dělení nulou nebo nesmyslům

    // 1. CHYTRÝ START: Spočítáme, kde přesně začít kreslit první viditelný chlívek.
    int start_x = grid_sx % gridsize;
    if (start_x > 0) start_x -= gridsize; 

    int start_y = TOPPICEHEIGHT + ((grid_sy - TOPPICEHEIGHT) % grid_height);
    if (start_y > TOPPICEHEIGHT) start_y -= grid_height; 

    // Procházemies obrazovku v mřížce s plynulým počátkem
    for (int y = start_y; y < height; y += grid_height) {
        // Ochrana, abychom nekreslili v horní liště (přetečení kvůli start_y v záporu)
        if (y + grid_height <= TOPPICEHEIGHT) continue; 

        for (int x = start_x; x < width; x += gridsize) {
            
            // 2. Opravený výpočet maticové pozice (xpos, ypos) pro aktuální chlívek
            int current_xpos = static_cast<int>(std::floor(static_cast<double>(x - grid_sx) / gridsize));
            int current_ypos = static_cast<int>(std::floor(static_cast<double>(y - grid_sy) / grid_height));

            // Definujeme obdélník, který se ořeže o horní lištu, pokud do ní zasahuje
            int draw_y = y;
            int draw_height = grid_height;
            if (draw_y < TOPPICEHEIGHT) {
                draw_height -= (TOPPICEHEIGHT - draw_y);
                draw_y = TOPPICEHEIGHT;
            }

            SDL_Rect cellRect = { x, draw_y, gridsize, draw_height };
            SDL_Rect fullCellRect = { x, y, gridsize, grid_height }; 

            // 3. Detekce, zda v tomto chlívku zrovna stojí myš
            bool is_hovered = (mousex >= fullCellRect.x && mousex < fullCellRect.x + fullCellRect.w &&
                               mousey >= fullCellRect.y && mousey < fullCellRect.y + fullCellRect.h && active && mousey > TOPPICEHEIGHT);

            // 4. Podíváme se, jestli je tato pozice obsazená
            auto it = std::find_if(project.seats.begin(), project.seats.end(), 
                [current_xpos, current_ypos](const seat& s) {
                    return s.xpos == current_xpos && s.ypos == current_ypos;
                });

            bool is_occupied = (it != project.seats.end());
            
            int current_seat_index = -1;
            bool is_seat_empty = false;
            SDL_Rect plusButtonRect = { 0, 0, 0, 0 };
            bool is_plus_hovered = false;

            if (is_occupied) {
                current_seat_index = static_cast<int>(std::distance(project.seats.begin(), it));
                
                // Zjistíme, jestli je lavice prázdná (nikdo na ní nesedí)
                if (it->student == -1) {
                    is_seat_empty = true;

                    // Definujeme malý čtvereček vlevo nahoře uvnitř políčka
                    int plus_w = gridsize / 4;
                    int plus_h = grid_height / 4;
                    if (plus_w < 12) plus_w = 12;
                    if (plus_h < 12) plus_h = 12;

                    plusButtonRect.x = fullCellRect.x + STANDARTBORDER + 2;
                    plusButtonRect.y = fullCellRect.y + STANDARTBORDER + 2;
                    plusButtonRect.w = plus_w;
                    plusButtonRect.h = plus_h;

                    // Hlídáme, zda myš stojí přímo na tom malém čtverečku s pluskem
                    is_plus_hovered = (is_hovered && 
                                       mousex >= plusButtonRect.x && mousex < plusButtonRect.x + plusButtonRect.w &&
                                       mousey >= plusButtonRect.y && mousey < plusButtonRect.y + plusButtonRect.h);

                    // POKUD HOVERUJEME PLUSKO, ZRUŠÍME HOVER CELÉHO POLÍČKA
                    if (is_plus_hovered) {
                        is_hovered = false;
                    }
                }
            }

            // 5. Správa kliknutí (LOGIKA UPDATE)
            if (clickup && active) {
                
                // KDYŽ SE KLIKNE NA MALÉ PLUSKO VLEVO NAHOŘE
                if (is_occupied && is_seat_empty && is_plus_hovered) {
                    clickup = false;
                    
                    it->student = pick_student();
                    if (it->student != -1) {
                        is_seat_empty = false;
                    }
                }
                // KDYŽ SE KLIKNE KAMKOLIV JINAM (propadne to přes is_hovered)
                else if (is_hovered) {
                    clickup = false; 
                    
                    // POKUD SWAPUJEME
                    if (swapping) {
                        if (!is_first_selected) {
                            // PRVNÍ KLIKNUTÍ: Zapamatujeme si pozici, ať je tu lavice nebo prázdno
                            first_x = current_xpos;
                            first_y = current_ypos;
                            first_selected_seat_index = current_seat_index; // -1 pokud je tu prázdno
                            is_first_selected = true;
                        } else {
                            // DRUHÉ KLIKNUTÍ (akce):
                            bool first_was_occupied = (first_selected_seat_index != -1);
                            bool second_is_occupied = is_occupied;

                            if (first_was_occupied && second_is_occupied) {
                                // SCÉNÁŘ A: Lavice <-> Lavice (prohodíme souřadnice)
                                secure_seat_swap(static_cast<size_t>(first_selected_seat_index), current_seat_index, 0, 0);
                            } else if (first_was_occupied && !second_is_occupied) {
                                // SCÉNÁŘ B: Lavice <-> Prázdno (přesuneme první lavici)
                                secure_seat_swap(static_cast<size_t>(first_selected_seat_index), -1, current_xpos, current_ypos);
                            } else if (!first_was_occupied && second_is_occupied) {
                                // SCÉNÁŘ C: Prázdno <-> Lavice (přesuneme druhou lavici na první pozici)
                                secure_seat_swap(static_cast<size_t>(current_seat_index), -1, first_x, first_y);
                            }
                            // SCÉNÁŘ D: Prázdno <-> Prázdno: Nic se neděje, oba vybrali prázdno.

                            // Úspěšně dokončeno, resetujeme stavy
                            swapping = false;
                            is_first_selected = false;
                            first_selected_seat_index = -1;
                            first_x = -1;
                            first_y = -1;
                            
                            // Zneplatníme aktuální data pro tento frame (pozice prvků se změnily)
                            is_occupied = false; 
                            current_seat_index = -1;
                            is_seat_empty = false;
                        }
                    } 
                    // KLASICKÉ PŘIDÁVÁNÍ / MAZÁNÍ (pokud není zapnutý swapping)
                    else {
                        if (is_occupied) {
                            project.seats.erase(it);
                            is_occupied = false;
                            current_seat_index = -1;
                            is_seat_empty = false;
                        } else {
                            seat new_seat;
                            new_seat.xpos = current_xpos;
                            new_seat.ypos = current_ypos;
                            new_seat.student = -1;
                            project.seats.push_back(new_seat);
                            
                            is_occupied = true;
                            it = project.seats.end() - 1;
                            current_seat_index = static_cast<int>(project.seats.size() - 1);
                            is_seat_empty = true;
                        }
                    }
                }
            }

            // Pokud chlívek po oříznutí o TOPPICEHEIGHT nemá žádnou výšku, nemá cenu ho kreslit
            if (draw_height <= 0) continue;

            // 6. VYKRESLOVÁNÍ POZADÍ (i prázdné políčko zmodrá)
            bool is_this_first_selected_pos = (is_first_selected && current_xpos == first_x && current_ypos == first_y);

            if (is_this_first_selected_pos) {
                // První vybrané políčko (lavice i prázdno) zmodrá barvou COLOR_SELECTED
                SDL_SetRenderDrawColor(renderer, COLOR_SELECTED, 255);
                SDL_RenderFillRect(renderer, &cellRect);
            } else if (is_hovered && clicked && active) {
                SDL_SetRenderDrawColor(renderer, COLOR_CLICKED, 255);
                SDL_RenderFillRect(renderer, &cellRect);
            } else if (is_hovered) {
                SDL_SetRenderDrawColor(renderer, COLOR_HOVER, 255);
                SDL_RenderFillRect(renderer, &cellRect);
            } else if (is_occupied) {
                SDL_SetRenderDrawColor(renderer, COLOR_BACKGROUND, 255);
                SDL_RenderFillRect(renderer, &cellRect);
            }

            // Pokud je chlívek obsazený a student v něm je platný, vykreslíme text a border
            if (is_occupied && current_seat_index != -1 && it->student >= 0 && it->student < project.students.size()) {
                
                std::string fullname = project.students[it->student].name;
                size_t space = fullname.find(' ');

                SDL_Texture* textTex1 = nullptr;
                SDL_Texture* textTex2 = nullptr;
                bool two_lines = false;

                if(space != std::string::npos) {
                    two_lines = true;
                    std::string line1 = fullname.substr(0, space);
                    std::string line2 = fullname.substr(space + 1);

                    textTex1 = createTextTexture(renderer, line1);
                    textTex2 = createTextTexture(renderer, line2);
                } else {
                    textTex1 = createTextTexture(renderer, fullname);
                }

                const float MAX_TEXT_SCALE = (maxtextscale/(float)fontsize)*(float)gridsize; // Maximální škálování textu, aby nepřesáhl velikost buňky

                if (textTex1 != nullptr) {
                    int padding = STANDARTBORDER + 2;

                    SDL_Rect availableRect = {
                        fullCellRect.x + padding,
                        fullCellRect.y + padding,
                        fullCellRect.w - (2 * padding),
                        fullCellRect.h - (2 * padding)
                    };

                    if(two_lines) {
                        int tex1_w, tex1_h;
                        int tex2_w, tex2_h;

                        SDL_QueryTexture(textTex1, NULL, NULL, &tex1_w, &tex1_h);
                        SDL_QueryTexture(textTex2, NULL, NULL, &tex2_w, &tex2_h);

                        SDL_Rect topHalf = { availableRect.x, availableRect.y, availableRect.w, availableRect.h / 2 };
                        SDL_Rect bottomHalf = { availableRect.x, availableRect.y + availableRect.h / 2, availableRect.w, availableRect.h / 2 };

                        float scale1 = std::min((float)topHalf.w / tex1_w, (float)topHalf.h / tex1_h);
                        float scale2 = std::min((float)bottomHalf.w / tex2_w, (float)bottomHalf.h / tex2_h);

                        scale1 = std::min(scale1, MAX_TEXT_SCALE);
                        scale2 = std::min(scale2, MAX_TEXT_SCALE);
                        
                        SDL_Rect dst1;
                        dst1.w = tex1_w * scale1;
                        dst1.h = tex1_h * scale1;
                        dst1.x = topHalf.x + (topHalf.w - dst1.w) / 2;
                        dst1.y = topHalf.y + (topHalf.h - dst1.h) / 2;

                        SDL_Rect dst2;
                        dst2.w = tex2_w * scale2;
                        dst2.h = tex2_h * scale2;
                        dst2.x = bottomHalf.x + (bottomHalf.w - dst2.w) / 2;
                        dst2.y = bottomHalf.y + (bottomHalf.h - dst2.h) / 2;

                        SDL_RenderCopy(renderer, textTex1, NULL, &dst1);
                        SDL_RenderCopy(renderer, textTex2, NULL, &dst2);
                        SDL_DestroyTexture(textTex2);
                    } else {
                        int tex_w, tex_h;
                        SDL_QueryTexture(textTex1, NULL, NULL, &tex_w, &tex_h);

                        float scale = std::min((float)availableRect.w / tex_w, (float)availableRect.h / tex_h);
                        scale = std::min(scale, MAX_TEXT_SCALE);
                        
                        SDL_Rect dst;
                        dst.w = tex_w * scale;
                        dst.h = tex_h * scale;
                        dst.x = availableRect.x + (availableRect.w - dst.w) / 2;
                        dst.y = availableRect.y + (availableRect.h - dst.h) / 2;

                        SDL_RenderCopy(renderer, textTex1, NULL, &dst);
                    }
                    SDL_DestroyTexture(textTex1);
                }

                // Vykreslení borderu
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                for (int b = 0; b < STANDARTBORDER; ++b) {
                    if (draw_height - (2 * b) > 0) {
                        SDL_Rect borderRect = { x + b, draw_y + b, gridsize - (2 * b), draw_height - (2 * b) };
                        SDL_RenderDrawRect(renderer, &borderRect);
                    }
                }
            } 
            // POKUD JE LAVICE OBSAZENÁ, ALE NIKDO NA NÍ NESEDÍ (student == -1) -> VYKRESLÍME PLUSKO
            else if (is_occupied && is_seat_empty) {
                
                SDL_Rect drawPlusRect = plusButtonRect;
                if (drawPlusRect.y < TOPPICEHEIGHT) {
                    drawPlusRect.h -= (TOPPICEHEIGHT - drawPlusRect.y);
                    drawPlusRect.y = TOPPICEHEIGHT;
                }

                if (drawPlusRect.h > 0) {
                    // Pozadí vnitřního čtverečku s pluskem
                    if (is_plus_hovered) {
                        SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255); // Šedá při hoveru
                    } else {
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Bílá v základu
                    }
                    SDL_RenderFillRect(renderer, &drawPlusRect);
                    
                    // Ohraničení čtverečku
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderDrawRect(renderer, &drawPlusRect);

                    // Textové "+" uvnitř čtverečku
                    SDL_Texture* plusTex = createTextTexture(renderer, "+");
                    if (plusTex != nullptr) {
                        int tex_w, tex_h;
                        SDL_QueryTexture(plusTex, NULL, NULL, &tex_w, &tex_h);

                        float scale = std::min((float)(plusButtonRect.w - 4) / tex_w, (float)(plusButtonRect.h - 4) / tex_h);
                        const float MAX_TEXT_SCALE = (maxtextscale/(float)fontsize)*(float)gridsize;
                        scale = std::min(scale, MAX_TEXT_SCALE);

                        SDL_Rect dst = {
                            static_cast<int>(plusButtonRect.x + (plusButtonRect.w - tex_w * scale) / 2),
                            static_cast<int>(plusButtonRect.y + (plusButtonRect.h - tex_h * scale) / 2),
                            static_cast<int>(tex_w * scale),
                            static_cast<int>(tex_h * scale)
                        };

                        if (dst.y + dst.h > TOPPICEHEIGHT) {
                            SDL_RenderCopy(renderer, plusTex, NULL, &dst);
                        }
                        SDL_DestroyTexture(plusTex);
                    }
                }

                // Vykreslení hlavního vnějšího borderu lavice
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                for (int b = 0; b < STANDARTBORDER; ++b) {
                    if (draw_height - (2 * b) > 0) {
                        SDL_Rect borderRect = { x + b, draw_y + b, gridsize - (2 * b), draw_height - (2 * b) };
                        SDL_RenderDrawRect(renderer, &borderRect);
                    }
                }
            }
            else {
                // Pro prázdné chlívky mřížky (kde není vůbec žádná lavice položená)
                if (!is_this_first_selected_pos) {
                    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
                    SDL_RenderDrawRect(renderer, &cellRect);
                }
            }
        }
    }
}