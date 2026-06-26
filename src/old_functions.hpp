// void rozesadit_dvojice(){
//     printf("rozesazuji dvojice\n");

//     //test funkcnosti create seating combination
//     // projectType pr;

//     // // Inicializujeme vektor na 17 prázdných lavic
//     // pr.lavice.resize(4); 

//     // // A teď už tvůj testovací kód poletí:
//     // printf("Pred cyklem: Pocet lavic v pr je %zu\n", pr.lavice.size());
//     // fflush(stdout);

//     // for (int i = 0; i < 20; i++) {
//     //     create_seating_combination(i, pr);
        
//     //     printf("Kombinace %d: ", i);
//     //     for (auto& d : pr.lavice) {
//     //         printf("%d ", d.dvojice+1);
//     //     }
//     //     printf("\n");
//     //     fflush(stdout);
//     // }
//     SDL_Surface* canvasSurface = SDL_CreateRGBSurface(0, width, height-TOPPICEHEIGHT, 32, 0, 0, 0, 0);
//     SDL_FillRect(canvasSurface, NULL, SDL_MapRGB(canvasSurface->format, 20, 20, 20));
//     panel p;
//     p.x=width/2-SMALL_PANEL_WIDTH/2;
//     p.y=height/2-SMALL_PANEL_HEIGHT/2;
//     p.w=SMALL_PANEL_WIDTH;
//     p.h=SMALL_PANEL_HEIGHT;
//     p.top_elements.push_back({
//         .type=ELEMENT_LABEL,
//         .label={
//             .xpos=20,
//             .ypos=0,
//             .text="Generování...",
//             .textsize=STANDARTPICEHEIGHT,
//             .origin_left=true
//         }
//     });
//     p.scrollable_elements.push_back({
//         .type=ELEMENT_LABEL,
//         .label={
//             .xpos=0,
//             .ypos=0,
//             .text="Aktualní skóre:",
//             .textsize=STANDARTPICEHEIGHT,
//             .origin_left=true
//         }
//     });
//     p.scrollable_elements.push_back({
//         .type=ELEMENT_LABEL,
//         .label={
//             .xpos=180,
//             .ypos=0,
//             .text="0.0",
//             .textsize=STANDARTPICEHEIGHT,
//             .origin_left=true
//         }
//     });
//     p.scrollable_elements.push_back({
//         .type=ELEMENT_LABEL,
//         .label={
//             .xpos=0,
//             .ypos=STANDARTPICEHEIGHT,
//             .text="Hledání...",
//             .textsize=STANDARTPICEHEIGHT,
//             .origin_left=true
//         }
//     });
//     p.scrollable_elements.push_back({
//         .type=ELEMENT_LABEL,
//         .label={
//             .xpos=180,
//             .ypos=STANDARTPICEHEIGHT*2,
//             .text="0.0",
//             .textsize=STANDARTPICEHEIGHT,
//             .origin_left=true
//         }
//     });
//     p.bottom_elements.push_back({
//         .type = ELEMENT_BUTTON,
//         .button = {
//             .texture = createTextTexture(renderer,"Ukončit"),
//             .xpos = p.w-125,
//             .ypos = 0,
//             .function = [](){
//                 still_generating=false;
//             }
//         }
//     });
//     printf("inicialised panel\n");
//     std::string endstring=" / 100";
//     int time=SDL_GetTicks64();
//     int time2=SDL_GetTicks64();
//     int dots=0;
//     printf("generation start\n");
//     int itter=0;
//     int itr=0;
//     still_generating=true;











//     uint64_t itters=2000000;

//     projectType pr=project;
//     uint64_t max_comb=factorial(project.lavice.size());
//     uint64_t start=0;
//     uint64_t step=(max_comb<=itters)?1:max_comb/itters;
//     projectType best=pr;
//     printf("starting generation, step: %d, max: %d\n", step, max_comb);
//     float best_score=-100000;
//     float result;
//     for (uint64_t comb=start;comb<max_comb && still_generating;comb+=step){
//         create_seating_combination(comb,pr);
//         lavice_to_seats(pr);
//         result=rate_search(pr);
//         if (result>best_score){
//             best_score=result;
//             best=pr;
//         }





//         if (SDL_GetTicks64() - time > RENDER_INTERVAL) {
//             if (SDL_GetTicks64()-time>RENDER_INTERVAL){ //update the events and draw stuff
//                 DrawGraphLineToSurface(canvasSurface,width,height-TOPPICEHEIGHT-100,start,max_comb,comb,result);

//                 time=SDL_GetTicks64();
//                 SDL_RenderClear(renderer);
//                 p.x=(width/2)-(SMALL_PANEL_WIDTH/2);
//                 p.y=(width/2)-(SMALL_PANEL_HEIGHT/2);
//                 if (clickup){clickup=false;}
//                 p.scrollable_elements[1].label.text=std::to_string((int)(best_score*100.0f))+endstring;
//     // (double)(comb - start) / (double)(max_comb - start);
//                 p.scrollable_elements[3].label.text=std::to_string((double)(comb - start) / (double)(max_comb - start));
//                 handle_events();
//                 active=false;






// // 1. Vyčistíš okno jako obvykle

// // 2. Vytvoříš dočasnou texturu z tvého permanentního canvasSurface
// SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(renderer, canvasSurface);

// // 3. Definuješ cílový obdélník: X=0, Y=TOPPICEHEIGHT
// SDL_Rect dst_rect = { 0, TOPPICEHEIGHT, width, height-TOPPICEHEIGHT };

// // 4. Blitneš (zkopíruješ) to do okna
// SDL_RenderCopy(renderer, tempTexture, NULL, &dst_rect);

// // 5. SMAŽEŠ dočasnou texturu z paměti grafiky, jinak ti spadne PC (memory leak)!
// SDL_DestroyTexture(tempTexture);






//                 project=pr;
//                 render_and_manage_grid();

//                 updatePanel(renderer, &p);
//                 click_in_meziprostor=false;
//                 if (clic && !clickedOnSomething){click_in_meziprostor=true;}
//                 SDL_RenderPresent(renderer);
//             }
//         }
//         if (SDL_GetTicks64()-time2>500){
//                 time2=SDL_GetTicks64();
//                 dots++;
//                 if (dots>6){
//                     dots=0;
//                 }
//                 std::string dotstring="";
//                 for (int i=0;i<dots;i++){
//                     dotstring+=".";
//                 }
//                 p.top_elements[0].label.text="Generování"+dotstring;
//             }





//     }
//     project=best;

// }