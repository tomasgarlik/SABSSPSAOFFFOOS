#include "includes.hpp"
button seznam_button = {
    .texture = createTextTexture(renderer,"Seznam žáků"),
    .xpos = 130,
    .ypos = 0,
    .function = [](){
        seznam_opened=true;
        update_seznam_panel_scrollables();
    }
};
button weights_button = {
    .texture = createTextTexture(renderer,"Váhy..."),
    .xpos = 290,
    .ypos = 0,
    .function = [](){
        weights_opened=true;
    }
};
button generate_button = {
    .texture = createTextTexture(renderer,"Vygenerovat..."),
    .xpos = 130,
    .ypos = STANDARTPICEHEIGHT,
    .function = [](){
        generate_opened=true;
    }
};
// seznam_panel.x=width/2-pxlenght(BIG_PANEL_WIDTH)/2;
// seznam_panel.y=height/2-pxlenght(TOPPICEHEIGHT)/2;
// seznam_panel.w=pxlenght(BIG_PANEL_WIDTH);
// seznam_panel.h=pxlenght(BIG_PANEL_HEIGHT);
seznam_panel.x=BIG_PANEL_X;
seznam_panel.y=BIG_PANEL_Y;
seznam_panel.w=BIG_PANEL_WIDTH;
seznam_panel.h=BIG_PANEL_HEIGHT;
seznam_panel.top_elements.push_back({
    .type = ELEMENT_TEXTBOX,
    .textbox = {
        .text = "Seznam žáků",
        .xpos = seznam_panel.w/2-100,
        .ypos = 0,
        .textsize=STANDARTPICEHEIGHT,
        .textwidth=80,
        .origin_left=true
    }
});
seznam_panel.bottom_elements.push_back({
    .type = ELEMENT_BUTTON,
    .button = {
        .texture = createTextTexture(renderer,"   OK   "),
        .xpos = seznam_panel.w-80,
        .ypos = 0,
        .function = []() {seznam_opened=false;}
    }
});
seznam_panel.bottom_elements.push_back({
    .type = ELEMENT_BUTTON,
    .button = {
        .texture = createTextTexture(renderer,"Import z CSV..."),
        .xpos = 0,
        .ypos = 0,
        .function = []() {import_csv();call_update_seznam_scrollables=true;}
    }
});
seznam_panel.bottom_elements.push_back({
    .type = ELEMENT_BUTTON,
    .button = {
        .texture = createTextTexture(renderer,"Export do CSV..."),
        .xpos = 180,
        .ypos = 0,
        .function = export_csv
    }
});
generate_panel.x=BIG_PANEL_X;
generate_panel.y=BIG_PANEL_Y;
generate_panel.w=BIG_PANEL_WIDTH;
generate_panel.h=BIG_PANEL_HEIGHT;
generate_panel.top_elements.push_back({
    .type = ELEMENT_TEXTBOX,
    .textbox = {
        .text = "Generovat...",
        .xpos = generate_panel.w/2-100,
        .ypos = 0,
        .textsize=STANDARTPICEHEIGHT,
        .textwidth=80,
        .origin_left=true
    }
});
generate_panel.bottom_elements.push_back({
    .type = ELEMENT_BUTTON,
    .button = {
        .texture = createTextTexture(renderer,"Generovat"),
        .xpos = generate_panel.w-125,
        .ypos = 0,
        .function = generate
    }
});
generate_panel.bottom_elements.push_back({
    .type = ELEMENT_BUTTON,
    .button = {
        .texture = createTextTexture(renderer,"Zavřít"),
        .xpos = generate_panel.w-200,
        .ypos = 0,
        .function = []() {generate_opened=false;}
    }
});
generate_panel.top_elements.push_back({
    .type = ELEMENT_TEXTBOX,
    .textbox = {
        .text = "Způsob generace:",
        .xpos = 0,
        .ypos = STANDARTPICEHEIGHT+STANDARTBORDER,
        .textsize=STANDARTPICEHEIGHT-STANDARTBORDER,
        .textwidth=80,
        .origin_left=true
    }
});
generate_panel.top_elements.push_back({
    .type=ELEMENT_DROPMENU,
    .dropmenu={
        .textures={createTextTexture(renderer,"Náhoda"), /*createTextTexture(renderer,"Genetický algorytmus"),*/ createTextTexture(renderer,"Hledací algorytmus")},
        .lenght=2,
        .titletype="select",
        .topwidth=320,
        .width=320,
        .xpos=230,
        .ypos=STANDARTPICEHEIGHT+STANDARTBORDER,
        .selected=0,
        .functions={
            [](){
                generate_panel.scrollable_elements.clear();
            },
            /*
            [](){
                generate_panel.scrollable_elements.clear();
                generate_panel.scrollable_elements.push_back({
                    .type=ELEMENT_LABEL,
                    .label={
                        .text="Váhy:",
                        .xpos=0,
                        .ypos=0,
                        .textsize=STANDARTPICEHEIGHT
                    }
                });
            },
            */
            [](){
                generate_panel.scrollable_elements.clear();
            }
        }
    }
});


editstudent_panel.x=BIG_PANEL_X;
editstudent_panel.y=BIG_PANEL_Y;
editstudent_panel.w=BIG_PANEL_WIDTH;
editstudent_panel.h=BIG_PANEL_HEIGHT;
editstudent_panel.top_elements.push_back({
    .type = ELEMENT_TEXTBOX,
    .textbox = {
        .text = "Upravit žáka",
        .xpos = editstudent_panel.w/2-100,
        .ypos = 0,
        .textsize=STANDARTPICEHEIGHT,
        .textwidth=80,
        .origin_left=true
    }
});
editstudent_panel.bottom_elements.push_back({
    .type = ELEMENT_BUTTON,
    .button = {
        .texture = createTextTexture(renderer,"   OK   "),
        .xpos = editstudent_panel.w-80,
        .ypos = 0,
        .function = []() {editstudent_opened=false;}
    }
});


wants_to_sit_with_panel.x=BIG_PANEL_X;
wants_to_sit_with_panel.y=BIG_PANEL_Y;
wants_to_sit_with_panel.w=BIG_PANEL_WIDTH;
wants_to_sit_with_panel.h=BIG_PANEL_HEIGHT;
wants_to_sit_with_panel.top_elements.push_back({
    .type = ELEMENT_TEXTBOX,
    .textbox = {
        .text = "Žák chce sedět s:",
        .xpos = wants_to_sit_with_panel.w/2-100,
        .ypos = 0,
        .textsize=STANDARTPICEHEIGHT,
        .textwidth=80,
        .origin_left=true
    }
});
wants_to_sit_with_panel.bottom_elements.push_back({
    .type = ELEMENT_BUTTON,
    .button = {
        .texture = createTextTexture(renderer,"   OK   "),
        .xpos = wants_to_sit_with_panel.w-80,
        .ypos = 0,
        .function = []() {wants_to_sit_with_opened=false;}
    }
});




musnt_sit_with_panel.x=BIG_PANEL_X;
musnt_sit_with_panel.y=BIG_PANEL_Y;
musnt_sit_with_panel.w=BIG_PANEL_WIDTH;
musnt_sit_with_panel.h=BIG_PANEL_HEIGHT;
musnt_sit_with_panel.top_elements.push_back({
    .type = ELEMENT_TEXTBOX,
    .textbox = {
        .text = "Žák nesmí sedět s:",
        .xpos = musnt_sit_with_panel.w/2-100,
        .ypos = 0,
        .textsize=STANDARTPICEHEIGHT,
        .textwidth=80,
        .origin_left=true
    }
});
musnt_sit_with_panel.bottom_elements.push_back({
    .type = ELEMENT_BUTTON,
    .button = {
        .texture = createTextTexture(renderer,"   OK   "),
        .xpos = musnt_sit_with_panel.w-80,
        .ypos = 0,
        .function = []() {musnt_sit_with_opened=false;}
    }
});



button swap_button={
    .texture=loadTexture("icons/swap.png", renderer),
    .xpos=tools_x,
    .ypos=0,
    .function=[](){swapping=true;SDL_SetCursor(cursor_swap);jetzt_normal_cursor=false;}
};
label scoretext={
    .text="Skóre: ",
    .origin_left=true,
    .textsize=STANDARTPICEHEIGHT-STANDARTBORDER*2,
    .xpos=tools_x,
    .ypos=STANDARTPICEHEIGHT
};;
label scorenum={
    .text="0.0f",
    .origin_left=true,
    .textsize=STANDARTPICEHEIGHT-STANDARTBORDER*2,
    .xpos=tools_x+80,
    .ypos=STANDARTPICEHEIGHT
};
label predek={
    .text="Přední část třídy",
    .origin_left=false,
    .textsize=STANDARTPICEHEIGHT-STANDARTBORDER*2,
    .xpos=width/2,
    .ypos=TOPPICEHEIGHT
};





export_panel.x=BIG_PANEL_X;
export_panel.y=BIG_PANEL_Y;
export_panel.w=BIG_PANEL_WIDTH;
export_panel.h=BIG_PANEL_HEIGHT;
export_panel.top_elements.push_back({
    .type = ELEMENT_TEXTBOX,
    .textbox = {
        .text = "Exportovat do PDF",
        .xpos = export_panel.w/2-100,
        .ypos = 0,
        .textsize=STANDARTPICEHEIGHT,
        .textwidth=80,
        .origin_left=true
    }
});
export_panel.bottom_elements.push_back({
    .type = ELEMENT_BUTTON,
    .button = {
        .texture = createTextTexture(renderer,"Exportovat"),
        .xpos = export_panel.w-129,
        .ypos = 0,
        .function = export_pdf
    }
});
export_panel.bottom_elements.push_back({
    .type = ELEMENT_BUTTON,
    .button = {
        .texture = createTextTexture(renderer,"Zrušit"),
        .xpos = export_panel.w-200,
        .ypos = 0,
        .function = [](){export_opened=false;}
    }
});
export_panel.scrollable_elements.push_back({
    .type = ELEMENT_LABEL,
    .label = {
        .text = "Nadpis:",
        .xpos = 0,
        .ypos = 0,
        .textsize=STANDARTPICEHEIGHT-STANDARTBORDER,
        .origin_left=true
    }
});
export_panel.scrollable_elements.push_back({
    .type = ELEMENT_TEXTBOX,
    .textbox = {
        .text = project.header,
        .xpos = 87,
        .ypos = 0,
        .textsize=STANDARTPICEHEIGHT-STANDARTBORDER,
        .origin_left=true
    }
});
export_panel.scrollable_elements.push_back({
    .type = ELEMENT_LABEL,
    .label = {
        .text = "Podnadpis:",
        .xpos = 0,
        .ypos = STANDARTPICEHEIGHT+STANDARTBORDER,
        .textsize=STANDARTPICEHEIGHT-STANDARTBORDER,
        .origin_left=true
    }
});
export_panel.scrollable_elements.push_back({
    .type = ELEMENT_TEXTBOX,
    .textbox = {
        .text = project.subheader,
        .xpos = 125,
        .ypos = STANDARTPICEHEIGHT+STANDARTBORDER,
        .textsize=STANDARTPICEHEIGHT-STANDARTBORDER,
        .origin_left=true
    }
});
export_panel.scrollable_elements.push_back({
    .type = ELEMENT_CHECKBOX,
    .checkbox = {
        .title = createTextTexture(renderer, "Otočit o 180°"),
        .xpos = 0,
        .ypos = (STANDARTPICEHEIGHT+STANDARTBORDER)*2,
        .checked=false
    }
});


weights_panel.x=BIG_PANEL_X;
weights_panel.y=BIG_PANEL_Y;
weights_panel.w=BIG_PANEL_WIDTH;
weights_panel.h=BIG_PANEL_HEIGHT;
weights_panel.top_elements.push_back({
    .type = ELEMENT_TEXTBOX,
    .textbox = {
        .text = "Váhy pro hledací algoritmus",
        .xpos = weights_panel.w/2,
        .ypos = 0,
        .textsize=STANDARTPICEHEIGHT,
        .textwidth=80,
        .origin_left=false
    }
});
weights_panel.bottom_elements.push_back({
    .type = ELEMENT_BUTTON,
    .button = {
        .texture = createTextTexture(renderer,"Zavřít"),
        .xpos = weights_panel.w-80,
        .ypos = 0,
        .function = [](){weights_opened=false;}
    }
});
weights_panel.bottom_elements.push_back({
    .type = ELEMENT_BUTTON,
    .button = {
        .texture = createTextTexture(renderer,"Výchozí"),
        .xpos = 0,
        .ypos = 0,
        .function = [](){
            // project.weights=default_weights;
            call_reset_weights=true;
            put_project_weights_to_panel();
        }
    }
});
weights_panel.scrollable_elements.push_back({
    .type=ELEMENT_SLIDER,
    .slider={
        .xpos=250,
        .ypos=(STANDARTPICEHEIGHT+STANDARTBORDER*2)*0,
        .min=0.0f,
        .max=1.0f,
        .value=0.0f,
        .moving=false,
        .width=250
    }
});
weights_panel.scrollable_elements.push_back({
    .type=ELEMENT_SLIDER,
    .slider={
        .xpos=250,
        .ypos=(STANDARTPICEHEIGHT+STANDARTBORDER*2)*1,
        .min=0.0f,
        .max=1.0f,
        .value=0.0f,
        .moving=false,
        .width=250
    }
});
weights_panel.scrollable_elements.push_back({
    .type=ELEMENT_SLIDER,
    .slider={
        .xpos=250,
        .ypos=(STANDARTPICEHEIGHT+STANDARTBORDER*2)*2,
        .min=0.0f,
        .max=1.0f,
        .value=0.0f,
        .moving=false,
        .width=250
    }
});
weights_panel.scrollable_elements.push_back({
    .type=ELEMENT_SLIDER,
    .slider={
        .xpos=250,
        .ypos=(STANDARTPICEHEIGHT+STANDARTBORDER*2)*3,
        .min=0.0f,
        .max=1.0f,
        .value=0.0f,
        .moving=false,
        .width=250
    }
});
weights_panel.scrollable_elements.push_back({
    .type=ELEMENT_SLIDER,
    .slider={
        .xpos=250,
        .ypos=(STANDARTPICEHEIGHT+STANDARTBORDER*2)*4,
        .min=0.0f,
        .max=10.0f,
        .value=3.6f,
        .moving=false,
        .width=250
    }
});
weights_panel.scrollable_elements.push_back({
    .type=ELEMENT_LABEL,
    .label={
        .text="Výška:",
        .xpos=0,
        .ypos=(STANDARTPICEHEIGHT+STANDARTBORDER*2)*0,
        .textsize=STANDARTPICEHEIGHT-STANDARTBORDER,
        .origin_left=true
    }
});
weights_panel.scrollable_elements.push_back({
    .type=ELEMENT_LABEL,
    .label={
        .text="Potřeba být ve předu:",
        .xpos=0,
        .ypos=(STANDARTPICEHEIGHT+STANDARTBORDER*2)*1,
        .textsize=STANDARTPICEHEIGHT-STANDARTBORDER,
        .origin_left=true
    }
});
weights_panel.scrollable_elements.push_back({
    .type=ELEMENT_LABEL,
    .label={
        .text="Chce sedět s:",
        .xpos=0,
        .ypos=(STANDARTPICEHEIGHT+STANDARTBORDER*2)*2,
        .textsize=STANDARTPICEHEIGHT-STANDARTBORDER,
        .origin_left=true
    }
});
weights_panel.scrollable_elements.push_back({
    .type=ELEMENT_LABEL,
    .label={
        .text="Je blízko u:",
        .xpos=0,
        .ypos=(STANDARTPICEHEIGHT+STANDARTBORDER*2)*3,
        .textsize=STANDARTPICEHEIGHT-STANDARTBORDER,
        .origin_left=true
    }
});
weights_panel.scrollable_elements.push_back({
    .type=ELEMENT_LABEL,
    .label={
        .text="Je blízko u dosah:",
        .xpos=0,
        .ypos=(STANDARTPICEHEIGHT+STANDARTBORDER*2)*4,
        .textsize=STANDARTPICEHEIGHT-STANDARTBORDER,
        .origin_left=true
    }
});