#include "includes.hpp"
void update0DropMenu(SDL_Renderer* renderer, dropmenu* menuptr, int xshift=0, int yshift=0){
    dropmenu menu=*menuptr;
    menu.xpos+=xshift;
    menu.ypos+=yshift;
    if (menu.opened){
        SDL_Rect rect={pxlenght(menu.xpos),pxlenght(menu.ypos),pxlenght(menu.topwidth), pxlenght(STANDARTPICEHEIGHT)};
        SDL_SetRenderDrawColor(renderer, COLOR_HOVER,255);
        SDL_RenderFillRoundRect(renderer, &rect);
        SDL_Rect rect0={pxlenght(menu.xpos),pxlenght(menu.ypos+STANDARTPICEHEIGHT),pxlenght(menu.width), pxlenght(STANDARTPICEHEIGHT*menu.lenght)};
        SDL_SetRenderDrawColor(renderer, COLOR_BACKGROUND,255);
        SDL_RenderFillRoundRect(renderer, &rect0);
        for (int i=0;i<menu.lenght;i++){
            if (mousex<pxlenght(menu.xpos+menu.width) && mousex>pxlenght(menu.xpos) && mousey<pxlenght(menu.ypos+STANDARTPICEHEIGHT*(i+2))){
                SDL_Rect rect1={pxlenght(menu.xpos+STANDARTBORDER),pxlenght(menu.ypos+STANDARTPICEHEIGHT*(i+1)+STANDARTBORDER),pxlenght(menu.width-STANDARTBORDER*2), pxlenght(STANDARTPICEHEIGHT-STANDARTBORDER*2)};
                SDL_SetRenderDrawColor(renderer, COLOR_SELECTED,255);
                if (clicked){SDL_SetRenderDrawColor(renderer, COLOR_CLICKED,255);}
                SDL_RenderFillRoundRect(renderer, &rect1);
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
        SDL_SetRenderDrawColor(renderer, COLOR_FOREGROUND, 255);
        for (int i=0;i<menu.lenght-1;i++){
            SDL_RenderDrawFatLine(renderer, pxlenght(menu.xpos+STANDARTBORDER), pxlenght(menu.ypos+STANDARTPICEHEIGHT*(i+2)), pxlenght(menu.xpos+menu.width-STANDARTBORDER), pxlenght(menu.ypos+STANDARTPICEHEIGHT*(i+2)), pxlenght(BORDERLINEFATNESS));
        }
        // Border for dropdown
        SDL_RenderDrawFatLine(renderer, pxlenght(menu.xpos), pxlenght(menu.ypos+STANDARTPICEHEIGHT), pxlenght(menu.xpos+menu.width), pxlenght(menu.ypos+STANDARTPICEHEIGHT), pxlenght(BORDERLINEFATNESS)); // top
        SDL_RenderDrawFatLine(renderer, pxlenght(menu.xpos), pxlenght(menu.ypos+STANDARTPICEHEIGHT*(menu.lenght+1)), pxlenght(menu.xpos+menu.width), pxlenght(menu.ypos+STANDARTPICEHEIGHT*(menu.lenght+1)), pxlenght(BORDERLINEFATNESS)); // bottom
        SDL_RenderDrawFatLine(renderer, pxlenght(menu.xpos), pxlenght(menu.ypos+STANDARTPICEHEIGHT), pxlenght(menu.xpos), pxlenght(menu.ypos+STANDARTPICEHEIGHT*(menu.lenght+1)), pxlenght(BORDERLINEFATNESS)); // left
        SDL_RenderDrawFatLine(renderer, pxlenght(menu.xpos+menu.width), pxlenght(menu.ypos+STANDARTPICEHEIGHT), pxlenght(menu.xpos+menu.width), pxlenght(menu.ypos+STANDARTPICEHEIGHT*(menu.lenght+1)), pxlenght(BORDERLINEFATNESS)); // right
        clickedOnSomething=true;
    }
    // Border for title bar (always shown)
    SDL_SetRenderDrawColor(renderer, COLOR_FOREGROUND, 255);
    SDL_RenderDrawFatLine(renderer, pxlenght(menu.xpos), pxlenght(menu.ypos), pxlenght(menu.xpos+menu.topwidth), pxlenght(menu.ypos), pxlenght(BORDERLINEFATNESS)); // top
    SDL_RenderDrawFatLine(renderer, pxlenght(menu.xpos), pxlenght(menu.ypos+STANDARTPICEHEIGHT), pxlenght(menu.xpos+menu.topwidth), pxlenght(menu.ypos+STANDARTPICEHEIGHT), pxlenght(BORDERLINEFATNESS)); // bottom
    SDL_RenderDrawFatLine(renderer, pxlenght(menu.xpos), pxlenght(menu.ypos), pxlenght(menu.xpos), pxlenght(menu.ypos+STANDARTPICEHEIGHT), pxlenght(BORDERLINEFATNESS)); // left
    SDL_RenderDrawFatLine(renderer, pxlenght(menu.xpos+menu.topwidth), pxlenght(menu.ypos), pxlenght(menu.xpos+menu.topwidth), pxlenght(menu.ypos+STANDARTPICEHEIGHT), pxlenght(BORDERLINEFATNESS)); // right
    if (menu.opened && mousex<pxlenght(menu.xpos+menu.width) && mousex>pxlenght(menu.xpos) && mousey<pxlenght(menu.ypos+STANDARTPICEHEIGHT*(menu.lenght+1))){menu.opened=true;clickedOnSomething=true;}
    else if (clicked && menu.opened && !(mousex<pxlenght(menu.xpos+menu.width) && mousex>pxlenght(menu.xpos) && mousey<pxlenght(menu.ypos+STANDARTPICEHEIGHT*(menu.lenght+1)))){menu.opened=false;clickedOnSomething=true;}
    if (mousex<pxlenght(menu.topwidth)+pxlenght(menu.xpos) && mousex>pxlenght(menu.xpos) && mousey>pxlenght(menu.ypos) && mousey<pxlenght(menu.ypos+STANDARTPICEHEIGHT)){
        SDL_Rect rect={pxlenght(menu.xpos),pxlenght(menu.ypos),pxlenght(menu.topwidth), pxlenght(STANDARTPICEHEIGHT)};
        if (clicked){SDL_SetRenderDrawColor(renderer, COLOR_CLICKED, 255);clickedOnSomething=true;}
        else {SDL_SetRenderDrawColor(renderer, COLOR_HOVER, 255);}
        SDL_RenderFillRoundRect(renderer, &rect);
        if (clickup){
            menu.opened=true;
            clickedOnSomething=true;
        }
    } else {
        SDL_Rect rect={pxlenght(menu.xpos),pxlenght(menu.ypos),pxlenght(menu.topwidth), pxlenght(STANDARTPICEHEIGHT)};
        SDL_SetRenderDrawColor(renderer, COLOR_BACKGROUND, 255);
        SDL_RenderFillRoundRect(renderer, &rect);
    }
    if (menu.titletype=="defined"){
        displayTex(renderer,menu.title,pxlenght(menu.xpos+STANDARTBORDER*2),pxlenght(menu.ypos+STANDARTBORDER),pxlenght(STANDARTPICEHEIGHT-STANDARTBORDER*2));
    } else if (menu.titletype=="select"){
        displayTex(renderer,menu.textures[menu.selected],pxlenght(menu.xpos+STANDARTBORDER*2),pxlenght(menu.ypos+STANDARTBORDER),pxlenght(STANDARTPICEHEIGHT-STANDARTBORDER*2));
    }
    menu.xpos-=xshift;
    menu.ypos-=yshift;
    *menuptr=menu;
    return;

}
void update0TextBox(SDL_Renderer* renderer, textbox* inputboxptr, int xshift=0, int yshift=0){
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
        SDL_SetRenderDrawColor(renderer, 50,50,50,255);
        SDL_RenderFillRoundRect(renderer, &rect);
        SDL_Rect rect2={pxlenght(inputbox.xpos-(getTextureWidth(texttex,inputbox.textsize)+STANDARTBORDER*2)/2+STANDARTBORDER), pxlenght(inputbox.ypos+STANDARTBORDER), pxlenght((getTextureWidth(texttex,inputbox.textsize)+STANDARTBORDER*2)-STANDARTBORDER*2), pxlenght(inputbox.textsize)};
        SDL_SetRenderDrawColor(renderer, 230,230,230,255);
        SDL_RenderFillRoundRect(renderer, &rect2, pxlenght(3));
        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
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
        if (inputbox.cursor_visible){SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255); SDL_RenderDrawFatLine(renderer,pxlenght(inputbox.xpos-(getTextureWidth(texttex,inputbox.textsize)+STANDARTBORDER*2)/2)+cursorlenpx, pxlenght(inputbox.ypos+STANDARTBORDER), pxlenght(inputbox.xpos-(getTextureWidth(texttex,inputbox.textsize)+STANDARTBORDER*2)/2)+cursorlenpx, pxlenght(inputbox.ypos+inputbox.textsize+STANDARTBORDER), pxlenght(3));}
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
    return;
}
void update0Label(SDL_Renderer* renderer, label* labelptr, int xshift=0, int yshift=0){
    label inputbox=*labelptr;
    inputbox.xpos+=xshift;
    inputbox.ypos+=yshift;
    std::string finaltext=inputbox.text;
    SDL_Texture* texttex=createTextTexture(renderer,finaltext);
    inputbox.textwidth=getTextureWidth(texttex,inputbox.textsize)+STANDARTBORDER*2;
    if (inputbox.origin_left){
        inputbox.xpos+=inputbox.textwidth/2;
    }
    SDL_Rect rect={pxlenght(inputbox.xpos-(getTextureWidth(texttex,inputbox.textsize)+STANDARTBORDER*2)/2), pxlenght(inputbox.ypos), pxlenght((getTextureWidth(texttex,inputbox.textsize)+STANDARTBORDER*2)), pxlenght(inputbox.textsize+STANDARTBORDER*2)};
    displayTex(renderer,texttex,pxlenght(inputbox.xpos+STANDARTBORDER-(getTextureWidth(texttex,inputbox.textsize)+STANDARTBORDER*2)/2), pxlenght(inputbox.ypos+STANDARTBORDER), pxlenght(inputbox.textsize));
    SDL_DestroyTexture(texttex);
    inputbox.xpos-=xshift;
    inputbox.ypos-=yshift;
    if (inputbox.origin_left){
        inputbox.xpos-=inputbox.textwidth/2;
    }
    *labelptr=inputbox;
    return;
}
void update0CheckBox(SDL_Renderer* renderer, checkbox* boxptr, int xshift=0, int yshift=0){
    checkbox box=*boxptr;
    box.xpos+=xshift;
    box.ypos+=yshift;
    box.just_changed=false;

    // Použijeme tvou standardní výšku, ať to lícuje s textem
    int boxSize = STANDARTPICEHEIGHT; 
    int borderOffset = 2; // tloušťka okraje

    SDL_Rect rect={pxlenght(box.xpos), pxlenght(box.ypos), pxlenght(boxSize), pxlenght(boxSize)};
    SDL_SetRenderDrawColor(renderer, COLOR_FOREGROUND, 255);
    SDL_RenderFillRect(renderer, &rect);
    
    SDL_Rect rect1={pxlenght(box.xpos + borderOffset), pxlenght(box.ypos + borderOffset), 
                    pxlenght(boxSize - borderOffset*2), pxlenght(boxSize - borderOffset*2)};
    
    // Detekce kolize s novou velikostí boxíku
    if (mousex>pxlenght(box.xpos) && mousex<pxlenght(box.xpos+boxSize) && mousey>pxlenght(box.ypos) && mousey<pxlenght(box.ypos+boxSize) && clicked){
        box.checked=!box.checked;   
        box.just_changed=true;
        clicked = false; // Zabrání blikání v dalším framu
    }   

    // Barvy vnitřku
    if (box.checked){
        SDL_SetRenderDrawColor(renderer, COLOR_SELECTED, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, COLOR_BACKGROUND, 255);
    }

    // Hover efekt (myš nad tlačítkem)
    if (mousex>pxlenght(box.xpos) && mousex<pxlenght(box.xpos+boxSize) && mousey>pxlenght(box.ypos) && mousey<pxlenght(box.ypos+boxSize)){
        if (box.checked){
            SDL_SetRenderDrawColor(renderer, COLOR_SELECTED_HOVER, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, COLOR_HOVER, 255);
        }
    }
    SDL_RenderFillRect(renderer, &rect1);

    // Vykreslení textu s normální velikostí (posunuto za větší boxík)
    displayTex(renderer, box.title, pxlenght(box.xpos + boxSize + STANDARTBORDER), pxlenght(box.ypos), pxlenght(boxSize));

    box.xpos-=xshift;
    box.ypos-=yshift;
    *boxptr=box;
    return;
}
void update0Slider(SDL_Renderer* renderer, slider* sliderptr, int xshift=0, int yshift=0) {
    // 1. LOKÁLNÍ KOPIE A POSUN SOUŘADNIC
    slider inputslider = *sliderptr;
    inputslider.xpos += xshift;
    inputslider.ypos += yshift;

    char text_buf[32];

    // 2. VYKRESLENÍ VALUE PŘED SLIDEREM (Velké písmo)
    int value_text_size = STANDARTPICEHEIGHT - STANDARTBORDER * 2;
    sprintf(text_buf, "%.1f", inputslider.value);
    SDL_Texture* val_tex = createTextTexture(renderer, text_buf);
    int val_w = getTextureWidth(val_tex, value_text_size);
    
    // Vykreslení hodnoty na úplný začátek komponenty
    displayTex(renderer, val_tex, pxlenght(inputslider.xpos), pxlenght(inputslider.ypos + STANDARTBORDER), pxlenght(value_text_size));
    SDL_DestroyTexture(val_tex);

    // Odsazení samotného slideru (osy a dráhy) za text hodnoty + malá rezerva
    int slider_data_x = inputslider.xpos + val_w + STANDARTBORDER;
    int slider_data_w = inputslider.width - (val_w + STANDARTBORDER);
    if (slider_data_w < 10) slider_data_w = 10; // Bezpečnostní pojistka proti podtečení šířky

    // 3. VÝPOČET GEOMETRIE JEZDCE (Obdélník: šířka je polovina výšky)
    int handle_h = STANDARTPICEHEIGHT - STANDARTBORDER * 2;
    int handle_w = handle_h / 2; 
    if (handle_w < 1) handle_w = 1;

    float range = inputslider.max - inputslider.min;
    float percentage = (range > 0.0f) ? ((inputslider.value - inputslider.min) / range) : 0.0f;
    
    int travel_width = slider_data_w - handle_w;
    int handle_x = slider_data_x + (int)(percentage * travel_width);
    int handle_y = inputslider.ypos + STANDARTBORDER;

    // 4. HITBOX A LOGIKA OVLÁDÁNÍ
    bool hover = (mousex >= handle_x && mousex <= handle_x + handle_w &&
                  mousey >= handle_y && mousey <= handle_y + handle_h);

    if (clicked) {
        if (hover) {
            inputslider.moving = true;
        }
    } else {
        inputslider.moving = false;
    }

    // Přepočet hodnoty při aktivním tažení
    if (inputslider.moving) {
        int local_mx = mousex - slider_data_x - (handle_w / 2);
        if (local_mx < 0) local_mx = 0;
        if (local_mx > travel_width) local_mx = travel_width;

        float new_pct = (float)local_mx / (float)travel_width;
        inputslider.value = inputslider.min + (new_pct * range);
    }

    // 5. RENDER OSY (VODOROVNÁ ČÁRA)
    int line_y = inputslider.ypos + (STANDARTPICEHEIGHT / 2);
    SDL_SetRenderDrawColor(renderer, COLOR_FOREGROUND, 255);
    SDL_RenderDrawLine(renderer, pxlenght(slider_data_x), pxlenght(line_y), pxlenght(slider_data_x + slider_data_w), pxlenght(line_y));

    // 6. RENDER JEZDCE (VÝPLŇ)
    if (inputslider.moving) {
        SDL_SetRenderDrawColor(renderer, COLOR_CLICKED, 255);
    } else if (hover) {
        SDL_SetRenderDrawColor(renderer, COLOR_HOVER, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, COLOR_BACKGROUND, 255);
    }
    
    SDL_Rect handle_rect = { pxlenght(handle_x), pxlenght(handle_y), pxlenght(handle_w), pxlenght(handle_h) };
    SDL_RenderFillRect(renderer, &handle_rect);

    // 7. RENDER JEZDCE (OBRYS S TLOUŠTKOU BORDERLINEFATNESS)
    SDL_SetRenderDrawColor(renderer, COLOR_FOREGROUND, 255);
    for (int i = 0; i < BORDERLINEFATNESS; i++) {
        SDL_Rect outline_rect = { 
            pxlenght(handle_x + i), 
            pxlenght(handle_y + i), 
            pxlenght(handle_w - i * 2), 
            pxlenght(handle_h - i * 2) 
        };
        SDL_RenderDrawRect(renderer, &outline_rect);
    }

    // 8. VYKRESLENÍ TEXTŮ POD SLIDEREM (Malé písmo: STANDARTPICEHEIGHT / 2)
    int small_text_size = STANDARTPICEHEIGHT / 2;
    int small_text_y = inputslider.ypos + STANDARTPICEHEIGHT;

    // Text MIN (Zarovnaný pod začátek osy)
    sprintf(text_buf, "%.1f", inputslider.min);
    SDL_Texture* min_tex = createTextTexture(renderer, text_buf);
    displayTex(renderer, min_tex, pxlenght(slider_data_x), pxlenght(small_text_y), pxlenght(small_text_size));
    SDL_DestroyTexture(min_tex);

    // Text MAX (Zarovnaný pod konec osy)
    sprintf(text_buf, "%.1f", inputslider.max);
    SDL_Texture* max_tex = createTextTexture(renderer, text_buf);
    int max_w = getTextureWidth(max_tex, small_text_size);
    displayTex(renderer, max_tex, pxlenght(slider_data_x + slider_data_w - max_w), pxlenght(small_text_y), pxlenght(small_text_size));
    SDL_DestroyTexture(max_tex);

    // 9. REVERT SOUŘADNIC A ZÁPIS ZPĚT DO POINTERU
    inputslider.xpos -= xshift;
    inputslider.ypos -= yshift;
    *sliderptr = inputslider;
}
void update0NumberInputBox(SDL_Renderer* renderer, numberinputbox* inputboxptr, int xshift=0, int yshift=0){
    numberinputbox inputbox=*inputboxptr;
    inputbox.xpos+=xshift;
    inputbox.ypos+=yshift;
    std::string finaltext=inputbox.text;
    SDL_Texture* texttex=createTextTexture(renderer,finaltext, inputbox.start_of_selected, inputbox.end_of_selected);
    SDL_Rect rect={pxlenght(inputbox.xpos), pxlenght(inputbox.ypos), pxlenght((inputbox.size)), pxlenght(STANDARTPICEHEIGHT)};
    SDL_SetRenderDrawColor(renderer, 50,50,50,255);
    SDL_RenderFillRoundRect(renderer, &rect);
    SDL_Rect rect2={pxlenght(inputbox.xpos+STANDARTBORDER), pxlenght(inputbox.ypos+STANDARTBORDER), pxlenght(inputbox.size-STANDARTBORDER*2), pxlenght(STANDARTPICEHEIGHT-STANDARTBORDER*2)};
    SDL_SetRenderDrawColor(renderer, 230,230,230,255);
    SDL_RenderFillRoundRect(renderer, &rect2, pxlenght(3));
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderDrawFatLine(renderer,pxlenght(inputbox.xpos+inputbox.size-STANDARTPICEHEIGHT/2), pxlenght(inputbox.ypos+1), pxlenght(inputbox.xpos+inputbox.size-STANDARTPICEHEIGHT/2), pxlenght(inputbox.ypos+STANDARTPICEHEIGHT-1), pxlenght(STANDARTBORDER)/2);
    SDL_RenderDrawFatLine(renderer,pxlenght(inputbox.xpos+inputbox.size-STANDARTPICEHEIGHT/2), pxlenght(inputbox.ypos+STANDARTPICEHEIGHT/2), pxlenght(inputbox.xpos+inputbox.size), pxlenght(inputbox.ypos+STANDARTPICEHEIGHT/2), pxlenght(STANDARTBORDER)/2);
    SDL_SetRenderDrawColor(renderer, 120,120,120,255);
    SDL_Point mousepoint={mousex,mousey};
    if (SDL_PointInRect(&mousepoint,&rect) && mousex>pxlenght(inputbox.xpos+inputbox.size-STANDARTPICEHEIGHT/2)){
        if (mousey>pxlenght(inputbox.ypos+STANDARTPICEHEIGHT/2)){
            SDL_SetRenderDrawColor(renderer, 80,80,80,255);
            SDL_RenderDrawFatLine(renderer,pxlenght(inputbox.xpos+inputbox.size-STANDARTPICEHEIGHT/2+3), pxlenght(inputbox.ypos+STANDARTPICEHEIGHT/2+4), pxlenght(inputbox.xpos+inputbox.size-STANDARTPICEHEIGHT/4-1), pxlenght(inputbox.ypos+STANDARTPICEHEIGHT-4), pxlenght(STANDARTBORDER));
            SDL_RenderDrawFatLine(renderer,pxlenght(inputbox.xpos+inputbox.size-4), pxlenght(inputbox.ypos+STANDARTPICEHEIGHT/2+4), pxlenght(inputbox.xpos+inputbox.size-STANDARTPICEHEIGHT/4-1), pxlenght(inputbox.ypos+STANDARTPICEHEIGHT-4), pxlenght(STANDARTBORDER));
            SDL_SetRenderDrawColor(renderer, 170,170,170,255);
            SDL_RenderDrawFatLine(renderer,pxlenght(inputbox.xpos+inputbox.size-STANDARTPICEHEIGHT/2+3), pxlenght(inputbox.ypos+STANDARTPICEHEIGHT/2-4), pxlenght(inputbox.xpos+inputbox.size-STANDARTPICEHEIGHT/4-1), pxlenght(inputbox.ypos+4), pxlenght(STANDARTBORDER));
            SDL_RenderDrawFatLine(renderer,pxlenght(inputbox.xpos+inputbox.size-4), pxlenght(inputbox.ypos+STANDARTPICEHEIGHT/2-4), pxlenght(inputbox.xpos+inputbox.size-STANDARTPICEHEIGHT/4-1), pxlenght(inputbox.ypos+4), pxlenght(STANDARTBORDER));
        } else {
            SDL_SetRenderDrawColor(renderer, 80,80,80,255);
            SDL_RenderDrawFatLine(renderer,pxlenght(inputbox.xpos+inputbox.size-STANDARTPICEHEIGHT/2+3), pxlenght(inputbox.ypos+STANDARTPICEHEIGHT/2-4), pxlenght(inputbox.xpos+inputbox.size-STANDARTPICEHEIGHT/4-1), pxlenght(inputbox.ypos+4), pxlenght(STANDARTBORDER));
            SDL_RenderDrawFatLine(renderer,pxlenght(inputbox.xpos+inputbox.size-4), pxlenght(inputbox.ypos+STANDARTPICEHEIGHT/2-4), pxlenght(inputbox.xpos+inputbox.size-STANDARTPICEHEIGHT/4-1), pxlenght(inputbox.ypos+4), pxlenght(STANDARTBORDER));
            SDL_SetRenderDrawColor(renderer, 170,170,170,255);
            SDL_RenderDrawFatLine(renderer,pxlenght(inputbox.xpos+inputbox.size-STANDARTPICEHEIGHT/2+3), pxlenght(inputbox.ypos+STANDARTPICEHEIGHT/2+4), pxlenght(inputbox.xpos+inputbox.size-STANDARTPICEHEIGHT/4-1), pxlenght(inputbox.ypos+STANDARTPICEHEIGHT-4), pxlenght(STANDARTBORDER));
            SDL_RenderDrawFatLine(renderer,pxlenght(inputbox.xpos+inputbox.size-4), pxlenght(inputbox.ypos+STANDARTPICEHEIGHT/2+4), pxlenght(inputbox.xpos+inputbox.size-STANDARTPICEHEIGHT/4-1), pxlenght(inputbox.ypos+STANDARTPICEHEIGHT-4), pxlenght(STANDARTBORDER));
        }
    } else {
        SDL_RenderDrawFatLine(renderer,pxlenght(inputbox.xpos+inputbox.size-STANDARTPICEHEIGHT/2+3), pxlenght(inputbox.ypos+STANDARTPICEHEIGHT/2-4), pxlenght(inputbox.xpos+inputbox.size-STANDARTPICEHEIGHT/4-1), pxlenght(inputbox.ypos+4), pxlenght(STANDARTBORDER));
        SDL_RenderDrawFatLine(renderer,pxlenght(inputbox.xpos+inputbox.size-4), pxlenght(inputbox.ypos+STANDARTPICEHEIGHT/2-4), pxlenght(inputbox.xpos+inputbox.size-STANDARTPICEHEIGHT/4-1), pxlenght(inputbox.ypos+4), pxlenght(STANDARTBORDER));
        SDL_RenderDrawFatLine(renderer,pxlenght(inputbox.xpos+inputbox.size-STANDARTPICEHEIGHT/2+3), pxlenght(inputbox.ypos+STANDARTPICEHEIGHT/2+4), pxlenght(inputbox.xpos+inputbox.size-STANDARTPICEHEIGHT/4-1), pxlenght(inputbox.ypos+STANDARTPICEHEIGHT-4), pxlenght(STANDARTBORDER));
        SDL_RenderDrawFatLine(renderer,pxlenght(inputbox.xpos+inputbox.size-4), pxlenght(inputbox.ypos+STANDARTPICEHEIGHT/2+4), pxlenght(inputbox.xpos+inputbox.size-STANDARTPICEHEIGHT/4-1), pxlenght(inputbox.ypos+STANDARTPICEHEIGHT-4), pxlenght(STANDARTBORDER));
    }

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
        if (inputbox.cursor_visible){SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255); SDL_RenderDrawFatLine(renderer,pxlenght(inputbox. xpos)+cursorlenpx, pxlenght(inputbox.ypos+STANDARTBORDER), pxlenght(inputbox. xpos)+cursorlenpx, pxlenght(inputbox.ypos+STANDARTPICEHEIGHT-STANDARTBORDER*2+STANDARTBORDER), pxlenght(3));}
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
    return;
}
void update0Button(SDL_Renderer* renderer, button* butptr, int xshift=0, int yshift=0) {
    // 1. "Vybalíme" pointer do lokální proměnné but (původně menu)
    button but = *butptr;
    but.xpos += xshift;
    but.ypos += yshift;
    int w, h;
    if (SDL_QueryTexture(but.texture, NULL, NULL, &w, &h) != 0) {
        // Pokud textura neexistuje nebo je špatná, dáme fail-safe poměr 1:1
        w = 1; h = 1;
    }

    float aspectRatio = (float)w / (float)h;
    float dynamicWidth = (float)STANDARTPICEHEIGHT * aspectRatio;

    // 3. Vykreslení ohraničení (Border) - používáme lokální 'but'
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    int x1 = pxlenght(but.xpos);
    int y1 = pxlenght(but.ypos);
    int x2 = pxlenght(but.xpos + dynamicWidth);
    int y2 = pxlenght(but.ypos + STANDARTPICEHEIGHT);
    int fat = pxlenght(BORDERLINEFATNESS);

    SDL_RenderDrawFatLine(renderer, x1, y1, x2, y1, fat); // top
    SDL_RenderDrawFatLine(renderer, x1, y2, x2, y2, fat); // bottom
    SDL_RenderDrawFatLine(renderer, x1, y1, x1, y2, fat); // left
    SDL_RenderDrawFatLine(renderer, x2, y1, x2, y2, fat); // right

    // 4. Collision Detection (Hover / Click)
    if (mousex < x2 && mousex > x1 && mousey > y1 && mousey < y2) {
        SDL_Rect rect = { x1, y1, pxlenght(dynamicWidth), pxlenght(STANDARTPICEHEIGHT) };
        
        if (clicked) {
            SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
            clickedOnSomething = true;
        } else {
            SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
        }
        SDL_RenderFillRoundRect(renderer, &rect);

        if (clickup) {
            but.function(); // Volání funkce z kopie
            clickedOnSomething = true;
        }
    } else {
        SDL_Rect rect = { x1, y1, pxlenght(dynamicWidth), pxlenght(STANDARTPICEHEIGHT) };
        SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
        SDL_RenderFillRoundRect(renderer, &rect);
    }
    if (but.force_selected){

        SDL_Rect rect = { x1, y1, pxlenght(dynamicWidth), pxlenght(STANDARTPICEHEIGHT) };
        SDL_SetRenderDrawColor(renderer, COLOR_SELECTED, 255);
        SDL_RenderFillRoundRect(renderer, &rect);
    }

    // 5. Vykreslení textury
    displayTex(renderer, but.texture, 
               pxlenght(but.xpos + STANDARTBORDER * 2), 
               pxlenght(but.ypos + STANDARTBORDER), 
               pxlenght(STANDARTPICEHEIGHT - STANDARTBORDER * 2));

    // 6. ZÁPIS ZPĚT: Tohle je to, co dělá tu "legálnost" tvého přístupu
    but.xpos -= xshift; // Vrátíme pozici zpět, aby se změny projevily pouze v rámci této funkce
    but.ypos -= yshift;
    *butptr = but; 
}
void update0Element(SDL_Renderer* renderer, element* elementptr, int xshift=0, int yshift=0){
    if (elementptr->type == ELEMENT_BUTTON) {
        update0Button(renderer, &elementptr->button, xshift, yshift);
    } else if (elementptr->type == ELEMENT_DROPMENU) {
        update0DropMenu(renderer, &elementptr->dropmenu, xshift, yshift);
    } else if (elementptr->type == ELEMENT_TEXTBOX) {
        update0TextBox(renderer, &elementptr->textbox, xshift, yshift);
    } else if (elementptr->type == ELEMENT_NUMBERINPUTBOX) {
        update0NumberInputBox(renderer, &elementptr->numberinputbox, xshift, yshift);
    } else if (elementptr->type == ELEMENT_LABEL) {
        update0Label(renderer, &elementptr->label, xshift, yshift);
    } else if (elementptr->type == ELEMENT_CHECKBOX) {
        update0CheckBox(renderer, &elementptr->checkbox, xshift, yshift);
    } else if (elementptr->type == ELEMENT_SLIDER) {
        update0Slider(renderer, &elementptr->slider, xshift, yshift);
    }
}
intVec2 get_element_pos(element* elementptr){
    if (elementptr->type == ELEMENT_BUTTON) {
        return {elementptr->button.xpos, elementptr->button.ypos};
    } else if (elementptr->type == ELEMENT_DROPMENU) {
        return {elementptr->dropmenu.xpos, elementptr->dropmenu.ypos};
    } else if (elementptr->type == ELEMENT_TEXTBOX) {
        return {elementptr->textbox.xpos, elementptr->textbox.ypos};
    } else if (elementptr->type == ELEMENT_NUMBERINPUTBOX) {
        return {elementptr->numberinputbox.xpos, elementptr->numberinputbox.ypos};
    } else if (elementptr->type == ELEMENT_LABEL) {
        return {elementptr->label.xpos, elementptr->label.ypos};
    } else if (elementptr->type == ELEMENT_CHECKBOX) {
        return {elementptr->checkbox.xpos, elementptr->checkbox.ypos};
    } else if (elementptr->type == ELEMENT_SLIDER) {
        return {elementptr->slider.xpos, elementptr->slider.ypos};
    }
    return {0, 0}; // Default fallback
}
void update0Panel(SDL_Renderer* renderer, panel* panelptr){
    panel pan=*panelptr;
    //background
    renderRect(pxlenght(pan.x), pxlenght(pan.y), pxlenght(pan.w), pxlenght(pan.h), 220,220,220);
    //find out the top size
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
    
    // Calculate scrollable content dimensions
    int scrollbar_area_start_y = pan.y + top_size + STANDARTPICEHEIGHT;
    int scrollbar_area_end_y = pan.y + pan.h - bottom_size - STANDARTPICEHEIGHT;
    int scrollbar_area_height = scrollbar_area_end_y - scrollbar_area_start_y;

    // int total_content_height = (int)pan.scrollable_elements.size() * STANDARTPICEHEIGHT + STANDARTBORDER * 4;
    int total_content_height=0;
    for (auto& e:pan.scrollable_elements){
        if (get_element_pos(&e).y>total_content_height){
            total_content_height=get_element_pos(&e).y+STANDARTPICEHEIGHT+STANDARTBORDER*4;
        }
    }     

    // Calculate scrollbar height and position
    int scrollbar_range = total_content_height - scrollbar_area_height + bottom_size + STANDARTPICEHEIGHT;
    int scrollbar_height = 1;
    int scrollbar_y = scrollbar_area_start_y;
    if (scrollbar_range > 0) {
        scrollbar_height = (scrollbar_area_height * scrollbar_area_height) / total_content_height;
        if (scrollbar_height < 10) scrollbar_height = 10; // Minimum scrollbar height
        scrollbar_y = scrollbar_area_start_y + (pan.scrolled * (scrollbar_area_height - scrollbar_height)) / scrollbar_range;
    }
    
    // Handle scrollbar click
    int scrollbar_x = pan.x + pan.w - STANDARTPICEHEIGHT/4;
    if (clicked && mousex >= pxlenght(scrollbar_x - STANDARTPICEHEIGHT/4) && mousex <= pxlenght(scrollbar_x + STANDARTPICEHEIGHT/4) &&
        mousey >= pxlenght(scrollbar_area_start_y) && mousey <= pxlenght(scrollbar_area_end_y)) {
        // Calculate new scroll position with center at click position
        int click_y_offset = mousey - pxlenght(scrollbar_area_start_y);
        if (scrollbar_range > 0) {
            int center_position = click_y_offset - scrollbar_height / 2;
            pan.scrolled = (center_position * scrollbar_range) / (scrollbar_area_height - scrollbar_height);
            if (pan.scrolled < 0) pan.scrolled = 0;
            if (pan.scrolled > scrollbar_range) pan.scrolled = scrollbar_range;
        }
        clickedOnSomething = true;
    }
    
    //draw scrollable
    for (auto& element : pan.scrollable_elements) {
        if (get_element_pos(&element).y-pan.scrolled > -STANDARTBORDER-STANDARTPICEHEIGHT && get_element_pos(&element).y-pan.scrolled < pan.h-STANDARTPICEHEIGHT*2-STANDARTBORDER*2) {
            update0Element(renderer, &element, pan.x+STANDARTBORDER, pan.y-pan.scrolled+STANDARTBORDER+top_size+STANDARTPICEHEIGHT);
        }
    }
    //draw top and bottom
    renderRect(pxlenght(pan.x), pxlenght(pan.y), pxlenght(pan.w), pxlenght(top_size+STANDARTPICEHEIGHT), COLOR_BACKGROUND);
    renderRect(pxlenght(pan.x), pxlenght(pan.h+pan.y-bottom_size-STANDARTPICEHEIGHT), pxlenght(pan.w), pxlenght(bottom_size+STANDARTPICEHEIGHT), COLOR_BACKGROUND);
    for (auto& element : pan.top_elements) {
        update0Element(renderer, &element, pan.x+STANDARTBORDER, pan.y+STANDARTBORDER);
    }
    for (auto& element : pan.bottom_elements) {
        update0Element(renderer, &element, pan.x+STANDARTBORDER, pan.y+STANDARTBORDER-STANDARTPICEHEIGHT-bottom_size+pan.h);
    }
    

    // Draw scrollbar
    if (scrollbar_range > 0) {
        // Check scrollbar interaction states
        bool scrollbar_hovered = mousex >= pxlenght(scrollbar_x - STANDARTPICEHEIGHT/4) && mousex <= pxlenght(scrollbar_x + STANDARTPICEHEIGHT/4) &&
                                mousey >= pxlenght(scrollbar_y) && mousey <= pxlenght(scrollbar_y + scrollbar_height);
        bool scrollbar_clicked = scrollbar_hovered && clicked;
        
        // Set scrollbar color based on state
        if (scrollbar_clicked) {
            renderRect(pxlenght(scrollbar_x - STANDARTPICEHEIGHT/4), pxlenght(scrollbar_y), pxlenght(STANDARTPICEHEIGHT/2), pxlenght(scrollbar_height), COLOR_CLICKED);
        } else if (scrollbar_hovered) {
            renderRect(pxlenght(scrollbar_x - STANDARTPICEHEIGHT/4), pxlenght(scrollbar_y), pxlenght(STANDARTPICEHEIGHT/2), pxlenght(scrollbar_height), COLOR_HOVER);
        } else {
            renderRect(pxlenght(scrollbar_x - STANDARTPICEHEIGHT/4), pxlenght(scrollbar_y), pxlenght(STANDARTPICEHEIGHT/2), pxlenght(scrollbar_height), COLOR_BACKGROUND);
        }
        
    }

    //scroll bar border
    SDL_SetRenderDrawColor(renderer, COLOR_FOREGROUND, 255);

    SDL_RenderDrawFatLine(renderer, pxlenght(pan.x + pan.w-STANDARTPICEHEIGHT/2), pxlenght(pan.y+top_size+STANDARTPICEHEIGHT), pxlenght(pan.x + pan.w-STANDARTPICEHEIGHT/2), pxlenght(pan.y + pan.h-bottom_size-STANDARTPICEHEIGHT), pxlenght(BORDERLINEFATNESS)); // right

    // Draw border around panel
    SDL_SetRenderDrawColor(renderer, COLOR_FOREGROUND, 255);
    SDL_RenderDrawFatLine(renderer, pxlenght(pan.x), pxlenght(pan.y), pxlenght(pan.x + pan.w), pxlenght(pan.y), pxlenght(BORDERLINEFATNESS)); // top
    SDL_RenderDrawFatLine(renderer, pxlenght(pan.x), pxlenght(pan.y + pan.h), pxlenght(pan.x + pan.w), pxlenght(pan.y + pan.h), pxlenght(BORDERLINEFATNESS)); // bottom
    SDL_RenderDrawFatLine(renderer, pxlenght(pan.x), pxlenght(pan.y), pxlenght(pan.x), pxlenght(pan.y + pan.h), pxlenght(BORDERLINEFATNESS)); // left
    SDL_RenderDrawFatLine(renderer, pxlenght(pan.x + pan.w), pxlenght(pan.y), pxlenght(pan.x + pan.w), pxlenght(pan.y + pan.h), pxlenght(BORDERLINEFATNESS)); // right
    *panelptr=pan;
    return;
}