#include "includes.hpp"

void updateDropMenu(SDL_Renderer* renderer, dropmenu* menuptr, int xshift=0, int yshift=0){
    if (gui_type==0){update0DropMenu(renderer, menuptr, xshift, yshift);}
    else if (gui_type==1){update1DropMenu(renderer, menuptr, xshift, yshift);}
    else if (gui_type==2){update2DropMenu(renderer, menuptr, xshift, yshift);}
    else if (gui_type==3){update3DropMenu(renderer, menuptr, xshift, yshift);}
}

void updateTextBox(SDL_Renderer* renderer, textbox* inputboxptr, int xshift=0, int yshift=0){
    if (gui_type==0){update0TextBox(renderer, inputboxptr, xshift, yshift);}
    else if (gui_type==1){update1TextBox(renderer, inputboxptr, xshift, yshift);}
    else if (gui_type==2){update2TextBox(renderer, inputboxptr, xshift, yshift);}
    else if (gui_type==3){update3TextBox(renderer, inputboxptr, xshift, yshift);}
}

void updateLabel(SDL_Renderer* renderer, label* labelptr, int xshift=0, int yshift=0){
    if (gui_type==0){update0Label(renderer, labelptr, xshift, yshift);}
    else if (gui_type==1){update1Label(renderer, labelptr, xshift, yshift);}
    else if (gui_type==2){update2Label(renderer, labelptr, xshift, yshift);}
    else if (gui_type==3){update3Label(renderer, labelptr, xshift, yshift);}
}

void updateCheckBox(SDL_Renderer* renderer, checkbox* boxptr, int xshift=0, int yshift=0){
    if (gui_type==0){update0CheckBox(renderer, boxptr, xshift, yshift);}
    else if (gui_type==1){update1CheckBox(renderer, boxptr, xshift, yshift);}
    else if (gui_type==2){update2CheckBox(renderer, boxptr, xshift, yshift);}
    else if (gui_type==3){update3CheckBox(renderer, boxptr, xshift, yshift);}
}

void updateSlider(SDL_Renderer* renderer, slider* sliderptr, int xshift=0, int yshift=0){
    if (gui_type==0){update0Slider(renderer, sliderptr, xshift, yshift);}
    else if (gui_type==1){update1Slider(renderer, sliderptr, xshift, yshift);}
    else if (gui_type==2){update2Slider(renderer, sliderptr, xshift, yshift);}
    else if (gui_type==3){update3Slider(renderer, sliderptr, xshift, yshift);}
}

void updateNumberInputBox(SDL_Renderer* renderer, numberinputbox* inputboxptr, int xshift=0, int yshift=0){
    if (gui_type==0){update0NumberInputBox(renderer, inputboxptr, xshift, yshift);}
    else if (gui_type==1){update1NumberInputBox(renderer, inputboxptr, xshift, yshift);}
    else if (gui_type==2){update2NumberInputBox(renderer, inputboxptr, xshift, yshift);}
    else if (gui_type==3){update3NumberInputBox(renderer, inputboxptr, xshift, yshift);}
}

void updateButton(SDL_Renderer* renderer, button* butptr, int xshift=0, int yshift=0){
    if (gui_type==0){update0Button(renderer, butptr, xshift, yshift);}
    else if (gui_type==1){update1Button(renderer, butptr, xshift, yshift);}
    else if (gui_type==2){update2Button(renderer, butptr, xshift, yshift);}
    else if (gui_type==3){update3Button(renderer, butptr, xshift, yshift);}
}

void updatePanel(SDL_Renderer* renderer, panel* panelptr){
    if (gui_type==0){update0Panel(renderer, panelptr);}
    else if (gui_type==1){update1Panel(renderer, panelptr);}
    else if (gui_type==2){update2Panel(renderer, panelptr);}
    else if (gui_type==3){update3Panel(renderer, panelptr);}
}
void updateElement(SDL_Renderer* renderer, element* elementptr, int xshift=0, int yshift=0){
    if (gui_type==0){update0Element(renderer, elementptr, xshift, yshift);}
    else if (gui_type==1){update1Element(renderer, elementptr, xshift, yshift);}
    else if (gui_type==2){update2Element(renderer, elementptr, xshift, yshift);}
    else if (gui_type==3){update3Element(renderer, elementptr, xshift, yshift);}
}
