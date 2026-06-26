#include "includes.hpp"
void init_export_panel(){
    export_panel.scrollable_elements[1].textbox.text=project.header;
    export_panel.scrollable_elements[3].textbox.text=project.subheader;
}
void export_page_to_png() {
    if (pagetex == nullptr) {
        SDL_Log("Chyba: pagetex je nullptr, neni co exportovat!");
        return;
    }

    int tex_w = 0, tex_h = 0;
    SDL_QueryTexture(pagetex, NULL, NULL, &tex_w, &tex_h);

    // 1. Zůstáváme u 32-bit pro bezpečné stažení z GPU bez rozhození pixelů
    SDL_Surface* surfaceRGBA = SDL_CreateRGBSurfaceWithFormat(0, tex_w, tex_h, 32, SDL_PIXELFORMAT_RGBA32);
    if (!surfaceRGBA) {
        SDL_Log("Chyba tvorby surfacu pro export: %s", SDL_GetError());
        return;
    }

    SDL_Texture* stary_target = SDL_GetRenderTarget(renderer);
    SDL_SetRenderTarget(renderer, pagetex);

    if (SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGBA32, surfaceRGBA->pixels, surfaceRGBA->pitch) != 0) {
        SDL_Log("Chyba pri cteni pixelu z GPU: %s", SDL_GetError());
        SDL_FreeSurface(surfaceRGBA);
        SDL_SetRenderTarget(renderer, stary_target);
        return;
    }

    SDL_SetRenderTarget(renderer, stary_target);

    // 2. KLÍČOVÝ KROK: Vytvoříme druhý, čistě 24-bitový RGB surface bez alfy...
    SDL_Surface* surfaceRGB = SDL_CreateRGBSurfaceWithFormat(0, tex_w, tex_h, 24, SDL_PIXELFORMAT_RGB24);
    if (surfaceRGB) {
        // ...a vestavěnou SDL funkcí do něj překopírujeme data. 
        // SDL automaticky zahodí alpha kanál a správně přepočítá pixely na CPU, takže se nic nerozhodí.
        SDL_BlitSurface(surfaceRGBA, NULL, surfaceRGB, NULL);
    }

    const char* pevny_nazev_souboru = "temp_render.png";
    
    // 3. Ukládáme ten zkonvertovaný RGB surface bez alfy
    if (surfaceRGB && IMG_SavePNG(surfaceRGB, pevny_nazev_souboru) == 0) {
        SDL_Log("Uspesne vyexportovano do kompatibilniho RGB: %s", pevny_nazev_souboru);
    } else {
        SDL_Log("Chyba pri ukladani PNG: %s", IMG_GetError());
    }

    // 4. Vyčištění obou surfaců z paměti
    SDL_FreeSurface(surfaceRGBA);
    if (surfaceRGB) {
        SDL_FreeSurface(surfaceRGB);
    }
}
SDL_Texture* generateProjectTexture(SDL_Renderer* renderer, int target_width, bool flip) {
    // 1. VÝPOČET VELIKOSTI FINÁLNÍ TEXTURY
    int grid_height = static_cast<int>(gridsize / grid_aspect);
    if (grid_height <= 0) grid_height = 1;

    if (project.seats.empty()) {
        // OPAVENO: Přidáno _RGBA32, aby překladač neházel chybu
        SDL_Texture* emptyTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, target_width, target_width);
        return emptyTex;
    }

    // Najdeme extrémy (min/max souřadnice), abychom věděli rozsah projektu
    int min_xpos = project.seats[0].xpos;
    int max_xpos = project.seats[0].xpos;
    int min_ypos = project.seats[0].ypos;
    int max_ypos = project.seats[0].ypos;

    for (const auto& s : project.seats) {
        if (s.xpos < min_xpos) min_xpos = s.xpos;
        if (s.xpos > max_xpos) max_xpos = s.xpos;
        if (s.ypos < min_ypos) min_ypos = s.ypos;
        if (s.ypos > max_ypos) max_ypos = s.ypos;
    }

    int range_x = (max_xpos - min_xpos) + 1;
    int range_y = (max_ypos - min_ypos) + 1;

    // 2. ZJISTÍME, KTERÉ SLOUPCE A ŘÁDKY JSOU OBSAZENÉ
    std::vector<bool> col_occupied(range_x, false);
    std::vector<bool> row_occupied(range_y, false);

    for (const auto& s : project.seats) {
        col_occupied[s.xpos - min_xpos] = true;
        row_occupied[s.ypos - min_ypos] = true;
    }

    // Spočítáme celkovou "váhu" mřížky (obsazený = 1.0, prázdný = 0.5)
    float total_units_x = 0.0f;
    for (bool occ : col_occupied) {
        total_units_x += occ ? 1.0f : 0.5f;
    }

    float total_units_y = 0.0f;
    for (bool occ : row_occupied) {
        total_units_y += occ ? 1.0f : 0.5f;
    }

    // Základní jednotková velikost plné buňky
    float unit_w = static_cast<float>(target_width) / total_units_x;
    float unit_h = unit_w / grid_aspect;

    int target_height = static_cast<int>(total_units_y * unit_h);
    if (target_height <= 0) target_height = 1;

    // 3. PŘEDPOČÍTÁME SI POZICE (OFFSETY) PRO KAŽDÝ INDEX (+1 pro pravý/dolní okraj kvůli slícování bez děr)
    std::vector<float> x_offsets(range_x + 1, 0.0f);
    float current_x = 0.0f;
    for (int i = 0; i < range_x; ++i) {
        x_offsets[i] = current_x;
        current_x += (col_occupied[i] ? 1.0f : 0.5f) * unit_w;
    }
    x_offsets[range_x] = current_x; 

    std::vector<float> y_offsets(range_y + 1, 0.0f);
    float current_y = 0.0f;
    for (int i = 0; i < range_y; ++i) {
        y_offsets[i] = current_y;
        current_y += (row_occupied[i] ? 1.0f : 0.5f) * unit_h;
    }
    y_offsets[range_y] = current_y; 

    // 4. VYTVOŘENÍ TARGET TEXTURY
    SDL_Texture* output_texture = SDL_CreateTexture(
        renderer, 
        SDL_PIXELFORMAT_RGBA32, 
        SDL_TEXTUREACCESS_TARGET, 
        target_width, 
        target_height
    );

    if (!output_texture) {
        SDL_Log("Chyba při vytváření exportní textury: %s", SDL_GetError());
        return nullptr;
    }

    SDL_Texture* prev_target = SDL_GetRenderTarget(renderer);
    SDL_SetRenderTarget(renderer, output_texture);

    // Vyčistíme na čistě bílé pozadí papíru
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 
    SDL_RenderClear(renderer);

    // 5. VYKRESLENÍ OBSAZENÝCH POLÍČEK
    for (const auto& s : project.seats) {
        int idx_x = s.xpos - min_xpos;
        int idx_y = s.ypos - min_ypos;

        // CHYTRÝ VÝPOČET PROTI 1PX DÍRÁM
        SDL_Rect cellRect;
        cellRect.x = static_cast<int>(x_offsets[idx_x]);
        cellRect.y = static_cast<int>(y_offsets[idx_y]);
        cellRect.w = static_cast<int>(x_offsets[idx_x + 1]) - cellRect.x;
        cellRect.h = static_cast<int>(y_offsets[idx_y + 1]) - cellRect.y;

        // OTOČENÍ O 180° (flip): zrcadlíme POZICI buňky přes střed plátna,
        // ale obsah buňky (text, border) se nijak nerotuje, takže text
        // zůstává čitelný a horizontální.
        if (flip) {
            int mirrored_x = target_width  - (cellRect.x + cellRect.w);
            int mirrored_y = target_height - (cellRect.y + cellRect.h);
            cellRect.x = mirrored_x;
            cellRect.y = mirrored_y;
        }

        // A. Pozadí lavice (tvoje světle šedá)
        SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
        SDL_RenderFillRect(renderer, &cellRect);

        // B. Logika textu žáka (tvoje dvouřádková verze)
        if (s.student >= 0 && s.student < project.students.size()) {
            std::string fullname = project.students[s.student].name;
            size_t space = fullname.find(' ');

            SDL_Texture* textTex1 = nullptr;
            SDL_Texture* textTex2 = nullptr;
            bool two_lines = false;

            if (space != std::string::npos) {
                two_lines = true;
                std::string line1 = fullname.substr(0, space);
                std::string line2 = fullname.substr(space + 1);

                textTex1 = createTextTexture(renderer, line1);
                textTex2 = createTextTexture(renderer, line2);
            } else {
                textTex1 = createTextTexture(renderer, fullname);
            }

            // Maximální měřítko textu přepočítané na reálnou šířku buňky v pixelech
            const float MAX_TEXT_SCALE = (maxtextscale/(float)fontsize) * (float)cellRect.w; 
            int padding = STANDARTBORDER + 2;

            SDL_Rect availableRect = {
                cellRect.x + padding,
                cellRect.y + padding,
                cellRect.w - (2 * padding),
                cellRect.h - (2 * padding)
            };

            if (textTex1 != nullptr) {
                if (two_lines) {
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
        }

        // C. Vykreslení obrysu (Borderu) kolem lavice
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Černý border
        for (int b = 0; b < STANDARTBORDER; ++b) {
            if (cellRect.h - (2 * b) > 0 && cellRect.w - (2 * b) > 0) {
                SDL_Rect borderRect = { 
                    cellRect.x + b, 
                    cellRect.y + b, 
                    cellRect.w - (2 * b), 
                    cellRect.h - (2 * b) 
                };
                SDL_RenderDrawRect(renderer, &borderRect);
            }
        }
    }

    // 4. RESET RENDEROVACÍHO CÍLE
    SDL_SetRenderTarget(renderer, prev_target);

    return output_texture;
}
void render_page(bool flip, bool final=false){
    SDL_DestroyTexture(pagetex);
    pagetex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, page_w, page_h);

    // 2. KRESLENÍ PROJEKTU
    SDL_SetRenderTarget(renderer, pagetex);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    


    if (final){
        font=hfont;
        fontsize=300;
    }
    int heading_h=130;
    SDL_Texture* texttex = createTextTexture(renderer, project.header);
    if (getTextureWidth(texttex, heading_h) > page_w - 200) {
        while (getTextureWidth(texttex, heading_h) > page_w - 200 && heading_h > 6){
            heading_h--;
        }
    }
    displayTex(renderer, texttex, page_w/2-(getTextureWidth(texttex, heading_h)/2), 90, heading_h);

    int subheading_h=80;
    texttex = createTextTexture(renderer, project.subheader);
    if (getTextureWidth(texttex, subheading_h) > page_w - 200) {
        while (getTextureWidth(texttex, subheading_h) > page_w - 200 && heading_h > 6){
            subheading_h--;
        }
    }
    displayTex(renderer, texttex, page_w/2-(getTextureWidth(texttex, subheading_h)/2), 110+heading_h, subheading_h);
    SDL_DestroyTexture(texttex);






// --- Tvé výchozí proměnné, které už máš (pro kontext) ---
// renderer, pagetex (hlavní plátno exportu), page_w, page_h, STANDARTBORDER, COLOR_FOREGROUND

// --- VSTUPNÍ TEXTURA S PROJEKTEM ---
// (Tuhle texturu ti vrátila ta minulá funkce, musíme zjistit její reálné rozměry)
int bnd_w = page_w - (2 * 100); // Maximální šířka s nějakým rozumným okrajem od okrajů papíru

SDL_Texture* projektTex = generateProjectTexture(renderer, bnd_w, flip); // Tvoje vygenerovaná textura bez gridu

int proj_w = 0, proj_h = 0;
SDL_QueryTexture(projektTex, NULL, NULL, &proj_w, &proj_h);

// --- DEFINICE BOUNDS (Maximální prostor, kam se to smí vykreslit) ---
int bnd_y = 480; // Y pozice "tam někde" pod nadpisem a čárou
int bnd_h = page_h - bnd_y - 100; // Maximální výška (necháváme 100px rezervu úplně dole)

// --- 1. ODČÍTÁNÍ RÁMEČKU (Aby se textura i s borderem vešla do bounds) ---
int dostupna_w = bnd_w - (2 * STANDARTBORDER);
int dostupna_h = bnd_h - (2 * STANDARTBORDER); // Pokud chceme border i dole, musíme ho započítat

if (dostupna_w < 0) dostupna_w = 0;
if (dostupna_h < 0) dostupna_h = 0;

// --- 2. VÝPOČET SCALE (Zachování Aspect Ratio) ---
int cilova_h = dostupna_h;
int cilova_w = (int)((float)proj_w * ((float)cilova_h / (float)proj_h));

// Kontrola přetečení šířky
if (cilova_w > dostupna_w) {
    cilova_w = dostupna_w;
    cilova_h = (int)((float)proj_h * ((float)cilova_w / (float)proj_w));
}

// --- 3. VYCENTROVÁNÍ NA STŘED (X pozice uprostřed page_w) ---
// Celková šířka včetně rámečků bude: cilova_w + (2 * STANDARTBORDER)
// Chceme, aby byl střed tohoto celku přesně v půlce page_w
int final_x = (page_w - cilova_w) / 2; // Tímto se to krásně vycentruje na střed papíru
int final_y = bnd_y + STANDARTBORDER;  // Posun o horní border dolů

// Obdélník pro samotnou texturu projektu uvnitř rámečku
SDL_Rect destRectTex = { final_x, final_y, cilova_w, cilova_h };

// --- 4. VYKRESLENÍ DO PAGETEX ---
// Nejdřív přepneme renderer na hlavní plátno exportu
SDL_Texture* stary_target = SDL_GetRenderTarget(renderer);
SDL_SetRenderTarget(renderer, pagetex);

// Vykreslíme texturu projektu
SDL_RenderCopy(renderer, projektTex, NULL, &destRectTex);


// Vrátíme renderer do původního stavu
SDL_SetRenderTarget(renderer, stary_target);




    if (final){
        font=lfont;
        fontsize=lfont_size;
    }






    SDL_SetRenderTarget(renderer, NULL); // Vrátíš kreslení na obrazovku

}
void export_pdf(){
    render_page(export_panel.scrollable_elements[4].checkbox.checked,true); // Vykreslíme finální stránku do pagetex

    // export_page_to_png(); // Prozatím jen exportujeme do PNG, PDF by vyžadovalo další knihovnu a implementaci




// 1. Otevření dialogu pro uložení souboru
    const char* filterPatterns[] = { "*.pdf" };
    const char* savePath = tinyfd_saveFileDialog(
        "Exportovat jako PDF",
        "bez názvu.pdf",
        1,
        filterPatterns,
        "PDF Dokument (*.pdf)"
    );

    if (!savePath) return; 

    std::string finalPdfPath(savePath);
    if (finalPdfPath.size() < 4 || finalPdfPath.compare(finalPdfPath.size() - 4, 4, ".pdf") != 0) {
        finalPdfPath += ".pdf";
    }

    // 2. STÁHNUTÍ PIXELŮ Z TEXTURY ROVNOU DO RAM (Zase do RGB24)
    int tex_w = 0, tex_h = 0;
    SDL_QueryTexture(pagetex, NULL, NULL, &tex_w, &tex_h);

    SDL_Surface* surfaceRGBA = SDL_CreateRGBSurfaceWithFormat(0, tex_w, tex_h, 32, SDL_PIXELFORMAT_RGBA32);
    if (!surfaceRGBA) return;

    SDL_Texture* stary_target = SDL_GetRenderTarget(renderer);
    SDL_SetRenderTarget(renderer, pagetex);
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGBA32, surfaceRGBA->pixels, surfaceRGBA->pitch);
    SDL_SetRenderTarget(renderer, stary_target);

    SDL_Surface* surfaceRGB = SDL_CreateRGBSurfaceWithFormat(0, tex_w, tex_h, 24, SDL_PIXELFORMAT_RGB24);
    if (!surfaceRGB) {
        SDL_FreeSurface(surfaceRGBA);
        return;
    }
    SDL_BlitSurface(surfaceRGBA, NULL, surfaceRGB, NULL);

    // 3. VYTVOŘENÍ PDF
    float page_width = PDF_A4_WIDTH;
    float page_height = PDF_A4_HEIGHT;

    struct pdf_doc *pdf = pdf_create(page_width, page_height, NULL);
    if (!pdf) {
        SDL_FreeSurface(surfaceRGBA);
        SDL_FreeSurface(surfaceRGB);
        return;
    }

    struct pdf_object *page = pdf_append_page(pdf);

    float margin = 20.0f;
    float img_w = page_width - (2 * margin);
    float img_h = page_height - (2 * margin);

    // 4. GENIÁLNÍ KROK: Místo souboru posíláme přímo pole pixelů z paměti (surfaceRGB->pixels)
    // Parametry: pdf, stránka, x, y, šířka, výška, ukazatel_na_data, šířka_pixelů, výška_pixelů
    int pdf_result = pdf_add_rgb24(pdf, page, margin, margin, img_w, img_h, 
                               (uint8_t*)surfaceRGB->pixels, tex_w, tex_h);
    if (pdf_result != 0) {
        pdf_destroy(pdf);
        SDL_FreeSurface(surfaceRGBA);
        SDL_FreeSurface(surfaceRGB);
        tinyfd_messageBox("Chyba", "pdfgen selhal i při přímém zápisu z paměti.", "ok", "error", 1);
        return;
    }

    // 5. ULOŽENÍ A ÚKLID
    pdf_save(pdf, finalPdfPath.c_str());
    pdf_destroy(pdf);

    SDL_FreeSurface(surfaceRGBA);
    SDL_FreeSurface(surfaceRGB);


    export_opened=false;


}