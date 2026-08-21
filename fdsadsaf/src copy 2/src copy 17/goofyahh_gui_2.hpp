#include "includes.hpp"
#include <cmath>

// Pomocná funkce pro kreslení totálně pokřivených, tekutých čar (peak cursed efekt)
// Verze: ULTRA-CURSED MEGAMELT EDITION
float twist=5.0f;
void drawCursedMeltedLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2) {
    // Čas běží rychleji (násobič zvýšen na 0.008f)
    float time_offset = SDL_GetTicks() * 0.008f;
    
    // Frekvence vln (hustota) - zvýšeno na 0.15f
    float frequency = 0.05f;
    
    // Amplituda vln (Hloubka vlnění) - brutálně zvýšeno na 10.0f pixelů!
    float amplitude = 2.5f;

    if (x1 == x2) { // Vertikální tekutá lajna
        int start_y = std::min(y1, y2);
        int end_y = std::max(y1, y2);
        for (int y = start_y; y <= end_y; y++) {
            int wave = (int)(sin(y * frequency + time_offset) * amplitude);
            SDL_RenderDrawPoint(renderer, x1 + wave, y);
        }
    } else if (y1 == y2) { // Horizontální tekutá lajna
        int start_x = std::min(x1, x2);
        int end_x = std::max(x1, x2);
        for (int x = start_x; x <= end_x; x++) {
            int wave = (int)(cos(x * frequency + time_offset) * amplitude);
            SDL_RenderDrawPoint(renderer, x, y1 + wave);
        }
    } else {
        // Šikmé lajny vlnit nebudeme, to by byl segfault dřív, než řekneš Lua
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }
}

// Hardcore 3D Bevel, který ale místo rovných čar používá roztavené lajny
void draw2Goofy3DBevel(SDL_Renderer* renderer, int x, int y, int w, int h, bool sunken, int fatness) {
    int top_left_color = sunken ? 60 : 255;      
    int bottom_right_color = sunken ? 255 : 40;  

    for (int i = 0; i < fatness; i++) {
        SDL_SetRenderDrawColor(renderer, top_left_color, top_left_color, top_left_color, 255);
        drawCursedMeltedLine(renderer, x + i, y + i, x + w - i, y + i);
        drawCursedMeltedLine(renderer, x + i, y + i, x + i, y + h - i);

        SDL_SetRenderDrawColor(renderer, bottom_right_color, bottom_right_color, bottom_right_color, 255);
        drawCursedMeltedLine(renderer, x + i, y + h - 1 - i, x + w - i, y + h - 1 - i);
        drawCursedMeltedLine(renderer, x + w - 1 - i, y + i, x + w - 1 - i, y + h - i);
    }
}

// Procedurální flekaté "plísňové" pozadí, aby to vypadalo špinavě a cursed
void renderCursedMoldyTexture(SDL_Renderer* renderer, int x, int y, int w, int h, int rB, int gB, int bB) {
    SDL_Rect baseRect = {x, y, w, h};
    SDL_SetRenderDrawColor(renderer, rB, gB, bB, 255);
    SDL_RenderFillRect(renderer, &baseRect);

    // Digitální hniloba
    for (int i = 0; i < w; i += 16) {
        for (int j = 0; j < h; j += 16) {
            int noise = ((i * 37) ^ (j * 59)) % 40;
            SDL_SetRenderDrawColor(renderer, 
                std::max(0, rB - noise + 15), 
                std::max(0, gB - noise - 25), 
                std::max(0, bB - noise + 20), 255);
                
            SDL_Rect block = {x + i, y + j, std::min(16, w - i), std::min(16, h - j)};
            SDL_RenderFillRect(renderer, &block);
        }
    }
}

void update2DropMenu(SDL_Renderer* renderer, dropmenu* menuptr, int xshift=0, int yshift=0){
    dropmenu menu=*menuptr;
    menu.xpos+=xshift;
    menu.ypos+=yshift;
    Uint32 ticks = SDL_GetTicks();
    
    if (menu.opened){
        SDL_Rect rect={pxlenght(menu.xpos),pxlenght(menu.ypos),pxlenght(menu.topwidth), pxlenght(STANDARTPICEHEIGHT)};
        SDL_SetRenderDrawColor(renderer, 150 + (ticks%50), 30, 200, 255);
        SDL_RenderFillRoundRect(renderer, &rect);
        
        SDL_Rect rect0={pxlenght(menu.xpos),pxlenght(menu.ypos+STANDARTPICEHEIGHT),pxlenght(menu.width), pxlenght(STANDARTPICEHEIGHT*menu.lenght)};
        renderCursedMoldyTexture(renderer, rect0.x, rect0.y, rect0.w, rect0.h, 45, 50, 55);
        
        for (int i=0;i<menu.lenght;i++){
            if (mousex<pxlenght(menu.xpos+menu.width) && mousex>pxlenght(menu.xpos) && mousey<pxlenght(menu.ypos+STANDARTPICEHEIGHT*(i+2))){
                SDL_Rect rect1={pxlenght(menu.xpos+STANDARTBORDER),pxlenght(menu.ypos+STANDARTPICEHEIGHT*(i+1)+STANDARTBORDER),pxlenght(menu.width-STANDARTBORDER*2), pxlenght(STANDARTPICEHEIGHT-STANDARTBORDER*2)};
                SDL_SetRenderDrawColor(renderer, 255, 0, 128, 255);
                if (clicked){ SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); }
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
        
        draw2Goofy3DBevel(renderer, rect0.x, rect0.y, rect0.w, rect0.h, false, pxlenght(BORDERLINEFATNESS));
        clickedOnSomething=true;
    }
    
    int title_x = pxlenght(menu.xpos);
    int title_y = pxlenght(menu.ypos);
    int title_w = pxlenght(menu.topwidth);
    int title_h = pxlenght(STANDARTPICEHEIGHT);
    draw2Goofy3DBevel(renderer, title_x, title_y, title_w, title_h, menu.opened, pxlenght(BORDERLINEFATNESS));

    if (menu.opened && mousex<pxlenght(menu.xpos+menu.width) && mousex>pxlenght(menu.xpos) && mousey<pxlenght(menu.ypos+STANDARTPICEHEIGHT*(menu.lenght+1))){menu.opened=true;clickedOnSomething=true;}
    else if (clicked && menu.opened && !(mousex<pxlenght(menu.xpos+menu.width) && mousex>pxlenght(menu.xpos) && mousey<pxlenght(menu.ypos+STANDARTPICEHEIGHT*(menu.lenght+1)))){menu.opened=false;clickedOnSomething=true;}
    
    if (mousex<pxlenght(menu.topwidth)+pxlenght(menu.xpos) && mousex>pxlenght(menu.xpos) && mousey>pxlenght(menu.ypos) && mousey<pxlenght(menu.ypos+STANDARTPICEHEIGHT)){
        SDL_Rect rect={pxlenght(menu.xpos)+2, pxlenght(menu.ypos)+2, pxlenght(menu.topwidth)-4, pxlenght(STANDARTPICEHEIGHT)-4};
        if (clicked){
            SDL_SetRenderDrawColor(renderer, 110, 110, 110, 255);
            clickedOnSomething=true;
        } else {
            SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
        }
        SDL_RenderFillRoundRect(renderer, &rect);
        if (clickup){
            menu.opened=true;
            clickedOnSomething=true;
        }
    } else {
        SDL_Rect rect={pxlenght(menu.xpos)+2, pxlenght(menu.ypos)+2, pxlenght(menu.topwidth)-4, pxlenght(STANDARTPICEHEIGHT)-4};
        SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
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

void update2TextBox(SDL_Renderer* renderer, textbox* inputboxptr, int xshift=0, int yshift=0){
    textbox inputbox=*inputboxptr;
    inputbox.xpos+=xshift;
    inputbox.ypos+=yshift;
    Uint32 ticks = SDL_GetTicks();
    std::string finaltext=inputbox.text;
    SDL_Texture* texttex=createTextTexture(renderer,finaltext, inputbox.start_of_selected, inputbox.end_of_selected);
    inputbox.textwidth=getTextureWidth(texttex,inputbox.textsize)+STANDARTBORDER*2;
    if (inputbox.origin_left){
        inputbox.xpos+=inputbox.textwidth/2;
    }
    SDL_Rect rect={pxlenght(inputbox.xpos-(getTextureWidth(texttex,inputbox.textsize)+STANDARTBORDER*2)/2), pxlenght(inputbox.ypos), pxlenght((getTextureWidth(texttex,inputbox.textsize)+STANDARTBORDER*2)), pxlenght(inputbox.textsize+STANDARTBORDER*2)};
    
    if (inputbox.active){
        if (ticks-inputbox.last_time_cursor_blink>CURSORPERIOD){
            inputbox.cursor_visible=!inputbox.cursor_visible;
            inputbox.last_time_cursor_blink=ticks;
        }
        
        draw2Goofy3DBevel(renderer, rect.x, rect.y, rect.w, rect.h, true, 2);
        SDL_Rect rect2={rect.x+2, rect.y+2, rect.w-4, rect.h-4};
        
        int green_pulse = 200 + (int)(sin(ticks * 0.005f) * 55.0f);
        SDL_SetRenderDrawColor(renderer, 255, green_pulse, 150, 255); 
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
        
        if (inputbox.cursor_visible){
            SDL_SetRenderDrawColor(renderer, (ticks * 2) % 255, (ticks * 3) % 255, (ticks * 5) % 255, 255); 
            SDL_RenderDrawFatLine(renderer, rect.x + cursorlenpx, rect.y + STANDARTBORDER, rect.x + cursorlenpx, rect.y + inputbox.textsize + STANDARTBORDER, pxlenght(4));
        }
    } else {
        draw2Goofy3DBevel(renderer, rect.x, rect.y, rect.w, rect.h, true, 2);
        SDL_Rect rect2={rect.x+2, rect.y+2, rect.w-4, rect.h-4};
        SDL_SetRenderDrawColor(renderer, 245, 245, 245, 255);
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

void update2Label(SDL_Renderer* renderer, label* labelptr, int xshift=0, int yshift=0){
    label inputbox=*labelptr;
    inputbox.xpos+=xshift;
    inputbox.ypos+=yshift;
    std::string finaltext=inputbox.text;
    SDL_Texture* texttex=createTextTexture(renderer,finaltext);
    inputbox.textwidth=getTextureWidth(texttex,inputbox.textsize)+STANDARTBORDER*2;
    if (inputbox.origin_left){
        inputbox.xpos+=inputbox.textwidth/2;
    }
    
    float time = SDL_GetTicks() * 0.05f;
    // TŘES TEXTU zvýšen pro peak cursed vibe
    int shake_x = (int)(sin(time) * 4.0f);
    int shake_y = (int)(cos(time * 1.3f) * 4.0f);

    int lx = pxlenght(inputbox.xpos+STANDARTBORDER-(getTextureWidth(texttex,inputbox.textsize)+STANDARTBORDER*2)/2) + shake_x;
    int ly = pxlenght(inputbox.ypos+STANDARTBORDER) + shake_y;
    
    int shadow_dist = 2 + (int)(sin(time * 0.2f) * twist);
    SDL_SetTextureColorMod(texttex, 0, 0, 0);
    displayTex(renderer, texttex, lx + shadow_dist, ly + shadow_dist, pxlenght(inputbox.textsize));
    
    SDL_SetTextureColorMod(texttex, 50, 255, 50); 
    displayTex(renderer, texttex, lx, ly, pxlenght(inputbox.textsize));
    
    SDL_DestroyTexture(texttex);
    inputbox.xpos-=xshift; inputbox.ypos-=yshift;
    if (inputbox.origin_left){ inputbox.xpos-=inputbox.textwidth/2; }
    *labelptr=inputbox;
    return;
}

void update2CheckBox(SDL_Renderer* renderer, checkbox* boxptr, int xshift=0, int yshift=0){
    checkbox box=*boxptr;
    box.xpos+=xshift;
    box.ypos+=yshift;
    box.just_changed=false;
    Uint32 ticks = SDL_GetTicks();

    int boxSize = STANDARTPICEHEIGHT; 
    SDL_Rect rect={pxlenght(box.xpos), pxlenght(box.ypos), pxlenght(boxSize), pxlenght(boxSize)};
    
    draw2Goofy3DBevel(renderer, rect.x, rect.y, rect.w, rect.h, true, 2);
    
    if (mousex>pxlenght(box.xpos) && mousex<pxlenght(box.xpos+boxSize) && mousey>pxlenght(box.ypos) && mousey<pxlenght(box.ypos+boxSize) && clicked){
        box.checked=!box.checked;   
        box.just_changed=true;
        clicked = false; 
    }   

    SDL_Rect innerRect = {rect.x+2, rect.y+2, rect.w-4, rect.h-4};
    if (box.checked){
        if ((ticks / 100) % 2 == 0) SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        else SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
    }

    if (mousex>pxlenght(box.xpos) && mousex<pxlenght(box.xpos+boxSize) && mousey>pxlenght(box.ypos) && mousey<pxlenght(box.ypos+boxSize)){
        if (!box.checked) SDL_SetRenderDrawColor(renderer, 255, 180, 0, 255);
    }
    SDL_RenderFillRect(renderer, &innerRect);

    if (box.checked) {
        int offset = (int)(sin(ticks * 0.02f) * twist);
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); 
        SDL_RenderDrawFatLine(renderer, innerRect.x + 2 + offset, innerRect.y + 2, innerRect.x + innerRect.w - 2 - offset, innerRect.y + innerRect.h - 2, 3);
        SDL_RenderDrawFatLine(renderer, innerRect.x + innerRect.w - 2, innerRect.y + 2 + offset, innerRect.x + 2, innerRect.y + innerRect.h - 2 - offset, 3);
    }

    displayTex(renderer, box.title, pxlenght(box.xpos + boxSize + STANDARTBORDER), pxlenght(box.ypos), pxlenght(boxSize));

    box.xpos-=xshift; box.ypos-=yshift;
    *boxptr=box;
    return;
}

void update2Slider(SDL_Renderer* renderer, slider* sliderptr, int xshift=0, int yshift=0) {
    slider inputslider = *sliderptr;
    inputslider.xpos += xshift;
    inputslider.ypos += yshift;
    Uint32 ticks = SDL_GetTicks();
    char text_buf[32];

    int value_text_size = STANDARTPICEHEIGHT - STANDARTBORDER * 2;
    sprintf(text_buf, "%.1f", inputslider.value);
    SDL_Texture* val_tex = createTextTexture(renderer, text_buf);
    int val_w = getTextureWidth(val_tex, value_text_size);
    
    SDL_SetTextureColorMod(val_tex, (ticks)%255, (ticks*2)%255, 255);
    displayTex(renderer, val_tex, pxlenght(inputslider.xpos), pxlenght(inputslider.ypos + STANDARTBORDER), pxlenght(value_text_size));
    SDL_DestroyTexture(val_tex);

    int slider_data_x = inputslider.xpos + val_w + STANDARTBORDER;
    int slider_data_w = inputslider.width - (val_w + STANDARTBORDER);
    if (slider_data_w < 10) slider_data_w = 10;

    int handle_h = STANDARTPICEHEIGHT - STANDARTBORDER * 2;
    int handle_w = handle_h / 2; 

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

    int line_y = inputslider.ypos + (STANDARTPICEHEIGHT / 2);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); 
    drawCursedMeltedLine(renderer, pxlenght(slider_data_x), pxlenght(line_y), pxlenght(slider_data_x + slider_data_w), pxlenght(line_y));

    draw2Goofy3DBevel(renderer, pxlenght(handle_x), pxlenght(handle_y), pxlenght(handle_w), pxlenght(handle_h), inputslider.moving, 2);
    SDL_Rect handle_rect = { pxlenght(handle_x)+2, pxlenght(handle_y)+2, pxlenght(handle_w)-4, pxlenght(handle_h)-4 };
    
    if (inputslider.moving) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); 
    } else if (hover) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255); 
    } else {
        SDL_SetRenderDrawColor(renderer, 140, 160, 140, 255); 
    }
    SDL_RenderFillRect(renderer, &handle_rect);

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

void update2NumberInputBox(SDL_Renderer* renderer, numberinputbox* inputboxptr, int xshift=0, int yshift=0){
    numberinputbox inputbox=*inputboxptr;
    inputbox.xpos+=xshift;
    inputbox.ypos+=yshift;
    std::string finaltext=inputbox.text;
    SDL_Texture* texttex=createTextTexture(renderer,finaltext, inputbox.start_of_selected, inputbox.end_of_selected);
    SDL_Rect rect={pxlenght(inputbox.xpos), pxlenght(inputbox.ypos), pxlenght((inputbox.size)), pxlenght(STANDARTPICEHEIGHT)};
    
    draw2Goofy3DBevel(renderer, rect.x, rect.y, rect.w, rect.h, true, 2);
    SDL_Rect rect2={rect.x+2, rect.y+2, rect.w-4, rect.h-4};
    SDL_SetRenderDrawColor(renderer, 220, 230, 220, 255);
    SDL_RenderFillRoundRect(renderer, &rect2, pxlenght(3));
    
    int btn_w = pxlenght(STANDARTPICEHEIGHT/2);
    int btn_x = rect.x + rect.w - btn_w;
    int btn_h = rect.h / 2;
    
    bool hover_up = (mousex > btn_x && mousex < rect.x + rect.w && mousey > rect.y && mousey < rect.y + btn_h);
    bool hover_down = (mousex > btn_x && mousex < rect.x + rect.w && mousey > rect.y + btn_h && mousey < rect.y + rect.h);

    draw2Goofy3DBevel(renderer, btn_x, rect.y, btn_w, btn_h, clicked && hover_up, 1);
    SDL_Rect up_rect = {btn_x+1, rect.y+1, btn_w-2, btn_h-2};
    SDL_SetRenderDrawColor(renderer, hover_up ? 255 : 150, 150, 150, 255);
    SDL_RenderFillRect(renderer, &up_rect);

    draw2Goofy3DBevel(renderer, btn_x, rect.y + btn_h, btn_w, btn_h, clicked && hover_down, 1);
    SDL_Rect down_rect = {btn_x+1, rect.y + btn_h + 1, btn_w-2, btn_h-2};
    SDL_SetRenderDrawColor(renderer, hover_down ? 255 : 150, 150, 150, 255);
    SDL_RenderFillRect(renderer, &down_rect);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    drawCursedMeltedLine(renderer, btn_x + btn_w/2, rect.y + 4, btn_x + 3, rect.y + btn_h - 3);
    drawCursedMeltedLine(renderer, btn_x + btn_w/2, rect.y + 4, btn_x + btn_w - 4, rect.y + btn_h - 3);
    drawCursedMeltedLine(renderer, btn_x + btn_w/2, rect.y + btn_h + btn_h - 4, btn_x + 3, rect.y + btn_h + 3);
    drawCursedMeltedLine(renderer, btn_x + btn_w/2, rect.y + btn_h + btn_h - 4, btn_x + btn_w - 4, rect.y + btn_h + 3);

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

void update2Button(SDL_Renderer* renderer, button* butptr, int xshift=0, int yshift=0) {
    button but = *butptr;
    but.xpos += xshift;
    but.ypos += yshift;
    Uint32 ticks = SDL_GetTicks();
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

    draw2Goofy3DBevel(renderer, x1, y1, total_w, total_h, (inside && clicked) || but.force_selected, pxlenght(BORDERLINEFATNESS));

    SDL_Rect rect = { x1 + 2, y1 + 2, total_w - 4, total_h - 4 };
    
    if (inside) {
        if (clicked) {
            SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
            clickedOnSomething = true;
        } else {
            if ((ticks / 200) % 2 == 0) SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
            else SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        }
        if (clickup) {
            but.function();
            clickedOnSomething = true;
        }
    } else {
        SDL_SetRenderDrawColor(renderer, 170, 170, 170, 255); 
    }
    
    if (but.force_selected){
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); 
    }
    SDL_RenderFillRoundRect(renderer, &rect);

    int push = ((inside && clicked) || but.force_selected) ? 2 : 0;
    
    // TLAČÍTKA SE KLEPOU PŘI HOVERU (nový cursed efekt)
    int hover_wobble_x = (inside && !clicked) ? (int)(sin(ticks * 0.05f) * 6.0f) : 0;
    int hover_wobble_y = (inside && !clicked) ? (int)(cos(ticks * 0.07f) * 6.0f) : 0;

    displayTex(renderer, but.texture, 
               pxlenght(but.xpos + STANDARTBORDER * 2) + push + hover_wobble_x, 
               pxlenght(but.ypos + STANDARTBORDER) + push + hover_wobble_y, 
               pxlenght(STANDARTPICEHEIGHT - STANDARTBORDER * 2));

    but.xpos -= xshift; but.ypos -= yshift;
    *butptr = but; 
}

void update2Element(SDL_Renderer* renderer, element* elementptr, int xshift=0, int yshift=0){
    if (elementptr->type == ELEMENT_BUTTON) { update2Button(renderer, &elementptr->button, xshift, yshift); } 
    else if (elementptr->type == ELEMENT_DROPMENU) { update2DropMenu(renderer, &elementptr->dropmenu, xshift, yshift); } 
    else if (elementptr->type == ELEMENT_TEXTBOX) { update2TextBox(renderer, &elementptr->textbox, xshift, yshift); } 
    else if (elementptr->type == ELEMENT_NUMBERINPUTBOX) { update2NumberInputBox(renderer, &elementptr->numberinputbox, xshift, yshift); } 
    else if (elementptr->type == ELEMENT_LABEL) { update2Label(renderer, &elementptr->label, xshift, yshift); } 
    else if (elementptr->type == ELEMENT_CHECKBOX) { update2CheckBox(renderer, &elementptr->checkbox, xshift, yshift); } 
    else if (elementptr->type == ELEMENT_SLIDER) { update2Slider(renderer, &elementptr->slider, xshift, yshift); }
}


void update2Panel(SDL_Renderer* renderer, panel* panelptr){
    panel pan=*panelptr;
    
    renderCursedMoldyTexture(renderer, pxlenght(pan.x), pxlenght(pan.y), pxlenght(pan.w), pxlenght(pan.h), 120, 130, 125);
    
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
    
    for (auto& element : pan.scrollable_elements) {
        if (get_element_pos(&element).y-pan.scrolled > -STANDARTBORDER-STANDARTPICEHEIGHT && get_element_pos(&element).y-pan.scrolled < pan.h-STANDARTPICEHEIGHT*2-STANDARTBORDER*2) {
            update2Element(renderer, &element, pan.x+STANDARTBORDER, pan.y-pan.scrolled+STANDARTBORDER+top_size+STANDARTPICEHEIGHT);
        }
    }
    
    renderCursedMoldyTexture(renderer, pxlenght(pan.x), pxlenght(pan.y), pxlenght(pan.w), pxlenght(top_size+STANDARTPICEHEIGHT), 40, 50, 45);
    renderCursedMoldyTexture(renderer, pxlenght(pan.x), pxlenght(pan.h+pan.y-bottom_size-STANDARTPICEHEIGHT), pxlenght(pan.w), pxlenght(bottom_size+STANDARTPICEHEIGHT), 40, 50, 45);
    
    draw2Goofy3DBevel(renderer, pxlenght(pan.x), pxlenght(pan.y), pxlenght(pan.w), pxlenght(top_size+STANDARTPICEHEIGHT), false, 2);
    draw2Goofy3DBevel(renderer, pxlenght(pan.x), pxlenght(pan.h+pan.y-bottom_size-STANDARTPICEHEIGHT), pxlenght(pan.w), pxlenght(bottom_size+STANDARTPICEHEIGHT), false, 2);

    for (auto& element : pan.top_elements) { update2Element(renderer, &element, pan.x+STANDARTBORDER, pan.y+STANDARTBORDER); }
    for (auto& element : pan.bottom_elements) { update2Element(renderer, &element, pan.x+STANDARTBORDER, pan.y+STANDARTBORDER-STANDARTPICEHEIGHT-bottom_size+pan.h); }
    
    if (scrollbar_range > 0) {
        bool scrollbar_hovered = mousex >= pxlenght(scrollbar_x - STANDARTPICEHEIGHT/4) && mousex <= pxlenght(scrollbar_x + STANDARTPICEHEIGHT/4) &&
                                mousey >= pxlenght(scrollbar_y) && mousey <= pxlenght(scrollbar_y + scrollbar_height);
        bool scrollbar_clicked = scrollbar_hovered && clicked;
        
        int sx = pxlenght(scrollbar_x - STANDARTPICEHEIGHT/4);
        int sy = pxlenght(scrollbar_y);
        int sw = pxlenght(STANDARTPICEHEIGHT/2);
        int sh = pxlenght(scrollbar_height);
        
        draw2Goofy3DBevel(renderer, sx, sy, sw, sh, scrollbar_clicked, 2);
        
        if (scrollbar_clicked) { SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255); } 
        else if (scrollbar_hovered) { SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255); } 
        else { SDL_SetRenderDrawColor(renderer, 160, 160, 160, 255); }
        
        SDL_Rect sb_rect = {sx+2, sy+2, sw-4, sh-4};
        SDL_RenderFillRect(renderer, &sb_rect);
    }

    draw2Goofy3DBevel(renderer, pxlenght(pan.x), pxlenght(pan.y), pxlenght(pan.w), pxlenght(pan.h), false, pxlenght(BORDERLINEFATNESS));
    
    *panelptr=pan;
    return;
}