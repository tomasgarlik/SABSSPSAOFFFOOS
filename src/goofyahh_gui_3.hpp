#include "includes.hpp"
#include <cmath>
#include <algorithm>

// ============================================================================
//  "Glass" UI theme  (Aero / Aqua vibe)
// ============================================================================

// ---------------------------- Paleta ---------------------------------------
#define GLASS_TOP        238, 247, 255   // Světlounce modré sklo (vršek)
#define GLASS_BOTTOM     206, 228, 246   // Sklo (spodek)
#define GLASS_BORDER      96, 149, 197   // Ocelově modrý obrys
#define GLASS_BORDER_LT  176, 205, 226   // Svetlejší obrys

#define ACCENT_TOP       158, 217, 255   // Aero modrá - světlá
#define ACCENT_BOTTOM     32, 138, 224   // Aero modrá - tmavá
#define ACCENT_HOVER_TOP 190, 230, 255
#define ACCENT_HOVER_BOT   68, 168, 240
#define ACCENT_PRESS_TOP   26, 108, 188
#define ACCENT_PRESS_BOT   14,  72, 142
#define ACCENT_GLOW        90, 180, 250

#define PANEL_TOP        248, 251, 254
#define PANEL_BOTTOM     215, 231, 244

#define TEXT_DARK         26,  38,  52
#define TEXT_MUTED         98, 114, 130
#define WHITE            255, 255, 255
#define SHADOW_SOFT        12,  22,  38

// ---------------------------- Helpery ---------------------------------------

void fillCircle(SDL_Renderer* renderer, int cx, int cy, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a=255){
    if (radius <= 0) return;
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    for (int dy = -radius; dy <= radius; dy++){
        int dx = (int)std::sqrt((double)(radius * radius - dy * dy));
        SDL_RenderDrawLine(renderer, cx - dx, cy + dy, cx + dx, cy + dy);
    }
}

void fillRoundedRect(SDL_Renderer* renderer, int x, int y, int w, int h, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a=255){
    if (w <= 0 || h <= 0) return;
    if (radius > w/2) radius = w/2;
    if (radius > h/2) radius = h/2;
    if (radius < 0) radius = 0;
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_Rect mid = {x + radius, y, w - radius * 2, h};
    SDL_RenderFillRect(renderer, &mid);
    if (radius > 0){
        SDL_Rect side_l = {x, y + radius, radius, h - radius * 2};
        SDL_RenderFillRect(renderer, &side_l);
        SDL_Rect side_r = {x + w - radius, y + radius, radius, h - radius * 2};
        SDL_RenderFillRect(renderer, &side_r);
        fillCircle(renderer, x + radius, y + radius, radius, r, g, b, a);
        fillCircle(renderer, x + w - radius - 1, y + radius, radius, r, g, b, a);
        fillCircle(renderer, x + radius, y + h - radius - 1, radius, r, g, b, a);
        fillCircle(renderer, x + w - radius - 1, y + h - radius - 1, radius, r, g, b, a);
    }
}

void fillGradientRect(SDL_Renderer* renderer, int x, int y, int w, int h, Uint8 r1, Uint8 g1, Uint8 b1, Uint8 r2, Uint8 g2, Uint8 b2, Uint8 a=255){
    if (w <= 0 || h <= 0) return;
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    for (int i = 0; i < h; i++){
        float t = (float)i / (float)(h > 1 ? h - 1 : 1);
        Uint8 r = (Uint8)(r1 + (r2 - r1) * t);
        Uint8 g = (Uint8)(g1 + (g2 - g1) * t);
        Uint8 b = (Uint8)(b1 + (b2 - b1) * t);
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
        SDL_RenderDrawLine(renderer, x, y + i, x + w - 1, y + i);
    }
}

void fillRoundedGradientRect(SDL_Renderer* renderer, int x, int y, int w, int h, int radius, Uint8 r1, Uint8 g1, Uint8 b1, Uint8 r2, Uint8 g2, Uint8 b2, Uint8 a=255){
    if (w <= 0 || h <= 0) return;
    if (radius > w/2) radius = w/2;
    if (radius > h/2) radius = h/2;
    if (radius < 0) radius = 0;
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    for (int i = 0; i < h; i++){
        float t = (float)i / (float)(h > 1 ? h - 1 : 1);
        Uint8 r = (Uint8)(r1 + (r2 - r1) * t);
        Uint8 g = (Uint8)(g1 + (g2 - g1) * t);
        Uint8 b = (Uint8)(b1 + (b2 - b1) * t);
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
        int inset = 0;
        if (radius > 0){
            if (i < radius){
                int dy = radius - i;
                inset = radius - (int)std::sqrt((double)(radius * radius - dy * dy));
            } else if (i >= h - radius){
                int dy = i - (h - radius) + 1;
                inset = radius - (int)std::sqrt((double)(radius * radius - dy * dy));
            }
        }
        SDL_RenderDrawLine(renderer, x + inset, y + i, x + w - inset - 1, y + i);
    }
}

void drawSoftGlow(SDL_Renderer* renderer, int x, int y, int w, int h, int radius, Uint8 r, Uint8 g, Uint8 b){
    for (int i = 4; i >= 1; i -= 1){
        fillRoundedRect(renderer, x - i, y - i, w + i * 2, h + i * 2, radius + i, r, g, b, 25);
    }
}

void drawSoftShadow(SDL_Renderer* renderer, int x, int y, int w, int h, int radius){
    for (int i = 6; i >= 1; i--){
        fillRoundedRect(renderer, x - i / 2, y + i / 2 + 1, w + i, h + i, radius + i / 2, SHADOW_SOFT, 18);
    }
}

void fillTriangleDown(SDL_Renderer* renderer, int x, int y, int size){
    if (size <= 0) return;
    int cx = x + size / 2;
    for (int i = 0; i < size; i++){
        int halfw = (int)((float)(size - 1 - i) / (float)size * (size / 2.0f));
        SDL_RenderDrawLine(renderer, cx - halfw, y + i, cx + halfw, y + i);
    }
}

void fillTriangleUp(SDL_Renderer* renderer, int x, int y, int size){
    if (size <= 0) return;
    int cx = x + size / 2;
    for (int i = 0; i < size; i++){
        int halfw = (int)((float)i / (float)size * (size / 2.0f));
        SDL_RenderDrawLine(renderer, cx - halfw, y + i, cx + halfw, y + i);
    }
}

// ============================================================================
//  BUTTON — Skleněné tlačítko (Přesný rozměr a pozice z Normal GUI)
// ============================================================================
void update3Button(SDL_Renderer* renderer, button* butptr, int xshift=0, int yshift=0) {
    button but = *butptr;
    but.xpos += xshift;
    but.ypos += yshift;

    int w, h;
    if (SDL_QueryTexture(but.texture, NULL, NULL, &w, &h) != 0) { w = 1; h = 1; }

    float aspectRatio = (float)w / (float)h;
    float dynamicWidth = (float)STANDARTPICEHEIGHT * aspectRatio;

    int x1 = pxlenght(but.xpos);
    int y1 = pxlenght(but.ypos);
    int x2 = pxlenght(but.xpos + dynamicWidth);
    int y2 = pxlenght(but.ypos + STANDARTPICEHEIGHT);

    SDL_Rect rect = { x1, y1, x2 - x1, y2 - y1 };
    bool hover = (mousex < x2 && mousex > x1 && mousey > y1 && mousey < y2);
    int radius = std::max(2, rect.h / 4);

    if (hover) {
        if (clicked) { clickedOnSomething = true; }
    }

    // 1. Hover/Active glow
    if (hover || but.force_selected) {
        drawSoftGlow(renderer, rect.x, rect.y, rect.w, rect.h, radius, ACCENT_GLOW);
    }

    // 2. Skleněné ohraničení (Border)
    if (hover || but.force_selected){
    fillRoundedRect(renderer, rect.x, rect.y, rect.w, rect.h, radius, GLASS_BORDER, 255);

    } else {
    fillRoundedRect(renderer, rect.x, rect.y, rect.w, rect.h, radius, GLASS_BORDER_LT, 255);

    }

    // 3. Výplň gradientem
    if (hover && clicked) {
        fillRoundedGradientRect(renderer, rect.x + 1, rect.y + 1, rect.w - 2, rect.h - 2, std::max(1, radius - 1), ACCENT_PRESS_TOP, ACCENT_PRESS_BOT);
    } else if (hover || but.force_selected) {
        fillRoundedGradientRect(renderer, rect.x + 1, rect.y + 1, rect.w - 2, rect.h - 2, std::max(1, radius - 1), ACCENT_HOVER_TOP, ACCENT_HOVER_BOT);
    } else {
        fillRoundedGradientRect(renderer, rect.x + 1, rect.y + 1, rect.w - 2, rect.h - 2, std::max(1, radius - 1), GLASS_TOP, GLASS_BOTTOM);
    }

    // 4. Skleněný odlesk (Gloss)
    fillRoundedRect(renderer, rect.x + 2, rect.y + 2, std::max(0, rect.w - 4), std::max(1, rect.h / 2 - 1), std::max(1, radius - 2), WHITE, (hover || but.force_selected) ? 75 : 120);

    if (hover && clickup) {
        but.function();
        clickedOnSomething = true;
    }

    // 5. Vykreslení textury přesně podle Normal GUI
    displayTex(renderer, but.texture, 
               pxlenght(but.xpos + STANDARTBORDER * 2), 
               pxlenght(but.ypos + STANDARTBORDER), 
               pxlenght(STANDARTPICEHEIGHT - STANDARTBORDER * 2));

    but.xpos -= xshift;
    but.ypos -= yshift;
    *butptr = but;
}

// ============================================================================
//  DROPMENU — Skleněné rozbalovací menu
// ============================================================================
void update3DropMenu(SDL_Renderer* renderer, dropmenu* menuptr, int xshift=0, int yshift=0){
    dropmenu menu = *menuptr;
    menu.xpos += xshift;
    menu.ypos += yshift;

    int topRadius = pxlenght(STANDARTPICEHEIGHT) / 4;

    if (menu.opened){
        SDL_Rect rect0 = { pxlenght(menu.xpos), pxlenght(menu.ypos + STANDARTPICEHEIGHT), pxlenght(menu.width), pxlenght(STANDARTPICEHEIGHT * menu.lenght) };
        int listRadius = pxlenght(STANDARTPICEHEIGHT) / 4;
        drawSoftShadow(renderer, rect0.x, rect0.y, rect0.w, rect0.h, listRadius);
        fillRoundedRect(renderer, rect0.x, rect0.y, rect0.w, rect0.h, listRadius, GLASS_BORDER, 255);
        fillRoundedGradientRect(renderer, rect0.x + 2, rect0.y + 2, rect0.w - 4, rect0.h - 4, std::max(1, listRadius - 2), PANEL_TOP, PANEL_BOTTOM);

        for (int i = 0; i < menu.lenght; i++){
            if (mousex < pxlenght(menu.xpos + menu.width) && mousex > pxlenght(menu.xpos) && mousey < pxlenght(menu.ypos + STANDARTPICEHEIGHT * (i + 2))){
                SDL_Rect rect1 = { pxlenght(menu.xpos + STANDARTBORDER), pxlenght(menu.ypos + STANDARTPICEHEIGHT * (i + 1) + STANDARTBORDER), pxlenght(menu.width - STANDARTBORDER * 2), pxlenght(STANDARTPICEHEIGHT - STANDARTBORDER * 2) };
                int itemRadius = std::min(rect1.h / 2, pxlenght(STANDARTPICEHEIGHT / 4));
                if (clicked) fillRoundedGradientRect(renderer, rect1.x, rect1.y, rect1.w, rect1.h, itemRadius, ACCENT_PRESS_TOP, ACCENT_PRESS_BOT);
                else fillRoundedGradientRect(renderer, rect1.x, rect1.y, rect1.w, rect1.h, itemRadius, ACCENT_HOVER_TOP, ACCENT_HOVER_BOT);
                if (clickup){
                    menu.opened = false;
                    menu.functions[i]();
                    menu.selected = i;
                }
                break;
            }
        }
        for (int i = 0; i < menu.lenght; i++){
            displayTex(renderer, menu.textures[i], pxlenght(menu.xpos + STANDARTBORDER * 2), pxlenght(menu.ypos + STANDARTPICEHEIGHT * (i + 1) + STANDARTBORDER), pxlenght(STANDARTPICEHEIGHT - STANDARTBORDER * 2));
        }
        SDL_SetRenderDrawColor(renderer, GLASS_BORDER_LT, 255);
        for (int i = 0; i < menu.lenght - 1; i++){
            SDL_RenderDrawFatLine(renderer, pxlenght(menu.xpos + STANDARTBORDER), pxlenght(menu.ypos + STANDARTPICEHEIGHT * (i + 2)), pxlenght(menu.xpos + menu.width - STANDARTBORDER), pxlenght(menu.ypos + STANDARTPICEHEIGHT * (i + 2)), pxlenght(1));
        }
        clickedOnSomething = true;
    }

    if (menu.opened && mousex < pxlenght(menu.xpos + menu.width) && mousex > pxlenght(menu.xpos) && mousey < pxlenght(menu.ypos + STANDARTPICEHEIGHT * (menu.lenght + 1))){ menu.opened = true; clickedOnSomething = true; }
    else if (clicked && menu.opened && !(mousex < pxlenght(menu.xpos + menu.width) && mousex > pxlenght(menu.xpos) && mousey < pxlenght(menu.ypos + STANDARTPICEHEIGHT * (menu.lenght + 1)))){ menu.opened = false; clickedOnSomething = true; }

    bool topHover = (mousex < pxlenght(menu.topwidth) + pxlenght(menu.xpos) && mousex > pxlenght(menu.xpos) && mousey > pxlenght(menu.ypos) && mousey < pxlenght(menu.ypos + STANDARTPICEHEIGHT));
    SDL_Rect topRect = { pxlenght(menu.xpos), pxlenght(menu.ypos), pxlenght(menu.topwidth), pxlenght(STANDARTPICEHEIGHT) };
    drawSoftShadow(renderer, topRect.x, topRect.y, topRect.w, topRect.h, topRadius);

    if (topHover){
        bool pressed = clicked;
        fillRoundedRect(renderer, topRect.x, topRect.y, topRect.w, topRect.h, topRadius, GLASS_BORDER, 255);
        if (pressed){
            fillRoundedGradientRect(renderer, topRect.x + 2, topRect.y + 2, topRect.w - 4, topRect.h - 4, std::max(1, topRadius - 2), ACCENT_PRESS_TOP, ACCENT_PRESS_BOT);
            clickedOnSomething = true;
        } else {
            fillRoundedGradientRect(renderer, topRect.x + 2, topRect.y + 2, topRect.w - 4, topRect.h - 4, std::max(1, topRadius - 2), ACCENT_HOVER_TOP, ACCENT_HOVER_BOT);
        }
        if (clickup){
            menu.opened = true;
            clickedOnSomething = true;
        }
    } else {
        fillRoundedRect(renderer, topRect.x, topRect.y, topRect.w, topRect.h, topRadius, GLASS_BORDER_LT, 255);
        fillRoundedGradientRect(renderer, topRect.x + 2, topRect.y + 2, topRect.w - 4, topRect.h - 4, std::max(1, topRadius - 2), GLASS_TOP, GLASS_BOTTOM);
    }
    fillRoundedRect(renderer, topRect.x + 3, topRect.y + 3, topRect.w - 6, topRect.h / 2 - 2, std::max(1, topRadius - 3), WHITE, 55);
    
    SDL_SetRenderDrawColor(renderer, topHover ? WHITE : TEXT_MUTED, 255);
    int chevSize = pxlenght(STANDARTPICEHEIGHT) / 4;
    fillTriangleDown(renderer, topRect.x + topRect.w - chevSize * 2, topRect.y + (topRect.h - chevSize) / 2, chevSize);

    if (menu.titletype == "defined"){
        displayTex(renderer, menu.title, pxlenght(menu.xpos + STANDARTBORDER * 2), pxlenght(menu.ypos + STANDARTBORDER), pxlenght(STANDARTPICEHEIGHT - STANDARTBORDER * 2));
    } else if (menu.titletype == "select"){
        displayTex(renderer, menu.textures[menu.selected], pxlenght(menu.xpos + STANDARTBORDER * 2), pxlenght(menu.ypos + STANDARTBORDER), pxlenght(STANDARTPICEHEIGHT - STANDARTBORDER * 2));
    }
    menu.xpos -= xshift;
    menu.ypos -= yshift;
    *menuptr = menu;
}

// ============================================================================
//  TEXTBOX — Skleněné textové pole
// ============================================================================
void update3TextBox(SDL_Renderer* renderer, textbox* inputboxptr, int xshift=0, int yshift=0){
    textbox inputbox = *inputboxptr;
    inputbox.xpos += xshift;
    inputbox.ypos += yshift;
    std::string finaltext = inputbox.text;
    SDL_Texture* texttex = createTextTexture(renderer, finaltext, inputbox.start_of_selected, inputbox.end_of_selected);
    inputbox.textwidth = getTextureWidth(texttex, inputbox.textsize) + STANDARTBORDER * 2;
    if (inputbox.origin_left){
        inputbox.xpos += inputbox.textwidth / 2;
    }
    SDL_Rect rect = { pxlenght(inputbox.xpos - (getTextureWidth(texttex, inputbox.textsize) + STANDARTBORDER * 2) / 2), pxlenght(inputbox.ypos), pxlenght(getTextureWidth(texttex, inputbox.textsize) + STANDARTBORDER * 2), pxlenght(inputbox.textsize + STANDARTBORDER * 2) };
    int fieldRadius = std::max(3, rect.h / 4);

    if (inputbox.active){
        drawSoftGlow(renderer, rect.x, rect.y, rect.w, rect.h, fieldRadius, ACCENT_GLOW);
        fillRoundedRect(renderer, rect.x, rect.y, rect.w, rect.h, fieldRadius, ACCENT_BOTTOM, 255);
    } else {
        fillRoundedRect(renderer, rect.x, rect.y, rect.w, rect.h, fieldRadius, GLASS_BORDER_LT, 255);
    }
    fillRoundedRect(renderer, rect.x + 2, rect.y + 2, rect.w - 4, rect.h - 4, std::max(1, fieldRadius - 2), WHITE, 255);
    fillGradientRect(renderer, rect.x + 4, rect.y + 3, std::max(0, rect.w - 8), std::max(2, rect.h / 6), SHADOW_SOFT, WHITE, 35);

    if (inputbox.active){
        if (SDL_GetTicks() - inputbox.last_time_cursor_blink > CURSORPERIOD){
            inputbox.cursor_visible = !inputbox.cursor_visible;
            inputbox.last_time_cursor_blink = SDL_GetTicks();
        }
        if (inputbox.cursorpos > stringlen(inputbox.text)) inputbox.cursorpos = stringlen(inputbox.text);
        if (inputbox.cursorpos < 0) inputbox.cursorpos = 0;
        int cursorlenpx = (int)((*(proportions + inputbox.cursorpos)) * (rect.w - STANDARTBORDER)) + STANDARTBORDER;
        if (stringlen(typedChar) != 0){
            if (inputbox.start_of_selected == -1 && inputbox.end_of_selected == -1){
                auto graphemes = utf8_split_graphemes(inputbox.text);
                graphemes.insert(graphemes.begin() + inputbox.cursorpos, typedChar);
                inputbox.text.clear();
                for (const auto& g : graphemes) inputbox.text += g;
                inputbox.cursorpos += stringlen(typedChar);
            } else {
                replace_text_range(&inputbox.text, inputbox.start_of_selected, inputbox.end_of_selected, typedChar);
                inputbox.cursorpos = inputbox.start_of_selected + 1;
                inputbox.start_of_selected = -1;
                inputbox.end_of_selected = -1;
            }
            inputbox.cursor_visible = true;
        }
        if (modkey & KMOD_SHIFT){
            if (specialkey == 'l' && inputbox.cursorpos > 0){
                if (inputbox.start_of_selected == -1 && inputbox.end_of_selected == -1){
                    inputbox.start_of_selected = inputbox.cursorpos - 1;
                    inputbox.end_of_selected = inputbox.cursorpos;
                } else {
                    if (inputbox.cursorpos == inputbox.start_of_selected) inputbox.start_of_selected--;
                    else inputbox.end_of_selected--;
                }
                inputbox.cursorpos--;
                inputbox.cursor_visible = true;
            } else if (specialkey == 'r' && inputbox.cursorpos < stringlen(inputbox.text)){
                if (inputbox.start_of_selected == -1 && inputbox.end_of_selected == -1){
                    inputbox.start_of_selected = inputbox.cursorpos;
                    inputbox.end_of_selected = inputbox.cursorpos + 1;
                } else {
                    if (inputbox.cursorpos == inputbox.start_of_selected) inputbox.start_of_selected++;
                    else inputbox.end_of_selected++;
                }
                inputbox.cursorpos++;
                inputbox.cursor_visible = true;
            } else if (specialkey == 'b'){
                inputbox.text = "";
                inputbox.cursorpos = 0;
                inputbox.cursor_visible = true;
            }
        } else {
            if (specialkey == 'l' && inputbox.cursorpos > 0){
                inputbox.cursorpos -= 1;
                inputbox.start_of_selected = -1;
                inputbox.end_of_selected = -1;
                inputbox.cursor_visible = true;
            } else if (specialkey == 'r' && inputbox.cursorpos < stringlen(inputbox.text)){
                inputbox.cursorpos += 1;
                inputbox.start_of_selected = -1;
                inputbox.end_of_selected = -1;
                inputbox.cursor_visible = true;
            } else if (specialkey == 'b'){
                if (inputbox.start_of_selected == -1 && inputbox.end_of_selected == -1){
                    replace_text_range(&inputbox.text, inputbox.cursorpos - 1, inputbox.cursorpos, "");
                } else {
                    replace_text_range(&inputbox.text, inputbox.start_of_selected, inputbox.end_of_selected, "");
                    inputbox.start_of_selected = -1;
                    inputbox.end_of_selected = -1;
                }
                inputbox.cursorpos--;
                inputbox.cursor_visible = true;
            }
        }
        if (inputbox.cursor_visible){
            SDL_SetRenderDrawColor(renderer, ACCENT_BOTTOM, 255);
            SDL_RenderDrawFatLine(renderer, pxlenght(inputbox.xpos - (getTextureWidth(texttex, inputbox.textsize) + STANDARTBORDER * 2) / 2) + cursorlenpx, pxlenght(inputbox.ypos + STANDARTBORDER), pxlenght(inputbox.xpos - (getTextureWidth(texttex, inputbox.textsize) + STANDARTBORDER * 2) / 2) + cursorlenpx, pxlenght(inputbox.ypos + inputbox.textsize + STANDARTBORDER), pxlenght(3));
        }
    }
    if (clic){
        SDL_Point mousepoint = {mousex, mousey};
        if (SDL_PointInRect(&mousepoint, &rect)){
            if (inputbox.active){
                inputbox.start_of_selected = -1;
                inputbox.end_of_selected = -1;
                int idx = 0;
                for (idx = 0; idx < stringlen(inputbox.text) && mousex - rect.x >= (int)((*(proportions + idx)) * (float)(rect.w - STANDARTBORDER * 2)); idx++){}
                inputbox.cursorpos = idx;
            } else {
                inputbox.start_of_selected = 0;
                inputbox.end_of_selected = stringlen(inputbox.text);
            }
            inputbox.active = true;
            clickedOnSomething = true;
        } else {
            inputbox.active = false;
            inputbox.start_of_selected = -1;
            inputbox.end_of_selected = -1;
        }
    }

    if (inputbox.start_of_selected == inputbox.end_of_selected){
        inputbox.start_of_selected = -1;
        inputbox.end_of_selected = -1;
    }
    displayTex(renderer, texttex, pxlenght(inputbox.xpos + STANDARTBORDER - (getTextureWidth(texttex, inputbox.textsize) + STANDARTBORDER * 2) / 2), pxlenght(inputbox.ypos + STANDARTBORDER), pxlenght(inputbox.textsize));
    SDL_DestroyTexture(texttex);
    inputbox.xpos -= xshift;
    inputbox.ypos -= yshift;
    if (inputbox.origin_left){
        inputbox.xpos -= inputbox.textwidth / 2;
    }
    *inputboxptr = inputbox;
}

// ============================================================================
//  LABEL
// ============================================================================
void update3Label(SDL_Renderer* renderer, label* labelptr, int xshift=0, int yshift=0){
    label inputbox = *labelptr;
    inputbox.xpos += xshift;
    inputbox.ypos += yshift;
    std::string finaltext = inputbox.text;
    SDL_Texture* texttex = createTextTexture(renderer, finaltext);
    inputbox.textwidth = getTextureWidth(texttex, inputbox.textsize) + STANDARTBORDER * 2;
    if (inputbox.origin_left){
        inputbox.xpos += inputbox.textwidth / 2;
    }
    displayTex(renderer, texttex, pxlenght(inputbox.xpos + STANDARTBORDER - (getTextureWidth(texttex, inputbox.textsize) + STANDARTBORDER * 2) / 2), pxlenght(inputbox.ypos + STANDARTBORDER), pxlenght(inputbox.textsize));
    SDL_DestroyTexture(texttex);
    inputbox.xpos -= xshift;
    inputbox.ypos -= yshift;
    if (inputbox.origin_left){
        inputbox.xpos -= inputbox.textwidth / 2;
    }
    *labelptr = inputbox;
}

// ============================================================================
//  CHECKBOX — Zaškrtávací políčko
// ============================================================================
void update3CheckBox(SDL_Renderer* renderer, checkbox* boxptr, int xshift=0, int yshift=0){
    checkbox box = *boxptr;
    box.xpos += xshift;
    box.ypos += yshift;
    box.just_changed = false;

    int boxSize = STANDARTPICEHEIGHT;

    bool hover = (mousex > pxlenght(box.xpos) && mousex < pxlenght(box.xpos + boxSize) && mousey > pxlenght(box.ypos) && mousey < pxlenght(box.ypos + boxSize));
    if (hover && clicked){
        box.checked = !box.checked;
        box.just_changed = true;
        clicked = false;
    }

    int side = (int)(pxlenght(boxSize) * 0.75f);
    int sqx = pxlenght(box.xpos) + (pxlenght(boxSize) - side) / 2;
    int sqy = pxlenght(box.ypos) + (pxlenght(boxSize) - side) / 2;
    int sqRadius = std::max(2, side / 4);

    if (hover){
        drawSoftGlow(renderer, sqx, sqy, side, side, sqRadius, ACCENT_GLOW);
    }

    if (box.checked){
        fillRoundedGradientRect(renderer, sqx, sqy, side, side, sqRadius, ACCENT_HOVER_TOP, ACCENT_HOVER_BOT);
        fillRoundedRect(renderer, sqx + 1, sqy + 1, side - 2, std::max(1, side / 2 - 1), std::max(1, sqRadius - 1), WHITE, 60);
        SDL_SetRenderDrawColor(renderer, WHITE, 255);
        int cx1 = (int)(sqx + side * 0.22f), cy1 = (int)(sqy + side * 0.55f);
        int cx2 = (int)(sqx + side * 0.42f), cy2 = (int)(sqy + side * 0.75f);
        int cx3 = (int)(sqx + side * 0.80f), cy3 = (int)(sqy + side * 0.28f);
        SDL_RenderDrawFatLine(renderer, cx1, cy1, cx2, cy2, pxlenght(2));
        SDL_RenderDrawFatLine(renderer, cx2, cy2, cx3, cy3, pxlenght(2));
    } else {
        fillRoundedRect(renderer, sqx, sqy, side, side, sqRadius, GLASS_BORDER_LT, 255);
        fillRoundedRect(renderer, sqx + 1, sqy + 1, std::max(0, side - 2), std::max(0, side - 2), std::max(1, sqRadius - 1), WHITE, 255);
        fillGradientRect(renderer, sqx + 2, sqy + 2, std::max(0, side - 4), std::max(1, side / 3), SHADOW_SOFT, WHITE, 25);
    }

    // Pozice textu lícuje s update0CheckBox
    displayTex(renderer, box.title, pxlenght(box.xpos + boxSize + STANDARTBORDER), pxlenght(box.ypos), pxlenght(boxSize));

    box.xpos -= xshift;
    box.ypos -= yshift;
    *boxptr = box;
}

// ============================================================================
//  SLIDER — Posuvník
// ============================================================================
void update3Slider(SDL_Renderer* renderer, slider* sliderptr, int xshift=0, int yshift=0) {
    slider inputslider = *sliderptr;
    inputslider.xpos += xshift;
    inputslider.ypos += yshift;

    char text_buf[32];
    int value_text_size = STANDARTPICEHEIGHT - STANDARTBORDER * 2;
    sprintf(text_buf, "%.1f", inputslider.value);
    SDL_Texture* val_tex = createTextTexture(renderer, text_buf);
    int val_w = getTextureWidth(val_tex, value_text_size);
    displayTex(renderer, val_tex, pxlenght(inputslider.xpos), pxlenght(inputslider.ypos + STANDARTBORDER), pxlenght(value_text_size));
    SDL_DestroyTexture(val_tex);

    int slider_data_x = inputslider.xpos + val_w + STANDARTBORDER;
    int slider_data_w = inputslider.width - (val_w + STANDARTBORDER);
    if (slider_data_w < 10) slider_data_w = 10;

    float range = inputslider.max - inputslider.min;
    float percentage = (range > 0.0f) ? ((inputslider.value - inputslider.min) / range) : 0.0f;
    percentage = std::clamp(percentage, 0.0f, 1.0f);

    int track_x = pxlenght(slider_data_x);
    int track_w = pxlenght(slider_data_w);
    int track_h = std::max(4, pxlenght(STANDARTPICEHEIGHT) / 4);
    int track_y = pxlenght(inputslider.ypos + STANDARTBORDER) + (pxlenght(STANDARTPICEHEIGHT - STANDARTBORDER) - track_h) / 2;
    int thumb_r = std::max(4, pxlenght(STANDARTPICEHEIGHT - STANDARTBORDER * 2) / 2);
    int thumb_cx = track_x + (int)(percentage * track_w);
    int thumb_cy = track_y + track_h / 2;

    bool hover = (mousex - thumb_cx) * (mousex - thumb_cx) + (mousey - thumb_cy) * (mousey - thumb_cy) <= thumb_r * thumb_r * 4;
    bool overTrack = mousex >= track_x && mousex <= track_x + track_w && mousey >= track_y - thumb_r && mousey <= track_y + track_h + thumb_r;

    if (clicked) {
        if (hover || overTrack) inputslider.moving = true;
    } else {
        inputslider.moving = false;
    }

    if (inputslider.moving) {
        int local_mx = mousex - track_x;
        float new_pct = (float)local_mx / (float)track_w;
        new_pct = std::clamp(new_pct, 0.0f, 1.0f);
        inputslider.value = inputslider.min + (new_pct * range);
        percentage = new_pct;
        thumb_cx = track_x + (int)(percentage * track_w);
    }

    fillGradientRect(renderer, track_x, track_y, track_w, track_h, GLASS_BORDER_LT, GLASS_BORDER, 255);
    fillGradientRect(renderer, track_x, track_y, std::max(0, thumb_cx - track_x), track_h, ACCENT_BOTTOM, ACCENT_TOP, 255);

    drawSoftShadow(renderer, thumb_cx - thumb_r, thumb_cy - thumb_r, thumb_r * 2, thumb_r * 2, thumb_r);
    if (inputslider.moving) fillCircle(renderer, thumb_cx, thumb_cy, thumb_r, ACCENT_PRESS_TOP);
    else if (hover) fillCircle(renderer, thumb_cx, thumb_cy, thumb_r, ACCENT_HOVER_TOP);
    else fillCircle(renderer, thumb_cx, thumb_cy, thumb_r, WHITE);
    fillCircle(renderer, thumb_cx, thumb_cy, std::max(1, thumb_r - 2), GLASS_BORDER_LT);
    if (!inputslider.moving && !hover) fillCircle(renderer, thumb_cx, thumb_cy, std::max(1, thumb_r - 3), WHITE);

    int small_text_size = STANDARTPICEHEIGHT / 2;
    int small_text_y = inputslider.ypos + STANDARTPICEHEIGHT;
    sprintf(text_buf, "%.1f", inputslider.min);
    SDL_Texture* min_tex = createTextTexture(renderer, text_buf);
    displayTex(renderer, min_tex, pxlenght(slider_data_x), pxlenght(small_text_y), pxlenght(small_text_size));
    SDL_DestroyTexture(min_tex);
    sprintf(text_buf, "%.1f", inputslider.max);
    SDL_Texture* max_tex = createTextTexture(renderer, text_buf);
    int max_w = getTextureWidth(max_tex, small_text_size);
    displayTex(renderer, max_tex, pxlenght(slider_data_x + slider_data_w - max_w), pxlenght(small_text_y), pxlenght(small_text_size));
    SDL_DestroyTexture(max_tex);

    inputslider.xpos -= xshift;
    inputslider.ypos -= yshift;
    *sliderptr = inputslider;
}

// ============================================================================
//  NUMBERINPUTBOX — Číselné pole se šipkami
// ============================================================================
void update3NumberInputBox(SDL_Renderer* renderer, numberinputbox* inputboxptr, int xshift=0, int yshift=0){
    numberinputbox inputbox = *inputboxptr;
    inputbox.xpos += xshift;
    inputbox.ypos += yshift;
    std::string finaltext = inputbox.text;
    SDL_Texture* texttex = createTextTexture(renderer, finaltext, inputbox.start_of_selected, inputbox.end_of_selected);

    SDL_Rect rect = { pxlenght(inputbox.xpos), pxlenght(inputbox.ypos), pxlenght(inputbox.size), pxlenght(STANDARTPICEHEIGHT) };
    int boxRadius = std::max(2, rect.h / 4);

    if (inputbox.active){
        drawSoftGlow(renderer, rect.x, rect.y, rect.w, rect.h, boxRadius, ACCENT_GLOW);
        fillRoundedRect(renderer, rect.x, rect.y, rect.w, rect.h, boxRadius, ACCENT_BOTTOM, 255);
    } else {
        fillRoundedRect(renderer, rect.x, rect.y, rect.w, rect.h, boxRadius, GLASS_BORDER_LT, 255);
    }
    fillRoundedRect(renderer, rect.x + 2, rect.y + 2, rect.w - 4, rect.h - 4, std::max(1, boxRadius - 2), WHITE, 255);

    int btn_w = pxlenght(STANDARTPICEHEIGHT / 2);
    int top_btn_hover = (mousex > pxlenght(inputbox.xpos + inputbox.size) - btn_w && mousex < pxlenght(inputbox.xpos + inputbox.size) && mousey > pxlenght(inputbox.ypos) && mousey < pxlenght(inputbox.ypos + STANDARTPICEHEIGHT / 2));
    int bot_btn_hover = (mousex > pxlenght(inputbox.xpos + inputbox.size) - btn_w && mousex < pxlenght(inputbox.xpos + inputbox.size) && mousey >= pxlenght(inputbox.ypos + STANDARTPICEHEIGHT / 2) && mousey < pxlenght(inputbox.ypos + STANDARTPICEHEIGHT));

    SDL_Rect top_rect = { pxlenght(inputbox.xpos + inputbox.size) - btn_w, pxlenght(inputbox.ypos), btn_w, pxlenght(STANDARTPICEHEIGHT / 2) };
    SDL_Rect bot_rect = { pxlenght(inputbox.xpos + inputbox.size) - btn_w, pxlenght(inputbox.ypos + STANDARTPICEHEIGHT / 2), btn_w, pxlenght(STANDARTPICEHEIGHT / 2) };

    if (top_btn_hover){
        if (clicked) fillRoundedRect(renderer, top_rect.x, top_rect.y + 2, top_rect.w - 2, top_rect.h - 2, std::max(1, boxRadius - 2), ACCENT_PRESS_TOP, 255);
        else fillRoundedRect(renderer, top_rect.x, top_rect.y + 2, top_rect.w - 2, top_rect.h - 2, std::max(1, boxRadius - 2), ACCENT_HOVER_TOP, 255);
    } else {
        fillRoundedRect(renderer, top_rect.x, top_rect.y + 2, top_rect.w - 2, top_rect.h - 2, std::max(1, boxRadius - 2), GLASS_TOP, 255);
    }
    if (bot_btn_hover){
        if (clicked) fillRoundedRect(renderer, bot_rect.x, bot_rect.y, bot_rect.w - 2, bot_rect.h - 2, std::max(1, boxRadius - 2), ACCENT_PRESS_TOP, 255);
        else fillRoundedRect(renderer, bot_rect.x, bot_rect.y, bot_rect.w - 2, bot_rect.h - 2, std::max(1, boxRadius - 2), ACCENT_HOVER_TOP, 255);
    } else {
        fillRoundedRect(renderer, bot_rect.x, bot_rect.y, bot_rect.w - 2, bot_rect.h - 2, std::max(1, boxRadius - 2), GLASS_TOP, 255);
    }

    SDL_SetRenderDrawColor(renderer, TEXT_MUTED, 255);
    int arrowSize = std::max(2, top_rect.h / 2);
    fillTriangleUp(renderer, top_rect.x + (top_rect.w - arrowSize) / 2, top_rect.y + (top_rect.h - arrowSize) / 2, arrowSize);
    fillTriangleDown(renderer, bot_rect.x + (bot_rect.w - arrowSize) / 2, bot_rect.y + (bot_rect.h - arrowSize) / 2, arrowSize);

    if (inputbox.active){
        if (SDL_GetTicks() - inputbox.last_time_cursor_blink > CURSORPERIOD){
            inputbox.cursor_visible = !inputbox.cursor_visible;
            inputbox.last_time_cursor_blink = SDL_GetTicks();
        }
        if (inputbox.cursorpos > stringlen(inputbox.text)) inputbox.cursorpos = stringlen(inputbox.text);
        if (inputbox.cursorpos < 0) inputbox.cursorpos = 0;
        int cursorlenpx = (int)((*(proportions + inputbox.cursorpos)) * (pxlenght((getTextureWidth(texttex, STANDARTPICEHEIGHT - STANDARTBORDER * 2) + STANDARTBORDER * 2)) - STANDARTBORDER)) + STANDARTBORDER;
        if (stringlen(typedChar) != 0 && isNumber(typedChar)){
            if (inputbox.start_of_selected == -1 && inputbox.end_of_selected == -1){
                inputbox.text += typedChar;
                inputbox.cursorpos += stringlen(typedChar);
            } else {
                replace_text_range(&inputbox.text, inputbox.start_of_selected, inputbox.end_of_selected, typedChar);
                inputbox.cursorpos = inputbox.start_of_selected + 1;
                inputbox.start_of_selected = -1;
                inputbox.end_of_selected = -1;
            }
            inputbox.cursor_visible = true;
        }
        if (modkey & KMOD_SHIFT){
            if (specialkey == 'l' && inputbox.cursorpos > 0){
                if (inputbox.start_of_selected == -1 && inputbox.end_of_selected == -1){
                    inputbox.start_of_selected = inputbox.cursorpos - 1;
                    inputbox.end_of_selected = inputbox.cursorpos;
                } else {
                    if (inputbox.cursorpos == inputbox.start_of_selected) inputbox.start_of_selected--;
                    else inputbox.end_of_selected--;
                }
                inputbox.cursorpos--;
                inputbox.cursor_visible = true;
            } else if (specialkey == 'r' && inputbox.cursorpos < stringlen(inputbox.text)){
                if (inputbox.start_of_selected == -1 && inputbox.end_of_selected == -1){
                    inputbox.start_of_selected = inputbox.cursorpos;
                    inputbox.end_of_selected = inputbox.cursorpos + 1;
                } else {
                    if (inputbox.cursorpos == inputbox.start_of_selected) inputbox.start_of_selected++;
                    else inputbox.end_of_selected++;
                }
                inputbox.cursorpos++;
                inputbox.cursor_visible = true;
            } else if (specialkey == 'b'){
                inputbox.text = "";
                inputbox.cursorpos = 0;
                inputbox.cursor_visible = true;
            }
        } else {
            if (specialkey == 'l' && inputbox.cursorpos > 0){
                inputbox.cursorpos -= 1;
                inputbox.start_of_selected = -1;
                inputbox.end_of_selected = -1;
                inputbox.cursor_visible = true;
            } else if (specialkey == 'r' && inputbox.cursorpos < stringlen(inputbox.text)){
                inputbox.cursorpos += 1;
                inputbox.start_of_selected = -1;
                inputbox.end_of_selected = -1;
                inputbox.cursor_visible = true;
            } else if (specialkey == 'b'){
                if (inputbox.start_of_selected == -1 && inputbox.end_of_selected == -1){
                    replace_text_range(&inputbox.text, inputbox.cursorpos - 1, inputbox.cursorpos, "");
                } else {
                    replace_text_range(&inputbox.text, inputbox.start_of_selected, inputbox.end_of_selected, "");
                    inputbox.start_of_selected = -1;
                    inputbox.end_of_selected = -1;
                }
                inputbox.cursorpos--;
                inputbox.cursor_visible = true;
            }
        }
        if (inputbox.cursor_visible){
            SDL_SetRenderDrawColor(renderer, ACCENT_BOTTOM, 255);
            SDL_RenderDrawFatLine(renderer, pxlenght(inputbox.xpos) + cursorlenpx, pxlenght(inputbox.ypos + STANDARTBORDER), pxlenght(inputbox.xpos) + cursorlenpx, pxlenght(inputbox.ypos + STANDARTPICEHEIGHT - STANDARTBORDER * 2 + STANDARTBORDER), pxlenght(3));
        }
    }
    if (clic){
        SDL_Point mousepoint = {mousex, mousey};
        if (SDL_PointInRect(&mousepoint, &rect)){
            if (mousex > pxlenght(inputbox.xpos + inputbox.size - STANDARTPICEHEIGHT / 2)){
                if (mousey > pxlenght(inputbox.ypos + STANDARTPICEHEIGHT / 2)){
                    inputbox.number--;
                    inputbox.text = std::to_string(inputbox.number);
                } else {
                    inputbox.number++;
                    inputbox.text = std::to_string(inputbox.number);
                }
            } else {
                if (inputbox.active){
                    inputbox.start_of_selected = -1;
                    inputbox.end_of_selected = -1;
                    int idx = 0;
                    for (idx = 0; mousex - rect.x >= (int)((*(proportions + idx)) * (float)(pxlenght((getTextureWidth(texttex, STANDARTPICEHEIGHT - STANDARTBORDER * 2) + STANDARTBORDER * 2)) - STANDARTBORDER * 2)); idx++){}
                    inputbox.cursorpos = idx;
                } else {
                    inputbox.start_of_selected = 0;
                    inputbox.end_of_selected = stringlen(inputbox.text);
                }
                inputbox.active = true;
            }
            clickedOnSomething = true;
        } else {
            inputbox.active = false;
            inputbox.start_of_selected = -1;
            inputbox.end_of_selected = -1;
        }
    }

    if (inputbox.start_of_selected == inputbox.end_of_selected){
        inputbox.start_of_selected = -1;
        inputbox.end_of_selected = -1;
    }
    displayTex(renderer, texttex, pxlenght(inputbox.xpos + STANDARTBORDER), pxlenght(inputbox.ypos + STANDARTBORDER), pxlenght(STANDARTPICEHEIGHT - STANDARTBORDER * 2));
    SDL_DestroyTexture(texttex);
    inputbox.number = stringToInt(inputbox.text);
    if (inputbox.number > inputbox.maxnum){ inputbox.number = inputbox.maxnum; inputbox.text = std::to_string(inputbox.maxnum); }
    if (inputbox.number < 0){ inputbox.number = 0; inputbox.text = "0"; }
    inputbox.xpos -= xshift;
    inputbox.ypos -= yshift;
    *inputboxptr = inputbox;
}

// ============================================================================
//  ELEMENT — Dispatcher
// ============================================================================
void update3Element(SDL_Renderer* renderer, element* elementptr, int xshift=0, int yshift=0){
    if (elementptr->type == ELEMENT_BUTTON) {
        update3Button(renderer, &elementptr->button, xshift, yshift);
    } else if (elementptr->type == ELEMENT_DROPMENU) {
        update3DropMenu(renderer, &elementptr->dropmenu, xshift, yshift);
    } else if (elementptr->type == ELEMENT_TEXTBOX) {
        update3TextBox(renderer, &elementptr->textbox, xshift, yshift);
    } else if (elementptr->type == ELEMENT_NUMBERINPUTBOX) {
        update3NumberInputBox(renderer, &elementptr->numberinputbox, xshift, yshift);
    } else if (elementptr->type == ELEMENT_LABEL) {
        update3Label(renderer, &elementptr->label, xshift, yshift);
    } else if (elementptr->type == ELEMENT_CHECKBOX) {
        update3CheckBox(renderer, &elementptr->checkbox, xshift, yshift);
    } else if (elementptr->type == ELEMENT_SLIDER) {
        update3Slider(renderer, &elementptr->slider, xshift, yshift);
    }
}

// ============================================================================
//  PANEL — Aero panel se stínem, oříznutím a oddělenou horní i spodní lištou
// ============================================================================
void update3Panel(SDL_Renderer* renderer, panel* panelptr){
    panel pan = *panelptr;

    int px_x = pxlenght(pan.x);
    int px_y = pxlenght(pan.y);
    int px_w = pxlenght(pan.w);
    int px_h = pxlenght(pan.h);

    int panelRadius = pxlenght(STANDARTPICEHEIGHT) / 3;
    if (panelRadius > px_h / 4) panelRadius = px_h / 4;
    if (panelRadius > px_w / 4) panelRadius = px_w / 4;

    // 1. Měkký vnější stín a pozadí celého panelu
    drawSoftShadow(renderer, px_x, px_y, px_w, px_h, panelRadius);
    fillRoundedRect(renderer, px_x, px_y, px_w, px_h, panelRadius, GLASS_BORDER, 255);
    fillRoundedGradientRect(renderer, px_x + 2, px_y + 2, px_w - 4, px_h - 4, std::max(1, panelRadius - 2), PANEL_TOP, PANEL_BOTTOM);

    // 2. Výpočet výšky horní a spodní sekce
    int top_size = 0;
    for (auto& element : pan.top_elements) {
        if (get_element_pos(&element).y > top_size) {
            top_size = get_element_pos(&element).y;
        }
    }
    int bottom_size = 0;
    for (auto& element : pan.bottom_elements) {
        if (get_element_pos(&element).y > bottom_size) {
            bottom_size = get_element_pos(&element).y;
        }
    }

    int header_h = 0;
    if (!pan.top_elements.empty()) {
        header_h = top_size + STANDARTPICEHEIGHT + STANDARTBORDER * 2;
    }
    int footer_h = 0;
    if (!pan.bottom_elements.empty()) {
        footer_h = bottom_size + STANDARTPICEHEIGHT + STANDARTBORDER * 2;
    }

    // 3. Výpočet skrolovatelné oblasti
    int scrollable_y_start = pan.y + header_h;
    int scrollable_area_h = pan.h - header_h - footer_h;
    if (scrollable_area_h < 0) scrollable_area_h = 0;

    int total_content_height = 0;
    for (auto& e : pan.scrollable_elements) {
        int ey = get_element_pos(&e).y + STANDARTPICEHEIGHT + STANDARTBORDER * 2;
        if (ey > total_content_height) {
            total_content_height = ey;
        }
    }

    int scrollbar_range = total_content_height - scrollable_area_h;
    if (scrollbar_range < 0) scrollbar_range = 0;

    int px_scroll_start = pxlenght(scrollable_y_start);
    int px_scroll_h = pxlenght(scrollable_area_h);

    int scrollbar_h = 1;
    if (scrollbar_range > 0 && total_content_height > 0) {
        scrollbar_h = (px_scroll_h * px_scroll_h) / pxlenght(total_content_height);
        if (scrollbar_h < 15) scrollbar_h = 15;
    }

    int scrollbar_x = pan.x + pan.w - STANDARTBORDER * 2 - STANDARTPICEHEIGHT / 4;
    int px_sb_x = pxlenght(scrollbar_x);
    int px_sb_w = pxlenght(STANDARTPICEHEIGHT / 3);
    if (px_sb_w < 6) px_sb_w = 6;

    int scrollbar_y_px = px_scroll_start;
    if (scrollbar_range > 0) {
        scrollbar_y_px = px_scroll_start + (int)(((float)pan.scrolled / (float)scrollbar_range) * (px_scroll_h - scrollbar_h));
    }

    if (clicked && mousex >= px_sb_x - 5 && mousex <= px_sb_x + px_sb_w + 5 &&
        mousey >= px_scroll_start && mousey <= px_scroll_start + px_scroll_h) {
        if (scrollbar_range > 0 && (px_scroll_h - scrollbar_h) > 0) {
            int click_y_rel = mousey - px_scroll_start - scrollbar_h / 2;
            float pct = (float)click_y_rel / (float)(px_scroll_h - scrollbar_h);
            pct = std::clamp(pct, 0.0f, 1.0f);
            pan.scrolled = (int)(pct * scrollbar_range);
        }
        clickedOnSomething = true;
    }

    // 4. Vykreslení skrolovatelných prvků s OŘEZÁNÍM (SDL Scissor Clip)
    SDL_Rect clipRect = { px_x, px_scroll_start, px_w, px_scroll_h };
    SDL_RenderSetClipRect(renderer, &clipRect);

    for (auto& element : pan.scrollable_elements) {
        update3Element(renderer, &element, pan.x + STANDARTBORDER, pan.y + header_h - pan.scrolled + STANDARTBORDER);
    }

    SDL_RenderSetClipRect(renderer, NULL); // Vypneme clip

    // 5. Horní lišta (Header) s vizuálním oddělovačem
    if (header_h > 0) {
        int px_hdr_h = pxlenght(header_h);
        fillRoundedGradientRect(renderer, px_x + 2, px_y + 2, px_w - 4, px_hdr_h - 2, std::max(1, panelRadius - 2), GLASS_TOP, GLASS_BOTTOM);
        fillGradientRect(renderer, px_x + 3, px_y + 3, px_w - 6, std::max(1, px_hdr_h / 3), WHITE, WHITE, 65);

        // Oddělovací skleněná linka
        SDL_SetRenderDrawColor(renderer, GLASS_BORDER, 255);
        SDL_RenderDrawFatLine(renderer, px_x + 4, px_y + px_hdr_h, px_x + px_w - 4, px_y + px_hdr_h, pxlenght(BORDERLINEFATNESS));
        SDL_SetRenderDrawColor(renderer, WHITE, 180);
        SDL_RenderDrawLine(renderer, px_x + 4, px_y + px_hdr_h + 1, px_x + px_w - 4, px_y + px_hdr_h + 1);

        for (auto& element : pan.top_elements) {
            update3Element(renderer, &element, pan.x + STANDARTBORDER, pan.y + STANDARTBORDER);
        }
    }

    // 6. Spodní lišta (Footer) s vizuálním oddělovačem
    if (footer_h > 0) {
        int px_ftr_y = px_y + px_h - pxlenght(footer_h);
        int px_ftr_h = pxlenght(footer_h);

        fillRoundedGradientRect(renderer, px_x + 2, px_ftr_y, px_w - 4, px_ftr_h - 2, std::max(1, panelRadius - 2), PANEL_BOTTOM, GLASS_BOTTOM);

        // Oddělovací skleněná linka
        SDL_SetRenderDrawColor(renderer, GLASS_BORDER, 255);
        SDL_RenderDrawFatLine(renderer, px_x + 4, px_ftr_y, px_x + px_w - 4, px_ftr_y, pxlenght(BORDERLINEFATNESS));
        SDL_SetRenderDrawColor(renderer, WHITE, 180);
        SDL_RenderDrawLine(renderer, px_x + 4, px_ftr_y + 1, px_x + px_w - 4, px_ftr_y + 1);

        for (auto& element : pan.bottom_elements) {
            update3Element(renderer, &element, pan.x + STANDARTBORDER, pan.y + pan.h - footer_h + STANDARTBORDER);
        }
    }

    // 7. Vykreslení skleněného scrollbaru
    if (scrollbar_range > 0) {
        bool sb_hovered = (mousex >= px_sb_x - 3 && mousex <= px_sb_x + px_sb_w + 3 &&
                           mousey >= scrollbar_y_px && mousey <= scrollbar_y_px + scrollbar_h);
        bool sb_clicked = sb_hovered && clicked;

        fillRoundedRect(renderer, px_sb_x, px_scroll_start, px_sb_w, px_scroll_h, px_sb_w / 2, GLASS_BORDER_LT, 60);

        if (sb_clicked) {
            fillRoundedGradientRect(renderer, px_sb_x, scrollbar_y_px, px_sb_w, scrollbar_h, px_sb_w / 2, ACCENT_PRESS_TOP, ACCENT_PRESS_BOT);
        } else if (sb_hovered) {
            fillRoundedGradientRect(renderer, px_sb_x, scrollbar_y_px, px_sb_w, scrollbar_h, px_sb_w / 2, ACCENT_HOVER_TOP, ACCENT_HOVER_BOT);
        } else {
            fillRoundedGradientRect(renderer, px_sb_x, scrollbar_y_px, px_sb_w, scrollbar_h, px_sb_w / 2, ACCENT_TOP, ACCENT_BOTTOM);
        }
    }

    *panelptr = pan;
}