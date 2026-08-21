#include "includes.hpp"
/*
SEARCH ALGORITHM

-first calculates new in_front for each student using in_front and height
-then looks at all the lavices ve tride a rozskupi single lavice onto 2lavice

-odted pojem lavice = 2lavice
-than makes a list of students at each student
    -iteracne vyrazovat podle musnt
-pak udela test jestli je (pocet lavic-pocet dvojic)>=(zaku co musi sami)

-nasledne se looks into prefered and makes dvojice
-hill clinbimg
    -bude zkouset vsechny moznosti, ale kazdych napr. 50 (tisic) moznosti skip
    -v oblasti nejuspesnejsich cisel pokracuje search

*/
void prepare_height(){
    projectType pr=project;
    std::sort(pr.students.begin(), pr.students.end(),
    [](const student& a, const student& b) {
        return a.height > b.height;
    });
    float n=0;
    float step=1.0f/(float)(project.students.size()-1);
    for (auto& s:pr.students){
        s.in_front=n;
        n+=step;
    }
    float w_infront=project.weights.in_front;
    float w_height=project.weights.height;
    float sum = w_infront + w_height;

    if (sum > 0.0f) {
        w_infront /= sum;
        w_height  /= sum;
    }
    for (auto& s:project.students){
        for (auto& s2:pr.students){
            if (s2.height==s.height){
                s.in_front=(s.in_front*w_infront)+(s2.in_front*w_height);
                break;
            }
        }
    }
    project.tallest=0;
    project.shortest=10000000;
    for (auto& s:project.students){
        if (s.height<project.shortest){
            project.shortest=s.height;
        }
        if (s.height>project.tallest){
            project.tallest=s.height;
        }
    }
}
bool generate_lavice() {
    project.lavice.clear();
    size_t n = project.seats.size();
    if (n == 0) return true;

    std::vector<bool> visited(n, false);

    for (size_t i = 0; i < n; ++i) {
        if (visited[i]) continue;

        std::vector<int> component;
        std::vector<int> queue;
        
        queue.push_back(i);
        visited[i] = true;

        size_t head = 0;
        while (head < queue.size()) {
            int curr_idx = queue[head++];
            component.push_back(curr_idx);

            for (size_t j = 0; j < n; ++j) {
                if (visited[j]) continue;

                int dx = std::abs(project.seats[curr_idx].xpos - project.seats[j].xpos);
                int dy = std::abs(project.seats[curr_idx].ypos - project.seats[j].ypos);

                if ((dx == 1 && dy == 0) || (dx == 0 && dy == 1)) {
                    visited[j] = true;
                    queue.push_back(j);
                }
            }
        }

        if (component.size() == 1) {
            msgbox("Chyba: Nalezena 1lavice bez souseda. Program podporuje pouze 2lavice.");
            return false;
        } 
        else if (component.size() > 2) {
            msgbox("Chyba: Nalezena skupina 3 a vice 1lavic vedle sebe! Program podporuje pouze samostatne 2lavice.");
            return false;
        } 
        else if (component.size() == 2) {
            lavice l;
            l.seat1 = component[0];
            l.seat2 = component[1];
            project.lavice.push_back(l);
        }
    }
    return true;
}
/*
vygeneruje rozlozeni dvojic do lavic podle cisla kombinace
*/
void create_seating_combination(uint64_t cislo_kombinace, projectType &project) {
    int n = project.lavice.size();
    if (n == 0) return;

    // 1. Předpočítáme si faktoriály, abychom z čísla ukrajovali od největšího
    // f[0] = 0!, f[1] = 1!, ..., f[17] = 17!
    std::vector<uint64_t> f(n + 1, 1);
    for (int i = 1; i <= n; i++) {
        f[i] = f[i - 1] * i;
    }

    // Pole pro sledování, které dvojice už sedí
    std::vector<bool> pouzito(n, false);

    // 2. Procházíme židle odpředu dozadu
    for (int i = 0; i < n; i++) {
        // Kolik prvků zbývá vybrat pro zbytek řady
        int zbyva_prvku = n - 1 - i; 
        
        // Zjistíme, kolikrát se faktoriál zbývajících míst vejde do našeho čísla
        uint64_t vybrany_index = cislo_kombinace / f[zbyva_prvku];
        
        // Zbytek si necháme pro další židle
        cislo_kombinace = cislo_kombinace % f[zbyva_prvku];

        // 3. Najdeme 'vybrany_index'-tou VOLNOU dvojici v pořadí (od 0 nahoru)
        uint64_t volne_nalezeno = 0;
        int id_dvojice = 0;
        
        for (int j = 0; j < n; j++) {
            if (!pouzito[j]) {
                if (volne_nalezeno == vybrany_index) {
                    id_dvojice = j;
                    break;
                }
                volne_nalezeno++;
            }
        }

        // 4. Usadíme ji na židli a zamkneme
        project.lavice[i].dvojice = id_dvojice;
        pouzito[id_dvojice] = true;
    }
}
/*
Finds all possible potencial neighbors for each student
*/
bool search_can(){
    std::erase_if(project.students, [](const auto& s) {
        return s.empty; // Bez závorek, protože je to bool proměnná
    });
    if (project.students.size()<project.seats.size()){
        for (int i=0;i<=(project.seats.size()-project.students.size());i++){
            project.students.push_back({
                .name="[Nikdo]",
                .empty=true,
                .musnt={},
                .prefered={},
                .sit_with_different_gender=true
            });
        }
    }
    for (int si=0;si<project.students.size();si++){
        student& s=project.students[si];
        s.can.clear();
        for (int n=0;n<project.students.size();n++){
            if ((!in(si, project.students[n].musnt) && !in(n,s.musnt) && si!=n && ((project.students[n].sit_with_different_gender && s.sit_with_different_gender) || (project.students[n].gender==s.gender))) || (project.students[n].empty || s.empty)){
                s.can.push_back(n);
            }
        }
    }
    for (auto& s:project.students){
        if (s.can.size()==0){
            std::stringstream ss;
            ss << "Vaše požadavky pro „žák nesmí sedět s\" nebo „žák nemůže sedět s žákem jiného pohlaví\" jsou matematicky nesplnitelné. Zadání si protiřecí. " 
            << s.name 
            << " nemá žádného potenciálního souseda. [search_can]";
            std::string message = ss.str();            msgbox(message.c_str());
            return false;
        }
    }
    return true;
}
/*
vypise dvojice i se score
*/
void vyprintuj_dvojice(const std::vector<dvojce>& dvojice, const projectType& project) {
    std::cout << "\n================================" << std::endl;
    std::cout << "          VÝPIS DVOJIC           " << std::endl;
    std::cout << "================================" << std::endl;

    for (size_t i = 0; i < dvojice.size(); i++) {
        const auto& d = dvojice[i];
        
        // Vytáhneme jména studentů z projektu pomocí indexů s1 a s2
        std::string jmeno1 = project.students[d.s1].name;
        std::string jmeno2 = project.students[d.s2].name;

        // Vyprintujeme pořadí, dvojici a jejich skóre
        // std::setw(15) zajistí, že jména budou zarovnaná a tabulka nebude rozlítaná
        std::cout << std::setw(2) << i + 1 << ". " 
                  << std::setw(12) << jmeno1 << "  +  " 
                  << std::setw(12) << jmeno2 
                  << "   -->   Skóre: " << d.score << std::endl;
    }
    std::cout << "=============================================\n" << std::endl;
}
bool find_dvojice(){
    std::vector<dvojce> dvojice={};
    float prevence_bezdomovectvi=100.0f;
    //creates all possible options
    for (int s=0;s<project.students.size();s++){
        for (auto i:project.students[s].can){
            if (s<i){
                dvojice.push_back({s,i, 0.0f});
            }
        }
    }
    for (auto& d:dvojice){
        // rates the dvojices
        // prefered rating
        float sc1=0.0f;
        float sc2=0.0f;
        if (project.students[d.s1].prefered.size()==0){
            sc1=1.0f;
        } else if (in(d.s1, project.students[d.s2].prefered)){
            int place=0;
            for (place=0;place<project.students[d.s2].prefered.size();place++){
                if (project.students[d.s2].prefered[place]==d.s1){
                    break;
                }
            }
            sc1=1.0f/(place+1);
        }
        if (project.students[d.s2].prefered.size()==0){
            sc2=1.0f;
        } else if (in(d.s2, project.students[d.s1].prefered)){
            int place=0;
            for (place=0;place<project.students[d.s1].prefered.size();place++){
                if (project.students[d.s1].prefered[place]==d.s2){
                    break;
                }
            }
            sc2=1.0f/(place+1);
        }
        // printf("heightscore: %f\n", ((1.0f-fabs(project.students[d.s1].in_front-project.students[d.s2].in_front))*(project.weights.in_front+project.weights.height)));
        d.score=((((project.students[d.s1].empty||project.students[d.s2].empty)?(1.0f):((sc1==0.0f||sc2==0.0f)?(((sc1+sc2)/2.0f)-project.penalizace_za_nespokojenost):((sc1+sc2)/2.0f)))+prevence_bezdomovectvi)*project.weights.prefered)   +   ((1.0f-fabs(project.students[d.s1].in_front-project.students[d.s2].in_front))*(project.weights.in_front+project.weights.height));
    }

    //sort ehtm
    // std::sort(dvojice.begin(), dvojice.end(), std::greater<dvojce>());
    std::sort(dvojice.begin(), dvojice.end(), [](const dvojce& a, const dvojce& b) {
        return a.score > b.score;
    });

    vyprintuj_dvojice(dvojice,project);








// --- 3. KONVERZE DATA S ŠABLONOU <int> (OPRAVENO) ---
    std::vector<mwmatching::Edge<int>> edges;

    for (const auto& d : dvojice) {
        int vaha = static_cast<int>(d.score * 1000.0f);
        if (d.score == 0.0f) {
            vaha = -999999; 
        }
        
        // Voláme přímo ten konstruktor, který kompilátor vyžaduje:
        // Edge(VertexId x, VertexId y, WeightType w)
        edges.push_back(mwmatching::Edge<int>(d.s1, d.s2, vaha));
    }

    // --- 4. VÝPOČET ---
    // Algoritmus se sám přizpůsobí podle toho, že dostal Edge<int>
    auto result_pairs = mwmatching::maximum_weight_matching(edges);

    // --- 5. ZPRACOVÁNÍ VÝSLEDKŮ (.first a .second) ---
    std::vector<dvojce> finalni_rozesazeni;
    int usazeni_studenti = 0;

    // result_pairs obsahuje dvojice typu std::pair.
    // První student je v .first, druhý v .second.
    for (const auto& pair : result_pairs) {
        int u = static_cast<int>(pair.first);
        int v = static_cast<int>(pair.second);

        // Najdeme původní skóre pro tuto dvojici
        float puvodni_score = 0.0f;
        for (const auto& d : dvojice) {
            if ((d.s1 == u && d.s2 == v) || (d.s1 == v && d.s2 == u)) {
                puvodni_score = d.score;
                break;
            }
        }

        // Pokud to spojilo lidi se zakázanou vazbou -> stop
        if (puvodni_score == 0.0f) {
            msgbox("Vaše požadavky pro „žák nesmí sedět s\" nebo „žák nemůže sedět s žákem jiného pohlaví\" jsou matematicky nesplnitelné. Zadání si protiřečí. [blossom find]");
            return false;
        }

        finalni_rozesazeni.push_back({u, v, puvodni_score});
        usazeni_studenti += 2;
    }

    float avg_score=0.0f;
    for (auto &d:finalni_rozesazeni){
        d.score-=prevence_bezdomovectvi;
        avg_score+=d.score;
    }
    avg_score/=finalni_rozesazeni.size();
    vyprintuj_dvojice(finalni_rozesazeni, project);
    printf("Skore tridy: %f\n", avg_score);

    if (finalni_rozesazeni.size()*2 < project.students.size()) {
        // msgbox("Nebylo možné napárovat všechny studenty. Podmínky si protiřečí. [blossom find]");

        // Nasypeme ID všech spárovaných studentů do setu pro rychlé vyhledávání
        std::set<int> sparovani;
        for (const auto& d : finalni_rozesazeni) {
            sparovani.insert(d.s1);
            sparovani.insert(d.s2);
        }

        // Najdeme, kdo chybí, a poskládáme jména přes stringstream
        std::stringstream ss;
        ss << "Nebylo možné napárovat všechny studenty. Podmínky si protiřečí.\n\nNespárovaní žáci:\n";

        bool prvni = true;
        for (size_t i = 0; i < project.students.size(); ++i) {
            // Kontrolujeme index 'i' místo 's.id'
            if (sparovani.find(static_cast<int>(i)) == sparovani.end()) {
                if (!prvni) {
                    ss << ", ";
                }
                ss << project.students[i].name;
                prvni = false;
            }
        }
        ss << "\n\n[blossom find]";

        msgbox(ss.str().c_str());




        return false;
    }

    project.dvojice = finalni_rozesazeni;








    return true;
}
/*
factorial
*/
uint64_t factorial(int n) {
    if (n < 0) return 0; // faktoriál pro záporná čísla neexistuje

    uint64_t result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}
/*
hill climbs and places each pair into a desk
*/
float rate_search(projectType project){
    float score=0.0f;

    for (auto se:project.seats){
        if (se.student==-1){continue;}
        student s=project.students[se.student];
        score+=(((project.back_ypos-se.ypos)/nonzero(se.ypos-project.front_ypos))/
                    nonzero(project.back_ypos-project.front_ypos))*s.in_front*(project.weights.in_front+project.weights.height);
        int ind=0;
        // for (int& s:s.prefered){
        //     float x=0;
        //     float y=0;
        //     for (auto& d:project.seats){
        //         if (d.student==s){
        //             x=(float)d.xpos;
        //             y=(float)d.ypos;
        //             break;
        //         }
        //     }
        //     if (sqrt(x*x+y*y)<project.weights.close_dist){
        //         score+=(project.weights.prefered)*(1.0f/(float)(ind+1));
        //     }
        //     ind++;
        // }
        // OPRAVENÝ VNITŘEK CYKLU PRO PREFERED:
for (int& s_pref : s.prefered) {
    float x = 0;
    float y = 0;
    bool nalezen = false;
    for (auto& d : project.seats) {
        if (d.student == s_pref) {
            x = (float)d.xpos;
            y = (float)d.ypos;
            nalezen = true;
            break;
        }
    }
    
    if (nalezen) {
        // ROZDÍL SOUŘADNIC MEZI NIMI:
        float dx = (float)se.xpos - x;
        float dy = (float)se.ypos - y;
        float vzdalenost = sqrt(dx*dx + dy*dy);

        // GRADIENT BONUSU:
        if (vzdalenost < project.weights.close_dist) {
            // Spočítáme základní váhu podle pořadí v preferencích (ind)
            float max_vaha = project.weights.close * (1.0f / (float)(ind + 1));

            if (vzdalenost <= 1.0f) {
                // Extrémně blízko -> plný bonus podle priority
                score += max_vaha;
            } else {
                // Lineární gradient mezi 1.0f (plný bonus) a project.weights.close_dist (žádný bonus)
                // Vzorec dává 1.0 pro vzdálenost 1.0f a klesá k 0.0 pro vzdálenost project.weights.close_dist
                float faktor = 1.0f - ((vzdalenost - 1.0f) / (project.weights.close_dist - 1.0f));
                
                // Přičteme poměrnou část bonusu
                score += max_vaha * faktor;
            }
        }
    }
    ind++;
}

for (int& s_musnt : s.musnt) { // procházíme lidi, se kterými student NESMÍ sedět
    float x = 0;
    float y = 0;
    bool nalezen = false;
    for (auto& d : project.seats) {
        if (d.student == s_musnt) {
            x = (float)d.xpos;
            y = (float)d.ypos;
            nalezen = true;
            break;
        }
    }
    
    if (nalezen) {
        // ROZDÍL SOUŘADNIC MEZI NIMI:
        float dx = (float)se.xpos - x;
        float dy = (float)se.ypos - y;
        float vzdalenost = sqrt(dx*dx + dy*dy);

        // GRADIENT PENALIZACE:
        if (vzdalenost < project.weights.close_dist) {
            if (vzdalenost <= 1.0f) {
                // Extrémně blízko (1.0f a méně) -> plná penalizace -1.0f
                score -= 1.0f;
            } else {
                // Lineární gradient mezi 1.0f (penalizace -1.0) a project.weights.close_dist (penalizace 0.0)
                // Vzorec: (vzdalenost - min) / (max - min) nám dá hodnotu 0.0 až 1.0
                // My to ale chceme otočeně (čím blíž, tím větší penalizace), proto: 1.0 - (...)
                float faktor = 1.0f - ((vzdalenost - 1.0f) / (project.weights.close_dist - 1.0f));
                
                score -= faktor; // odečteme hodnotu z gradientu (např. 0.5f)
            }
        }
    }
    ind++; // předpokládám, že index si tu pro jistotu necháváš, i když pro .musnt ho teď na výpočet skóre nepoužíváme
}


    }
    score/=(float)(project.seats.size());





    return score;
}
// Funkce pro nakreslení jedné žluté vertikální čáry přímo do SDL_Surface
void DrawGraphLineToSurface(SDL_Surface* canvasSurface, int canvas_width, int graph_height, uint64_t start, uint64_t max_comb, uint64_t comb, float result) {
    
    // Ošetření dělení nulou, kdyby náhodou max_comb == start
    if (max_comb <= start) return;

    // 1. PŘEPOČET X s přesností na double (kvůli obřím uint64_t číslům)
    double progress = (double)(comb - start) / (double)(max_comb - start);
    int line_x = (int)(progress * canvas_width);

    // Ošetření, aby čára nevyletěla z pole pixelů surface
    if (line_x < 0) line_x = 0;
    if (line_x >= canvas_width) line_x = canvas_width - 1;

    // 2. PŘEPOČET Y: Výška čáry
    int line_height = (int)(result * graph_height);
    
    // Výpočet startu a konce kreslení (v SDL roste Y dolů)
    int y_start = graph_height - line_height;
    int y_end = graph_height;

    // 3. KRESLENÍ DO SURFACE (Zápis přímo do pixelů)
    // Vygenerujeme si žlutou barvu ve formátu, jaký používá tvůj surface
    Uint32 yellow_color = SDL_MapRGB(canvasSurface->format, 255, 255, 0);

    // Uzamčení surface, pokud to vyžaduje (dobrý zvyk pro přímý zápis)
    if (SDL_MUSTLOCK(canvasSurface)) {
        if (SDL_LockSurface(canvasSurface) < 0) return;
    }

    // Ukazatel na pixely našeho surface (předpokládáme 32-bitový formát)
    Uint32* pixels = (Uint32*)canvasSurface->pixels;
    int pitch_pixels = canvasSurface->pitch / 4; // Šířka řádku v pixelech (bitech / 4)

    // Vykreslení 1px široké vertikální čáry od y_start do y_end
    for (int y = y_start; y < y_end; y++) {
        if (y >= 0 && y < graph_height) {
            pixels[y * pitch_pixels + line_x] = yellow_color;
        }
    }

    // Odemčení surface
    if (SDL_MUSTLOCK(canvasSurface)) {
        SDL_UnlockSurface(canvasSurface);
    }
}
/*
puts desk information into seats
*/
void lavice_to_seats(projectType& pr){
    for (auto& l:pr.lavice){
        pr.seats[l.seat1].student=pr.dvojice[l.dvojice].s1;
        pr.seats[l.seat2].student=pr.dvojice[l.dvojice].s2;
    }
}
/*
posadi dvojice do lavic pomoci hill climberu
*/
void rozesadit_dvojice(){
    printf("rozesazuji dvojice\n");

    SDL_Surface* canvasSurface = SDL_CreateRGBSurface(0, width, height-TOPPICEHEIGHT, 32, 0, 0, 0, 0);
    SDL_FillRect(canvasSurface, NULL, SDL_MapRGB(canvasSurface->format, 20, 20, 20));

    panel p;
    p.x=width/2-SMALL_PANEL_WIDTH/2;
    p.y=height/2-SMALL_PANEL_HEIGHT/2;
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
        .label={.xpos=0,.ypos=0,.text="Nejlepší skóre:",.textsize=STANDARTPICEHEIGHT,.origin_left=true}
    });

    p.scrollable_elements.push_back({
        .type=ELEMENT_LABEL,
        .label={.xpos=190,.ypos=0,.text="0.0",.textsize=STANDARTPICEHEIGHT,.origin_left=true}
    });

    p.scrollable_elements.push_back({
        .type=ELEMENT_LABEL,
        .label={.xpos=0,.ypos=STANDARTPICEHEIGHT,.text="Hledání...",.textsize=STANDARTPICEHEIGHT,.origin_left=true}
    });

    p.scrollable_elements.push_back({
        .type=ELEMENT_LABEL,
        .label={.xpos=50,.ypos=STANDARTPICEHEIGHT*2,.text="0.0",.textsize=STANDARTPICEHEIGHT,.origin_left=true}
    });

    p.bottom_elements.push_back({
        .type = ELEMENT_BUTTON,
        .button = {
            .texture = createTextTexture(renderer,"Ukončit"),
            .xpos = p.w-125,
            .ypos = 0,
            .function = [](){ still_generating=false; }
        }
    });

    printf("inicialised panel\n");

    std::string endstring="";
    int time=SDL_GetTicks64();
    int time2=SDL_GetTicks64();
    int dots=0;

    still_generating=true;

    uint64_t itters=500000;

    projectType pr=project;
    uint64_t max_comb=factorial(project.lavice.size());
    uint64_t start=0;
    uint64_t step=(max_comb<=itters)?1:max_comb/itters;
    float graphstep=1.0f/(float)width;
    double progress=0.0;
    double last_progress=0.0;
    projectType best=pr;
    float best_score=-100000;
    float result;

    // =========================================================
    // 🔥 ADD: PEAK STORAGE (NEINVAZIVNÍ)
    // =========================================================
    struct Peak {
        uint64_t comb;
        float score;
    };

    std::vector<Peak> peaks;

    auto add_peak = [&](uint64_t c, float s){
        peaks.push_back({c,s});
        if (peaks.size() > 50) {
            std::sort(peaks.begin(), peaks.end(),
                [](const Peak& a, const Peak& b){
                    return a.score > b.score;
                });
            peaks.resize(20);
        }
    };

    printf("starting generation, step: %llu max: %llu\n",
        (unsigned long long)step,
        (unsigned long long)max_comb);

    // =========================================================
    // PASS 1 (TVŮJ KÓD BEZE ZMĚNY + ADD PEAKS)
    // =========================================================
    for (uint64_t comb=start;comb<max_comb && still_generating;comb+=step){

        create_seating_combination(comb,pr);
        lavice_to_seats(pr);

        result=rate_search(pr);

        if (result>best_score){
            best_score=result;
            best=pr;
        }

        // 🔥 ADD ONLY
        add_peak(comb, result);
        progress=((double)(comb - start) / (double)(max_comb - start))*100.0f;
        if (last_progress+graphstep<progress){
            last_progress=progress;
            DrawGraphLineToSurface(canvasSurface,width,height-TOPPICEHEIGHT-100,start,max_comb,comb,result);
        }

        if (SDL_GetTicks64() - time > RENDER_INTERVAL) {
            if (SDL_GetTicks64()-time>RENDER_INTERVAL){


                time=SDL_GetTicks64();

                SDL_RenderClear(renderer);

                p.x=(width/2)-(SMALL_PANEL_WIDTH/2);
                p.y=(height/2)-(SMALL_PANEL_HEIGHT/2);

                if (clickup){clickup=false;}

                p.scrollable_elements[1].label.text=
                    std::to_string((int)(best_score*100.0f))+endstring;

                p.scrollable_elements[3].label.text=
                    std::to_string(progress);

                handle_events();
                active=false;

                SDL_Texture* tempTexture =
                    SDL_CreateTextureFromSurface(renderer, canvasSurface);

                SDL_Rect dst_rect = {0, TOPPICEHEIGHT, width, height-TOPPICEHEIGHT};
                SDL_RenderCopy(renderer, tempTexture, NULL, &dst_rect);
                SDL_DestroyTexture(tempTexture);

                project=pr;
                render_and_manage_grid();

                updatePanel(renderer, &p);
                click_in_meziprostor=false;
                if (clic && !clickedOnSomething){click_in_meziprostor=true;}

                SDL_RenderPresent(renderer);
            }
        }

        if (SDL_GetTicks64()-time2>500){
            time2=SDL_GetTicks64();
            dots++;
            if (dots>6) dots=0;

            std::string dotstring="";
            for (int i=0;i<dots;i++) dotstring+=".";

            p.top_elements[0].label.text="Generování"+dotstring;
        }
    }

    // =========================================================
    // 🔥 PASS 2: ZOOM AROUND PEAKS (ADD ONLY)
    // =========================================================
    for (int pass=0; pass<3 && still_generating; pass++){
        if (peaks.empty()) break;
        SDL_FillRect(
            canvasSurface,
            NULL,
            SDL_MapRGB(canvasSurface->format, 0, 0, 0)
        );
        std::vector<Peak> new_peaks;

        uint64_t window = std::max<uint64_t>(1, max_comb / (8ULL << pass));
        uint64_t local_step = std::max<uint64_t>(1, window / 200);

        for (auto &pk : peaks){

            uint64_t s = (pk.comb > window ? pk.comb - window : 0);
            uint64_t e = std::min(max_comb, pk.comb + window);

            for (uint64_t comb=s; comb<e && still_generating; comb+=local_step){

                create_seating_combination(comb,pr);
                lavice_to_seats(pr);

                result=rate_search(pr);

                if (result>best_score){
                    best_score=result;
                    best=pr;
                }

                new_peaks.push_back({comb,result});

        if (SDL_GetTicks64() - time > RENDER_INTERVAL) {
            if (SDL_GetTicks64()-time>RENDER_INTERVAL){

                DrawGraphLineToSurface(canvasSurface,width,height-TOPPICEHEIGHT-100,s,e,comb,result);

                time=SDL_GetTicks64();

                SDL_RenderClear(renderer);

                p.x=(width/2)-(SMALL_PANEL_WIDTH/2);
                p.y=(height/2)-(SMALL_PANEL_HEIGHT/2);

                if (clickup){clickup=false;}

                p.scrollable_elements[1].label.text=
                    std::to_string((int)(best_score*100.0f))+endstring;

                p.scrollable_elements[3].label.text=
                    std::to_string((double)(comb - start) / (double)(max_comb - start));

                handle_events();
                active=false;

                SDL_Texture* tempTexture =
                    SDL_CreateTextureFromSurface(renderer, canvasSurface);

                SDL_Rect dst_rect = {0, TOPPICEHEIGHT, width, height-TOPPICEHEIGHT};
                SDL_RenderCopy(renderer, tempTexture, NULL, &dst_rect);
                SDL_DestroyTexture(tempTexture);

                project=pr;
                render_and_manage_grid();

                updatePanel(renderer, &p);
                click_in_meziprostor=false;
                if (clic && !clickedOnSomething){click_in_meziprostor=true;}

                SDL_RenderPresent(renderer);
            }
        }

        if (SDL_GetTicks64()-time2>500){
            time2=SDL_GetTicks64();
            dots++;
            if (dots>6) dots=0;

            std::string dotstring="";
            for (int i=0;i<dots;i++) dotstring+=".";

            p.top_elements[0].label.text="Generování"+dotstring;
        }
            }
        }

        std::sort(new_peaks.begin(), new_peaks.end(),
            [](const Peak&a,const Peak&b){return a.score>b.score;});

        if (new_peaks.size()>20) new_peaks.resize(20);

        peaks=new_peaks;
    }

    project=best;
    if (!project.students.empty() &&
        project.students.back().empty)
    {
        project.students.pop_back();
    }
    project.score=best_score;
}
void restore_in_front(){
    for (auto& s:project.students){
        s.in_front=s.old_in_front;
    }
}
void generate_search(){
    // if (project.students.size()<project.seats.size()-1){msgbox("Trřída obsahuje o 2 nebo více míst víc než počet žáků.");return;}
    if (project.students.size()<1){msgbox("Třída musí obsahovat žáky.");return;}
    if (project.seats.size()<1){msgbox("Třída musí obsahovat lavice.");return;}
    prepare_height();
    find_front_and_back();
    if (!generate_lavice()){return;}
    if (project.lavice.size()>20){msgbox("Maximální počet lavic pro tento algoritmus na tomto počítači je 20 (40 žáků). Snižte počet lavic, nebo si kupte 128bitový procesor.");return;}
    if (!search_can()){return;}
    if (!find_dvojice()){return;}
    rozesadit_dvojice();
    restore_in_front();
}