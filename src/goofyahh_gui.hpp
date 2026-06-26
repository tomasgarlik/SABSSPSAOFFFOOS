#include "includes.hpp"

// Pomocná ultra-goofy funkce pro vykreslení hardcore 3D "Bevel" okraje, co dává ten pravej 2000 office stín
void drawGoofy3DBevel(SDL_Renderer* renderer, int x, int y, int w, int h, bool sunken, int fatness) {
    // Když je "sunken" (zamáčknuté tlačítko/textbox), barvy stínů se prohodí
    int top_left_color = sunken ? 80 : 255;      // Světlá nahoře/vlevo (nebo stín, když je zamáčknuto)
    int bottom_right_color = sunken ? 255 : 60;  // Tmavá dole/vpravo

    // Vykreslíme tlusté 3D stíny po vrstvách
    for (int i = 0; i < fatness; i++) {
        // Horní linka
        SDL_SetRenderDrawColor(renderer, top_left_color, top_left_color, top_left_color, 255);
        SDL_RenderDrawLine(renderer, x + i, y + i, x + w - i, y + i);
        // Levá linka
        SDL_RenderDrawLine(renderer, x + i, y + i, x + i, y + h - i);

        // Dolní linka
        SDL_SetRenderDrawColor(renderer, bottom_right_color, bottom_right_color, bottom_right_color, 255);
        SDL_RenderDrawLine(renderer, x + i, y + h - 1 - i, x + w - i, y + h - 1 - i);
        // Pravá linka
        SDL_RenderDrawLine(renderer, x + w - 1 - i, y + i, x + w - 1 - i, y + h - i);
    }
}

void update1DropMenu(SDL_Renderer* renderer, dropmenu* menuptr, int xshift=0, int yshift=0){
    dropmenu menu=*menuptr;
    menu.xpos+=xshift;
    menu.ypos+=yshift;
    if (menu.opened){
        SDL_Rect rect={pxlenght(menu.xpos),pxlenght(menu.ypos),pxlenght(menu.topwidth), pxlenght(STANDARTPICEHEIGHT)};
        // Goofy Hover barva - Magická fialová
        SDL_SetRenderDrawColor(renderer, 180, 50, 230, 255);
        SDL_RenderFillRoundRect(renderer, &rect);
        
        SDL_Rect rect0={pxlenght(menu.xpos),pxlenght(menu.ypos+STANDARTPICEHEIGHT),pxlenght(menu.width), pxlenght(STANDARTPICEHEIGHT*menu.lenght)};
        // Pozadí dropdownu - Retro slizově zelená/šedá
        SDL_SetRenderDrawColor(renderer, 40, 45, 50, 255);
        SDL_RenderFillRoundRect(renderer, &rect0);
        
        for (int i=0;i<menu.lenght;i++){
            if (mousex<pxlenght(menu.xpos+menu.width) && mousex>pxlenght(menu.xpos) && mousey<pxlenght(menu.ypos+STANDARTPICEHEIGHT*(i+2))){
                SDL_Rect rect1={pxlenght(menu.xpos+STANDARTBORDER),pxlenght(menu.ypos+STANDARTPICEHEIGHT*(i+1)+STANDARTBORDER),pxlenght(menu.width-STANDARTBORDER*2), pxlenght(STANDARTPICEHEIGHT-STANDARTBORDER*2)};
                // Výběr položky v menu: Hot Pink oheň!
                SDL_SetRenderDrawColor(renderer, 255, 0, 128, 255);
                if (clicked){ SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); } // Kliknutí zežloutne
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
        
        // Rozdělovníky v menu - 3D oddělovače
        for (int i=0;i<menu.lenght-1;i++){
            SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
            SDL_RenderDrawLine(renderer, pxlenght(menu.xpos+STANDARTBORDER), pxlenght(menu.ypos+STANDARTPICEHEIGHT*(i+2)), pxlenght(menu.xpos+menu.width-STANDARTBORDER), pxlenght(menu.ypos+STANDARTPICEHEIGHT*(i+2)));
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawLine(renderer, pxlenght(menu.xpos+STANDARTBORDER), pxlenght(menu.ypos+STANDARTPICEHEIGHT*(i+2)+1), pxlenght(menu.xpos+menu.width-STANDARTBORDER), pxlenght(menu.ypos+STANDARTPICEHEIGHT*(i+2)+1));
        }
        
        // Komplet 3D Bevel kolem celého vyskočeného dropdownu
        drawGoofy3DBevel(renderer, rect0.x, rect0.y, rect0.w, rect0.h, false, pxlenght(BORDERLINEFATNESS));
        clickedOnSomething=true;
    }
    
    // Vykreslení 3D title baru menu tlačítka
    int title_x = pxlenght(menu.xpos);
    int title_y = pxlenght(menu.ypos);
    int title_w = pxlenght(menu.topwidth);
    int title_h = pxlenght(STANDARTPICEHEIGHT);
    drawGoofy3DBevel(renderer, title_x, title_y, title_w, title_h, menu.opened, pxlenght(BORDERLINEFATNESS));

    if (menu.opened && mousex<pxlenght(menu.xpos+menu.width) && mousex>pxlenght(menu.xpos) && mousey<pxlenght(menu.ypos+STANDARTPICEHEIGHT*(menu.lenght+1))){menu.opened=true;clickedOnSomething=true;}
    else if (clicked && menu.opened && !(mousex<pxlenght(menu.xpos+menu.width) && mousex>pxlenght(menu.xpos) && mousey<pxlenght(menu.ypos+STANDARTPICEHEIGHT*(menu.lenght+1)))){menu.opened=false;clickedOnSomething=true;}
    
    if (mousex<pxlenght(menu.topwidth)+pxlenght(menu.xpos) && mousex>pxlenght(menu.xpos) && mousey>pxlenght(menu.ypos) && mousey<pxlenght(menu.ypos+STANDARTPICEHEIGHT)){
        SDL_Rect rect={pxlenght(menu.xpos)+2, pxlenght(menu.ypos)+2, pxlenght(menu.topwidth)-4, pxlenght(STANDARTPICEHEIGHT)-4};
        if (clicked){
            SDL_SetRenderDrawColor(renderer, 130, 130, 130, 255); // Šedá zatlačená klasika
            clickedOnSomething=true;
        } else {
            SDL_SetRenderDrawColor(renderer, 0, 160, 255, 255); // Diskotéková modř při hoveru
        }
        SDL_RenderFillRoundRect(renderer, &rect);
        if (clickup){
            menu.opened=true;
            clickedOnSomething=true;
        }
    } else {
        SDL_Rect rect={pxlenght(menu.xpos)+2, pxlenght(menu.ypos)+2, pxlenght(menu.topwidth)-4, pxlenght(STANDARTPICEHEIGHT)-4};
        SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255); // Standardní hnusná šedá
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

void update1TextBox(SDL_Renderer* renderer, textbox* inputboxptr, int xshift=0, int yshift=0){
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
        
        // Aktivní vstup: Propadlý hard-core 3D Bevel s toxicky žlutobílým vnitřkem
        drawGoofy3DBevel(renderer, rect.x, rect.y, rect.w, rect.h, true, 2);
        SDL_Rect rect2={rect.x+2, rect.y+2, rect.w-4, rect.h-4};
        SDL_SetRenderDrawColor(renderer, 255, 255, 200, 255); // Retro nažloutlý papír
        SDL_RenderFillRoundRect(renderer, &rect2, pxlenght(3));
        
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
        
        // Zpracování kláves (ponecháno beze změn pro 100% funkčnost)
        if (modkey & KMOD_SHIFT){
            if (specialkey=='l' && inputbox.cursorpos>0){
                if (inputbox.start_of_selected==-1 && inputbox.end_of_selected==-1){
                    inputbox.start_of_selected=inputbox.cursorpos-1;
                    inputbox.end_of_selected=inputbox.cursorpos;
                } else { if (inputbox.cursorpos==inputbox.start_of_selected){ inputbox.start_of_selected--; } else { inputbox.end_of_selected--; } }
                inputbox.cursorpos--; inputbox.cursor_visible=true;
            } else if (specialkey=='r' && inputbox.cursorpos<stringlen(inputbox.text)){
                if (inputbox.start_of_selected==-1 && inputbox.end_of_selected==-1){
                    inputbox.start_of_selected=inputbox.cursorpos; inputbox.end_of_selected=inputbox.cursorpos+1;
                } else { if (inputbox.cursorpos==inputbox.start_of_selected){ inputbox.start_of_selected++; } else { inputbox.end_of_selected++; } }
                inputbox.cursorpos++; inputbox.cursor_visible=true;
            } else if (specialkey=='b'){ inputbox.text=""; inputbox.cursorpos=0; inputbox.cursor_visible=true; }
        } else {
            if (specialkey=='l' && inputbox.cursorpos>0){ inputbox.cursorpos-=1; inputbox.start_of_selected=-1; inputbox.end_of_selected=-1; inputbox.cursor_visible=true; }
            else if (specialkey=='r' && inputbox.cursorpos<stringlen(inputbox.text)){ inputbox.cursorpos+=1; inputbox.start_of_selected=-1; inputbox.end_of_selected=-1; inputbox.cursor_visible=true; }
            else if (specialkey=='b'){
                if (inputbox.start_of_selected==-1 && inputbox.end_of_selected==-1){ replace_text_range(&inputbox.text, inputbox.cursorpos-1, inputbox.cursorpos, ""); }
                else { replace_text_range(&inputbox.text, inputbox.start_of_selected, inputbox.end_of_selected, ""); inputbox.start_of_selected=-1; inputbox.end_of_selected=-1; }
                inputbox.cursorpos--; inputbox.cursor_visible=true;
            }
        }
        
        // Párty diskotékový kurzor - mění barvy (červená/modrá podle času)
        if (inputbox.cursor_visible){
            Uint32 ticks = SDL_GetTicks();
            SDL_SetRenderDrawColor(renderer, (ticks % 255), 0, 255 - (ticks % 255), 255); 
            SDL_RenderDrawFatLine(renderer, rect.x + cursorlenpx, rect.y + STANDARTBORDER, rect.x + cursorlenpx, rect.y + inputbox.textsize + STANDARTBORDER, pxlenght(4));
        }
    } else {
        // Neaktivní textbox: Klasický zapadlý stín, sněhově bílá
        drawGoofy3DBevel(renderer, rect.x, rect.y, rect.w, rect.h, true, 2);
        SDL_Rect rect2={rect.x+2, rect.y+2, rect.w-4, rect.h-4};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRoundRect(renderer, &rect2, pxlenght(3));
    }
    
    if (clic){
        SDL_Point mousepoint={mousex,mousey};
        if (SDL_PointInRect(&mousepoint,&rect)){
            if (inputbox.active){
                inputbox.start_of_selected=-1; inputbox.end_of_selected=-1;
                for (i = 0; i < stringlen(inputbox.text) && mousex - rect.x >= (int)((*(proportions + i)) * (float)(rect.w - STANDARTBORDER * 2)); i++){inputbox.cursorpos = i;}
                inputbox.cursorpos=i;
            } else { inputbox.start_of_selected=0; inputbox.end_of_selected=stringlen(inputbox.text); }
            inputbox.active=true; clickedOnSomething=true;
        } else { inputbox.active=false; inputbox.start_of_selected=-1; inputbox.end_of_selected=-1; }
    }

    if (inputbox.start_of_selected == inputbox.end_of_selected){ inputbox.start_of_selected=-1; inputbox.end_of_selected=-1; }
    displayTex(renderer,texttex,pxlenght(inputbox.xpos+STANDARTBORDER-(getTextureWidth(texttex,inputbox.textsize)+STANDARTBORDER*2)/2), pxlenght(inputbox.ypos+STANDARTBORDER), pxlenght(inputbox.textsize));
    SDL_DestroyTexture(texttex);
    inputbox.xpos-=xshift; inputbox.ypos-=yshift;
    if (inputbox.origin_left){ inputbox.xpos-=inputbox.textwidth/2; }
    *inputboxptr=inputbox;
    return;
}

void update1Label(SDL_Renderer* renderer, label* labelptr, int xshift=0, int yshift=0){
    label inputbox=*labelptr;
    inputbox.xpos+=xshift;
    inputbox.ypos+=yshift;
    std::string finaltext=inputbox.text;
    SDL_Texture* texttex=createTextTexture(renderer,finaltext);
    inputbox.textwidth=getTextureWidth(texttex,inputbox.textsize)+STANDARTBORDER*2;
    if (inputbox.origin_left){
        inputbox.xpos+=inputbox.textwidth/2;
    }
    // Labely necháme ploché, ale hodíme pod ně ultra-goofy stín jako ve WordArtu z roku 2000!
    int lx = pxlenght(inputbox.xpos+STANDARTBORDER-(getTextureWidth(texttex,inputbox.textsize)+STANDARTBORDER*2)/2);
    int ly = pxlenght(inputbox.ypos+STANDARTBORDER);
    
    // Černý stín o 2 pixely vedle
    SDL_SetTextureColorMod(texttex, 0, 0, 0);
    displayTex(renderer,texttex, lx + 2, ly + 2, pxlenght(inputbox.textsize));
    
    // Klasický svítivě bílo-modrý text navrchu
    SDL_SetTextureColorMod(texttex, 0, 255, 255); 
    displayTex(renderer,texttex, lx, ly, pxlenght(inputbox.textsize));
    
    SDL_DestroyTexture(texttex);
    inputbox.xpos-=xshift; inputbox.ypos-=yshift;
    if (inputbox.origin_left){ inputbox.xpos-=inputbox.textwidth/2; }
    *labelptr=inputbox;
    return;
}

void update1CheckBox(SDL_Renderer* renderer, checkbox* boxptr, int xshift=0, int yshift=0){
    checkbox box=*boxptr;
    box.xpos+=xshift;
    box.ypos+=yshift;
    box.just_changed=false;

    int boxSize = STANDARTPICEHEIGHT; 
    SDL_Rect rect={pxlenght(box.xpos), pxlenght(box.ypos), pxlenght(boxSize), pxlenght(boxSize)};
    
    // Checkbox dostane parádní hluboký 3D profil
    drawGoofy3DBevel(renderer, rect.x, rect.y, rect.w, rect.h, true, 2);
    
    if (mousex>pxlenght(box.xpos) && mousex<pxlenght(box.xpos+boxSize) && mousey>pxlenght(box.ypos) && mousey<pxlenght(box.ypos+boxSize) && clicked){
        box.checked=!box.checked;   
        box.just_changed=true;
        clicked = false; 
    }   

    // Výplň vnitřku checkboxu
    SDL_Rect innerRect = {rect.x+2, rect.y+2, rect.w-4, rect.h-4};
    if (box.checked){
        // Když je zaškrtnuto: Radioaktivní jedovatá zeleň!
        SDL_SetRenderDrawColor(renderer, 50, 255, 50, 255);
    } else {
        // Nezaškrtnuto: Černá prázdnota matrixu
        SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255);
    }

    // Hover efekt pro checkbox: Sytě oranžový plamen
    if (mousex>pxlenght(box.xpos) && mousex<pxlenght(box.xpos+boxSize) && mousey>pxlenght(box.ypos) && mousey<pxlenght(box.ypos+boxSize)){
        if (box.checked){
            SDL_SetRenderDrawColor(renderer, 100, 255, 100, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 100, 0, 255);
        }
    }
    SDL_RenderFillRect(renderer, &innerRect);

    // Pokud je checked, práskneme dovnitř ještě svítivě žlutej retro křížek "X" z tlustých čar
    if (box.checked) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderDrawFatLine(renderer, innerRect.x + 3, innerRect.y + 3, innerRect.x + innerRect.w - 3, innerRect.y + innerRect.h - 3, 3);
        SDL_RenderDrawFatLine(renderer, innerRect.x + innerRect.w - 3, innerRect.y + 3, innerRect.x + 3, innerRect.y + innerRect.h - 3, 3);
    }

    // Text vedle checkboxu
    displayTex(renderer, box.title, pxlenght(box.xpos + boxSize + STANDARTBORDER), pxlenght(box.ypos), pxlenght(boxSize));

    box.xpos-=xshift; box.ypos-=yshift;
    *boxptr=box;
    return;
}

void update1Slider(SDL_Renderer* renderer, slider* sliderptr, int xshift=0, int yshift=0) {
    slider inputslider = *sliderptr;
    inputslider.xpos += xshift;
    inputslider.ypos += yshift;
    char text_buf[32];

    int value_text_size = STANDARTPICEHEIGHT - STANDARTBORDER * 2;
    sprintf(text_buf, "%.1f", inputslider.value);
    SDL_Texture* val_tex = createTextTexture(renderer, text_buf);
    int val_w = getTextureWidth(val_tex, value_text_size);
    
    // Hodnota před sliderem (Zářivě žlutá)
    SDL_SetTextureColorMod(val_tex, 255, 255, 0);
    displayTex(renderer, val_tex, pxlenght(inputslider.xpos), pxlenght(inputslider.ypos + STANDARTBORDER), pxlenght(value_text_size));
    SDL_DestroyTexture(val_tex);

    int slider_data_x = inputslider.xpos + val_w + STANDARTBORDER;
    int slider_data_w = inputslider.width - (val_w + STANDARTBORDER);
    if (slider_data_w < 10) slider_data_w = 10;

    int handle_h = STANDARTPICEHEIGHT - STANDARTBORDER * 2;
    int handle_w = handle_h / 2; 
    if (handle_w < 1) handle_w = 1;

    float range = inputslider.max - inputslider.min;
    float percentage = (range > 0.0f) ? ((inputslider.value - inputslider.min) / range) : 0.0f;
    
    int travel_width = slider_data_w - handle_w;
    int handle_x = slider_data_x + (int)(percentage * travel_width);
    int handle_y = inputslider.ypos + STANDARTBORDER;

    bool hover = (mousex >= handle_x && mousex <= handle_x + handle_w && mousey >= handle_y && mousey <= handle_y + handle_h);

    if (clicked) { if (hover) { inputslider.moving = true; } } else { inputslider.moving = false; }

    if (inputslider.moving) {
        int local_mx = mousex - slider_data_x - (handle_w / 2);
        if (local_mx < 0) local_mx = 0;
        if (local_mx > travel_width) local_mx = travel_width;
        float new_pct = (float)local_mx / (float)travel_width;
        inputslider.value = inputslider.min + (new_pct * range);
    }

    // Osa slideru: Uděláme ji jako vyhloubenou 3D drážku (černá vrchní linka, bílá spodní)
    int line_y = inputslider.ypos + (STANDARTPICEHEIGHT / 2);
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderDrawLine(renderer, pxlenght(slider_data_x), pxlenght(line_y), pxlenght(slider_data_x + slider_data_w), pxlenght(line_y));
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer, pxlenght(slider_data_x), pxlenght(line_y + 1), pxlenght(slider_data_x + slider_data_w), pxlenght(line_y + 1));

    // Vykreslení jezdce: Totální kovový 3D Bevel knoflík z Windows 95!
    drawGoofy3DBevel(renderer, pxlenght(handle_x), pxlenght(handle_y), pxlenght(handle_w), pxlenght(handle_h), inputslider.moving, 2);
    SDL_Rect handle_rect = { pxlenght(handle_x)+2, pxlenght(handle_y)+2, pxlenght(handle_w)-4, pxlenght(handle_h)-4 };
    
    if (inputslider.moving) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Při tažení svítí zeleně!
    } else if (hover) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255); // Hover je fialový blesk!
    } else {
        SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255); // Klasická šedá hliníková klávesa
    }
    SDL_RenderFillRect(renderer, &handle_rect);

    // Malé popisky MIN a MAX pod sliderem
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

    inputslider.xpos -= xshift; inputslider.ypos -= yshift;
    *sliderptr = inputslider;
}

void update1NumberInputBox(SDL_Renderer* renderer, numberinputbox* inputboxptr, int xshift=0, int yshift=0){
    numberinputbox inputbox=*inputboxptr;
    inputbox.xpos+=xshift;
    inputbox.ypos+=yshift;
    std::string finaltext=inputbox.text;
    SDL_Texture* texttex=createTextTexture(renderer,finaltext, inputbox.start_of_selected, inputbox.end_of_selected);
    SDL_Rect rect={pxlenght(inputbox.xpos), pxlenght(inputbox.ypos), pxlenght((inputbox.size)), pxlenght(STANDARTPICEHEIGHT)};
    
    // Zapadlý 3D box pro číslo
    drawGoofy3DBevel(renderer, rect.x, rect.y, rect.w, rect.h, true, 2);
    SDL_Rect rect2={rect.x+2, rect.y+2, rect.w-4, rect.h-4};
    SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
    SDL_RenderFillRoundRect(renderer, &rect2, pxlenght(3));
    
    // Tlačítka Nahoru/Dolů na boku dostanou vlastní 3D Bevel efekt!
    int btn_w = pxlenght(STANDARTPICEHEIGHT/2);
    int btn_x = rect.x + rect.w - btn_w;
    int btn_h = rect.h / 2;
    
    // Detekce, na který čudlík míříme
    bool hover_up = (mousex > btn_x && mousex < rect.x + rect.w && mousey > rect.y && mousey < rect.y + btn_h);
    bool hover_down = (mousex > btn_x && mousex < rect.x + rect.w && mousey > rect.y + btn_h && mousey < rect.y + rect.h);

    // Horní šipka (Tlačítko nahoru)
    drawGoofy3DBevel(renderer, btn_x, rect.y, btn_w, btn_h, clicked && hover_up, 1);
    SDL_Rect up_rect = {btn_x+1, rect.y+1, btn_w-2, btn_h-2};
    SDL_SetRenderDrawColor(renderer, hover_up ? 200 : 170, 170, 170, 255);
    SDL_RenderFillRect(renderer, &up_rect);

    // Spodní šipka (Tlačítko dolů)
    drawGoofy3DBevel(renderer, btn_x, rect.y + btn_h, btn_w, btn_h, clicked && hover_down, 1);
    SDL_Rect down_rect = {btn_x+1, rect.y + btn_h + 1, btn_w-2, btn_h-2};
    SDL_SetRenderDrawColor(renderer, hover_down ? 200 : 170, 170, 170, 255);
    SDL_RenderFillRect(renderer, &down_rect);

    // Kreslení primitivních retro šipek (pár pixelů)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    // Šipka nahoru
    SDL_RenderDrawLine(renderer, btn_x + btn_w/2, rect.y + 4, btn_x + 3, rect.y + btn_h - 3);
    SDL_RenderDrawLine(renderer, btn_x + btn_w/2, rect.y + 4, btn_x + btn_w - 4, rect.y + btn_h - 3);
    // Šipka dolů
    SDL_RenderDrawLine(renderer, btn_x + btn_w/2, rect.y + btn_h + btn_h - 4, btn_x + 3, rect.y + btn_h + 3);
    SDL_RenderDrawLine(renderer, btn_x + btn_w/2, rect.y + btn_h + btn_h - 4, btn_x + btn_w - 4, rect.y + btn_h + 3);

    // Obsluha vstupů a kurzoru (ponecháno přesně dle originálu pro 100% matiku)
    if (inputbox.active){
        if (SDL_GetTicks()-inputbox.last_time_cursor_blink>CURSORPERIOD){
            inputbox.cursor_visible=!inputbox.cursor_visible; inputbox.last_time_cursor_blink=SDL_GetTicks();
        }
        if (inputbox.cursorpos > stringlen(inputbox.text)) inputbox.cursorpos = stringlen(inputbox.text);
        if (inputbox.cursorpos < 0) inputbox.cursorpos = 0;
        int cursorlenpx=(int)((*(proportions+inputbox.cursorpos))*(pxlenght((getTextureWidth(texttex,STANDARTPICEHEIGHT-STANDARTBORDER*2)+STANDARTBORDER*2))-STANDARTBORDER))+STANDARTBORDER;
        if (stringlen(typedChar)!=0 && isNumber(typedChar)){
            if (inputbox.start_of_selected==-1 && inputbox.end_of_selected==-1){ inputbox.text+=typedChar; inputbox.cursorpos+=stringlen(typedChar); } 
            else { replace_text_range(&inputbox.text, inputbox.start_of_selected, inputbox.end_of_selected, typedChar); inputbox.cursorpos=inputbox.start_of_selected+1; inputbox.start_of_selected=-1; inputbox.end_of_selected=-1; }
            inputbox.cursor_visible=true;
        }
        // Shift a navigace
        if (modkey & KMOD_SHIFT){
            if (specialkey=='l' && inputbox.cursorpos>0){
                if (inputbox.start_of_selected==-1 && inputbox.end_of_selected==-1){ inputbox.start_of_selected=inputbox.cursorpos-1; inputbox.end_of_selected=inputbox.cursorpos; } 
                else { if (inputbox.cursorpos==inputbox.start_of_selected){ inputbox.start_of_selected--; } else { inputbox.end_of_selected--; } }
                inputbox.cursorpos--; inputbox.cursor_visible=true;
            } else if (specialkey=='r' && inputbox.cursorpos<stringlen(inputbox.text)){
                if (inputbox.start_of_selected==-1 && inputbox.end_of_selected==-1){ inputbox.start_of_selected=inputbox.cursorpos; inputbox.end_of_selected=inputbox.cursorpos+1; } 
                else { if (inputbox.cursorpos==inputbox.start_of_selected){ inputbox.start_of_selected++; } else { inputbox.end_of_selected++; } }
                inputbox.cursorpos++; inputbox.cursor_visible=true;
            } else if (specialkey=='b'){ inputbox.text=""; inputbox.cursorpos=0; inputbox.cursor_visible=true; }
        } else {
            if (specialkey=='l' && inputbox.cursorpos>0){ inputbox.cursorpos-=1; inputbox.start_of_selected=-1; inputbox.end_of_selected=-1; inputbox.cursor_visible=true; }
            else if (specialkey=='r' && inputbox.cursorpos<stringlen(inputbox.text)){ inputbox.cursorpos+=1; inputbox.start_of_selected=-1; inputbox.end_of_selected=-1; inputbox.cursor_visible=true; }
            else if (specialkey=='b'){
                if (inputbox.start_of_selected==-1 && inputbox.end_of_selected==-1){ replace_text_range(&inputbox.text, inputbox.cursorpos-1, inputbox.cursorpos, ""); }
                else { replace_text_range(&inputbox.text, inputbox.start_of_selected, inputbox.end_of_selected, ""); inputbox.start_of_selected=-1; inputbox.end_of_selected=-1; }
                inputbox.cursorpos--; inputbox.cursor_visible=true;
            }
        }
        if (inputbox.cursor_visible){ SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); SDL_RenderDrawFatLine(renderer, rect.x+cursorlenpx, rect.y+STANDARTBORDER, rect.x+cursorlenpx, rect.y+STANDARTPICEHEIGHT-STANDARTBORDER, pxlenght(3)); }
    }
    
    if (clic){
        SDL_Point mousepoint={mousex,mousey};
        if (SDL_PointInRect(&mousepoint,&rect)){
            if (mousex > btn_x){
                if (mousey > rect.y + btn_h){ inputbox.number--; inputbox.text=std::to_string(inputbox.number); } 
                else { inputbox.number++; inputbox.text=std::to_string(inputbox.number); }
            } else {
                if (inputbox.active){
                    inputbox.start_of_selected=-1; inputbox.end_of_selected=-1;
                    for (i=0;mousex-rect.x>=(int)((*(proportions+i))*(float)(pxlenght((getTextureWidth(texttex,STANDARTPICEHEIGHT-STANDARTBORDER*2)+STANDARTBORDER*2))-STANDARTBORDER*2));i++)
                    inputbox.cursorpos=i;
                } else { inputbox.start_of_selected=0; inputbox.end_of_selected=stringlen(inputbox.text); }
                inputbox.active=true;
            }
            clickedOnSomething=true;
        } else { inputbox.active=false; inputbox.start_of_selected=-1; inputbox.end_of_selected=-1; }
    }

    if (inputbox.start_of_selected == inputbox.end_of_selected){ inputbox.start_of_selected=-1; inputbox.end_of_selected=-1; }
    displayTex(renderer,texttex,pxlenght(inputbox.xpos+STANDARTBORDER), pxlenght(inputbox.ypos+STANDARTBORDER), pxlenght(STANDARTPICEHEIGHT-STANDARTBORDER*2));
    SDL_DestroyTexture(texttex);
    inputbox.number=stringToInt(inputbox.text);
    if (inputbox.number>inputbox.maxnum){inputbox.number=inputbox.maxnum; inputbox.text=std::to_string(inputbox.maxnum);}
    if (inputbox.number<0){inputbox.number=0; inputbox.text="0";}
    inputbox.xpos-=xshift; inputbox.ypos-=yshift;
    *inputboxptr=inputbox;
    return;
}

void update1Button(SDL_Renderer* renderer, button* butptr, int xshift=0, int yshift=0) {
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
    int total_w = x2 - x1;
    int total_h = y2 - y1;

    bool inside = (mousex < x2 && mousex > x1 && mousey > y1 && mousey < y2);

    // Kreslení nefalšovaného masivního 3D Bevel tlačítka!
    drawGoofy3DBevel(renderer, x1, y1, total_w, total_h, (inside && clicked) || but.force_selected, pxlenght(BORDERLINEFATNESS));

    SDL_Rect rect = { x1 + 2, y1 + 2, total_w - 4, total_h - 4 };
    
    // Logika barev na základě interakce
    if (inside) {
        if (clicked) {
            SDL_SetRenderDrawColor(renderer, 140, 140, 140, 255); // Zatlačené, tmavé
            clickedOnSomething = true;
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255); // Hover: Neonová fuksie!
        }
        if (clickup) {
            but.function();
            clickedOnSomething = true;
        }
    } else {
        SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255); // Default: Šedá cementová klasika
    }
    
    if (but.force_selected){
        SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255); // Vybrané tlačítko svítí azurově!
    }
    SDL_RenderFillRoundRect(renderer, &rect);

    // Kreslení samotného textu/ikonky (posune se o 1px dolů a doprava při stisku pro dokonalou 3D iluzi)
    int push = ((inside && clicked) || but.force_selected) ? 2 : 0;
    displayTex(renderer, but.texture, 
               pxlenght(but.xpos + STANDARTBORDER * 2) + push, 
               pxlenght(but.ypos + STANDARTBORDER) + push, 
               pxlenght(STANDARTPICEHEIGHT - STANDARTBORDER * 2));

    but.xpos -= xshift; but.ypos -= yshift;
    *butptr = but; 
}

void update1Element(SDL_Renderer* renderer, element* elementptr, int xshift=0, int yshift=0){
    if (elementptr->type == ELEMENT_BUTTON) { update1Button(renderer, &elementptr->button, xshift, yshift); } 
    else if (elementptr->type == ELEMENT_DROPMENU) { update1DropMenu(renderer, &elementptr->dropmenu, xshift, yshift); } 
    else if (elementptr->type == ELEMENT_TEXTBOX) { update1TextBox(renderer, &elementptr->textbox, xshift, yshift); } 
    else if (elementptr->type == ELEMENT_NUMBERINPUTBOX) { update1NumberInputBox(renderer, &elementptr->numberinputbox, xshift, yshift); } 
    else if (elementptr->type == ELEMENT_LABEL) { update1Label(renderer, &elementptr->label, xshift, yshift); } 
    else if (elementptr->type == ELEMENT_CHECKBOX) { update1CheckBox(renderer, &elementptr->checkbox, xshift, yshift); } 
    else if (elementptr->type == ELEMENT_SLIDER) { update1Slider(renderer, &elementptr->slider, xshift, yshift); }
}



void update1Panel(SDL_Renderer* renderer, panel* panelptr){
    panel pan=*panelptr;
    
    // Hlavní pozadí panelu: Prvorepubliková ocelová šedomodrá, žádná nuda!
    renderRect(pxlenght(pan.x), pxlenght(pan.y), pxlenght(pan.w), pxlenght(pan.h), 160, 175, 185);
    
    int top_size=0;
    for (auto& element : pan.top_elements) { if (get_element_pos(&element).y > top_size) { top_size = get_element_pos(&element).y; } }
    int bottom_size=0;
    for (auto& element : pan.bottom_elements) { if (get_element_pos(&element).y > bottom_size) { bottom_size = get_element_pos(&element).y; } }
    top_size+=STANDARTBORDER+STANDARTBORDER;
    bottom_size+=STANDARTBORDER+STANDARTBORDER;
    
    int scrollbar_area_start_y = pan.y + top_size + STANDARTPICEHEIGHT;
    int scrollbar_area_end_y = pan.y + pan.h - bottom_size - STANDARTPICEHEIGHT;
    int scrollbar_area_height = scrollbar_area_end_y - scrollbar_area_start_y;

    int total_content_height=0;
    for (auto& e:pan.scrollable_elements){
        if (get_element_pos(&e).y>total_content_height){ total_content_height=get_element_pos(&e).y+STANDARTPICEHEIGHT+STANDARTBORDER*4; }
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
    
    // Vykreslení vnitřních posuvných prvků
    for (auto& element : pan.scrollable_elements) {
        if (get_element_pos(&element).y-pan.scrolled > -STANDARTBORDER-STANDARTPICEHEIGHT && get_element_pos(&element).y-pan.scrolled < pan.h-STANDARTPICEHEIGHT*2-STANDARTBORDER*2) {
            update0Element(renderer, &element, pan.x+STANDARTBORDER, pan.y-pan.scrolled+STANDARTBORDER+top_size+STANDARTPICEHEIGHT);
        }
    }
    
    // Top a Bottom lišty - Temně šedé barové panely
    renderRect(pxlenght(pan.x), pxlenght(pan.y), pxlenght(pan.w), pxlenght(top_size+STANDARTPICEHEIGHT), 70, 75, 80);
    renderRect(pxlenght(pan.x), pxlenght(pan.h+pan.y-bottom_size-STANDARTPICEHEIGHT), pxlenght(pan.w), pxlenght(bottom_size+STANDARTPICEHEIGHT), 70, 75, 80);
    
    // 3D Bevel oddělení pro horní a dolní pevnou lištu
    drawGoofy3DBevel(renderer, pxlenght(pan.x), pxlenght(pan.y), pxlenght(pan.w), pxlenght(top_size+STANDARTPICEHEIGHT), false, 2);
    drawGoofy3DBevel(renderer, pxlenght(pan.x), pxlenght(pan.h+pan.y-bottom_size-STANDARTPICEHEIGHT), pxlenght(pan.w), pxlenght(bottom_size+STANDARTPICEHEIGHT), false, 2);

    for (auto& element : pan.top_elements) { update0Element(renderer, &element, pan.x+STANDARTBORDER, pan.y+STANDARTBORDER); }
    for (auto& element : pan.bottom_elements) { update0Element(renderer, &element, pan.x+STANDARTBORDER, pan.y+STANDARTBORDER-STANDARTPICEHEIGHT-bottom_size+pan.h); }
    
    // Vykreslení 3D posuvníku (Scrollbaru) na boku
    if (scrollbar_range > 0) {
        bool scrollbar_hovered = mousex >= pxlenght(scrollbar_x - STANDARTPICEHEIGHT/4) && mousex <= pxlenght(scrollbar_x + STANDARTPICEHEIGHT/4) &&
                                mousey >= pxlenght(scrollbar_y) && mousey <= pxlenght(scrollbar_y + scrollbar_height);
        bool scrollbar_clicked = scrollbar_hovered && clicked;
        
        int sx = pxlenght(scrollbar_x - STANDARTPICEHEIGHT/4);
        int sy = pxlenght(scrollbar_y);
        int sw = pxlenght(STANDARTPICEHEIGHT/2);
        int sh = pxlenght(scrollbar_height);
        
        drawGoofy3DBevel(renderer, sx, sy, sw, sh, scrollbar_clicked, 2);
        
        if (scrollbar_clicked) { SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255); } 
        else if (scrollbar_hovered) { SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); } // Hover na scrollbaru svítí zeleně!
        else { SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255); }
        
        SDL_Rect sb_rect = {sx+2, sy+2, sw-4, sh-4};
        SDL_RenderFillRect(renderer, &sb_rect);
    }

    // Celkový masivní vnější 3D rám kolem úplně celého hlavního panelu
    drawGoofy3DBevel(renderer, pxlenght(pan.x), pxlenght(pan.y), pxlenght(pan.w), pxlenght(pan.h), false, pxlenght(BORDERLINEFATNESS));
    
    *panelptr=pan;
    return;
}