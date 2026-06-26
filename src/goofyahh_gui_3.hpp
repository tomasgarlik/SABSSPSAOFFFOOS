#include "includes.hpp"

// Explicitní definice přesných Raspi-Config barev (Klasická TUI paleta)
#define RASPI_RED_SELECT    168, 0, 0        // Typický červený výběr aktivní položky
#define RASPI_RED_CLICKED   120, 0, 0        // Tmavší červená při kliknutí
#define RASPI_GREY_BOX      210, 210, 210    // Šedé tělo hlavního okna a panelů
#define RASPI_GREY_BUTTON   192, 192, 192    // Standardní šedá pro neaktivní prvky
#define RASPI_TEXT_BLACK    0, 0, 0          // Černý text, osy a ohraničení
#define RASPI_WHITE         255, 255, 255    // Bílé pozadí vstupů a bílý text na červené
#define RASPI_SHADOW_GREY   80, 80, 80       // TUI pseudo-3D stín pod okny

// Pomocná interní funkce pro vykreslení jednoho retro textového znaku / symbolu
void drawTUIChar(SDL_Renderer* renderer, const char* character, int x, int y, int size) {
    SDL_Texture* charTex = createTextTexture(renderer, character);
    displayTex(renderer, charTex, x, y, size);
    SDL_DestroyTexture(charTex);
}

void update3Button(SDL_Renderer* renderer, button* butptr, int xshift=0, int yshift=0) {
    button but = *butptr;
    but.xpos += xshift;
    but.ypos += yshift;
    
    int w, h;
    if (SDL_QueryTexture(but.texture, NULL, NULL, &w, &h) != 0) { w = 1; h = 1; }

    float aspectRatio = (float)w / (float)h;
    float dynamicWidth = (float)STANDARTPICEHEIGHT * aspectRatio;

    // Pro raspi-config přidáme k šířce prostor na boční zobáčky < >
    int extra_space = STANDARTPICEHEIGHT;
    int x1 = pxlenght(but.xpos);
    int y1 = pxlenght(but.ypos);
    int x2 = pxlenght(but.xpos + dynamicWidth + extra_space);
    int y2 = pxlenght(but.ypos + STANDARTPICEHEIGHT);

    SDL_Rect rect = { x1, y1, x2 - x1, y2 - y1 };
    bool hover = (mousex < x2 && mousex > x1 && mousey > y1 && mousey < y2);

    if (hover) {
        if (clicked) {
            SDL_SetRenderDrawColor(renderer, RASPI_RED_CLICKED, 255);
            clickedOnSomething = true;
        } else {
            SDL_SetRenderDrawColor(renderer, RASPI_RED_SELECT, 255); // Aktivní položka zčervená
        }
        SDL_RenderFillRect(renderer, &rect);

        if (clickup) {
            but.function();
            clickedOnSomething = true;
        }
    } else if (but.force_selected) {
        SDL_SetRenderDrawColor(renderer, RASPI_RED_SELECT, 255);
        SDL_RenderFillRect(renderer, &rect);
    }

    // Vykreslení textu a specifických TUI zobáčků < > při výběru
    if (hover || but.force_selected) {
        int char_s = pxlenght(STANDARTPICEHEIGHT - STANDARTBORDER * 2);
        drawTUIChar(renderer, "<", rect.x + pxlenght(STANDARTBORDER), rect.y + pxlenght(STANDARTBORDER), char_s);
        displayTex(renderer, but.texture, rect.x + pxlenght(STANDARTPICEHEIGHT / 2), rect.y + pxlenght(STANDARTBORDER), char_s);
        drawTUIChar(renderer, ">", rect.x + rect.w - pxlenght(STANDARTPICEHEIGHT / 2), rect.y + pxlenght(STANDARTBORDER), char_s);
    } else {
        // Neaktivní tlačítko nemá ohraničení ani zobáčky, splývá jako pouhý text s panelem
        displayTex(renderer, but.texture, rect.x + pxlenght(STANDARTPICEHEIGHT / 2), rect.y + pxlenght(STANDARTBORDER), pxlenght(STANDARTPICEHEIGHT - STANDARTBORDER * 2));
    }

    but.xpos -= xshift;
    but.ypos -= yshift;
    *butptr = but;
}

void update3DropMenu(SDL_Renderer* renderer, dropmenu* menuptr, int xshift=0, int yshift=0){
    dropmenu menu=*menuptr;
    menu.xpos+=xshift;
    menu.ypos+=yshift;
    
    if (menu.opened){
        SDL_Rect rect={pxlenght(menu.xpos),pxlenght(menu.ypos),pxlenght(menu.topwidth), pxlenght(STANDARTPICEHEIGHT)};
        SDL_SetRenderDrawColor(renderer, RASPI_RED_SELECT, 255);
        SDL_RenderFillRect(renderer, &rect);
        
        SDL_Rect rect0={pxlenght(menu.xpos),pxlenght(menu.ypos+STANDARTPICEHEIGHT),pxlenght(menu.width), pxlenght(STANDARTPICEHEIGHT*menu.lenght)};
        SDL_SetRenderDrawColor(renderer, RASPI_GREY_BOX, 255);
        SDL_RenderFillRect(renderer, &rect0);
        
        for (int i=0;i<menu.lenght;i++){
            if (mousex<pxlenght(menu.xpos+menu.width) && mousex>pxlenght(menu.xpos) && mousey<pxlenght(menu.ypos+STANDARTPICEHEIGHT*(i+2))){
                SDL_Rect rect1={pxlenght(menu.xpos+STANDARTBORDER),pxlenght(menu.ypos+STANDARTPICEHEIGHT*(i+1)+STANDARTBORDER),pxlenght(menu.width-STANDARTBORDER*2), pxlenght(STANDARTPICEHEIGHT-STANDARTBORDER*2)};
                SDL_SetRenderDrawColor(renderer, RASPI_RED_SELECT, 255);
                if (clicked){SDL_SetRenderDrawColor(renderer, RASPI_RED_CLICKED, 255);}
                SDL_RenderFillRect(renderer, &rect1);
                if (clickup){
                    menu.opened=false;
                    menu.functions[i]();
                    menu.selected=i;
                }
                break;
            }
        }
        for (int i=0;i<menu.lenght;i++){
            displayTex(renderer, menu.textures[i], pxlenght(menu.xpos+STANDARTBORDER*2), pxlenght(menu.ypos+STANDARTPICEHEIGHT*(i+1)+STANDARTBORDER), pxlenght(STANDARTPICEHEIGHT-STANDARTBORDER*2));
        }
        SDL_SetRenderDrawColor(renderer, RASPI_TEXT_BLACK, 255);
        for (int i=0;i<menu.lenght-1;i++){
            SDL_RenderDrawFatLine(renderer, pxlenght(menu.xpos+STANDARTBORDER), pxlenght(menu.ypos+STANDARTPICEHEIGHT*(i+2)), pxlenght(menu.xpos+menu.width-STANDARTBORDER), pxlenght(menu.ypos+STANDARTPICEHEIGHT*(i+2)), pxlenght(BORDERLINEFATNESS));
        }
        // Ostré TUI okraje rozbaleného dropdownu
        SDL_RenderDrawFatLine(renderer, pxlenght(menu.xpos), pxlenght(menu.ypos+STANDARTPICEHEIGHT), pxlenght(menu.xpos+menu.width), pxlenght(menu.ypos+STANDARTPICEHEIGHT), pxlenght(BORDERLINEFATNESS));
        SDL_RenderDrawFatLine(renderer, pxlenght(menu.xpos), pxlenght(menu.ypos+STANDARTPICEHEIGHT*(menu.lenght+1)), pxlenght(menu.xpos+menu.width), pxlenght(menu.ypos+STANDARTPICEHEIGHT*(menu.lenght+1)), pxlenght(BORDERLINEFATNESS));
        SDL_RenderDrawFatLine(renderer, pxlenght(menu.xpos), pxlenght(menu.ypos+STANDARTPICEHEIGHT), pxlenght(menu.xpos), pxlenght(menu.ypos+STANDARTPICEHEIGHT*(menu.lenght+1)), pxlenght(BORDERLINEFATNESS));
        SDL_RenderDrawFatLine(renderer, pxlenght(menu.xpos+menu.width), pxlenght(menu.ypos+STANDARTPICEHEIGHT), pxlenght(menu.xpos+menu.width), pxlenght(menu.ypos+STANDARTPICEHEIGHT*(menu.lenght+1)), pxlenght(BORDERLINEFATNESS));
        clickedOnSomething=true;
    }
    
    SDL_SetRenderDrawColor(renderer, RASPI_TEXT_BLACK, 255);
    SDL_RenderDrawFatLine(renderer, pxlenght(menu.xpos), pxlenght(menu.ypos), pxlenght(menu.xpos+menu.topwidth), pxlenght(menu.ypos), pxlenght(BORDERLINEFATNESS));
    SDL_RenderDrawFatLine(renderer, pxlenght(menu.xpos), pxlenght(menu.ypos+STANDARTPICEHEIGHT), pxlenght(menu.xpos+menu.topwidth), pxlenght(menu.ypos+STANDARTPICEHEIGHT), pxlenght(BORDERLINEFATNESS));
    SDL_RenderDrawFatLine(renderer, pxlenght(menu.xpos), pxlenght(menu.ypos), pxlenght(menu.xpos), pxlenght(menu.ypos+STANDARTPICEHEIGHT), pxlenght(BORDERLINEFATNESS));
    SDL_RenderDrawFatLine(renderer, pxlenght(menu.xpos+menu.topwidth), pxlenght(menu.ypos), pxlenght(menu.xpos+menu.topwidth), pxlenght(menu.ypos+STANDARTPICEHEIGHT), pxlenght(BORDERLINEFATNESS));
    
    if (menu.opened && mousex<pxlenght(menu.xpos+menu.width) && mousex>pxlenght(menu.xpos) && mousey<pxlenght(menu.ypos+STANDARTPICEHEIGHT*(menu.lenght+1))){menu.opened=true;clickedOnSomething=true;}
    else if (clicked && menu.opened && !(mousex<pxlenght(menu.xpos+menu.width) && mousex>pxlenght(menu.xpos) && mousey<pxlenght(menu.ypos+STANDARTPICEHEIGHT*(menu.lenght+1)))){menu.opened=false;clickedOnSomething=true;}
    
    if (mousex<pxlenght(menu.topwidth)+pxlenght(menu.xpos) && mousex>pxlenght(menu.xpos) && mousey>pxlenght(menu.ypos) && mousey<pxlenght(menu.ypos+STANDARTPICEHEIGHT)){
        SDL_Rect rect={pxlenght(menu.xpos),pxlenght(menu.ypos),pxlenght(menu.topwidth), pxlenght(STANDARTPICEHEIGHT)};
        if (clicked){SDL_SetRenderDrawColor(renderer, RASPI_RED_CLICKED, 255);clickedOnSomething=true;}
        else {SDL_SetRenderDrawColor(renderer, RASPI_RED_SELECT, 255);}
        SDL_RenderFillRect(renderer, &rect);
        if (clickup){
            menu.opened=true;
            clickedOnSomething=true;
        }
    } else {
        SDL_Rect rect={pxlenght(menu.xpos),pxlenght(menu.ypos),pxlenght(menu.topwidth), pxlenght(STANDARTPICEHEIGHT)};
        SDL_SetRenderDrawColor(renderer, RASPI_GREY_BOX, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
    if (menu.titletype=="defined"){
        displayTex(renderer,menu.title,pxlenght(menu.xpos+STANDARTBORDER*2),pxlenght(menu.ypos+STANDARTBORDER),pxlenght(STANDARTPICEHEIGHT-STANDARTBORDER*2));
    } else if (menu.titletype=="select"){
        displayTex(renderer,menu.textures[menu.selected],pxlenght(menu.xpos+STANDARTBORDER*2),pxlenght(menu.ypos+STANDARTBORDER),pxlenght(STANDARTPICEHEIGHT-STANDARTBORDER*2));
    }
    menu.xpos-=xshift;
    menu.ypos-=yshift;
    *menuptr=menu;
}

void update3TextBox(SDL_Renderer* renderer, textbox* inputboxptr, int xshift=0, int yshift=0){
    textbox inputbox=*inputboxptr;
    inputbox.xpos+=xshift;
    inputbox.ypos+=yshift;
    std::string finaltext=inputbox.text;
    SDL_Texture* texttex=createTextTexture(renderer,finaltext, inputbox.start_of_selected, inputbox.end_of_selected);
    inputbox.textwidth=getTextureWidth(texttex,inputbox.textsize)+STANDARTBORDER*2;
    if (inputbox.origin_left){
        inputbox.xpos+=inputbox.textwidth/2;
    }
    SDL_Rect rect={pxlenght(inputbox.xpos-(getTextureWidth(texttex,inputbox.textsize)+STANDARTBORDER*2)/2), pxlenght(inputbox.ypos), pxlenght((getTextureWidth(texttex,inputbox.textsize)+STANDARTBORDER*2)), pxlenght(inputbox.textsize+STANDARTBORDER*2)};
    
    if (inputbox.active){
        if (SDL_GetTicks()-inputbox.last_time_cursor_blink>CURSORPERIOD){
            inputbox.cursor_visible=!inputbox.cursor_visible;
            inputbox.last_time_cursor_blink=SDL_GetTicks();
        }
        SDL_SetRenderDrawColor(renderer, RASPI_TEXT_BLACK, 255);
        SDL_RenderFillRect(renderer, &rect);
        
        SDL_Rect rect2={pxlenght(inputbox.xpos-(getTextureWidth(texttex,inputbox.textsize)+STANDARTBORDER*2)/2+STANDARTBORDER), pxlenght(inputbox.ypos+STANDARTBORDER), pxlenght((getTextureWidth(texttex,inputbox.textsize)+STANDARTBORDER*2)-STANDARTBORDER*2), pxlenght(inputbox.textsize)};
        SDL_SetRenderDrawColor(renderer, RASPI_WHITE, 255);
        SDL_RenderFillRect(renderer, &rect2);
        
        SDL_SetRenderDrawColor(renderer, RASPI_TEXT_BLACK, 255);
        if (inputbox.cursorpos > stringlen(inputbox.text)) inputbox.cursorpos = stringlen(inputbox.text);
        if (inputbox.cursorpos < 0) inputbox.cursorpos = 0;
        int cursorlenpx=(int)((*(proportions+inputbox.cursorpos))*(rect.w-STANDARTBORDER))+STANDARTBORDER;
        if (stringlen(typedChar)!=0){
            if (inputbox.start_of_selected==-1 && inputbox.end_of_selected==-1){
                auto graphemes = utf8_split_graphemes(inputbox.text);
                graphemes.insert(graphemes.begin() + inputbox.cursorpos, typedChar);
                inputbox.text.clear();
                for (const auto& g : graphemes) inputbox.text += g;
                inputbox.cursorpos+=stringlen(typedChar);
            } else {
                replace_text_range(&inputbox.text, inputbox.start_of_selected, inputbox.end_of_selected, typedChar);
                inputbox.cursorpos=inputbox.start_of_selected+1;
                inputbox.start_of_selected=-1;
                inputbox.end_of_selected=-1;
            }
            inputbox.cursor_visible=true;
        }
        if (modkey & KMOD_SHIFT){
            if (specialkey=='l' && inputbox.cursorpos>0){
                if (inputbox.start_of_selected==-1 && inputbox.end_of_selected==-1){
                    inputbox.start_of_selected=inputbox.cursorpos-1;
                    inputbox.end_of_selected=inputbox.cursorpos;
                } else {
                    if (inputbox.cursorpos==inputbox.start_of_selected){
                        inputbox.start_of_selected--;
                    } else {
                        inputbox.end_of_selected--;
                    }
                }
                inputbox.cursorpos--;
                inputbox.cursor_visible=true;
            } else if (specialkey=='r' && inputbox.cursorpos<stringlen(inputbox.text)){
                if (inputbox.start_of_selected==-1 && inputbox.end_of_selected==-1){
                    inputbox.start_of_selected=inputbox.cursorpos;
                    inputbox.end_of_selected=inputbox.cursorpos+1;
                } else {
                    if (inputbox.cursorpos==inputbox.start_of_selected){
                        inputbox.start_of_selected++;
                    } else {
                        inputbox.end_of_selected++;
                    }
                }
                inputbox.cursorpos++;
                inputbox.cursor_visible=true;
            } else if (specialkey=='b'){
                inputbox.text="";
                inputbox.cursorpos=0;
                inputbox.cursor_visible=true;
            }
        } else {
            if (specialkey=='l' && inputbox.cursorpos>0){
                inputbox.cursorpos-=1;
                inputbox.start_of_selected=-1;
                inputbox.end_of_selected=-1;
                inputbox.cursor_visible=true;
            } else if (specialkey=='r' && inputbox.cursorpos<stringlen(inputbox.text)){
                inputbox.cursorpos+=1;
                inputbox.start_of_selected=-1;
                inputbox.end_of_selected=-1;
                inputbox.cursor_visible=true;
            } else if (specialkey=='b'){
                if (inputbox.start_of_selected==-1 && inputbox.end_of_selected==-1){
                    replace_text_range(&inputbox.text, inputbox.cursorpos-1, inputbox.cursorpos, "");
                } else {
                    replace_text_range(&inputbox.text, inputbox.start_of_selected, inputbox.end_of_selected, "");
                    inputbox.start_of_selected=-1;
                    inputbox.end_of_selected=-1;
                }
                inputbox.cursorpos--;
                inputbox.cursor_visible=true;
            }
        }
        if (inputbox.cursor_visible){SDL_SetRenderDrawColor(renderer, RASPI_SHADOW_GREY, 255); SDL_RenderDrawFatLine(renderer,pxlenght(inputbox.xpos-(getTextureWidth(texttex,inputbox.textsize)+STANDARTBORDER*2)/2)+cursorlenpx, pxlenght(inputbox.ypos+STANDARTBORDER), pxlenght(inputbox.xpos-(getTextureWidth(texttex,inputbox.textsize)+STANDARTBORDER*2)/2)+cursorlenpx, pxlenght(inputbox.ypos+inputbox.textsize+STANDARTBORDER), pxlenght(3));}
    }
    if (clic){
        SDL_Point mousepoint={mousex,mousey};
        if (SDL_PointInRect(&mousepoint,&rect)){
            if (inputbox.active){
                inputbox.start_of_selected=-1;
                inputbox.end_of_selected=-1;
                for (i = 0; i < stringlen(inputbox.text) && mousex - rect.x >= (int)((*(proportions + i)) * (float)(rect.w - STANDARTBORDER * 2)); i++){inputbox.cursorpos = i;}
                inputbox.cursorpos=i;
            } else {
                inputbox.start_of_selected=0;
                inputbox.end_of_selected=stringlen(inputbox.text);
            }
            inputbox.active=true;
            clickedOnSomething=true;
        } else {
            inputbox.active=false;
            inputbox.start_of_selected=-1;
            inputbox.end_of_selected=-1;
        }
    }

    if (inputbox.start_of_selected == inputbox.end_of_selected){
        inputbox.start_of_selected=-1;
        inputbox.end_of_selected=-1;
    }
    displayTex(renderer,texttex,pxlenght(inputbox.xpos+STANDARTBORDER-(getTextureWidth(texttex,inputbox.textsize)+STANDARTBORDER*2)/2), pxlenght(inputbox.ypos+STANDARTBORDER), pxlenght(inputbox.textsize));
    SDL_DestroyTexture(texttex);
    inputbox.xpos-=xshift;
    inputbox.ypos-=yshift;
    if (inputbox.origin_left){
        inputbox.xpos-=inputbox.textwidth/2;
    }
    *inputboxptr=inputbox;
}

void update3Label(SDL_Renderer* renderer, label* labelptr, int xshift=0, int yshift=0){
    label inputbox=*labelptr;
    inputbox.xpos+=xshift;
    inputbox.ypos+=yshift;
    std::string finaltext=inputbox.text;
    SDL_Texture* texttex=createTextTexture(renderer,finaltext);
    inputbox.textwidth=getTextureWidth(texttex,inputbox.textsize)+STANDARTBORDER*2;
    if (inputbox.origin_left){
        inputbox.xpos+=inputbox.textwidth/2;
    }
    // Klasický čistě černý TUI text
    displayTex(renderer,texttex,pxlenght(inputbox.xpos+STANDARTBORDER-(getTextureWidth(texttex,inputbox.textsize)+STANDARTBORDER*2)/2), pxlenght(inputbox.ypos+STANDARTBORDER), pxlenght(inputbox.textsize));
    SDL_DestroyTexture(texttex);
    inputbox.xpos-=xshift;
    inputbox.ypos-=yshift;
    if (inputbox.origin_left){
        inputbox.xpos-=inputbox.textwidth/2;
    }
    *labelptr=inputbox;
}

void update3CheckBox(SDL_Renderer* renderer, checkbox* boxptr, int xshift=0, int yshift=0){
    checkbox box=*boxptr;
    box.xpos+=xshift;
    box.ypos+=yshift;
    box.just_changed=false;

    int boxSize = STANDARTPICEHEIGHT;
    
    // Detekce hoveru a kliknutí
    bool hover = (mousex > pxlenght(box.xpos) && mousex < pxlenght(box.xpos + boxSize) && mousey > pxlenght(box.ypos) && mousey < pxlenght(box.ypos + boxSize));
    if (hover && clicked){
        box.checked=!box.checked;   
        box.just_changed=true;
        clicked = false; 
    }   

    // V raspi-configu se místo grafického čtverečku renderuje text "[ ]" nebo "[*]"
    const char* bracket_text = box.checked ? "[*]" : "[ ]";
    
    // Při hoveru nebo zakliknutí dostane samotný stavový boxík sytě červené TUI pozadí
    if (hover) {
        SDL_Rect rect={pxlenght(box.xpos), pxlenght(box.ypos), pxlenght(boxSize * 1.5f), pxlenght(boxSize)};
        SDL_SetRenderDrawColor(renderer, clicked ? RASPI_RED_CLICKED : RASPI_RED_SELECT, 255);
        SDL_RenderFillRect(renderer, &rect);
    }

    // Vykreslení textu stavu zaškrtnutí
    drawTUIChar(renderer, bracket_text, pxlenght(box.xpos + STANDARTBORDER), pxlenght(box.ypos), pxlenght(boxSize));

    // Vykreslení popisku vedle checkboxu (černý text)
    displayTex(renderer, box.title, pxlenght(box.xpos + boxSize * 1.8f), pxlenght(box.ypos), pxlenght(boxSize));

    box.xpos-=xshift;
    box.ypos-=yshift;
    *boxptr=box;
}

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

    // Výpočet pozice jezdce na základě znaků
    int char_width = pxlenght(STANDARTPICEHEIGHT / 2);
    int total_chars = (pxlenght(slider_data_w) / char_width);
    if (total_chars < 3) total_chars = 3;
    int handle_char_index = (int)(percentage * (total_chars - 1));

    // Logika ovládání myší
    int handle_x_px = pxlenght(slider_data_x) + handle_char_index * char_width;
    int handle_y_px = pxlenght(inputslider.ypos + STANDARTBORDER);
    bool hover = (mousex >= handle_x_px && mousex <= handle_x_px + char_width &&
                  mousey >= handle_y_px && mousey <= handle_y_px + pxlenght(STANDARTPICEHEIGHT));

    if (clicked) {
        if (hover || (mousex >= pxlenght(slider_data_x) && mousex <= pxlenght(slider_data_x + slider_data_w) && mousey >= handle_y_px && mousey <= handle_y_px + pxlenght(STANDARTPICEHEIGHT))) {
            inputslider.moving = true;
        }
    } else {
        inputslider.moving = false;
    }

    if (inputslider.moving) {
        int local_mx = mousex - pxlenght(slider_data_x);
        float new_pct = (float)local_mx / (float)pxlenght(slider_data_w);
        new_pct = std::clamp(new_pct, 0.0f, 1.0f);
        inputslider.value = inputslider.min + (new_pct * range);
        percentage = (range > 0.0f) ? ((inputslider.value - inputslider.min) / range) : 0.0f;
        handle_char_index = (int)(percentage * (total_chars - 1));
    }

    // Kreslení TUI osy pomocí textových znaků '─' a jezdce '░'
    for (int i = 0; i < total_chars; i++) {
        int cx = pxlenght(slider_data_x) + i * char_width;
        if (i == handle_char_index) {
            // Zbarvení bloku jezdce do červena při interakci
            if (inputslider.moving) {
                renderRect(cx, handle_y_px, char_width, pxlenght(STANDARTPICEHEIGHT - STANDARTBORDER), RASPI_RED_CLICKED);
            } else if (hover) {
                renderRect(cx, handle_y_px, char_width, pxlenght(STANDARTPICEHEIGHT - STANDARTBORDER), RASPI_RED_SELECT);
            }
            drawTUIChar(renderer, "░", cx, handle_y_px, pxlenght(STANDARTPICEHEIGHT - STANDARTBORDER));
        } else {
            drawTUIChar(renderer, "─", cx, handle_y_px + pxlenght(STANDARTPICEHEIGHT / 4), pxlenght(STANDARTPICEHEIGHT / 2));
        }
    }

    // Min / Max textové popisky pod osou
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

void update3NumberInputBox(SDL_Renderer* renderer, numberinputbox* inputboxptr, int xshift=0, int yshift=0){
    numberinputbox inputbox=*inputboxptr;
    inputbox.xpos+=xshift;
    inputbox.ypos+=yshift;
    std::string finaltext=inputbox.text;
    SDL_Texture* texttex=createTextTexture(renderer,finaltext, inputbox.start_of_selected, inputbox.end_of_selected);
    
    SDL_Rect rect={pxlenght(inputbox.xpos), pxlenght(inputbox.ypos), pxlenght((inputbox.size)), pxlenght(STANDARTPICEHEIGHT)};
    SDL_SetRenderDrawColor(renderer, RASPI_TEXT_BLACK, 255);
    SDL_RenderFillRect(renderer, &rect);
    
    SDL_Rect rect2={pxlenght(inputbox.xpos+STANDARTBORDER), pxlenght(inputbox.ypos+STANDARTBORDER), pxlenght(inputbox.size-STANDARTBORDER*2), pxlenght(STANDARTPICEHEIGHT-STANDARTBORDER*2)};
    SDL_SetRenderDrawColor(renderer, RASPI_WHITE, 255);
    SDL_RenderFillRect(renderer, &rect2);
    
    // TUI Šipky změny čísla nahoru/dolu přes znaky ▲ a ▼
    int btn_w = pxlenght(STANDARTPICEHEIGHT / 2);
    int top_btn_hover = (mousex > pxlenght(inputbox.xpos + inputbox.size) - btn_w && mousex < pxlenght(inputbox.xpos + inputbox.size) && mousey > pxlenght(inputbox.ypos) && mousey < pxlenght(inputbox.ypos + STANDARTPICEHEIGHT / 2));
    int bot_btn_hover = (mousex > pxlenght(inputbox.xpos + inputbox.size) - btn_w && mousex < pxlenght(inputbox.xpos + inputbox.size) && mousey >= pxlenght(inputbox.ypos + STANDARTPICEHEIGHT / 2) && mousey < pxlenght(inputbox.ypos + STANDARTPICEHEIGHT));

    SDL_Rect top_rect = { pxlenght(inputbox.xpos + inputbox.size) - btn_w, pxlenght(inputbox.ypos), btn_w, pxlenght(STANDARTPICEHEIGHT / 2) };
    SDL_Rect bot_rect = { pxlenght(inputbox.xpos + inputbox.size) - btn_w, pxlenght(inputbox.ypos + STANDARTPICEHEIGHT / 2), btn_w, pxlenght(STANDARTPICEHEIGHT / 2) };

    if (top_btn_hover) renderRect(top_rect.x, top_rect.y, top_rect.w, top_rect.h, clicked ? RASPI_RED_CLICKED : RASPI_RED_SELECT);
    if (bot_btn_hover) renderRect(bot_rect.x, bot_rect.y, bot_rect.w, bot_rect.h, clicked ? RASPI_RED_CLICKED : RASPI_RED_SELECT);

    drawTUIChar(renderer, "▲", top_rect.x, top_rect.y, top_rect.h);
    drawTUIChar(renderer, "▼", bot_rect.x, bot_rect.y, bot_rect.h);

    if (inputbox.active){
        if (SDL_GetTicks()-inputbox.last_time_cursor_blink>CURSORPERIOD){
            inputbox.cursor_visible=!inputbox.cursor_visible;
            inputbox.last_time_cursor_blink=SDL_GetTicks();
        }
        if (inputbox.cursorpos > stringlen(inputbox.text)) inputbox.cursorpos = stringlen(inputbox.text);
        if (inputbox.cursorpos < 0) inputbox.cursorpos = 0;
        int cursorlenpx=(int)((*(proportions+inputbox.cursorpos))*(pxlenght((getTextureWidth(texttex,STANDARTPICEHEIGHT-STANDARTBORDER*2)+STANDARTBORDER*2))-STANDARTBORDER))+STANDARTBORDER;
        if (stringlen(typedChar)!=0 && isNumber(typedChar)){
            if (inputbox.start_of_selected==-1 && inputbox.end_of_selected==-1){
                inputbox.text+=typedChar;
                inputbox.cursorpos+=stringlen(typedChar);
            } else {
                replace_text_range(&inputbox.text, inputbox.start_of_selected, inputbox.end_of_selected, typedChar);
                inputbox.cursorpos=inputbox.start_of_selected+1;
                inputbox.start_of_selected=-1;
                inputbox.end_of_selected=-1;
            }
            inputbox.cursor_visible=true;
        }
        if (modkey & KMOD_SHIFT){
            if (specialkey=='l' && inputbox.cursorpos>0){
                if (inputbox.start_of_selected==-1 && inputbox.end_of_selected==-1){
                    inputbox.start_of_selected=inputbox.cursorpos-1;
                    inputbox.end_of_selected=inputbox.cursorpos;
                } else {
                    if (inputbox.cursorpos==inputbox.start_of_selected){
                        inputbox.start_of_selected--;
                    } else {
                        inputbox.end_of_selected--;
                    }
                }
                inputbox.cursorpos--;
                inputbox.cursor_visible=true;
            } else if (specialkey=='r' && inputbox.cursorpos<stringlen(inputbox.text)){
                if (inputbox.start_of_selected==-1 && inputbox.end_of_selected==-1){
                    inputbox.start_of_selected=inputbox.cursorpos;
                    inputbox.end_of_selected=inputbox.cursorpos+1;
                } else {
                    if (inputbox.cursorpos==inputbox.start_of_selected){
                        inputbox.start_of_selected++;
                    } else {
                        inputbox.end_of_selected++;
                    }
                }
                inputbox.cursorpos++;
                inputbox.cursor_visible=true;
            } else if (specialkey=='b'){
                inputbox.text="";
                inputbox.cursorpos=0;
                inputbox.cursor_visible=true;
            }
        } else {
            if (specialkey=='l' && inputbox.cursorpos>0){
                inputbox.cursorpos-=1;
                inputbox.start_of_selected=-1;
                inputbox.end_of_selected=-1;
                inputbox.cursor_visible=true;
            } else if (specialkey=='r' && inputbox.cursorpos<stringlen(inputbox.text)){
                inputbox.cursorpos+=1;
                inputbox.start_of_selected=-1;
                inputbox.end_of_selected=-1;
                inputbox.cursor_visible=true;
            } else if (specialkey=='b'){
                if (inputbox.start_of_selected==-1 && inputbox.end_of_selected==-1){
                    replace_text_range(&inputbox.text, inputbox.cursorpos-1, inputbox.cursorpos, "");
                } else {
                    replace_text_range(&inputbox.text, inputbox.start_of_selected, inputbox.end_of_selected, "");
                    inputbox.start_of_selected=-1;
                    inputbox.end_of_selected=-1;
                }
                inputbox.cursorpos--;
                inputbox.cursor_visible=true;
            }
        }
        if (inputbox.cursor_visible){SDL_SetRenderDrawColor(renderer, RASPI_SHADOW_GREY, 255); SDL_RenderDrawFatLine(renderer,pxlenght(inputbox. xpos)+cursorlenpx, pxlenght(inputbox.ypos+STANDARTBORDER), pxlenght(inputbox. xpos)+cursorlenpx, pxlenght(inputbox.ypos+STANDARTPICEHEIGHT-STANDARTBORDER*2+STANDARTBORDER), pxlenght(3));}
    }
    if (clic){
        SDL_Point mousepoint={mousex,mousey};
        if (SDL_PointInRect(&mousepoint,&rect)){
            if (mousex>pxlenght(inputbox.xpos+inputbox.size-STANDARTPICEHEIGHT/2)){
                if (mousey>pxlenght(inputbox.ypos+STANDARTPICEHEIGHT/2)){
                    inputbox.number--;
                    inputbox.text=std::to_string(inputbox.number);
                } else {
                    inputbox.number++;
                    inputbox.text=std::to_string(inputbox.number);
                }
            } else {
                if (inputbox.active){
                    inputbox.start_of_selected=-1;
                    inputbox.end_of_selected=-1;
                    for (i=0;mousex-rect.x>=(int)((*(proportions+i))*(float)(pxlenght((getTextureWidth(texttex,STANDARTPICEHEIGHT-STANDARTBORDER*2)+STANDARTBORDER*2))-STANDARTBORDER*2));i++)
                    inputbox.cursorpos=i;
                } else {
                    inputbox.start_of_selected=0;
                    inputbox.end_of_selected=stringlen(inputbox.text);
                }
                inputbox.active=true;
            }
            clickedOnSomething=true;
        } else {
            inputbox.active=false;
            inputbox.start_of_selected=-1;
            inputbox.end_of_selected=-1;
        }
    }

    if (inputbox.start_of_selected == inputbox.end_of_selected){
        inputbox.start_of_selected=-1;
        inputbox.end_of_selected=-1;
    }
    displayTex(renderer,texttex,pxlenght(inputbox.xpos+STANDARTBORDER), pxlenght(inputbox.ypos+STANDARTBORDER), pxlenght(STANDARTPICEHEIGHT-STANDARTBORDER*2));
    SDL_DestroyTexture(texttex);
    inputbox.number=stringToInt(inputbox.text);
    if (inputbox.number>inputbox.maxnum){inputbox.number=inputbox.maxnum; inputbox.text=std::to_string(inputbox.maxnum);}
    if (inputbox.number<0){inputbox.number=0; inputbox.text="0";}
    inputbox.xpos-=xshift;
    inputbox.ypos-=yshift;
    *inputboxptr=inputbox;
}

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

void update3Panel(SDL_Renderer* renderer, panel* panelptr){
    panel pan=*panelptr;
    
    int px_x = pxlenght(pan.x);
    int px_y = pxlenght(pan.y);
    int px_w = pxlenght(pan.w);
    int px_h = pxlenght(pan.h);
    int char_s = pxlenght(STANDARTPICEHEIGHT / 2); // Krok mřížky ohraničení

    // 1. Vykreslení pseudo-3D stínu panelu ze znaků ' ' (prázdno na stínovém rectu)
    renderRect(px_x + char_s, px_y + char_s, px_w, px_h, RASPI_SHADOW_GREY);

    // 2. Hlavní šedé tělo raspi-config boxu
    renderRect(px_x, px_y, px_w, px_h, RASPI_GREY_BOX);
    
    // 3. GENERACE OKRAJŮ POMOCÍ ZNAKŮ (┌ ─ ┐ │ └ ┘)
    // Rohy okna
    drawTUIChar(renderer, "┌", px_x, px_y, char_s);
    drawTUIChar(renderer, "┐", px_x + px_w - char_s, px_y, char_s);
    drawTUIChar(renderer, "└", px_x, px_y + px_h - char_s, char_s);
    drawTUIChar(renderer, "┘", px_x + px_w - char_s, px_y + px_h - char_s, char_s);

    // Horizontální linky (top a bottom okraje)
    for (int cx = px_x + char_s; cx < px_x + px_w - char_s; cx += char_s) {
        drawTUIChar(renderer, "─", cx, px_y, char_s);
        drawTUIChar(renderer, "─", cx, px_y + px_h - char_s, char_s);
    }
    // Vertikální linky (boční okraje okna)
    for (int cy = px_y + char_s; cy < px_y + px_h - char_s; cy += char_s) {
        drawTUIChar(renderer, "│", px_x, cy, char_s);
        drawTUIChar(renderer, "│", px_x + px_w - char_s, cy, char_s);
    }

    // --- Výpočty rozložení prvků ---
    int top_size=0;
    for (auto& element : pan.top_elements) {
        if (get_element_pos(&element).y > top_size) {
            top_size = get_element_pos(&element).y;
        }
    }
    int bottom_size=0;
    for (auto& element : pan.bottom_elements) {
        if (get_element_pos(&element).y > bottom_size) {
            bottom_size = get_element_pos(&element).y;
        }
    }
    top_size+=STANDARTBORDER+STANDARTBORDER;
    bottom_size+=STANDARTBORDER+STANDARTBORDER;
    
    int scrollbar_area_start_y = pan.y + top_size + STANDARTPICEHEIGHT;
    int scrollbar_area_end_y = pan.y + pan.h - bottom_size - STANDARTPICEHEIGHT;
    int scrollbar_area_height = scrollbar_area_end_y - scrollbar_area_start_y;

    int total_content_height=0;
    for (auto& e:pan.scrollable_elements){
        if (get_element_pos(&e).y>total_content_height){
            total_content_height=get_element_pos(&e).y+STANDARTPICEHEIGHT+STANDARTBORDER*4;
        }
    }     

    int scrollbar_range = total_content_height - scrollbar_area_height + bottom_size + STANDARTPICEHEIGHT;
    int scrollbar_height = 1;
    int scrollbar_y = scrollbar_area_start_y;
    if (scrollbar_range > 0) {
        scrollbar_height = (scrollbar_area_height * scrollbar_area_height) / total_content_height;
        if (scrollbar_height < 10) scrollbar_height = 10; 
        scrollbar_y = scrollbar_area_start_y + (pan.scrolled * (scrollbar_area_height - scrollbar_height)) / scrollbar_range;
    }
    
    int scrollbar_x = pan.x + pan.w - STANDARTPICEHEIGHT/4;
    if (clicked && mousex >= pxlenght(scrollbar_x - STANDARTPICEHEIGHT/4) && mousex <= pxlenght(scrollbar_x + STANDARTPICEHEIGHT/4) &&
        mousey >= pxlenght(scrollbar_area_start_y) && mousey <= pxlenght(scrollbar_area_end_y)) {
        int click_y_offset = mousey - pxlenght(scrollbar_area_start_y);
        if (scrollbar_range > 0) {
            int center_position = click_y_offset - scrollbar_height / 2;
            pan.scrolled = (center_position * scrollbar_range) / (scrollbar_area_height - scrollbar_height);
            if (pan.scrolled < 0) pan.scrolled = 0;
            if (pan.scrolled > scrollbar_range) pan.scrolled = scrollbar_range;
        }
        clickedOnSomething = true;
    }
    
    // Vykreslení vnitřních prvků (s offsetem ohraničení, aby nepřetékaly přes TUI okraje)
    for (auto& element : pan.scrollable_elements) {
        if (get_element_pos(&element).y-pan.scrolled > -STANDARTBORDER-STANDARTPICEHEIGHT && get_element_pos(&element).y-pan.scrolled < pan.h-STANDARTPICEHEIGHT*2-STANDARTBORDER*2) {
            update3Element(renderer, &element, pan.x+STANDARTBORDER+STANDARTPICEHEIGHT/2, pan.y-pan.scrolled+STANDARTBORDER+top_size+STANDARTPICEHEIGHT);
        }
    }
    
    // Čisté krycí lišty pro oříznutí scrollu (v barvě těla panelu)
    renderRect(px_x + char_s, px_y + char_s, px_w - char_s * 2, pxlenght(top_size + STANDARTPICEHEIGHT) - char_s, RASPI_GREY_BOX);
    renderRect(px_x + char_s, pxlenght(pan.h + pan.y - bottom_size - STANDARTPICEHEIGHT), px_w - char_s * 2, pxlenght(bottom_size + STANDARTPICEHEIGHT) - char_s, RASPI_GREY_BOX);
    
    // Vykreslení fixních prvků nahoře a dole
    for (auto& element : pan.top_elements) {
        update3Element(renderer, &element, pan.x+STANDARTBORDER+STANDARTPICEHEIGHT/2, pan.y+STANDARTBORDER+STANDARTPICEHEIGHT/2);
    }
    for (auto& element : pan.bottom_elements) {
        update3Element(renderer, &element, pan.x+STANDARTBORDER+STANDARTPICEHEIGHT/2, pan.y+STANDARTBORDER-STANDARTPICEHEIGHT-bottom_size+pan.h-STANDARTPICEHEIGHT/2);
    }
    
    // Stylizovaný retro scrollbar (Jezdec jako výrazný červený blok)
    if (scrollbar_range > 0) {
        bool scrollbar_hovered = mousex >= pxlenght(scrollbar_x - STANDARTPICEHEIGHT/4) && mousex <= pxlenght(scrollbar_x + STANDARTPICEHEIGHT/4) &&
                                mousey >= pxlenght(scrollbar_y) && mousey <= pxlenght(scrollbar_y + scrollbar_height);
        bool scrollbar_clicked = scrollbar_hovered && clicked;
        
        if (scrollbar_clicked) {
            renderRect(pxlenght(scrollbar_x - STANDARTPICEHEIGHT/4), pxlenght(scrollbar_y), pxlenght(STANDARTPICEHEIGHT/2), pxlenght(scrollbar_height), RASPI_RED_CLICKED);
        } else if (scrollbar_hovered) {
            renderRect(pxlenght(scrollbar_x - STANDARTPICEHEIGHT/4), pxlenght(scrollbar_y), pxlenght(STANDARTPICEHEIGHT/2), pxlenght(scrollbar_height), RASPI_RED_SELECT);
        } else {
            renderRect(pxlenght(scrollbar_x - STANDARTPICEHEIGHT/4), pxlenght(scrollbar_y), pxlenght(STANDARTPICEHEIGHT/2), pxlenght(scrollbar_height), RASPI_SHADOW_GREY);
        }
    }
    
    *panelptr=pan;
    return;
}