int pxlenght(int lenght, int offset=0){return (int)((float)(lenght+offset)*scale);}
int rpxlenght(int lenght, int offset=0){return (int)((float)(lenght+offset)/scale);}
std::vector<std::string> utf8_split_graphemes(const std::string& s) {
    std::vector<std::string> out;
    size_t i = 0;
    while (i < s.size()) {
        unsigned char c = (unsigned char)s[i];
        size_t len = 1;
        if ((c & 0x80) == 0) len = 1;             // ASCII
        else if ((c & 0xE0) == 0xC0) len = 2;    // 2-byte
        else if ((c & 0xF0) == 0xE0) len = 3;    // 3-byte
        else if ((c & 0xF8) == 0xF0) len = 4;    // 4-byte
        else {
            // Neplatný UTF-8, můžeme volit, jak naložit
            len = 1;
        }
        out.push_back(s.substr(i, len));
        i += len;
    }
    return out;
}
int stringlen(std::string strrr){return utf8_split_graphemes(strrr).size();}
bool isNumber(const std::string& str) {
    for (char c : str) {
        if (!isdigit(c)) return false;
    }
    return !str.empty(); // aby "" nevrátilo true
}
int stringToInt(const std::string& str) {
    try {
        return std::stoi(str); // použije C++ funkci, vyhodí výjimku pokud failne
    } catch (...) {
        return 0; // fallback, když to není platné číslo
    }
}
SDL_Color hexStringToColor(std::string* hexStr) {
    std::string hex = *hexStr;

    auto isValidHexChar = [](char c) -> bool {
        return std::isxdigit(static_cast<unsigned char>(c));
    };

    auto hexToInt = [](char c) -> int {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'A' && c <= 'F') return 10 + (c - 'A');
        if (c >= 'a' && c <= 'f') return 10 + (c - 'a');
        return 0;
    };

    if (hex.size() != 7 || hex[0] != '#' || !std::all_of(hex.begin() + 1, hex.end(), isValidHexChar)) {
        printf("[hexStringToColor] Invalid hex code: \"%s\"\n", hex.c_str());
        *hexStr = "#FFAA44";
        return SDL_Color{255, 200, 100, 255};
    }

    int r = hexToInt(hex[1]) * 16 + hexToInt(hex[2]);
    int g = hexToInt(hex[3]) * 16 + hexToInt(hex[4]);
    int b = hexToInt(hex[5]) * 16 + hexToInt(hex[6]);

    return SDL_Color{Uint8(r), Uint8(g), Uint8(b), 255};
}
bool tobool(int val){
    if (val==1){return true;}
    if (val==0){return false;}
    return false;
}
std::string colorToHexString(const SDL_Color& color) {
    std::ostringstream ss;
    ss << "#" 
       << std::uppercase << std::setfill('0') << std::hex
       << std::setw(2) << int(color.r)
       << std::setw(2) << int(color.g)
       << std::setw(2) << int(color.b);
    return ss.str();
}
int booltoint(bool val){
    if (val){return 1;}
    return 0;
}
void emptyfunc(){return;}
void replace_text_range(std::string* inputptr, int start, int end, const std::string& replacement) {
    auto chars = utf8_split_graphemes(*inputptr);
    if (start < 0) start = 0;
    if (end > (int)chars.size()) end = (int)chars.size();
    if (start > end) std::swap(start, end);

    std::string result;
    for (int i = 0; i < start; ++i) result += chars[i];
    result += replacement;
    for (int i = end; i < (int)chars.size(); ++i) result += chars[i];

    *inputptr = result;
}
// Naše funkce randint, která vrací číslo VČETNĚ min a max
int randint(int min, int max) {
    // 1. Statické zařízení pro získání náhodného seedu z hardwaru
    static std::random_device rd; 
    
    // 2. Statický generátor (Mersenne Twister), který seedneme pomocí rd
    // Používáme 'static', aby se generátor inicializoval jen JEDNOU a ne při každém volání
    static std::mt19937 gen(rd()); 
    
    // 3. Definujeme rozsah (distribuci) od min do max
    std::uniform_int_distribution<int> distrib(min, max);
    
    // 4. Vygenerujeme a vrátíme číslo
    return distrib(gen);
}

template <typename T> // finds a variable in a std::vector
bool in(const T& hodnota, const std::vector<T>& vektor) {
    return std::find(vektor.begin(), vektor.end(), hodnota) != vektor.end();
}
void init_weights(){
    //not meant to be used in the final version
    for (int i=0;i<project.students.size();i++){
        student& s=project.students[i];
        // s.weights.height=0.2f;
        // s.weights.in_front=0.2f;
        // s.weights.musnt=0.2f;
        // s.weights.prefered=0.2f;
        // s.weights.sit_with_another_gender=0.2f;
        s.weights.musnt = 0.50f; 

        // 2. VÝŠKA (Vysoká priorita)
        // Držíme se zadání, že s ní počítáme. Je hned druhá nejdůležitější,
        // protože přes sebe žáci neuvidí na tabuli.
        s.weights.height = 0.20f;

        // 3. PREFEROVANÍ SOUSEDÉ (Střední priorita)
        // Je fajn, když sedí s kamarádem, ale učitel radši oželí kamaráda,
        // než aby mu v lavici vznikla bojová zóna.
        s.weights.prefered = 0.15f;

        // 4. CHCI DOPŘEDU / DOZADU (Nižší priorita)
        // Čistě ergonomický faktor.
        s.weights.in_front = 0.10f;

        // 5. GENDER (Nejnižší priorita / Bonusový faktor)
        // Je to spíš takové přání "bylo by hezké, kdyby se namíchali", 
        // ale nesmí to rozbít důležitější věci.
        s.weights.sit_with_another_gender = 0.05f;
    }
}
inline void log(std::string text){
    printf("%s\n", text.c_str());
}
inline float nonzero(float num){
    return (num==0.0f) ? 1.0f : num;
}
inline void msgbox(const char* text){
    SDL_ShowSimpleMessageBox(
        SDL_MESSAGEBOX_ERROR,
        "Upozornění",
        text,
        nullptr
    );
}
void get_avgx(){
    float avgx=0.0f;
    for (auto s:project.seats){
        avgx+=s.xpos;
    }
    avgx/=(float)(project.seats.size());
    project.avgx=std::round(avgx);
}