#include "U8glib.h"
U8GLIB_PCD8544 u8g(8, 9, 11, 10, 12);  // CLK=8, DIN=9, CE=11, DC=10, RST=12 ///// GND=- VCC=+ BL=+

//constants
const unsigned int simSpeed = 50; //higher value means slower speed
const unsigned int buttons[] = {4,3,2};
const unsigned char menuItems[][5] {"////","STAT","FEED","TALK","////","STAT","GROW","HARF"};

//dynamic                          {"HP","HUN","HAP","SAN","WGT"}
unsigned int statValues[]          {100 , 100 , 100 , 100 , 1   }; //Start stat values
unsigned int currentRandom = 0;
unsigned int spores = 0;
bool isAlive = true;
bool wasAlive = false;
bool wasDead = false;
bool interupt = false;
bool itemPressed = false;
bool addedStats = false;
unsigned int interuptCounter = 0;
unsigned int animCounter = 0;

//input
unsigned int currentMenuItem = 0;
unsigned int currentSubMenuItem = 0;
bool buttonOnePressed = false;
bool buttonTwoPressed = false;
bool buttonThreePressed = false;

//images
/////////////////////////////////////happy
const unsigned char MH1[] PROGMEM = {
  0x03,0xF0, // ......######....
  0x1F,0xFC, // ...###########..
  0x3F,0xFE, // ..#############.
  0x7F,0xFF, // .###############
  0xF0,0xFF, // ####....########
  0xC0,0x01, // ##.............#
  0x6C,0x0E, // .##.##......###.
  0x18,0x08, // ...##.......#...
  0x0B,0x38, // ....#.##..###...
  0x1B,0x34, // ...##.##..##.#..
  0x10,0x04, // ...#.........#..
  0x31,0xC4, // ..##...###...#..
  0x60,0x83, // .##.....#.....##
  0x88,0x09, // #...#.......#..#
  0xF0,0x07, // ####.........###
  0x30,0x02, // ..##..........#.
  0x20,0x02, // ..#...........#.
  0x20,0x42, // ..#......#....#.
  0x11,0xE4, // ...#...####..#..
  0x0E,0x38  // ....###...###...
};

const unsigned char MH2[] PROGMEM = {
  0x3F,0xF0, // ..##########....
  0x7F,0xFC, // .#############..
  0xF1,0xFE, // ####...########.
  0xC0,0x02, // ##............#.
  0x9C,0xE1, // #..###..###....#
  0xCC,0x1E, // ##..##.....####.
  0x78,0x10, // .####......#....
  0x0B,0x38, // ....#.##..###...
  0x6B,0x3B, // .##.#.##..###.##
  0x78,0x0F, // .####.......####
  0x49,0xCD, // .#..#..###..##.#
  0x40,0x81, // .#......#......#
  0x20,0x02, // ..#...........#.
  0x30,0x04, // ..##.........#..
  0x10,0x06, // ...#.........##.
  0x30,0x02, // ..##..........#.
  0x20,0x02, // ..#...........#.
  0x20,0x42, // ..#......#....#.
  0x11,0xE4, // ...#...####..#..
  0x0E,0x38  // ....###...###...
};

/////////////////////////////////////nutral
const unsigned char MN1[] PROGMEM = {
  0x3F,0xF0, // ..##########....
  0x7F,0xFC, // .#############..
  0xF1,0xFE, // ####...########.
  0xC0,0x02, // ##............#.
  0x9C,0xE1, // #..###..###....#
  0xCC,0x1E, // ##..##.....####.
  0x78,0x10, // .####......#....
  0x0B,0x38, // ....#.##..###...
  0x0B,0x38, // ....#.##..###...
  0x18,0x08, // ...##.......#...
  0x10,0xC8, // ...#....##..#...
  0x10,0x04, // ...#.........#..
  0x20,0x0A, // ..#.........#.#.
  0x48,0x09, // .#..#.......#..#
  0x48,0x07, // .#..#........###
  0x30,0x02, // ..##..........#.
  0x20,0x02, // ..#...........#.
  0x20,0x42, // ..#......#....#.
  0x11,0xE4, // ...#...####..#..
  0x0E,0x38  // ....###...###...
};

const unsigned char MN2[] PROGMEM = {
  0x03,0xF0, // ......######....
  0x1F,0xFC, // ...###########..
  0x3F,0xFE, // ..#############.
  0x7F,0xFF, // .###############
  0xF0,0xFF, // ####....########
  0xC0,0x01, // ##.............#
  0x6C,0x0E, // .##.##......###.
  0x18,0x08, // ...##.......#...
  0x0B,0x38, // ....#.##..###...
  0x1B,0x34, // ...##.##..##.#..
  0x10,0x04, // ...#.........#..
  0x10,0xC4, // ...#....##...#..
  0x20,0x02, // ..#...........#.
  0x28,0x0A, // ..#.#.......#.#.
  0x48,0x09, // .#..#.......#..#
  0x50,0x07, // .#.#.........###
  0x30,0x02, // ..##..........#.
  0x20,0x42, // ..#......#....#.
  0x11,0xE4, // ...#...####..#..
  0x0E,0x38  // ....###...###...
};

/////////////////////////////////////sad
const unsigned char MS1[] PROGMEM = {
  0x3F,0xF0, // ..##########....
  0x7F,0xFC, // .#############..
  0xF1,0xFE, // ####...########.
  0xC0,0x02, // ##............#.
  0x9C,0xE1, // #..###..###....#
  0xCC,0x1E, // ##..##.....####.
  0x78,0x10, // .####......#....
  0x09,0x28, // ....#..#..#.#...
  0x0B,0x38, // ....#.##..###...
  0x18,0x08, // ...##.......#...
  0x10,0xC8, // ...#....##..#...
  0x11,0x44, // ...#...#.#...#..
  0x20,0x0A, // ..#.........#.#.
  0x48,0x0A, // .#..#.......#.#.
  0x48,0x0A, // .#..#.......#.#.
  0x68,0x06, // .##.#........##.
  0x30,0x02, // ..##..........#.
  0x20,0x42, // ..#......#....#.
  0x11,0xE4, // ...#...####..#..
  0x0E,0x38  // ....###...###...
};

const unsigned char MS2[] PROGMEM = {
  0x03,0xF0, // ......######....
  0x1F,0xFC, // ...###########..
  0x3F,0xFE, // ..#############.
  0x7F,0xFF, // .###############
  0xF0,0xFF, // ####....########
  0xC0,0x01, // ##.............#
  0x6C,0x0E, // .##.##......###.
  0x18,0x08, // ...##.......#...
  0x08,0x08, // ....#.......#...
  0x1B,0x34, // ...##.##..##.#..
  0x10,0x04, // ...#.........#..
  0x10,0xC4, // ...#....##...#..
  0x21,0x42, // ..#....#.#....#.
  0x28,0x0A, // ..#.#.......#.#.
  0x48,0x0A, // .#..#.......#.#.
  0x48,0x0A, // .#..#.......#.#.
  0x28,0x06, // ..#.#........##.
  0x30,0x42, // ..##.....#....#.
  0x11,0xE4, // ...#...####..#..
  0x0E,0x38  // ....###...###...
};

////////////////////////////////////Depressed
const unsigned char MD[] PROGMEM = {
  0x3F,0xF0, // ..##########....
  0x7F,0xFC, // .#############..
  0xF1,0xFE, // ####...########.
  0xC0,0x02, // ##............#.
  0x9C,0xE1, // #..###..###....#
  0xCC,0x1E, // ##..##.....####.
  0x7C,0x10, // .#####.....#....
  0x05,0x30, // .....#.#..##....
  0x06,0xA0, // .....##.#.#.....
  0x04,0xC0, // .....#..##......
  0x04,0x40, // .....#...#......
  0x04,0x40, // .....#...#......
  0x04,0x40, // .....#...#......
  0x08,0x40, // ....#....#......
  0x08,0x20, // ....#.....#.....
  0x08,0x10, // ....#......#....
  0x08,0x10, // ....#......#....
  0x08,0x10, // ....#......#....
  0x08,0x30, // ....#.....##....
  0x07,0xC0  // .....#####......
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  //sets up the buttons
  for (int i = 0; i < 3 ;i++) { pinMode(buttons[i], INPUT); }

  //uses A0's float value to set the random seed
  randomSeed(analogRead(0));

  //font for dispaly
  u8g.setFont(u8g_font_4x6);

  //starts the serial
  //Serial.begin(57600);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop(void) {
  //get inputs
  inputs();
  
  //stat calculations
  statStuff();

  //Draw stuff to screen
  u8g.firstPage(); do { 
    
    preDraw();
    if (interupt != true) {
    drawText(); drawImages(); 
    }
    
  } while( u8g.nextPage() );

  //timer for interupt
  if (interupt == true) {
    if (interuptCounter < 100) { interuptCounter += 1; } else {
      interupt = false;
      interuptCounter = 0;
    }
  }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void statStuff(void) {
  currentRandom = random(5 + simSpeed + spores * 5); //calculate random

  //decrease hunger
  if (currentRandom == 1) {statValues[1] -= 1;}

  //decrease happynes
  if (currentRandom == 5) {statValues[2] -= 1;}

  //when sad
  if (statValues[2] < 10) {
    if (currentRandom == 2) {statValues[3] -= 1;} //decrease sanity
    if (statValues[1] <= 0) { //when depressed
      if (currentRandom == 3) { 
        statValues[0] -= 1; //decrease health
      }
      if (currentRandom == 4) { 
        statValues[0] -= 1; //decrease health
        statValues[3] -= 1; //decrese sanity
     }
    }
  }
  
  //when hungry
  if (statValues[1] < 30) {
    if (currentRandom == 2) {statValues[2] -= 1;} //decrease happynes
    
    if (statValues[1] <= 0) { //when starving
      if (currentRandom == 3) {
        statValues[0] -= 1; //decrease health
        statValues[2] -= 1; //decrease happyness
      }
      if (currentRandom == 4) { 
        statValues[2] -= 1; //decrease happynes
        statValues[3] -= 1; //decrese sanity
     }
    }
  }

  for (int i = 0; i < 5 ;i++) { //clamps values below 100
    if (statValues[i] > 500) {
      statValues[i] = 0;
    } else if (statValues[i] > 100) {
      statValues[i] = 100;
    }
  }
      
  if (statValues[0] == 0) { //sets values to 0 if pet is dead
    isAlive = false;
    for (int i = 0; i < 5 ;i++) {
      statValues[i] = 0;
    }
  }
  if (isAlive == false) {
    for (int i = 0; i < 5 ;i++) {
      statValues[i] = 0;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void inputs(void) {
  //button 1 to chainge menu
  if (digitalRead(buttons[0]) == LOW && interupt == false) {buttonOnePressed = false;}
  else if (digitalRead(buttons[0]) == HIGH && buttonOnePressed == false) {
    buttonOnePressed = true;
    if (currentMenuItem < 7) {
      currentMenuItem += 1;
    } else {currentMenuItem = 0;}
  }
  //button 2 to chainge sub menu
  if (currentMenuItem > 1 && currentMenuItem < 7 && interupt == false) {
   if (digitalRead(buttons[1]) == LOW) {buttonTwoPressed = false;}
   else if (digitalRead(buttons[1]) == HIGH && buttonTwoPressed == false) {
      buttonTwoPressed = true;
      if (currentSubMenuItem < 2) {
        currentSubMenuItem += 1;
      } else {currentSubMenuItem = 0;}
    }
  }
  //checks if button 3 is pressed
  if (currentMenuItem > 1 && currentMenuItem < 8 && interupt == false) {
   if (digitalRead(buttons[2]) == LOW) {buttonThreePressed = false;}
   else if (digitalRead(buttons[2]) == HIGH && buttonThreePressed == false) {
      buttonThreePressed = true;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void preDraw(void) {

  //////////////////////////////////////////////////////////////////////////////////////////////////////Feed
  
  if (currentMenuItem == 2 && isAlive == true) { 
    if (buttonThreePressed) {
      itemPressed = true;
    }
    if (itemPressed && buttonThreePressed == false) {
      itemPressed = false;
      interupt = true;
    }
    if (interupt) {
      u8g.undoScale();
      if (currentSubMenuItem == 0) { ///////////////////////////////////////////water
        if (statValues[1] < 90){
          u8g.drawStr(5, 10, "Your pet enjoys the");
          u8g.drawStr(5, 16, "Water");
          u8g.drawStr(5, 22, "+5 HUN +10 HP +5 HAP");
          
          if (addedStats == false) { //adds stats, but only once
            statValues[1] += 5;
            statValues[0] += 10;
            statValues[2] += 5;
            addedStats = true;
          }
          
        } else {  
          u8g.drawStr(5, 10, "Your pet is not"); 
          u8g.drawStr(5, 16, "thirsty"); 
          }
        
      } else if (currentSubMenuItem == 1) { ////////////////////////////////wood
        if (statValues[1] < 80){
          u8g.drawStr(5, 10, "Your pet ate the");
          u8g.drawStr(5, 16, "Wood");
          u8g.drawStr(5, 22, "+20 HUN +5 HP +5 HAP");
          u8g.drawStr(5, 28, "+1 WGT");
          if (addedStats == false) { //adds stats, but only once
            statValues[1] += 20;
            statValues[0] += 5;
            statValues[2] += 5;
            statValues[4] += 1;
            addedStats = true;
          }
          
        } else {  
          u8g.drawStr(5, 10, "Your pet is not"); 
          u8g.drawStr(5, 16, "hungry"); 
          }
      } else if (currentSubMenuItem == 2) { ///////////////////////////spores
        if (spores == 0) {
          u8g.drawStr(5, 10, "you dont have");
          u8g.drawStr(5, 16, "any spores left"); 
        } else {
          if (addedStats == false) { //adds stats, but only once
            for (int i = 0; i < 4 ;i++) {
              statValues[i] = 100;
            }
            spores -= 1;
            addedStats = true;
          }
          u8g.drawStr(5, 10, "Your pet consumed");
          u8g.drawStr(5, 16, "the Spore");
          u8g.drawStr(5, 22, "all stats have been");
          u8g.drawStr(5, 28, "reset");
        }
      }
    } else { addedStats = false;}
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////TALk

  if (currentMenuItem == 3  && isAlive == true) { 
    if (buttonThreePressed) {
      itemPressed = true;
    }
    if (itemPressed && buttonThreePressed == false) {
      itemPressed = false;
      interupt = true;
    }
    if (interupt) {
      u8g.undoScale();
          u8g.drawStr(5, 10, "You talked to your");
          u8g.drawStr(5, 16, "pet");
          u8g.drawStr(5, 22, "+15 HAP +1 SAN");
      if (addedStats == false) {
            statValues[2] += 15;
            statValues[3] += 1;
            addedStats = true;
      }
    } else { addedStats = false;}
  
  }


  //////////////////////////////////////////////////////////////////////////////////////////////////////grow

  if (currentMenuItem == 6) { 
    if (buttonThreePressed) {
      itemPressed = true;
    }
    if (itemPressed && buttonThreePressed == false) {
      itemPressed = false;
      interupt = true;
    }
    if (interupt) {
      u8g.undoScale();

      
      if (isAlive == true && wasDead == false) {
        u8g.drawStr(5, 10, "Your pet is still");
        u8g.drawStr(5, 16, "alive, you cant");
        u8g.drawStr(5, 22, "grow a new one");
      } else {
        u8g.drawStr(5, 10, "You grew a new");
        u8g.drawStr(5, 16, "pet!");
        if (addedStats == false) {
            for (int i = 0; i < 4 ;i++) {
              statValues[i] = 100;
            }
            statValues[4] = 0;
            isAlive = true;
            addedStats = true;
            wasDead = true;
          }
      }
    } else { addedStats = false; wasDead = false;}
  
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////Harvest

  if (currentMenuItem == 7) { 
    if (buttonThreePressed) {
      itemPressed = true;
    }
    if (itemPressed && buttonThreePressed == false) {
      itemPressed = false;
      interupt = true;
    }
    if (interupt) {
      u8g.undoScale();

      
      if (isAlive == true || wasAlive == true) {
        u8g.drawStr(5, 10, "Your harvested");
        u8g.drawStr(5, 16, "your pet you got");
        u8g.drawStr(5, 22, "its weight in ");
        u8g.drawStr(5, 28, "spores");
        if (addedStats == false) {
            isAlive = false;
            spores += statValues[4];
            wasAlive = true;
            addedStats = true;
          }
      } else {
        u8g.drawStr(5, 10, "Your pet is dead,");
        u8g.drawStr(5, 16, "you cant harvest");
        u8g.drawStr(5, 22, "it anymore");
      }
    
    } else { addedStats = false; wasAlive = false;}
  
  }

  
}
  
  

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void drawText(void) {
  u8g.undoScale();

  //menu indicator //sporecounter
  if (currentMenuItem != 0 && currentMenuItem != 4) { 
    u8g.drawStr(25, 6, menuItems[currentMenuItem]); 
    u8g.setPrintPos(45, 6);
    u8g.print(spores);
  }
  
  /////////////////////////////////////////////////////////////STAT
  
  if (currentMenuItem == 1 || currentMenuItem == 5) { 
    unsigned char statItems[][4] {"HP","HUN","HAP","SAN","WGT"};

      
    for (int i = 0; i < 5 ;i++){
      int y = 16 + (i * 7);
      u8g.setPrintPos(45, y);
      u8g.print(statValues[i]);
      u8g.drawStr(25, y, statItems[i]);
    }
  }

  ////////////////////////////////////////////////////////////FEED
  
  if (currentMenuItem == 2) { 
    const unsigned char foodItems[][6] {"Water","Wood","Spore"};

    for (int i = 0; i < 3 ;i++){
      int y = 20 + (i * 8);
      u8g.drawStr(25, y, foodItems[i]);

      if (currentSubMenuItem == i ) {
        if (buttonThreePressed) {
          u8g.drawBox(23, y - 7, 30, 9);
        } else {
          u8g.drawFrame(23, y - 7, 30, 9);
        }
      }
    }
  }

   //////////////////////////////////////////////////////////talk

  if (currentMenuItem == 3) {
    u8g.drawStr(27, 20, "Talk?");
    if (buttonThreePressed) {
          u8g.drawBox(25, 13, 24, 9);
        } else {
          u8g.drawFrame(25, 13, 24, 9);
        }
  }

  //////////////////////////////////////////////////////////GROW

  if (currentMenuItem == 6) {
    u8g.drawStr(25, 20, "Grow new");
    u8g.drawStr(25, 28, " pet?");
    if (buttonThreePressed) {
          u8g.drawBox(23, 13, 35, 17);
        } else {
          u8g.drawFrame(23, 13, 35, 17);
        }
  }

 //////////////////////////////////////////////////////////harvest

  if (currentMenuItem == 7) {
    u8g.drawStr(25, 20, "Harvest?");
    if (buttonThreePressed) {
          u8g.drawBox(23, 13, 35, 9);
        } else {
          u8g.drawFrame(23, 13, 35, 9);
        }
  }
  
  /////////////////////////////////////////////////menu boxes (draws 2 boxes per cycle)
  for (int i = 0; i < 4 ;i++) { 
    unsigned int boxY = i * 13;
    unsigned int boxWidth = 19;
    unsigned int boxHeight = 9;

    //draws left box
    u8g.drawStr(2, 7 + boxY, menuItems[i]);
    u8g.drawFrame(0, boxY ,boxWidth, boxHeight);

    //draws right box
    u8g.drawStr(67, 7 + boxY, menuItems[i + 4]);
    u8g.drawFrame(65, boxY ,boxWidth, boxHeight);
    
    //fills the selected menu item
    if (currentMenuItem == i ) {u8g.drawBox(0, boxY, boxWidth, boxHeight);}
    if (currentMenuItem == i + 4) {u8g.drawBox(65, boxY, boxWidth, boxHeight);}
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void drawImages(void) {
  u8g.setScale2x2();

  //draw pet
  if (currentMenuItem == 0 || currentMenuItem == 4) { 

    
    if (isAlive == false) {
      u8g.drawStr(15, 14, "RIP");
      return;
    }
    
    //Displays pet
    animCounter += 1;
    if (animCounter > 400) {
      animCounter = 0;
    }

    if (statValues[2] > 75) { //////////happy emotion
      if (animCounter < 250) {
          u8g.drawBitmapP( 13, 2, 2, 20, MH1); 
      } else {
          u8g.drawBitmapP( 13, 2, 2, 20, MH2); 
      }
    } else if (statValues[2] > 40) { /////nutral emotion
      if (animCounter < 250) {
          u8g.drawBitmapP( 13, 2, 2, 20, MN1); 
      } else {
          u8g.drawBitmapP( 13, 2, 2, 20, MN2); 
      }
    } else if (statValues[2] > 5) { /////SAD emotion
      if (animCounter < 250) {
          u8g.drawBitmapP( 13, 2, 2, 20, MS1); 
      } else {
          u8g.drawBitmapP( 13, 2, 2, 20, MS2); 
      }
      } else if (statValues[2] < 6) { /////Depressed
          u8g.drawBitmapP( 13, 2, 2, 20, MD); 
    }
    }
}
