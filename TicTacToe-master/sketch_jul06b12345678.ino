#include <URTouch.h>
//#include <URTouchCD.h>

#include <memorysaver.h>
#include <UTFT.h>

extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];
extern uint8_t SevenSegmentFull[];
extern uint8_t GroteskBold32x64[];
extern uint8_t Calibri24x32GR[];

UTFT GLCD(ITDB32S,38,39,40,41);
URTouch Touch(6,5,4,3,2);

int turn = 0; //Between 0 and 8

int x = -1;
int y = -1;
const int offset_x = 99;
const int offset_y = 19;
const int square_size = 66;
const int border_size = 2;

int firstMove = -1;

int board[9][3] = 
{
  {-1, offset_x - 1, offset_y - 1 }, //ROW 1
  {-1, offset_x - 1 + square_size + border_size, offset_y - 1 },
  {-1, offset_x - 1 + 2 * square_size + 2 * border_size, offset_y - 1 },
  {-1, offset_x - 1, offset_y - 1 + square_size + border_size}, //ROW 2
  {-1, offset_x - 1 + square_size + border_size, offset_y - 1  + square_size + border_size},
  {-1, offset_x - 1 + 2 * square_size + 2 * border_size, offset_y - 1  + square_size + border_size},
  {-1, offset_x - 1, offset_y - 1 + 2 * square_size + 2 * border_size}, //ROW 3
  {-1, offset_x - 1 + square_size + border_size, offset_y - 1  + 2 * square_size + 2 * border_size},
  {-1, offset_x - 1 + 2 * square_size + 2 * border_size, offset_y - 1  + 2 * square_size + 2 * border_size},
};

void draw_x(int ax, int ay) {
  GLCD.setColor(76, 0, 255);
  for(int i = 0; i < 18; i++) {
    GLCD.drawLine(ax + 6 + i, ay + 3, ax + 66  - 6 - 18 + i, ay + 66 - 3);
    GLCD.drawLine(ax + 6 + 18 + 18 + i, ay + 3, ax + 6 + i, ay + 66 - 3);
  }
}

int oneMoveWin(int player) {
  if(board[0][0] == player && board[1][0] == player) return 2;
  else if(board[0][0] == player && board[2][0] == player && board[1][0] == -1) return 1;
  else if(board[1][0] == player && board[2][0] == player && board[0][0] == -1) return 0;
  else if(board[3][0] == player && board[4][0] == player && board[5][0] == -1) return 5;
  else if(board[3][0] == player && board[5][0] == player && board[4][0] == -1) return 4;
  else if(board[4][0] == player && board[5][0] == player && board[3][0] == -1) return 3;
  else if(board[6][0] == player && board[7][0] == player && board[8][0] == -1) return 8;
  else if(board[6][0] == player && board[8][0] == player && board[7][0] == -1) return 7;
  else if(board[7][0] == player && board[8][0] == player && board[6][0] == -1) return 6;
  else if(board[1][0] == player && board[4][0] == player && board[7][0] == -1) return 7;
  else if(board[3][0] == player && board[6][0] == player && board[0][0] == -1) return 0;
  else if(board[2][0] == player && board[8][0] == player && board[5][0] == -1) return 5;
  else if(board[6][0] == player && board[2][0] == player && board[4][0] == -1) return 4;
  else if(board[4][0] == player && board[8][0] == player && board[0][0] == -1) return 0;
  else if(board[0][0] == player && board[3][0] == player && board[6][0] == -1) return 6;
  else if(board[0][0] == player && board[6][0] == player && board[3][0] == -1) return 3;
  else if(board[1][0] == player && board[7][0] == player && board[4][0] == -1) return 4;
  else if(board[2][0] == player && board[5][0] == player && board[8][0] == -1) return 8;
  else if(board[5][0] == player && board[8][0] == player && board[2][0] == -1) return 2;
  else if(board[0][0] == player && board[4][0] == player && board[8][0] == -1) return 8;
  else if(board[0][0] == player && board[8][0] == player && board[4][0] == -1) return 4;
  else if(board[2][0] == player && board[4][0] == player && board[6][0] == -1) return 6;
  else if(board[4][0] == player && board[6][0] == player && board[2][0] == -1) return 2;
  else return -1;
}

int hasWon(){
   if(
    board[0][0] == 0 && board[1][0] == 0 && board[2][0] == 0 ||
    board[3][0] == 0 && board[4][0] == 0 && board[5][0] == 0 ||
    board[6][0] == 0 && board[7][0] == 0 && board[8][0] == 0 ||
    board[0][0] == 0 && board[3][0] == 0 && board[6][0] == 0 ||
    board[1][0] == 0 && board[4][0] == 0 && board[7][0] == 0 ||
    board[2][0] == 0 && board[5][0] == 0 && board[8][0] == 0 ||
    board[0][0] == 0 && board[4][0] == 0 && board[8][0] == 0 ||
    board[2][0] == 0 && board[4][0] == 0 && board[6][0] == 0
    )
    return 0; 
  
  else if(
    board[0][0] == 1 && board[1][0] == 1 && board[2][0] == 1 ||
    board[3][0] == 1 && board[4][0] == 1 && board[5][0] == 1 ||
    board[6][0] == 1 && board[7][0] == 1 && board[8][0] == 1 ||
    board[0][0] == 1 && board[3][0] == 1 && board[6][0] == 1 ||
    board[1][0] == 1 && board[4][0] == 1 && board[7][0] == 1 ||
    board[2][0] == 1 && board[5][0] == 1 && board[8][0] == 1 ||
    board[0][0] == 1 && board[4][0] == 1 && board[8][0] == 1 ||
    board[2][0] == 1 && board[4][0] == 1 && board[6][0] == 1
    )
    return 1;
    
  else return -1;
}

void draw_o(int ax, int ay) {
  GLCD.setColor(255, 0, 0);
  for(int i = 0; i < 18; i++) {
    GLCD.drawCircle(ax + 32, ay + 32, i + 12);
  }
}

void makeTurn(int field, int player) {
    board[field][0] = player;
    if(player == 0)
      draw_o(board[field][1], board[field][2]);
    else if(player == 1)
      draw_x(board[field][1], board[field][2]);
    turn++;
}

void setup() {
    Serial.begin(9600);
  
  // put your setup code here, to run once:
  // Setup the LCD
    GLCD.InitLCD();
    GLCD.clrScr();
    GLCD.fillScr(227,227,227);
    GLCD.setFont(GroteskBold32x64);
    GLCD.setContrast(64);
    GLCD.setBrightness(16);

    Touch.InitTouch();
    Touch.setPrecision(PREC_MEDIUM);

    randomSeed(analogRead(0));

    GLCD.setBackColor(227,227,227);

    GLCD.setColor(0,0,0);
    GLCD.drawLine(offset_x - 1 + square_size, offset_y - 1, offset_x - 1 + square_size, offset_y - 1 + 3 * square_size + 2 * border_size);
    GLCD.drawLine(offset_x - 1 + 2 * square_size + border_size, offset_y - 1, offset_x - 1 + 2 * square_size + border_size, offset_y - 1 + 3 * square_size + 2 * border_size);
    GLCD.drawLine(offset_x - 1, offset_y - 1 + square_size, offset_x - 1 + 3 * square_size + 2 * border_size, offset_y - 1 + square_size);
    GLCD.drawLine(offset_x - 1, offset_y - 1 + 2 * square_size + border_size, offset_x - 1 + 3 * square_size + 2 * border_size, offset_y - 1 + 2 * square_size + border_size);

    //draw_x(offset_x - 1, offset_y - 1);
    //draw_o(offset_x - 1 + square_size + border_size, offset_y - 1);

    //GLCD.drawLine(0, 0, 319, 239);
    //GLCD.drawLine(1, 0, 319, 238);
    //GLCD.drawLine(0, 1, 318, 239);
    //GLCD.drawLine(2, 0, 319, 237);

    for(int i = 0; i < 9; i++)
    {
      int topLeftX = board[i][1];
      int topLeftY = board[i][2];
      Serial.print("topLeftX=");
      Serial.print(topLeftX);
      Serial.print("topLeftY=");
      Serial.print(topLeftY);
      Serial.print('\n');
    }

    int randCorner = random(0,3);
    switch(randCorner) {
      case 0:
        makeTurn(0,0);
        firstMove = 0;
        break;
      case 1:
        makeTurn(2,0);
        firstMove = 2;
        break;
      case 2:
        makeTurn(6,0);
        firstMove = 6;
        break;
      case 4:
        makeTurn(8,0);
        firstMove = 8;
        break;
    }
}

void loop() {
  // put your main code here, to run repeatedly:

  //get touch coordinates
  if(Touch.dataAvailable()) {
    Touch.read();
    x = Touch.getX();
    y = Touch.getY();

    //Find if the user selected a square
    for(int i = 0; i < 9; i++)
    {
      int topLeftX = board[i][1];
      int topLeftY = board[i][2];
      int bottomRightX = topLeftX + square_size;
      int bottomRightY = topLeftY + square_size;

      if(x > topLeftX && y > topLeftY && x < bottomRightX && y < bottomRightY)
      {
        if(board[i][0] == -1)
        {
          makeTurn(i,1);
          Serial.print(topLeftX);
          Serial.print(" ");
          Serial.print(topLeftY);
          Serial.print('\n');
        }
      }
    }

    Serial.print(".Turn = ");
    Serial.print(turn);
    Serial.print('\n');
    if(turn == 2) {
        if(firstMove == 0) {
            if(board[8][0] == -1)
              makeTurn(8, 0);
            else
              if(random(0,1) == 1) {
                makeTurn(6,0); 
              }
              else {
                makeTurn(2,0);
              }
          }
          else if(firstMove == 2) {
            if(board[6][0] == -1)
              makeTurn(6, 0);
            else
              if(random(0,1) == 1) {
                makeTurn(0,0); 
              }
              else {
                makeTurn(8,0);
              }
          }
          else if(firstMove == 6) {
            if(board[2][0] == -1)
              makeTurn(2, 0);
            else
              if(random(0,1) == 1) {
                makeTurn(0,0); 
              }
              else {
                makeTurn(8,0);
              }
          }
          else if(firstMove == 8) {
            if(board[0][0] == -1)
              makeTurn(0, 0);
            else
              if(random(0,1) == 1) {
                makeTurn(6,0); 
              }
              else {
                makeTurn(2,0);
              }
          }
      }
      if(turn == 4) {
        Serial.print("In case 4");
        int freeCellsFound = 0;
        int ourNextMove = oneMoveWin(0);
        int opponentNextMove = oneMoveWin(1);
        if(ourNextMove > -1) {
          makeTurn(ourNextMove, 0);
        }
        else if(opponentNextMove > -1) {
          makeTurn(opponentNextMove, 0);
        }
        else {
          Serial.print("(4) Searching for free cells...");
          int randCell = random(0,4);
          for(int i = 0; i < 9; i++) {        
            if(board[i][0] == -1) {
              if(randCell == freeCellsFound) {
                Serial.print(i);
                makeTurn(i, 0);
                break;
              }
              freeCellsFound++;
            }
          }
        }
      }
      if(turn == 6) {
        Serial.print("In case 6");
        int freeCellsFound = 0;
        int ourNextMove = oneMoveWin(0);
        int opponentNextMove = oneMoveWin(1);
        if(ourNextMove > -1) {
          makeTurn(ourNextMove, 0);
        }
        else if(opponentNextMove > -1) {
          makeTurn(opponentNextMove, 0);
        }
        else {
          Serial.print("(4) Searching for free cells...");
          int randCell = random(0,2);
          for(int i = 0; i < 9; i++) {        
            if(board[i][0] == -1) {
              if(randCell == freeCellsFound) {
                Serial.print(i);
                makeTurn(i, 0);
                break;
              }
              freeCellsFound++;
            }
          }
        } 
      }
      if(turn == 8) {
        Serial.print("In case 8");
        int freeCellsFound = 0;
        int ourNextMove = oneMoveWin(0);
        int opponentNextMove = oneMoveWin(1);
        if(ourNextMove > -1) {
          makeTurn(ourNextMove, 0);
        }
        else if(opponentNextMove > -1) {
          makeTurn(opponentNextMove, 0);
        }
        else {
          Serial.print("(4) Searching for free cells...");
          int randCell = 0;
          for(int i = 0; i < 9; i++) {        
            if(board[i][0] == -1) {
              if(randCell == freeCellsFound) {
                Serial.print(i);
                makeTurn(i, 0);
                break;
              }
              freeCellsFound++;
            }
          }
        }
      }
    }
    int winner = hasWon();
    if(winner == 1) {
      GLCD.setColor(0,0,255);
      GLCD.print("You won!", 32, 88);
      winner = -1;
    }
    else if(winner == 0){
      GLCD.setColor(255,0,0);
      GLCD.print("You lost!", 16, 88);
      winner = -1;
    }
    else if(turn == 9) {
      GLCD.setColor(0,0,0);
      GLCD.print("Draw!", 80, 88);
      turn = -1;
    }
  }
