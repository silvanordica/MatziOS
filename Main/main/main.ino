// IMPORTANT: ELEGOO_TFTLCD LIBRARY MUST BE SPECIFICALLY
// CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.
// SEE RELEVANT COMMENTS IN Elegoo_TFTLCD.h FOR SETUP.
//Technical support:goodtft@163.com


//Global Variables:
#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

#define BOXSIZE 40
#define PENRADIUS 3
#define BS1 20
#define BS1GAP 10
#define SCREENSIZEX 100
#define SCREENSIZEY 300
#define ICONSIZE 60
#define ICONGAP 15

//Color Definitions
int oldcolor, currentcolor;

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

// Color definitions
#define ILI9341_BLACK       0x0000      /*   0,   0,   0 */
#define ILI9341_NAVY        0x000F      /*   0,   0, 128 */
#define ILI9341_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define ILI9341_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define ILI9341_MAROON      0x7800      /* 128,   0,   0 */
#define ILI9341_PURPLE      0x780F      /* 128,   0, 128 */
#define ILI9341_OLIVE       0x7BE0      /* 128, 128,   0 */
#define ILI9341_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define ILI9341_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define ILI9341_BLUE        0x001F      /*   0,   0, 255 */
#define ILI9341_GREEN       0x07E0      /*   0, 255,   0 */
#define ILI9341_CYAN        0x07FF      /*   0, 255, 255 */
#define ILI9341_RED         0xF800      /* 255,   0,   0 */
#define ILI9341_MAGENTA     0xF81F      /* 255,   0, 255 */
#define ILI9341_YELLOW      0xFFE0      /* 255, 255,   0 */
#define ILI9341_WHITE       0xFFFF      /* 255, 255, 255 */
#define ILI9341_ORANGE      0xFD20      /* 255, 165,   0 */
#define ILI9341_GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define ILI9341_PINK        0xF81F

#define BUTTON_X 40
#define BUTTON_Y 100
#define BUTTON_W 60
#define BUTTON_H 30
#define BUTTON_SPACING_X 20
#define BUTTON_SPACING_Y 20
#define BUTTON_TEXTSIZE 2

// text box where numbers go
#define TEXT_X 10
#define TEXT_Y 10
#define TEXT_W 220
#define TEXT_H 50
#define TEXT_TSIZE 3
#define TEXT_TCOLOR ILI9341_MAGENTA
// the data (phone #) we store in the textfield
#define TEXT_LEN 12
char textfield[TEXT_LEN+1] = "";
uint8_t textfield_i=0;

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

//Touch For New ILI9341 TP
#define TS_MINX 120
#define TS_MAXX 900

#define TS_MINY 70
#define TS_MAXY 920
// We have a status line for like, is FONA working
#define STATUS_X 10
#define STATUS_Y 65

#define MINPRESSURE 10
#define MAXPRESSURE 1000
//Touch For New ILI9341 TP
#define TS_MINX 120
#define TS_MAXX 900

#define TS_MINY 70
#define TS_MAXY 920

int programtrigger = 0;
bool initflag = true;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Elegoo_TFTLCD tft;

Elegoo_GFX_Button buttons[15];
/* create 15 buttons, in classic candybar phone style */
char buttonlabels[15][5] = {"Send", "Clr", "End", "1", "2", "3", "4", "5", "6", "7", "8", "9", "*", "0", "#" };
uint16_t buttoncolors[15] = {ILI9341_DARKGREEN, ILI9341_DARKGREY, ILI9341_RED, 
                             ILI9341_BLUE, ILI9341_BLUE, ILI9341_BLUE, 
                             ILI9341_BLUE, ILI9341_BLUE, ILI9341_BLUE, 
                             ILI9341_BLUE, ILI9341_BLUE, ILI9341_BLUE, 
                             ILI9341_ORANGE, ILI9341_BLUE, ILI9341_ORANGE};






                             
    
//Main startup routine
void setup(void) 
{
  Serial.begin(9600);
  Serial.println(F("Paint!"));
  
  tft.reset();
  
  uint16_t identifier = tft.readID();
  if(identifier == 0x9325) {
    Serial.println(F("Found ILI9325 LCD driver"));
  } else if(identifier == 0x9328) {
    Serial.println(F("Found ILI9328 LCD driver"));
  } else if(identifier == 0x4535) {
    Serial.println(F("Found LGDP4535 LCD driver"));
  }else if(identifier == 0x7575) {
    Serial.println(F("Found HX8347G LCD driver"));
  } else if(identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  } else if(identifier == 0x8357) {
    Serial.println(F("Found HX8357D LCD driver"));
  } else if(identifier==0x0101)
  {     
      identifier=0x9341;
       Serial.println(F("Found 0x9341 LCD driver"));
  }else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    Serial.println(F("If using the Elegoo 2.8\" TFT Arduino shield, the line:"));
    Serial.println(F("  #define USE_Elegoo_SHIELD_PINOUT"));
    Serial.println(F("should appear in the library header (Elegoo_TFT.h)."));
    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
    Serial.println(F("Also if using the breakout, double-check that all wiring"));
    Serial.println(F("matches the tutorial."));
    identifier=0x9341;
   
  }

  tft.begin(identifier);
  tft.setRotation(2);
  tft.fillScreen(BLACK);
  //tft.drawRect(0, 0, BOXSIZE, BOXSIZE, WHITE);
  currentcolor = BLACK;
  pinMode(13, OUTPUT);
}


void loop(void) {
  if(programtrigger == 0)
  {
      if(initflag == true)
      {
        //startup();
      }
      initializeMainScreen();
      programtrigger = 1;
  }

  //Main screen
  if(programtrigger == 1)
  {
      digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);

  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    /*
    Serial.print("X = "); Serial.print(p.x);
    Serial.print("\tY = "); Serial.print(p.y);
    Serial.print("\tPressure = "); Serial.println(p.z);
    */
    
    if (p.y < (TS_MINY-5)) {
      Serial.println("erase");
      // press the bottom of the screen to erase 
      tft.fillRect(0, BOXSIZE, tft.width(), tft.height()-BOXSIZE, BLACK);
    }
    // scale from 0->1023 to tft.width
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    //p.x = tft.width()-map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = (tft.height()-map(p.y, TS_MINY, TS_MAXY, tft.height(), 0));
     //p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);
    
    
    if (((p.y-PENRADIUS) > BOXSIZE) && ((p.y+PENRADIUS) < tft.height())) {
      tft.fillCircle(p.x, p.y, PENRADIUS, currentcolor);
    }

  //Start paint
    if(p.x > ICONGAP && p.x < ICONGAP+ICONSIZE && p.y > ICONGAP && p.y < ICONSIZE+ICONGAP)
    {
      programtrigger = 2;
      tft.fillScreen(BLACK);
      tft.fillRect(0, BOXSIZE, tft.width(), tft.height()-BOXSIZE, BLACK);
      initflag = true;
    }

      //Start phonecall
    if(p.x > ICONGAP*2+ICONSIZE && p.x < ICONGAP*2+ICONSIZE*2 && p.y > ICONGAP && p.y < ICONSIZE+ICONGAP)
    {
      programtrigger = 3;
      tft.fillRect(0, BOXSIZE, tft.width(), tft.height()-BOXSIZE, BLACK);
      initflag = true;
    }
  }
}

    if(programtrigger == 2)//paint
    {
      if(initflag)
      {
        tft.fillRect(0, BOXSIZE, tft.width(), tft.height()-BOXSIZE, BLACK);
        initializePaint();
      }
      
      digitalWrite(13, HIGH);
      TSPoint p = ts.getPoint();
      digitalWrite(13, LOW);

      // if sharing pins, you'll need to fix the directions of the touchscreen pins
      //pinMode(XP, OUTPUT);
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      //pinMode(YM, OUTPUT);

      if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    
    if (p.y < (TS_MINY-5)) {
      Serial.println("erase");
      // press the bottom of the screen to erase 
      tft.fillRect(0, BOXSIZE, tft.width(), tft.height()-BOXSIZE, BLACK);
    }
    // scale from 0->1023 to tft.width
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    //p.x = tft.width()-map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = (tft.height()-map(p.y, TS_MINY, TS_MAXY, tft.height(), 0));
     //p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);
    /*
    Serial.print("("); Serial.print(p.x);
    Serial.print(", "); Serial.print(p.y);
    Serial.println(")");
    */
    if (p.y < BOXSIZE) {
       oldcolor = currentcolor;

       if (p.x < BOXSIZE) { 
         currentcolor = RED; 
         tft.drawRect(0, 0, BOXSIZE, BOXSIZE, WHITE);
       } else if (p.x < BOXSIZE*2) {
         currentcolor = YELLOW;
         tft.drawRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, WHITE);
       } else if (p.x < BOXSIZE*3) {
         currentcolor = GREEN;
         tft.drawRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, WHITE);
       } else if (p.x < BOXSIZE*4) {
         currentcolor = CYAN;
         tft.drawRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, WHITE);
       } else if (p.x < BOXSIZE*5) {
         currentcolor = BLUE;
         tft.drawRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, WHITE);
       } else if (p.x < BOXSIZE*6) {
         currentcolor = MAGENTA;
         tft.drawRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, WHITE);
       }
         else if (p.x < BOXSIZE) { 
         currentcolor = BLACK; 
         tft.drawRect(0, BOXSIZE*6, BOXSIZE, BOXSIZE, WHITE);
       }

       if (oldcolor != currentcolor) {
          if (oldcolor == RED) tft.fillRect(0, 0, BOXSIZE, BOXSIZE, RED);
          if (oldcolor == YELLOW) tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, YELLOW);
          if (oldcolor == GREEN) tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, GREEN);
          if (oldcolor == CYAN) tft.fillRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, CYAN);
          if (oldcolor == BLUE) tft.fillRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, BLUE);
          if (oldcolor == MAGENTA) tft.fillRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, MAGENTA);
       }
    }
    //Erase everything

    if(p.x > BS1GAP && p.x < BS1GAP+BS1 && p.y > SCREENSIZEY-BS1GAP && p.y < SCREENSIZEY-BS1GAP+BS1)
    {
      tft.fillRect(0, BOXSIZE, tft.width(), tft.height()-BOXSIZE, BLACK);
      tft.fillRect(BS1GAP, SCREENSIZEY-BS1GAP, BS1, BS1, WHITE); //redraw erase button
      programtrigger = 1;
      initializeMainScreen();
    }

    
    
    if (((p.y-PENRADIUS) > BOXSIZE) && ((p.y+PENRADIUS) < tft.height())) {
      tft.fillCircle(p.x, p.y, PENRADIUS, currentcolor);
    }
  }
}//end paint program

if(programtrigger == 3)//phonecall
{
   if(initflag)
   {
      tft.fillRect(0, BOXSIZE, tft.width(), tft.height()-BOXSIZE, BLACK);
      initializePhoneCall();
   }
  /*TSPoint p;
  p = ts.getPoint(); 
  */
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);

  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
  
 // p = ts.getPoint(); 
  /*
  if (ts.bufferSize()) {
    
  } else {
    // this is our way of tracking touch 'release'!
    p.x = p.y = p.z = -1;
  }*/
  
  // Scale from ~0->4000 to tft.width using the calibration #'s
  /*
  if (p.z != -1) {
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
    Serial.print("("); Serial.print(p.x); Serial.print(", "); 
    Serial.print(p.y); Serial.print(", "); 
    Serial.print(p.z); Serial.println(") ");
  }*/
   if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    // scale from 0->1023 to tft.width
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = (tft.height()-map(p.y, TS_MINY, TS_MAXY, tft.height(), 0));
   }
   
  // go thru all the buttons, checking if they were pressed
  for (uint8_t b=0; b<15; b++) {
    if (buttons[b].contains(p.x, p.y)) {
      //Serial.print("Pressing: "); Serial.println(b);
      buttons[b].press(true);  // tell the button it is pressed
    } else {
      buttons[b].press(false);  // tell the button it is NOT pressed
    }
  }

  // now we can ask the buttons if their state has changed
  for (uint8_t b=0; b<15; b++) {
    if (buttons[b].justReleased()) {
      // Serial.print("Released: "); Serial.println(b);
      buttons[b].drawButton();  // draw normal
    }
    
    if (buttons[b].justPressed()) {
        buttons[b].drawButton(true);  // draw invert!
        
        // if a numberpad button, append the relevant # to the textfield
        if (b >= 3) {
          if (textfield_i < TEXT_LEN) {
            textfield[textfield_i] = buttonlabels[b][0];
            textfield_i++;
      textfield[textfield_i] = 0; // zero terminate
            
           // fona.playDTMF(buttonlabels[b][0]);
          }
        }

        // clr button! delete char
        if (b == 1) {
          
          textfield[textfield_i] = 0;
          if (textfield > 0) {
            textfield_i--;
            textfield[textfield_i] = ' ';
          }
        }

        // update the current text field
        Serial.println(textfield);
        tft.setCursor(TEXT_X + 2, TEXT_Y+10);
        tft.setTextColor(TEXT_TCOLOR, ILI9341_BLACK);
        tft.setTextSize(TEXT_TSIZE);
        tft.print(textfield);

        // its always OK to just hang up
        if (b == 2) {
          status(F("Hanging up"));
          //fona.hangUp();
        }
        // we dont really check that the text field makes sense
        // just try to call
        if (b == 0) {
          status(F("Calling"));
          Serial.print("Calling "); Serial.print(textfield);
          
          //fona.callPhone(textfield);
        }
        
      delay(100); // UI debouncing
    }
  }
}
  
  
  //delay(1000);delay(1000);delay(1000);delay(1000);delay(1000);  
}






//INITIALIZATION ROUTINES FOR APPS

void initializeMainScreen(void)
{
  //draw paint icon
  tft.fillScreen(BLACK);
  tft.fillRect(ICONGAP, ICONGAP, ICONSIZE, ICONSIZE, YELLOW);
  tft.fillRect(ICONGAP+ICONSIZE-ICONGAP-5, ICONGAP*2, 5, 5, BLACK);
  tft.fillRect(ICONGAP+ICONSIZE-ICONGAP-10, ICONGAP*2+5, 5, 5, BLACK);
  tft.fillRect(ICONGAP+ICONSIZE-ICONGAP-15, ICONGAP*2+10, 5, 5, BLACK);
  tft.fillRect(ICONGAP+ICONSIZE-ICONGAP-20, ICONGAP*2+15, 5, 5, BLACK);
  tft.fillRect(ICONGAP+ICONSIZE-ICONGAP-25, ICONGAP*2+20, 5, 5, BLACK);
  tft.fillRect(ICONGAP+ICONSIZE-ICONGAP-30, ICONGAP*2+25, 5, 5, BLACK);
  tft.fillRect(ICONGAP+ICONSIZE-ICONGAP-35, ICONGAP*2+30, 5, 5, BLACK);
  tft.fillRect(ICONGAP+ICONSIZE-ICONGAP-40, ICONGAP*2+35, 5, 5, BLUE);
  tft.fillRect(ICONGAP+ICONSIZE-ICONGAP, ICONGAP*2, 5, 5, BLACK);
  tft.fillRect(ICONGAP+ICONSIZE-ICONGAP-5, ICONGAP*2+5, 5, 5, BLACK);
  tft.fillRect(ICONGAP+ICONSIZE-ICONGAP-10, ICONGAP*2+10, 5, 5, BLACK);
  tft.fillRect(ICONGAP+ICONSIZE-ICONGAP-15, ICONGAP*2+15, 5, 5, BLACK);
  tft.fillRect(ICONGAP+ICONSIZE-ICONGAP-20, ICONGAP*2+20, 5, 5, BLACK);
  tft.fillRect(ICONGAP+ICONSIZE-ICONGAP-25, ICONGAP*2+25, 5, 5, BLACK);
  tft.fillRect(ICONGAP+ICONSIZE-ICONGAP-30, ICONGAP*2+30, 5, 5, BLACK);
  tft.fillRect(ICONGAP+ICONSIZE-ICONGAP-35, ICONGAP*2+30, 5, 5, BLACK);
  tft.fillRect(ICONGAP+ICONSIZE-ICONGAP-10, ICONGAP*2, 5, 5, BLACK);
  tft.fillRect(ICONGAP+ICONSIZE-ICONGAP-15, ICONGAP*2+5, 5, 5, BLACK);
  tft.fillRect(ICONGAP+ICONSIZE-ICONGAP-20, ICONGAP*2+10, 5, 5, BLACK);
  tft.fillRect(ICONGAP+ICONSIZE-ICONGAP-25, ICONGAP*2+15, 5, 5, BLACK);
  tft.fillRect(ICONGAP+ICONSIZE-ICONGAP-30, ICONGAP*2+20, 5, 5, BLACK);
  tft.fillRect(ICONGAP+ICONSIZE-ICONGAP-35, ICONGAP*2+25, 5, 5, BLACK);
  tft.fillRect(ICONGAP+ICONSIZE-ICONGAP-40, ICONGAP*2+30, 5, 5, BLACK);
  tft.fillRect(ICONGAP+ICONSIZE-ICONGAP, ICONGAP*2, 5, 5, BLACK);
  tft.fillRect(ICONGAP+ICONSIZE-ICONGAP-5, ICONGAP*2+5, 5, 5, BLACK);
  tft.fillRect(ICONGAP+ICONSIZE-ICONGAP-10, ICONGAP*2+10, 5, 5, BLACK);
  tft.fillRect(ICONGAP+ICONSIZE-ICONGAP-15, ICONGAP*2+15, 5, 5, BLACK);
  tft.fillRect(ICONGAP+ICONSIZE-ICONGAP-20, ICONGAP*2+20, 5, 5, BLACK);
  tft.fillRect(ICONGAP+ICONSIZE-ICONGAP-25, ICONGAP*2+25, 5, 5, BLACK);
  tft.fillRect(ICONGAP+ICONSIZE-ICONGAP-30, ICONGAP*2+30, 5, 5, BLACK);
  tft.fillRect(ICONGAP+ICONSIZE-ICONGAP-20, ICONGAP*2+35, 5, 5, BLUE);
  tft.fillRect(ICONGAP+ICONSIZE-ICONGAP-10, ICONGAP*2+35, 5, 5, BLUE);
  tft.fillRect(ICONGAP+ICONSIZE-ICONGAP, ICONGAP*2+35, 5, 5, BLUE);

//draw call icon
  tft.fillRect(ICONGAP*2+ICONSIZE, ICONGAP, ICONSIZE, ICONSIZE, GREEN);
  tft.fillRect(ICONGAP*2+ICONSIZE*1.3, ICONGAP+5, 15, 15, BLACK);
  tft.fillRect(ICONGAP*2+ICONSIZE*1.3, ICONSIZE-10, 15, 15, BLACK);
  tft.fillRect(ICONGAP*2+ICONSIZE*1.3+10, ICONGAP+10, 10, 10, BLACK);
  tft.fillRect(ICONGAP*2+ICONSIZE*1.3+15, ICONGAP+20, 10, 10, BLACK);
  tft.fillRect(ICONGAP*2+ICONSIZE*1.3+15, ICONGAP+30, 10, 10, BLACK);
  tft.fillRect(ICONGAP*2+ICONSIZE*1.3+10, ICONGAP+40, 10, 10, BLACK);

  
//draw call icon
  tft.fillRect(ICONGAP*3+ICONSIZE*2, ICONGAP, ICONSIZE, ICONSIZE, RED);
  /*tft.fillRect(ICONGAP*3+ICONSIZE*1.3, ICONGAP+5, 15, 15, BLACK);
  tft.fillRect(ICONGAP*3+ICONSIZE*1.3, ICONSIZE-10, 15, 15, BLACK);
  tft.fillRect(ICONGAP*3+ICONSIZE*1.3+10, ICONGAP+10, 10, 10, BLACK);
  tft.fillRect(ICONGAP*3+ICONSIZE*1.3+15, ICONGAP+20, 10, 10, BLACK);
  tft.fillRect(ICONGAP*3+ICONSIZE*1.3+15, ICONGAP+30, 10, 10, BLACK);
  tft.fillRect(ICONGAP*3+ICONSIZE*1.3+10, ICONGAP+40, 10, 10, BLACK);
*/
  currentcolor = BLACK;
}

void initializePaint(void) 
{
  Serial.begin(9600);
  Serial.println(F("Paint!"));
  
  tft.reset();
  
  uint16_t identifier = tft.readID();
  if(identifier == 0x9325) {
    Serial.println(F("Found ILI9325 LCD driver"));
  } else if(identifier == 0x9328) {
    Serial.println(F("Found ILI9328 LCD driver"));
  } else if(identifier == 0x4535) {
    Serial.println(F("Found LGDP4535 LCD driver"));
  }else if(identifier == 0x7575) {
    Serial.println(F("Found HX8347G LCD driver"));
  } else if(identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  } else if(identifier == 0x8357) {
    Serial.println(F("Found HX8357D LCD driver"));
  } else if(identifier==0x0101)
  {     
      identifier=0x9341;
       Serial.println(F("Found 0x9341 LCD driver"));
  }else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    Serial.println(F("If using the Elegoo 2.8\" TFT Arduino shield, the line:"));
    Serial.println(F("  #define USE_Elegoo_SHIELD_PINOUT"));
    Serial.println(F("should appear in the library header (Elegoo_TFT.h)."));
    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
    Serial.println(F("Also if using the breakout, double-check that all wiring"));
    Serial.println(F("matches the tutorial."));
    identifier=0x9341;
   
  }

  tft.begin(identifier);
  tft.setRotation(2);

  tft.fillScreen(BLACK);

  tft.fillRect(0, 0, BOXSIZE, BOXSIZE, RED);
  tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, YELLOW);
  tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, GREEN);
  tft.fillRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, CYAN);
  tft.fillRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, BLUE);
  tft.fillRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, MAGENTA);
  // tft.fillRect(BOXSIZE*6, 0, BOXSIZE, BOXSIZE, WHITE);
  tft.fillRect(BS1GAP, SCREENSIZEY-BS1GAP, BS1, BS1, WHITE);
  //tft.fillRect(BS1GAP, SCREENSIZEY-BS1GAP+BS1, 2, 2, GREEN);
 
  tft.drawRect(0, 0, BOXSIZE, BOXSIZE, WHITE);
  currentcolor = RED;
 
  pinMode(13, OUTPUT);
  initflag = false;
}

void startup(void)
{
  tft.fillScreen(BLACK);
  unsigned long start = micros();
  tft.setCursor(0, 0);
  
  tft.setTextColor(WHITE);  tft.setTextSize(1);
  tft.println("Starting up Matzi's OS");
  delay(300);
  tft.println("Loading Variables");
  delay(100);
  tft.println("Allocating Space");
  delay(300);
  tft.println("Loading Drivers or whatever");
  delay(600);
  tft.setTextColor(RED);
  tft.println("Detected 3234 warnings, 0 Errors");
  tft.setTextColor(WHITE);
  tft.println("Loading Apps...");
  delay(500);
  tft.setTextColor(GREEN);
  delay(200);
  tft.println("\n\nWelcome to the worst phone ever");
  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.print("\n\n\n\n");
  delay(500);
  tft.print(".");
  delay(500);
  tft.print(".");
  delay(500);
  tft.print(".");

  delay(100);
  initflag = false;
}

void initializePhoneCall(void)
{
    Serial.begin(9600);
  Serial.println(F("TFT LCD test"));

#ifdef USE_Elegoo_SHIELD_PINOUT
  Serial.println(F("Using Elegoo 2.8\" TFT Arduino Shield Pinout"));
#else
  Serial.println(F("Using Elegoo 2.8\" TFT Breakout Board Pinout"));
#endif

  Serial.print("TFT size is "); Serial.print(tft.width()); Serial.print("x"); Serial.println(tft.height());

  tft.reset();

  uint16_t identifier = tft.readID();
  if(identifier == 0x9325) {
    Serial.println(F("Found ILI9325 LCD driver"));
  } else if(identifier == 0x9328) {
    Serial.println(F("Found ILI9328 LCD driver"));
  } else if(identifier == 0x4535) {
    Serial.println(F("Found LGDP4535 LCD driver"));
  }else if(identifier == 0x7575) {
    Serial.println(F("Found HX8347G LCD driver"));
  } else if(identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  } else if(identifier == 0x8357) {
    Serial.println(F("Found HX8357D LCD driver"));
  } else if(identifier==0x0101)
  {     
      identifier=0x9341;
       Serial.println(F("Found 0x9341 LCD driver"));
  }else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    Serial.println(F("If using the Elegoo 2.8\" TFT Arduino shield, the line:"));
    Serial.println(F("  #define USE_Elegoo_SHIELD_PINOUT"));
    Serial.println(F("should appear in the library header (Elegoo_TFT.h)."));
    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
    Serial.println(F("Also if using the breakout, double-check that all wiring"));
    Serial.println(F("matches the tutorial."));
    identifier=0x9341;
   
  }

  tft.begin(identifier);
  tft.setRotation(2);
  tft.fillScreen(BLACK);
  
  // create buttons
  for (uint8_t row=0; row<5; row++) {
    for (uint8_t col=0; col<3; col++) {
      buttons[col + row*3].initButton(&tft, BUTTON_X+col*(BUTTON_W+BUTTON_SPACING_X), 
                 BUTTON_Y+row*(BUTTON_H+BUTTON_SPACING_Y),    // x, y, w, h, outline, fill, text
                  BUTTON_W, BUTTON_H, ILI9341_WHITE, buttoncolors[col+row*3], ILI9341_WHITE,
                  buttonlabels[col + row*3], BUTTON_TEXTSIZE); 
      buttons[col + row*3].drawButton();
    }
  }
  
  // create 'text field'
  tft.drawRect(TEXT_X, TEXT_Y, TEXT_W, TEXT_H, ILI9341_WHITE);
  initflag = false;
}


/////////////////////////////////



//ADDITIONAL ROUTINES
// Print something in the mini status bar with either flashstring
void status(const __FlashStringHelper *msg) {
  tft.fillRect(STATUS_X, STATUS_Y, 240, 8, ILI9341_BLACK);
  tft.setCursor(STATUS_X, STATUS_Y);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(1);
  tft.print(msg);
}
// or charstring
void status(char *msg) {
  tft.fillRect(STATUS_X, STATUS_Y, 240, 8, ILI9341_BLACK);
  tft.setCursor(STATUS_X, STATUS_Y);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(1);
  tft.print(msg);
}
///////////////////////////////
