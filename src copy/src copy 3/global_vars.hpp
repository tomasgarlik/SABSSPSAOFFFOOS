float scale;
int width, height;
int i,j,k,l;
bool retina;
bool running;
bool clic;
bool clickedOnSomething;
float proportions[512];
TTF_Font* lfont = nullptr;
TTF_Font* hfont = nullptr;
TTF_Font* font = nullptr;
std::string typedChar;
char specialkey;
SDL_Keymod modkey;
int mousex,mousey;
bool clicked;
bool clickup;
int newpx;
int newpy;
std::string now_color="#FF0000";
bool found;
bool click_in_meziprostor;
bool customcol;
int xoffset;
int yoffset;
SDL_Renderer* renderer;
bool seznam_opened=false;
bool generate_opened=false;
bool export_opened=false;
bool weights_opened=false;
const weights default_weights={
    .height=0.35,
    .in_front=0.17,
    .prefered=0.4,
    .yap_groups=0.0f,
    .close=0.08f,
    .close_dist=3.6f
};
projectType project={
    .guesswork_itterations=5000000,
    .faktor_socialni_rovnosti=0.0f,
    .penalizace_za_nespokojenost=2.0f,
    .weights=default_weights
};
SDL_Rect ect;
bool editstudent_opened=false;
bool wants_to_sit_with_opened=false;
bool musnt_sit_with_opened=false;
SDL_Event event;
int gend=0;
bool rn=true;
int now_student_gender=0;
int now_student=0;
bool call_wants_to_sit_with=false;
bool call_musnt_sit_with=false;
bool call_update_seznam_scrollables=false;
student default_student={
    .name="Jan Novák",
    .height=180,
    .gender=0,
    .in_front=0.2f,
    .sit_with_different_gender=true,
    .prefered={},
    .musnt={}
};
int gridsize=80;
int grid_sx=0;
int grid_sy=0;
float grid_aspect=1.2f;
bool is_panning = false;
bool active=true;
int tools_x=400;
bool swapping=false;
bool is_first_selected = false; // Máme už vybrané první políčko?
int first_selected_seat_index = -1; // Index prvního vybraného sedadla v project.seats
SDL_Cursor* cursor_default = nullptr;
SDL_Cursor* cursor_swap = nullptr;
bool jetzt_normal_cursor=true;
bool still_generating=true;
SDL_Window* window;
SDL_Texture* pagetex;
// int page_w=2100;
// int page_h=2970;
int page_w=2480;
int page_h=3508;
float maxtextscale=0.20f;
int fontsize=100;
int lfont_size=80;
bool delete_student=false;
int gui_type=2;
float call_reset_weights=false;
int now_wheel_x=0;
int now_wheel_y=0;