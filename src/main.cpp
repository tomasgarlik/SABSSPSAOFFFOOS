#include "includes.hpp"

int select_gender(int student){
    rn=true;
    label l={
        .text="Vyberte pohlaví",
        .textsize=STANDARTPICEHEIGHT,
        .xpos=width/2,
        .ypos=0,
        .origin_left=false
    };
    panel p={};
    p.top_elements.push_back({.type=ELEMENT_LABEL,.label=l});
    p.bottom_elements.push_back({
        .type = ELEMENT_BUTTON,
        .button = {
            .texture = createTextTexture(renderer,"Zrušit"),
            .xpos =width-160,
            .ypos=0,
            .function = [](){rn=false;}
        }
    });

    int index=0;
    for (auto& g:genders){
        p.scrollable_elements.push_back({
            .type = ELEMENT_BUTTON,
            .button = {
                .texture = createTextTexture(renderer,g),
                .xpos = 0,
                .ypos=(int)(p.scrollable_elements.size())*(STANDARTPICEHEIGHT),
                .function = [index]() {gend=index;rn=false;}
            }
        });
        index++;
    }
    p.w=width;
    p.h=height;
    p.x=0;
    p.y=0;


    while (rn && running){
        SDL_RenderClear(renderer);
        if (clickup){clickup=false;}
        handle_events();
        updatePanel(renderer, &p);
        click_in_meziprostor=false;
        if (clic && !clickedOnSomething){click_in_meziprostor=true;}
        SDL_RenderPresent(renderer);
    }
    return gend;
}
void confirm_student(int student){
    project.students[student].name=editstudent_panel.scrollable_elements[1].textbox.text;
    project.students[student].height=editstudent_panel.scrollable_elements[3].numberinputbox.number;
    project.students[student].gender=now_student_gender;
    project.students[student].sit_with_different_gender=editstudent_panel.scrollable_elements[7].checkbox.checked;
}
int select_student(){
    rn=true;
    label l={
        .text="Vyberte žáka",
        .textsize=STANDARTPICEHEIGHT,
        .xpos=width/2,
        .ypos=0,
        .origin_left=false
    };
    panel p={};
    p.top_elements.push_back({.type=ELEMENT_LABEL,.label=l});
    p.bottom_elements.push_back({
        .type = ELEMENT_BUTTON,
        .button = {
            .texture = createTextTexture(renderer,"Zrušit"),
            .xpos =width-160,
            .ypos=0,
            .function = [](){rn=false;}
        }
    });



    
    int index=0;
    for (auto& s:project.students){
        p.scrollable_elements.push_back({
            .type = ELEMENT_BUTTON,
            .button = {
                .texture = createTextTexture(renderer,s.name),
                .xpos = 0,
                .ypos=(int)(p.scrollable_elements.size())*(STANDARTPICEHEIGHT+STANDARTBORDER*2),
                .function = [index]() {gend=index;rn=false;}
            }
        });
        index++;
    }
    p.w=width;
    p.h=height;
    p.x=0;
    p.y=0;


    while (rn && running){
        SDL_RenderClear(renderer);
        if (clickup){clickup=false;}
        handle_events();
        updatePanel(renderer, &p);
        click_in_meziprostor=false;
        if (clic && !clickedOnSomething){click_in_meziprostor=true;}
        SDL_RenderPresent(renderer);
    }
    return gend;
}
void wants_to_sit_with(){
    wants_to_sit_with_panel.scrollable_elements.clear();
    int index=0;
    for (auto& si:project.students[now_student].prefered){
        student s=project.students[si];
        element e;
        e.type=ELEMENT_LABEL;
        e.label.text=s.name;
        e.label.xpos=40;
        e.label.ypos=(int)(wants_to_sit_with_panel.scrollable_elements.size())*(STANDARTPICEHEIGHT);
        e.label.textsize=pxlenght(STANDARTPICEHEIGHT-STANDARTBORDER*4);
        e.label.origin_left=true;
        wants_to_sit_with_panel.scrollable_elements.push_back(e);

        index++;
    }
    index=0;
    for (auto& si:project.students[now_student].prefered){
        student s=project.students[si];
        element e;
        e.type=ELEMENT_BUTTON;
        e.button.texture=createTextTexture(renderer, "  X  ", -1, -1, 255,0,0),
        e.button.xpos=420;
        e.button.ypos=(int)(index)*(STANDARTPICEHEIGHT);
        e.button.force_selected=false;
        e.button.function=[index](){
            project.students[now_student].prefered.erase(project.students[now_student].prefered.begin() + index);
            call_wants_to_sit_with=true;
        },
        wants_to_sit_with_panel.scrollable_elements.push_back(e);

        index++;
    }
    index=0;
    for (auto& si:project.students[now_student].prefered){
        if (index==0){index++; continue;}
        student s=project.students[si];
        element e;
        e.type=ELEMENT_BUTTON;
        e.button.texture=loadTexture("icons/up.png", renderer),
        e.button.xpos=340;
        e.button.ypos=(int)(index)*(STANDARTPICEHEIGHT);
        e.button.force_selected=false;
        e.button.function=[index]() { // zachytíme index v době vytvoření
            auto& pref = project.students[now_student].prefered;
            
            // Tady musíme přepočítat reálný index ve vectoru 
            // (protože v cyklu jsi přeskočil index 0!)
            size_t vec_idx = index; 

            // Kontrola, abychom neletěli mimo paměť (před index 0)
            if (vec_idx > 0 && vec_idx < pref.size()) {
                std::swap(pref[vec_idx], pref[vec_idx - 1]);
                call_wants_to_sit_with = true;
            }
        };
        wants_to_sit_with_panel.scrollable_elements.push_back(e);

        index++;
    }
    index=0;
    for (auto& si:project.students[now_student].prefered){
        if (index==project.students[now_student].prefered.size()-1){index++; continue;}
        student s=project.students[si];
        element e;
        e.type=ELEMENT_BUTTON;
        e.button.texture=loadTexture("icons/down.png", renderer),
        e.button.xpos=380;
        e.button.ypos=(int)(index)*(STANDARTPICEHEIGHT);
        e.button.force_selected=false;
        e.button.function=e.button.function = [index]() {
            auto& pref = project.students[now_student].prefered;
            size_t vec_idx = index;

            // Kontrola, abychom nepřelezli konec vectoru
            if (vec_idx < pref.size() - 1) {
                std::swap(pref[vec_idx], pref[vec_idx + 1]);
                call_wants_to_sit_with = true;
            }
        };
        wants_to_sit_with_panel.scrollable_elements.push_back(e);

        index++;
    }
    index=0;
    for (auto& si:project.students[now_student].prefered){
        student s=project.students[si];
        element e;
        e.type=ELEMENT_LABEL;
        e.label.text=std::to_string(index+1)+".";
        e.label.xpos=0;
        e.label.ypos=(int)(index)*(STANDARTPICEHEIGHT);
        e.label.textsize=pxlenght(STANDARTPICEHEIGHT-STANDARTBORDER*4);
        e.label.origin_left=true;
        wants_to_sit_with_panel.scrollable_elements.push_back(e);

        index++;
    }
    wants_to_sit_with_panel.scrollable_elements.push_back({
        .type = ELEMENT_BUTTON,
        .button = {
            .texture = createTextTexture(renderer,"    +    "),
            .xpos = 0,
            .ypos = (int)((wants_to_sit_with_panel.scrollable_elements.size()+2)/5)*(STANDARTPICEHEIGHT+STANDARTBORDER*2),
            .function = []() {wants_to_sit_with_opened=true;
            
                int st=select_student();
                project.students[now_student].prefered.push_back(st);

                call_wants_to_sit_with=true;
                
                printf("now student: %d\nprefered: ", now_student);
                for (int x : project.students[now_student].prefered) {
                    printf("%d ", x);
                }
                printf("\n");
                // student s=project.students[st];
                // element e;
                // e.type=ELEMENT_LABEL;
                // e.label.text=s.name;
                // e.label.xpos=0;
                // e.label.ypos=(int)(wants_to_sit_with_panel.scrollable_elements.size())*(STANDARTPICEHEIGHT+STANDARTBORDER*2);
                // e.label.textsize=pxlenght(STANDARTPICEHEIGHT-STANDARTBORDER*4);
                // e.label.origin_left=true;
                // wants_to_sit_with_panel.scrollable_elements.insert(wants_to_sit_with_panel.scrollable_elements.begin(),e);

            
            }
        }
    });
}
void musnt_sit_with(){
    musnt_sit_with_panel.scrollable_elements.clear();
    int index=0;
    for (auto& si:project.students[now_student].musnt){
        student s=project.students[si];
        element e;
        e.type=ELEMENT_LABEL;
        e.label.text=s.name;
        e.label.xpos=0;
        e.label.ypos=(int)(musnt_sit_with_panel.scrollable_elements.size())*(STANDARTPICEHEIGHT);
        e.label.textsize=pxlenght(STANDARTPICEHEIGHT-STANDARTBORDER*4);
        e.label.origin_left=true;
        musnt_sit_with_panel.scrollable_elements.push_back(e);

        index++;
    }
    index=0;
    for (auto& si:project.students[now_student].musnt){
        student s=project.students[si];
        element e;
        e.type=ELEMENT_BUTTON;
        e.button.texture=createTextTexture(renderer, "  X  "),
        e.button.xpos=380;
        e.button.ypos=(int)(index)*(STANDARTPICEHEIGHT);
        e.button.force_selected=false;
        e.button.function=[index](){
            project.students[now_student].musnt.erase(project.students[now_student].musnt.begin() + index);
            call_musnt_sit_with=true;
        },
        musnt_sit_with_panel.scrollable_elements.push_back(e);

        index++;
    }
    index=0;
    for (auto& si:project.students[now_student].musnt){
        student s=project.students[si];
        element e;
        e.type=ELEMENT_LABEL;
        e.label.text="";//std::to_string(index+1)+".";
        e.label.xpos=0;
        e.label.ypos=(int)(index)*(STANDARTPICEHEIGHT);
        e.label.textsize=pxlenght(STANDARTPICEHEIGHT-STANDARTBORDER*4);
        e.label.origin_left=true;
        musnt_sit_with_panel.scrollable_elements.push_back(e);

        index++;
    }
    musnt_sit_with_panel.scrollable_elements.push_back({
        .type = ELEMENT_BUTTON,
        .button = {
            .texture = createTextTexture(renderer,"    +    "),
            .xpos = 0,
            .ypos = (int)(musnt_sit_with_panel.scrollable_elements.size()/2)*(STANDARTPICEHEIGHT+STANDARTBORDER*2),
            .function = []() {musnt_sit_with_opened=true;
            
                int st=select_student();
                project.students[now_student].musnt.push_back(st);

                call_musnt_sit_with=true;
                

            
            }
        }
    });
}
void edit_student(int student){
    now_student=student;
    editstudent_opened=true;
    now_student_gender=project.students[student].gender;
    editstudent_panel.scrollable_elements.clear();
    editstudent_panel.scrollable_elements.push_back({
        .type=ELEMENT_LABEL,
        .label={
            .text="Jméno:",
            .xpos=0,
            .ypos=0,
            .textsize=STANDARTPICEHEIGHT-STANDARTBORDER,
            .origin_left=true
        }
    });
    editstudent_panel.scrollable_elements.push_back({ //name
        .type=ELEMENT_TEXTBOX,
        .textbox={
            .text=project.students[student].name,
            .xpos=100,
            .ypos=0,
            .textsize=STANDARTPICEHEIGHT,
            .origin_left=true
        }
    });
    editstudent_panel.scrollable_elements.push_back({
        .type=ELEMENT_LABEL,
        .label={
            .text="Výška (cm):",
            .xpos=0,
            .ypos=STANDARTPICEHEIGHT+STANDARTBORDER*2,
            .textsize=STANDARTPICEHEIGHT-STANDARTBORDER,
            .origin_left=true
        }
    });
    editstudent_panel.scrollable_elements.push_back({ //height
        .type=ELEMENT_NUMBERINPUTBOX,
        .numberinputbox={
            .text=std::to_string(project.students[student].height),
            .xpos=150,
            .ypos=STANDARTPICEHEIGHT+STANDARTBORDER*2,
            .number=(int)project.students[student].height,
            .active=false,
            .size=60,
            .maxnum=9999
        }
    });
    editstudent_panel.scrollable_elements.push_back({
        .type=ELEMENT_LABEL,
        .label={
            .text="Pohlaví:",
            .xpos=0,
            .ypos=(STANDARTPICEHEIGHT+STANDARTBORDER*2)*2,
            .textsize=STANDARTPICEHEIGHT-STANDARTBORDER,
            .origin_left=true
        }
    });
    editstudent_panel.scrollable_elements.push_back({
        .type=ELEMENT_LABEL,
        .label={
            .xpos=100,
            .ypos=(STANDARTPICEHEIGHT+STANDARTBORDER*2)*2,
            .text=genders[now_student_gender],
            .textsize=STANDARTPICEHEIGHT-STANDARTBORDER,
            .origin_left=true
        }
    });
    editstudent_panel.scrollable_elements.push_back({
        .type=ELEMENT_BUTTON,
        .button={
            .xpos=BIG_PANEL_WIDTH-100,
            .ypos=(STANDARTPICEHEIGHT+STANDARTBORDER*2)*2,
            .texture=createTextTexture(renderer,"Změnit"),
            .function=[student](){now_student_gender=select_gender(student);editstudent_panel.scrollable_elements[5].label.text=genders[now_student_gender];}
        }
    });
    editstudent_panel.scrollable_elements.push_back({ //sit_with_another
        .type=ELEMENT_CHECKBOX,
        .checkbox={
            .xpos=0,
            .ypos=(STANDARTPICEHEIGHT+STANDARTBORDER*2)*3,
            .title=createTextTexture(renderer,"Může sedět s žákem jiného pohlaví"),
            .checked=project.students[student].sit_with_different_gender
        }
    });
    editstudent_panel.scrollable_elements.push_back({
        .type=ELEMENT_LABEL,
        .label={
            .text="Potřeba sedět ve předu:",
            .xpos=0,
            .ypos=(STANDARTPICEHEIGHT+STANDARTBORDER*2)*4,
            .textsize=STANDARTPICEHEIGHT-STANDARTBORDER,
            .origin_left=true
        }
    });
    editstudent_panel.scrollable_elements.push_back({ //in front
        .type=ELEMENT_SLIDER,
        .slider={
            .xpos=270,
            .ypos=(STANDARTPICEHEIGHT+STANDARTBORDER*2)*4,
            .min=0.0f,
            .max=1.0f,
            .value=project.students[student].in_front,
            .moving=false,
            .width=150
        }
    });
    editstudent_panel.scrollable_elements.push_back({
        .type=ELEMENT_LABEL,
        .label={
            .text="Chce sedět s:",
            .xpos=0,
            .ypos=(STANDARTPICEHEIGHT+STANDARTBORDER*2)*5,
            .textsize=STANDARTPICEHEIGHT-STANDARTBORDER,
            .origin_left=true
        }
    });
    editstudent_panel.scrollable_elements.push_back({
        .type=ELEMENT_BUTTON,
        .button={
            .texture=createTextTexture(renderer," ... "),
            .xpos=180,
            .ypos=(STANDARTPICEHEIGHT+STANDARTBORDER*2)*5,
            .function=[](){wants_to_sit_with_opened=true;wants_to_sit_with();}
        }
    });
    editstudent_panel.scrollable_elements.push_back({
        .type=ELEMENT_LABEL,
        .label={
            .text="Nesmí sedět s:",
            .xpos=0,
            .ypos=(STANDARTPICEHEIGHT+STANDARTBORDER*2)*6,
            .textsize=STANDARTPICEHEIGHT-STANDARTBORDER,
            .origin_left=true
        }
    });
    editstudent_panel.scrollable_elements.push_back({
        .type=ELEMENT_BUTTON,
        .button={
            .texture=createTextTexture(renderer," ... "),
            .xpos=180,
            .ypos=(STANDARTPICEHEIGHT+STANDARTBORDER*2)*6,
            .function=[](){musnt_sit_with_opened=true;musnt_sit_with();}
        }
    });
    editstudent_panel.scrollable_elements.push_back({
        .type=ELEMENT_BUTTON,
        .button={
            .texture=createTextTexture(renderer,"Odstranit žáka", -1, -1, 255,0,0),
            .xpos=0,
            .ypos=(STANDARTPICEHEIGHT+STANDARTBORDER*2)*7,
            .function=[](){
                editstudent_opened=false;
                delete_student=true;
                call_update_seznam_scrollables = true;
            }
        }
    });
}
void sync_edit_student_data(){
    editstudent_panel.scrollable_elements[5].label.text=genders[now_student_gender];
    project.students[now_student].name=editstudent_panel.scrollable_elements[1].textbox.text;
    call_update_seznam_scrollables=true;
    project.students[now_student].height=editstudent_panel.scrollable_elements[3].numberinputbox.number;
    project.students[now_student].in_front=editstudent_panel.scrollable_elements[9].slider.value;
    project.students[now_student].sit_with_different_gender=editstudent_panel.scrollable_elements[7].checkbox.checked;
    project.students[now_student].gender=now_student_gender;



}
void update_seznam_panel_scrollables(){
    for (auto& e : seznam_panel.scrollable_elements) {
        if (e.type == ELEMENT_BUTTON && e.button.texture != nullptr) {
            SDL_DestroyTexture(e.button.texture); // <--- TOHLE TI ZACHRÁNÍ RAMKU
            e.button.texture = nullptr;
        }
    }
    seznam_panel.scrollable_elements.clear();
    int index=0;
    for (auto& s:project.students){
        element e;
        e.type=ELEMENT_LABEL;
        e.label.text=s.name;
        e.label.xpos=0;
        e.label.ypos=(int)(seznam_panel.scrollable_elements.size()/2)*(STANDARTPICEHEIGHT+STANDARTBORDER*2);
        e.label.textsize=pxlenght(STANDARTPICEHEIGHT-STANDARTBORDER*4);
        e.label.origin_left=true;
        seznam_panel.scrollable_elements.push_back(e);

        seznam_panel.scrollable_elements.push_back({
            .type = ELEMENT_BUTTON,
            .button = {
                .texture = createTextTexture(renderer,"..."),
                .xpos = BIG_PANEL_WIDTH-50,
                .ypos=(int)(seznam_panel.scrollable_elements.size()/2)*(STANDARTPICEHEIGHT+STANDARTBORDER*2),
                .function = [index]() {edit_student(index);}
            }
        });
        index++;
    }
    seznam_panel.scrollable_elements.push_back({
        .type = ELEMENT_BUTTON,
        .button = {
            .texture = createTextTexture(renderer,"    +    "),
            .xpos = 0,
            .ypos = (int)(seznam_panel.scrollable_elements.size()/2)*(STANDARTPICEHEIGHT+STANDARTBORDER*2),
            .function = []() {
                project.students.push_back(default_student);
                now_student=project.students.size()-1;
                editstudent_opened=true;
                edit_student(now_student);
                call_update_seznam_scrollables=true;
            }
        }
    });
}
void put_project_weights_to_panel(){
    weights_panel.scrollable_elements[0].slider.value=project.weights.height;
    weights_panel.scrollable_elements[1].slider.value=project.weights.in_front;
    weights_panel.scrollable_elements[2].slider.value=project.weights.prefered;
    weights_panel.scrollable_elements[3].slider.value=project.weights.close;
    weights_panel.scrollable_elements[4].slider.value=project.weights.close_dist;
}
void newfunc(){
    project.students.clear();
    project.seats.clear();
    project.yapp_groups.clear();
    project.header = "Zasedací pořádek 1.A";
    project.subheader = "Radim Vysoký";
    project.comment = "";
    project.filename = "";
    project.weights=default_weights;
    project.penalizace_za_nespokojenost=2.0f;
}
void quitfunc(){
    SDL_Quit();
    running=false;
    exit(0);
}
void init_cursors() {
    // Vytvoříme výchozí systémový kurzor (šipku)
    cursor_default = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    // Vytvoříme kurzor, který symbolizuje ruční prohazování/akci (např. ručičku)
    // Na některých OS to bude přesně ten prohazovací symbol, jinde jen jiná šipka/ručička
    cursor_swap = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND); 
}
void generate(){
    if (project.seats.size()<project.students.size()){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Upozornění", "Nelze vygenerovat, protože ve třídě je více žáků než míst.", NULL);
        return;
    }
    // if (generate_panel.top_elements[2].dropmenu.selected==0){
    //     printf("random shall be generated\n");
    //     generate_random();
    // } else if (generate_panel.top_elements[2].dropmenu.selected==1){
    //     printf("genetic shall be generated\n");
    //     generate_genetic();
    // } else if (generate_panel.top_elements[2].dropmenu.selected==2){
    //     printf("search shall be generated\n");
    //     generate_search();
    // }
    if (generate_panel.top_elements[2].dropmenu.selected==0){
        printf("random shall be generated\n");
        generate_random();
    } else if (generate_panel.top_elements[2].dropmenu.selected==1){
        printf("search shall be generated\n");
        generate_search();
    }
}
void reset_project_weights(){
    project.weights=default_weights;
}
int main(int argc, char* argv[]) {
    #include "init.cpp"
    #include "topbar.hpp"
    //for debuging
    // project.students.push_back({
    //     .name = "Jan Novák",
    //     .height = 150,
    //     .in_front = 1.0f,
    //     .prefered = {},
    //     .musnt = {3},
    // });
    // project.students.push_back({
    //     .name = "Radim Ritka",
    //     .height = 150,
    //     .in_front = 1.0f,
    //     .prefered = {4},
    //     .musnt = {1},
    // });
    // project.students.push_back({
    //     .name = "Stavek Boritel",
    //     .height = 150,
    //     .in_front = 1.0f,
    //     .prefered = {},
    //     .musnt = {6},
    // });
    // project.students.push_back({
    //     .name = "Pepa z Depa",
    //     .height = 150,
    //     .in_front = 1.0f,
    //     .prefered = {0},
    //     .musnt = {},
    // });
    // project.students.push_back({
    //     .name = "Igor Kebab",
    //     .height = 150,
    //     .in_front = 1.0f,
    //     .prefered = {4},
    //     .musnt = {1},
    // });
    // project.students.push_back({
    //     .name = "Milan Hladilka",
    //     .height = 150,
    //     .in_front = 1.0f,
    //     .prefered = {1, 2, 3},
    //     .musnt = {3},
    // });
    // project.students.push_back({
    //     .name = "Vladislav Kroupa",
    //     .height = 150,
    //     .in_front = 1.0f,
    //     .prefered = {5},
    //     .musnt = {},
    // });
    // project.students.push_back({
    //     .name = "Stanislav Kroupa",
    //     .height = 150,
    //     .in_front = 1.0f,
    //     .prefered = {2},
    //     .musnt = {},
    // });

    // project.students.push_back({
    //     .name = "Viktor Durian",
    //     .height = 150,
    //     .in_front = 1.0f,
    //     .prefered = {2, 4},
    //     .musnt = {},
    // });
    newfunc();
    update_seznam_panel_scrollables();
    while (running){
        if (clickup){clickup=false;}
        find_front_and_back();
        SDL_GetRendererOutputSize(renderer, &width, &height);
        handle_events();
        for (int i=0;i<project.students.size();i++){
            const auto& s = project.students[i];
            if (s.empty || s.name=="[Nikdo]"){
                project.students.erase(project.students.begin() + i);
                i--; // Adjust index after removal
            }
        }
        SDL_RenderClear(renderer);
        SDL_Rect rect = { 0, 0, width, height };
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &rect);

        if (!(wants_to_sit_with_opened || musnt_sit_with_opened || editstudent_opened || seznam_opened || generate_opened || filemenu.opened || export_opened || weights_opened)){
            active=true;

            int old_gridsize = gridsize;

            if (now_wheel_y > 0) {
                gridsize += ZOOM_SPEED;
                if (gridsize > MAX_GRID_SIZE) {
                    gridsize = MAX_GRID_SIZE;
                }
            }
            else if (now_wheel_y < 0) {
                gridsize -= ZOOM_SPEED;
                if (gridsize < MIN_GRID_SIZE) {
                    gridsize = MIN_GRID_SIZE;
                }
            }

            if (gridsize != old_gridsize) {
                float centerX = width / 2.0f;
                float centerY = height / 2.0f;
                float zoomFactor = (float)gridsize / (float)old_gridsize;

                grid_sx = centerX - (centerX - grid_sx) * zoomFactor;
                grid_sy = centerY - (centerY - grid_sy) * zoomFactor;
            }
        } else {
            active=false;
        }
        render_and_manage_grid();
        get_avgx();
        predek.ypos=project.front_ypos*gridsize/grid_aspect+grid_sy-(STANDARTPICEHEIGHT+STANDARTBORDER*2);
        predek.xpos=project.avgx*gridsize+grid_sx+gridsize;
        updateLabel(renderer, &predek);

        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_Rect rect1 = {0,0,width,pxlenght(TOPPICEHEIGHT)};
        SDL_RenderFillRect(renderer, &rect1);

        updateButton(renderer, &seznam_button);
        updateButton(renderer, &weights_button);
        updateButton(renderer, &generate_button);
        swap_button.force_selected=swapping;
        updateButton(renderer, &swap_button);
        if (wants_to_sit_with_opened){
            updatePanel(renderer, &wants_to_sit_with_panel);
        } else if (musnt_sit_with_opened){
            updatePanel(renderer, &musnt_sit_with_panel);
        } else if (editstudent_opened){
            updatePanel(renderer, &editstudent_panel);
            sync_edit_student_data();
        } else if (seznam_opened){
            updatePanel(renderer, &seznam_panel);
        } else if (generate_opened){
            updatePanel(renderer, &generate_panel);
        } else if (export_opened){
            updatePanel(renderer, &export_panel);
            project.header=export_panel.scrollable_elements[1].textbox.text;
            project.subheader=export_panel.scrollable_elements[3].textbox.text;
            render_page(export_panel.scrollable_elements[4].checkbox.checked, false);
            




// --- Tvé výchozí proměnné a konstanty ---
// pagetex, page_w, page_h
// TOPPICEHEIGHT, width, height
// export_panel.x, export_panel.w
// STANDARTBORDER, COLOR_FOREGROUND

// 1. Spočítáme základní dostupný prostor pro CELÝ prvek (včetně rámečku)
int startX = export_panel.x + export_panel.w;
int startY = TOPPICEHEIGHT;

int maximalniSirka = width - startX;
int maximalniVyska = height - startY;

// 2. Zmenšíme dostupný prostor pro TEXTURU o okraje (rámeček je z obou stran)
// Na šířku i výšku ubereme 2 * STANDARTBORDER
int dostupnaSirkaProTex = maximalniSirka - (2 * STANDARTBORDER);
int dostupnaVyskaProTex = maximalniVyska - (2 * STANDARTBORDER);

// Bezpečnostní pojistka: pokud by byl prostor moc malý, ať nepočítáme se zápornými čísly
if (dostupnaSirkaProTex < 0) dostupnaSirkaProTex = 0;
if (dostupnaVyskaProTex < 0) dostupnaVyskaProTex = 0;

// 3. Spočítáme velikost textury se zachováním poměru stran (uvnitř zmenšeného prostoru)
int cilovaVyskaTex = dostupnaVyskaProTex;
int cilovaSirkaTex = (int)((float)page_w * ((float)cilovaVyskaTex / (float)page_h));

if (cilovaSirkaTex > dostupnaSirkaProTex) {
    cilovaSirkaTex = dostupnaSirkaProTex;
    cilovaVyskaTex = (int)((float)page_h * ((float)cilovaSirkaTex / (float)page_w));
}

// 4. Určíme pozici textury (bude posunutá o STANDARTBORDER doprava a dolů)
SDL_Rect destRectTex;
destRectTex.x = startX + STANDARTBORDER;
destRectTex.y = startY + STANDARTBORDER;
destRectTex.w = cilovaSirkaTex;
destRectTex.h = cilovaVyskaTex;

// 5. VYKRESLENÍ TEXTURY
SDL_RenderCopy(renderer, pagetex, NULL, &destRectTex);

// 6. VYKRESLENÍ TLUSTÉHO RÁMEČKU (směrem ven od textury)
SDL_SetRenderDrawColor(renderer, COLOR_FOREGROUND,255);

for (int i = 0; i < STANDARTBORDER; i++) {
    SDL_Rect borderRect;
    // Kreslíme obdélníky těsně kolem textury, každý další je o 1 pixel větší
    borderRect.x = destRectTex.x - i - 1;
    borderRect.y = destRectTex.y - i - 1;
    borderRect.w = destRectTex.w + (i * 2) + 2;
    borderRect.h = destRectTex.h + (i * 2) + 2;

    SDL_RenderDrawRect(renderer, &borderRect);
}



            
        } else if (weights_opened){
            put_project_weights_to_panel();
            updatePanel(renderer, &weights_panel);   
            if (call_reset_weights){
                reset_project_weights();
                call_reset_weights=false;
            } else {    
                project.weights.height=weights_panel.scrollable_elements[0].slider.value;
                project.weights.in_front=weights_panel.scrollable_elements[1].slider.value;
                project.weights.prefered=weights_panel.scrollable_elements[2].slider.value;
                project.weights.close=weights_panel.scrollable_elements[3].slider.value;
                project.weights.close_dist=weights_panel.scrollable_elements[4].slider.value;
            }
        }
        if (call_wants_to_sit_with){
            wants_to_sit_with();
            call_wants_to_sit_with=false;
        }
        if (call_musnt_sit_with){
            musnt_sit_with();
            call_musnt_sit_with=false;
        }
        if (call_update_seznam_scrollables){
            update_seznam_panel_scrollables();
            call_update_seznam_scrollables=false;
        }
        click_in_meziprostor=false;
        if (delete_student){
            delete_student=false;

            for (auto& s : project.students) {
                // 1. Nejprve upravíme hodnoty, které jsou větší než mazaný student
                for (auto& m : s.musnt) {
                    if (m > now_student) {
                        m--;
                    }
                }
                // 2. Až teď bezpečně smažeme všechny výskyty 'now_student' najednou
                s.musnt.erase(std::remove(s.musnt.begin(), s.musnt.end(), now_student), s.musnt.end());

                // To samé provedeme pro prefered
                for (auto& p : s.prefered) {
                    if (p > now_student) {
                        p--;
                    }
                }
                s.prefered.erase(std::remove(s.prefered.begin(), s.prefered.end(), now_student), s.prefered.end());
            }
            for (auto& s:project.seats){
                if (s.student > now_student) {
                    s.student--;
                } else if (s.student == now_student) {
                    s.student = -1; // nebo nějaká jiná hodnota, která znamená "žádný student"
                }
            }

            // Nakonec bezpečně smažeš studenta z hlavního seznamu
            project.students.erase(project.students.begin() + now_student);
            update_seznam_panel_scrollables();
        }
        updateDropMenu(renderer, &filemenu);

        
        if (clic && !clickedOnSomething){click_in_meziprostor=true;}
        // printf("now student: %d\nprefered: ", now_student);
        // for (int x : project.students[now_student].prefered) {
        //     printf("%d ", x);
        // }
        // printf("\n");
        SDL_RenderPresent(renderer);
        SDL_Delay(20); // makes memory leak slower
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}