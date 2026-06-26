#include "includes.hpp"
void generate_random(){
    size_t num_students = project.students.size();
    size_t num_seats = project.seats.size();

    // Pokud nemáme žádné židle nebo studenty, nemá cenu nic dělat
    if (num_seats == 0) return;

    // 1. Inicializace náhodného generátoru
    std::random_device rd;
    std::mt19937 g(rd());

    // 2. Nejdřív připravíme a ZAMÍCHÁME studenty
    std::vector<int> student_indices(num_students);
    std::iota(student_indices.begin(), student_indices.end(), 0);
    std::shuffle(student_indices.begin(), student_indices.end(), g);

    // 3. Teď připravíme a ZAMÍCHÁME indexy židlí (0, 1, 2, ..., num_seats - 1)
    std::vector<size_t> seat_indices(num_seats);
    std::iota(seat_indices.begin(), seat_indices.end(), 0);
    std::shuffle(seat_indices.begin(), seat_indices.end(), g);

    // 4. Všem židlím nejdříve nastavíme -1 (vyčistíme je jako prázdné)
    for (size_t i = 0; i < num_seats; ++i) {
        project.seats[i].student = -1;
    }

    // 5. Rozmístíme studenty na náhodně vybrané židle
    size_t limit = std::min(num_seats, num_students);
    for (size_t i = 0; i < limit; ++i) {
        // Vezmeme i-tý náhodný index židle a posadíme tam i-tého náhodného studenta
        size_t random_seat_idx = seat_indices[i];
        project.seats[random_seat_idx].student = student_indices[i];
    }
}
void find_front_and_back(){
    // printf("finding front and back...\n");
    project.front_ypos=1000000000;
    for (auto s:project.seats){
        if (s.ypos<project.front_ypos){
            project.front_ypos=s.ypos;
        }
    }
    project.back_ypos=project.front_ypos;
    for (auto s:project.seats){
        if (s.ypos>project.back_ypos){
            project.back_ypos=s.ypos;
        }
    }
    // printf("the front is %d and the back is %d\n", project.front_ypos, project.back_ypos);
    SDL_Delay(10);
}
int find_left_neighbor(seat s){
    for (auto seat:project.seats){
        if (seat.xpos==s.ypos-1){
            return seat.student;
        }
    }
    return -1;
}
int find_right_neighbor(seat s){
    for (auto seat:project.seats){
        if (seat.xpos==s.ypos+1){
            return seat.student;
        }
    }
    return -1;
}
void swap_seats(int s1, int s2){
    int s=project.seats[s1].student;
    project.seats[s1].student=project.seats[s2].student;
    project.seats[s2].student=s;
    return;
}
float rate_project(){
    //rates the current project by the different aspects
    float score=0.0f;
    float this_score=0.0f;
    for (auto se:project.seats){
        if (se.student==-1){continue;}
        student s=project.students[se.student];

        int left_neighbor=((find_left_neighbor(se)!=-1) ? find_left_neighbor(se) : -1);
        int right_neighbor=((find_right_neighbor(se)!=-1) ? find_right_neighbor(se) : -1);


        this_score=(
                        (
                            (
                                ((project.back_ypos-se.ypos)/nonzero(se.ypos-project.front_ypos))/
                                nonzero(project.back_ypos-project.front_ypos)
                            )*s.weights.in_front //in front factor
                        )+
                        (
                            (
                                ((in(left_neighbor, s.prefered) ? 1.0f : 0.0f)+
                                (in(right_neighbor, s.prefered) ? 1.0f : 0.0f))/2.0f
                            )*s.weights.prefered
                        )+
                        (
                            (
                                ((in(left_neighbor, s.musnt) ? 0.0f : 1.0f)+
                                (in(right_neighbor, s.musnt) ? 0.0f : 1.0f))/2.0f
                            )*s.weights.musnt
                        )+
                        (
                            (
                                (
                                    (
                                        (left_neighbor!=-1)?(
                                            (
                                                !s.sit_with_different_gender && (project.students[left_neighbor].gender!=s.gender)
                                            ) ? 0.0f : 1.0f
                                        ) : 1.0f
                                    ) + (
                                        (right_neighbor!=-1)?(
                                            (
                                                !s.sit_with_different_gender && (project.students[right_neighbor].gender!=s.gender)
                                            ) ? 0.0f : 1.0f
                                        ) : 1.0f
                                    )
                                )/2.0f
                            )*s.weights.sit_with_another_gender
                        )
                    );
                    
        score+=this_score;
    }
    score/=project.seats.size();
    return score;
    // return 0.0f;
}
void generate_genetic(){
    printf("generating geneticaly\n");
    still_generating=true;
    float best_score=0.0f;
    float now_score;
    find_front_and_back(); //run or it will crash
    generate_random();
    projectType best_yet=project;
    projectType zahola;
    printf("inicialising panel...\n");

    panel p;
    p.x=width/2-SMALL_PANEL_WIDTH/2;
    p.y=width/2-SMALL_PANEL_HEIGHT/2;
    p.w=SMALL_PANEL_WIDTH;
    p.h=SMALL_PANEL_HEIGHT;
    p.top_elements.push_back({
        .type=ELEMENT_LABEL,
        .label={
            .xpos=20,
            .ypos=0,
            .text="Generování...",
            .textsize=STANDARTPICEHEIGHT,
            .origin_left=true
        }
    });
    p.scrollable_elements.push_back({
        .type=ELEMENT_LABEL,
        .label={
            .xpos=0,
            .ypos=0,
            .text="Aktualní skóre:",
            .textsize=STANDARTPICEHEIGHT,
            .origin_left=true
        }
    });
    p.scrollable_elements.push_back({
        .type=ELEMENT_LABEL,
        .label={
            .xpos=180,
            .ypos=0,
            .text="0.0",
            .textsize=STANDARTPICEHEIGHT,
            .origin_left=true
        }
    });
    p.scrollable_elements.push_back({
        .type=ELEMENT_LABEL,
        .label={
            .xpos=0,
            .ypos=STANDARTPICEHEIGHT,
            .text="Náhodné zkoušení...",
            .textsize=STANDARTPICEHEIGHT,
            .origin_left=true
        }
    });
    p.bottom_elements.push_back({
        .type = ELEMENT_BUTTON,
        .button = {
            .texture = createTextTexture(renderer,"Ukončit"),
            .xpos = p.w-125,
            .ypos = 0,
            .function = [](){
                still_generating=false;
            }
        }
    });
    printf("inicialised panel\n");
    std::string endstring=" / 100";
    int time=SDL_GetTicks64();
    int time2=SDL_GetTicks64();
    int dots=0;
    still_generating=true;
    printf("generation start\n");
    bool guessing=true;
    int itter=0;
    int itr=0;
    int num_of_seats=project.seats.size()-1;
    init_weights();

    while (still_generating){
        if (guessing){
            generate_random();
            if (itter>project.guesswork_itterations){
                guessing=false;
                p.scrollable_elements[2].label.text="Iterace...";
                printf("start itterating\n");
            }
        } else {
            swap_seats(randint(0,num_of_seats), randint(0,num_of_seats));
        }
        itter++;
        now_score=rate_project();
        if (now_score>best_score){
            best_score=now_score;
            best_yet=project;
        } else if (!guessing) { 
            project = best_yet; 
        }
        if (itter>itr){
            itr=itter+100;
            if (SDL_GetTicks64()-time>RENDER_INTERVAL){ //update the events and draw stuff
                time=SDL_GetTicks64();
                zahola=project;
                project=best_yet;
                SDL_RenderClear(renderer);
                p.x=(width/2)-(SMALL_PANEL_WIDTH/2);
                p.y=(width/2)-(SMALL_PANEL_HEIGHT/2);
                if (clickup){clickup=false;}
                p.scrollable_elements[1].label.text=std::to_string((int)(best_score*100.0f))+endstring;
                handle_events();
                active=false;
                render_and_manage_grid();
                updatePanel(renderer, &p);
                click_in_meziprostor=false;
                if (clic && !clickedOnSomething){click_in_meziprostor=true;}
                SDL_RenderPresent(renderer);
                project=zahola;
            }
            if (SDL_GetTicks64()-time2>500){
                time2=SDL_GetTicks64();
                dots++;
                if (dots>6){
                    dots=0;
                }
                std::string dotstring="";
                for (int i=0;i<dots;i++){
                    dotstring+=".";
                }
                p.top_elements[0].label.text="Generování"+dotstring;
            }
        }

    }
    project=best_yet;
    project.score=best_score;
}