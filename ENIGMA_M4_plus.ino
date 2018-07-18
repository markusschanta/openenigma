/* S&T GeoTronics LLC  **** OPEN ENIGMA ****

   This code is provided as is. S&T GeoTronics LLC, it's Partners and Associates provide it freely,
   with no warranty of it's functionality, fitness for use or usefulness. In no way shall S&T GeoTronics LLC,
   it's Partners and Associates be liable for the use, misuse, or damages of any kind due to the use or
   misuse of this code or any associated hardware provided.
   
   Enigma Code. This Arduino Mega custom shield is programmed to replicate
   exactly the behavior of a true German M4 Enigma machine.
   It uses 4 16-Segment units, 5 LEDs, 26 Lamps setup as keyboard, 26 keyboard buttons
   & 10 Function keys. The 115 light emitting diodes are multi-plexed to minimize the 
   amount of pins needed down to 38 and all 36 pushbuttons keys are sharing a total of 4 pins.
   Designed, assembled & programmed by Marc Tessier & James Sanderson 9/20/13
   Modified for our Prototype PCB pair on 12/4/13.
   Modified to obey doublesteping and have M3 function 22 MAR 14
   This code shall remain in public domain as regulated under the creative commons licence.
*/

// Define the variables
unsigned long time = millis();
unsigned long otime = time;
int inpin[4] = {A0, A1, A2, A3};
int inval[4] = {0, 0, 0, 0};
int keyval = 100;
boolean windex = 0;
boolean windex1 = 0;
boolean windex2 = 0;
int lampval = 100;
int procesval = 0;
int procesvala = 0;
int mode = 0;
unsigned long mtime;
int mdex =0;
int behavior = 1;

// Define each Nixie character
int dig1 = 37;
int dig2 = 37;
int dig3 = 37;
int dig4 = 37;

int data[3][40] ={ {36,36,36,36,18,39,19,36,6,4,14,19,17,14,13,8,2,18,36,4,13,8,6,12,0,36,12,0,17,10,36,30,36,13,3,19,36,36,36,36},
                  {36,36,36,36,18,39,19,36,6,4,14,19,17,14,13,8,2,18,36,4,13,8,6,12,0,36,12,0,17,10,36,30,36,36,3,19,36,36,36,36},
                  {36,36,36,36,18,39,19,36,6,4,14,19,17,14,13,8,2,18,36,4,13,8,6,12,0,36,12,0,17,10,36,29,36,36,3,19,36,36,36,36} };

// Define the 16-Segments Pins as 2 Arrays
int segment[17] = {24,22,25,31,38,36,32,30,28,26,23,27,33,35,34,29,37}; //cathode array
int anode[4] = {39,41,43,45}; //annode array commin annode

// Define the 26 Lamps as a 2D Array 
int lamparray[26] [2] = {{12,2},{13,7},{13,5},{12,4},{11,4},{12,5,},{12,6},{12,7},{11,9},{12,8},
                         {12,9},{13,10},{13,9},{13,8},{11,10},{13,2},{11,2},{11,5},{12,3},{11,6},
                         {11,8},{13,6},{11,3},{13,4},{13,3},{11,7}};

//  Define the 12 Lamp Pins for initialization
int lamppin[12] = {2,3,4,5,6,7,8,9,10,11,12,13}; //2 to 10 cathode, 11 to 13 common annode
 
 // Define each LTP587P Segments: A,B,C,D,E,F,G,H,K,M,N,P,R,S,T,U,dp               
boolean segmentvals[40][17] = { { 0,0,0,0,1,1,0,0,1,1,1,0,1,1,1,0,1 },  // = A 0
                                { 0,0,0,0,0,0,1,1,1,0,1,0,1,0,1,1,1 },  // = B 1
                                { 0,0,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1 },  // = C 2
                                { 0,0,0,0,0,0,1,1,1,0,1,1,1,0,1,1,1 },  // = D 3
                                { 0,0,1,1,0,0,0,0,1,1,1,0,1,1,1,0,1 },  // = E 4
                                { 0,0,1,1,1,1,0,0,1,1,1,0,1,1,1,0,1 },  // = F 5
                                { 0,0,1,0,0,0,0,0,1,1,1,0,1,1,1,1,1 },  // = G 6
                                { 1,1,0,0,1,1,0,0,1,1,1,0,1,1,1,0,1 },  // = H 7
                                { 0,0,1,1,0,0,1,1,1,0,1,1,1,0,1,1,1 },  // = I 8
                                { 1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1 },  // = J 9
                                { 1,1,1,1,1,1,0,0,1,1,0,1,0,1,1,0,1 },  // = K 10
                                { 1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1 },  // = L 11 
                                { 1,1,0,0,1,1,0,0,0,1,0,1,1,1,1,1,1 },  // = M 12
                                { 1,1,0,0,1,1,0,0,0,1,1,1,0,1,1,1,1 },  // = N 13
                                { 0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1 },  // = O 14
                                { 0,0,0,1,1,1,0,0,1,1,1,0,1,1,1,0,1 },  // = P 15
                                { 0,0,0,0,0,0,0,0,1,1,1,1,0,1,1,1,1 },  // = Q 16
                                { 0,0,0,1,1,1,0,0,1,1,1,0,0,1,1,0,1 },  // = R 17
                                { 0,0,1,0,0,0,1,0,1,1,1,0,1,1,1,0,1 },  // = S 18
                                { 0,0,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1 },  // = T 19
                                { 1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1 },  // = U 20
                                { 1,1,1,1,1,1,0,0,1,1,0,1,1,1,0,1,1 },  // = V 21
                                { 1,1,0,0,1,1,0,0,1,1,1,1,0,1,0,1,1 },  // = W 22
                                { 1,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,1 },  // = X 23
                                { 1,1,1,1,1,1,1,1,0,1,0,1,1,0,1,1,1 },  // = Y 24
                                { 0,0,1,1,0,0,1,1,1,1,0,1,1,1,0,1,1 },  // = Z 25
                                { 0,0,0,0,0,0,0,0,1,1,0,1,1,1,0,1,1 },  // = 0 26
                                { 1,1,0,0,1,1,1,1,1,1,0,1,1,1,1,1,1 },  // = 1 27
                                { 0,0,0,1,0,0,0,1,1,1,1,0,1,1,1,0,1 },  // = 2 28
                                { 0,0,0,0,0,0,1,1,1,1,1,0,1,1,1,1,1 },  // = 3 29
                                { 1,1,0,0,1,1,1,0,1,1,1,0,1,1,1,0,1 },  // = 4 30
                                { 0,0,1,0,0,0,1,0,1,1,1,0,1,1,1,0,1 },  // = 5 31
                                { 0,0,1,0,0,0,0,0,1,1,1,0,1,1,1,0,1 },  // = 6 32
                                { 0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1 },  // = 7 33
                                { 0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,0,1 },  // = 8 34
                                { 0,0,0,0,0,0,1,0,1,1,1,0,1,1,1,0,1 },  // = 9 35
                                { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },  // = Space 36
                                { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },  // = Full Lit 37
                                { 1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1 },  // = SS 38
                                { 0,1,1,1,0,0,0,1,0,0,1,1,0,1,1,0,1} }; // = & 39
//              LTP587P Segments: A,B,C,D,E,F,G,H,K,M,N,P,R,S,T,U,dp

// Define the 5 Mode LEDs 
int led1 = 40;
int led2 = 42;
int led3 = 44;
int led4 = 46;
int led5 = 48;

                                      //4,10,12, 5,11, 6, 3,16,21,25,13,19,14,22,24, 7,23,20,18,15, 0, 8, 1,17, 2, 9
//Define the rotor values                 A  B  C  D  E  F  G  H  I  J  K  L  M  N  O  P  Q
static const int rotorvals[12][78] = { {  4,10,12, 5,11, 6, 3,16,21,25,13,19,14,22,24, 7,123,20,18,15, 0, 8, 1,17, 2, 9,
                                         4,10,12, 5,11, 6, 3,16,21,25,13,19,14,22,24, 7,123,20,18,15, 0, 8, 1,17, 2, 9,
                                         4,10,12, 5,11, 6, 3,16,21,25,13,19,14,22,24, 7,123,20,18,15, 0, 8, 1,17, 2, 9 },  // wheel 1

                                       {  0, 9, 3,10,118, 8,17,20,23, 1,11, 7,22,19,12, 2,16, 6,25,13,15,24, 5,21,14, 4,
                                          0, 9, 3,10,118, 8,17,20,23, 1,11, 7,22,19,12, 2,16, 6,25,13,15,24, 5,21,14, 4,
                                          0, 9, 3,10,118, 8,17,20,23, 1,11, 7,22,19,12, 2,16, 6,25,13,15,24, 5,21,14, 4 },  // wheel 2
                                         
                                       {  1, 3, 5, 7, 9,11, 2,15,17,19,23,21,25,13,24, 4, 8,22, 6, 0,10,112,20,18,16,14,
                                          1, 3, 5, 7, 9,11, 2,15,17,19,23,21,25,13,24, 4, 8,22, 6, 0,10,112,20,18,16,14,
                                          1, 3, 5, 7, 9,11, 2,15,17,19,23,21,25,13,24, 4, 8,22, 6, 0,10,112,20,18,16,14 },  // wheel 3
                                         
                                       {  4,18,14,21,15,25, 9, 0,24,116,20, 8,17, 7,23,11,13, 5,19, 6,10, 3, 2,12,22, 1,
                                          4,18,14,21,15,25, 9, 0,24,116,20, 8,17, 7,23,11,13, 5,19, 6,10, 3, 2,12,22, 1,
                                          4,18,14,21,15,25, 9, 0,24,116,20, 8,17, 7,23,11,13, 5,19, 6,10, 3, 2,12,22, 1 },  // wheel 4
                                           
                                       { 21,25, 1,17, 6, 8,19,24,20,15,18, 3,13, 7,11,23, 0,22,12, 9,16,14, 5, 4, 2,110, 
                                         21,25, 1,17, 6, 8,19,24,20,15,18, 3,13, 7,11,23, 0,22,12, 9,16,14, 5, 4, 2,110,
                                         21,25, 1,17, 6, 8,19,24,20,15,18, 3,13, 7,11,23, 0,22,12, 9,16,14, 5, 4, 2,110  },  // wheel 5
                                         
                                       {  9,15, 6,21,14,20,12, 5,24,16, 1, 4,113, 7,25,17, 3,10, 0,18,23,11, 8, 2,19,122,
                                          9,15, 6,21,14,20,12, 5,24,16, 1, 4,113, 7,25,17, 3,10, 0,18,23,11, 8, 2,19,122,
                                          9,15, 6,21,14,20,12, 5,24,16, 1, 4,113, 7,25,17, 3,10, 0,18,23,11, 8, 2,19,122 },  // wheel 6
                                          
                                       { 13,25, 9, 7, 6,17, 2,23,12,24,18,22,101,14,20, 5, 0, 8,21,11,15, 4,10,16, 3,119,
                                         13,25, 9, 7, 6,17, 2,23,12,24,18,22,101,14,20, 5, 0, 8,21,11,15, 4,10,16, 3,119,
                                         13,25, 9, 7, 6,17, 2,23,12,24,18,22,101,14,20, 5, 0, 8,21,11,15, 4,10,16, 3,119 },  // wheel 7
                                         
                                       { 5,10,16, 7,19,11,23,14, 2, 1, 9,18,115, 3,25,17, 0,12, 4,22,13, 8,20,24, 6,121,
                                         5,10,16, 7,19,11,23,14, 2, 1, 9,18,115, 3,25,17, 0,12, 4,22,13, 8,20,24, 6,121,
                                         5,10,16, 7,19,11,23,14, 2, 1, 9,18,115, 3,25,17, 0,12, 4,22,13, 8,20,24, 6,121 },    // wheel 8
                                         
                                       { 11,4,24,9,21,2,13,8,23,22,15,1,16,12,3,17,19,0,10,25,6,5,20,7,14,18,
                                         11,4,24,9,21,2,13,8,23,22,15,1,16,12,3,17,19,0,10,25,6,5,20,7,14,18,
                                         11,4,24,9,21,2,13,8,23,22,15,1,16,12,3,17,19,0,10,25,6,5,20,7,14,18 },    // Beta
                                         
                                       { 5,18,14,10,0,13,20,4,17,7,12,1,19,8,24,2,22,11,16,15,25,23,21,6,9,3,
                                         5,18,14,10,0,13,20,4,17,7,12,1,19,8,24,2,22,11,16,15,25,23,21,6,9,3,
                                         5,18,14,10,0,13,20,4,17,7,12,1,19,8,24,2,22,11,16,15,25,23,21,6,9,3 },    // Gamma
                                         
                                       { 4,13,10,16,0,20,24,22,9,8,2,14,15,1,11,12,3,23,25,21,5,19,7,17,6,18, 
                                         4,13,10,16,0,20,24,22,9,8,2,14,15,1,11,12,3,23,25,21,5,19,7,17,6,18,
                                         4,13,10,16,0,20,24,22,9,8,2,14,15,1,11,12,3,23,25,21,5,19,7,17,6,18},    // = UKW-B
                                         
                                       { 17,3,14,1,9,13,19,10,21,4,7,12,11,5,2,22,25,0,23,6,24,8,15,18,20,16,
                                         17,3,14,1,9,13,19,10,21,4,7,12,11,5,2,22,25,0,23,6,24,8,15,18,20,16,
                                         17,3,14,1,9,13,19,10,21,4,7,12,11,5,2,22,25,0,23,6,24,8,15,18,20,16 }    // = UKW-C 
                                        };
                                        
                                    
static const int rotorvali[10][78] = { { 20,22,24, 6, 0, 3, 5,15,21,25, 1, 4, 2,10,12,19, 7,23,18,11,17, 8,13,16,14, 9,
                                         20,22,24, 6, 0, 3, 5,15,21,25, 1, 4, 2,10,12,19, 7,23,18,11,17, 8,13,16,14, 9,
                                         20,22,24, 6, 0, 3, 5,15,21,25, 1, 4, 2,10,12,19, 7,23,18,11,17, 8,13,16,14, 9  },  //wheel 1 i
//                                   
                                      {  0, 9,15, 2,25,22,17,11, 5, 1, 3,10,14,19,24,20,16, 6, 4,13, 7,23,12, 8,21,18, 
                                         0, 9,15, 2,25,22,17,11, 5, 1, 3,10,14,19,24,20,16, 6, 4,13, 7,23,12, 8,21,18, 
                                         0, 9,15, 2,25,22,17,11, 5, 1, 3,10,14,19,24,20,16, 6, 4,13, 7,23,12, 8,21,18  },  //wheel 2 i
                                        
                                      { 19, 0, 6, 1,15, 2,18, 3,16, 4,20, 5,21,13,25, 7,24, 8,23, 9,22,11,17,10,14,12, 
                                        19, 0, 6, 1,15, 2,18, 3,16, 4,20, 5,21,13,25, 7,24, 8,23, 9,22,11,17,10,14,12, 
                                        19, 0, 6, 1,15, 2,18, 3,16, 4,20, 5,21,13,25, 7,24, 8,23, 9,22,11,17,10,14,12 },  //wheel 3 i
                                        
                                        
                                       { 7,25,22,21, 0,17,19,13,11, 6,20,15,23,16, 2, 4, 9,12, 1,18,10, 3,24,14, 8, 5, 
                                         7,25,22,21, 0,17,19,13,11, 6,20,15,23,16, 2, 4, 9,12, 1,18,10, 3,24,14, 8, 5,
                                         7,25,22,21, 0,17,19,13,11, 6,20,15,23,16, 2, 4, 9,12, 1,18,10, 3,24,14, 8, 5 },  //wheel 4 i
                                         
                                       { 16, 2,24,11,23,22, 4,13, 5,19,25,14,18,12,21, 9,20, 3,10, 6, 8, 0,17,15, 7, 1, 
                                         16, 2,24,11,23,22, 4,13, 5,19,25,14,18,12,21, 9,20, 3,10, 6, 8, 0,17,15, 7, 1, 
                                         16, 2,24,11,23,22, 4,13, 5,19,25,14,18,12,21, 9,20, 3,10, 6, 8, 0,17,15, 7, 1 },  //wheel 5 i
                                         
                                       { 18,10,23,16,11, 7, 2,13,22, 0,17,21,06,12, 4, 1, 9,15,19,24, 5, 3, 25,20, 8,14,
                                         18,10,23,16,11, 7, 2,13,22, 0,17,21,06,12, 4, 1, 9,15,19,24, 5, 3, 25,20, 8,14,
                                         18,10,23,16,11, 7, 2,13,22, 0,17,21,06,12, 4, 1, 9,15,19,24, 5, 3, 25,20, 8,14 },  //wheel 6 i
                                     
                                       { 16,12,6,24,21,15,4,3,17,2,22,19,8,0,13,20,23,5,10,25,14,18,11,7,9,1,
                                         16,12,6,24,21,15,4,3,17,2,22,19,8,0,13,20,23,5,10,25,14,18,11,7,9,1,
                                         16,12,6,24,21,15,4,3,17,2,22,19,8,0,13,20,23,5,10,25,14,18,11,7,9,1 },  //wheel 7 i
                                        
                                       { 16,9,8,13,18,0,24,3,21,10,1,5,17,20,7,12,2,15,11,4,22,25,19,6,23,14,
                                         16,9,8,13,18,0,24,3,21,10,1,5,17,20,7,12,2,15,11,4,22,25,19,6,23,14,
                                         16,9,8,13,18,0,24,3,21,10,1,5,17,20,7,12,2,15,11,4,22,25,19,6,23,14 },   //wheel 8 i
                                         
                                       { 17,11,5,14,1,21,20,23,7,3,18,0,13,6,24,10,12,15,25,16,22,4,9,8,2,19,
                                         17,11,5,14,1,21,20,23,7,3,18,0,13,6,24,10,12,15,25,16,22,4,9,8,2,19,
                                         17,11,5,14,1,21,20,23,7,3,18,0,13,6,24,10,12,15,25,16,22,4,9,8,2,19 },   //Beta i
                                         
                                       { 4,11,15,25,7,0,23,9,13,24,3,17,10,5,2,19,18,8,1,12,6,22,16,21,14,20,
                                         4,11,15,25,7,0,23,9,13,24,3,17,10,5,2,19,18,8,1,12,6,22,16,21,14,20,
                                         4,11,15,25,7,0,23,9,13,24,3,17,10,5,2,19,18,8,1,12,6,22,16,21,14,20 } }; //Gamma i
                                                                                
                                        
// Define a 2D Array for keeping the wheel locations & positions
int wheel[4][3] = {{29,0,0},{28,0,0},{27,0,0},{1,0,0}};
int reflect[2] = {1,0};

// Define Array for plugbord values 25 x2 position 0 holds use position 1 holds value int plugu holds the total nomber of plugs used (10 max)
int plugval [2][26] = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                       {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25}};
int pluguse = 0; int paindex = 0; int pbindex = 1;


void setup() {
// Initialize all 38 LED pins as Output  
  for (int index = 0; index <= 11; index++) {
  pinMode(lamppin[index], OUTPUT);
  digitalWrite(lamppin[index],1);
  }
  for (int index = 0 ; index <= 3; index++) {
    pinMode (anode[index], OUTPUT);
    digitalWrite (anode[index], 1);
  }
  for (int index = 0 ; index <= 16; index++) {
    pinMode (segment[index], OUTPUT);
    digitalWrite (segment[index], 1);
  }  
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
  pinMode(led4,OUTPUT);
  pinMode(led5,OUTPUT);
  
 //Serial.begin(9600);
// Initialize all 4 pusbutton pins as Input    
  for (int index = 0; index <= 3; index++) {
    pinMode(inpin[index], INPUT);
  }

}

void loop() {
// Keyboard debounce & test for new key pressed  
  time = millis();
  if (time > otime + 500) {keyval = readkbde();}
  if ((mode == 0) && (keyval == 46) && (windex ==1)){ behavior ++; windex =0; if(behavior > 2) {behavior = 0; } }
  if((keyval == 45) && (windex ==1)) {modeselect();}
// The whole Enigma machine operation revolves around which Operating Mode is current  
  if(mode == 0) {mode0();}
  else if(mode == 1) {mode1();}
  else if(mode == 2) {mode2();}
  else if(mode == 3) {mode3();}
  else if(mode == 4) {mode4();}
  else if(mode == 5) {mode5();}
  else {    }

//Serial.println(keyval); // for debuging prints keybord value to serial monitor// for run or del

}

// This function takes care of figuring out which key has been pressed & returns a unique Integer
 int readkbde() {
 int kval = 100;
 for (int index = 0; index <= 3; index++) { inval[index] = analogRead(inpin[index]);   } //Reads analog input values
 if((inval[0] > 982) && (inval[1] > 973) && (inval[2] > 966) && (inval[3] > 973))  {kval = 100;} // no key press
 else if((inval[0] < 981) && (inval[0] > 903)) {kval = 49;} //up arrow 4
 else if((inval[0] < 902) && (inval[0] > 831)) {kval = 48;} //up arrow 3
 else if((inval[0] < 830) && (inval[0] > 762)) {kval = 47;} //up arrow 2
 else if((inval[0] < 761) && (inval[0] > 693)) {kval = 46;} //up arrow 1
 else if((inval[0] < 692) && (inval[0] > 622)) {kval = 45;} //mode
 else if((inval[0] < 621) && (inval[0] > 545)) {kval = 44;} //enter
 else if((inval[0] < 544) && (inval[0] > 458)) {kval = 43;}
 else if((inval[0] < 457) && (inval[0] > 358)) {kval = 42;}
 else if((inval[0] < 357) && (inval[0] > 237)) {kval = 41;}
 else if(inval[0] < 236 ) {kval = 40;}
 else if((inval[1] < 972) && (inval[1] > 875)) {kval = 16;}
 else if((inval[1] < 874) && (inval[1] > 784)) {kval = 22;}
 else if((inval[1] < 783) && (inval[1] > 696)) {kval = 4;}
 else if((inval[1] < 695) && (inval[1] > 605)) {kval = 17;}
 else if((inval[1] < 604) && (inval[1] > 507)) {kval = 19;}
 else if((inval[1] < 506) && (inval[1] > 395)) {kval = 25;}
 else if((inval[1] < 394) && (inval[1] > 261)) {kval = 20;}
 else if((inval[1] < 260) && (inval[1] > 94)) {kval = 8;}
 else if(inval[1] < 93 ) {kval = 14;}
 else if((inval[2] < 965) && (inval[2] > 855)) {kval = 0;}
 else if((inval[2] < 854) && (inval[2] > 752)) {kval = 18;}
 else if((inval[2] < 751) && (inval[2] > 650)) {kval = 3;}
 else if((inval[2] < 649) && (inval[2] > 542)) {kval = 5;}
 else if((inval[2] < 541) && (inval[2] > 421)) {kval = 6;}
 else if((inval[2] < 420) && (inval[2] > 278)) {kval = 7;}
 else if((inval[2] < 277) && (inval[2] > 100)) {kval = 9;}
 else if(inval[2] < 99 ) {kval = 10;}
 else if((inval[3] < 972) && (inval[3] > 875)) {kval = 15;}
 else if((inval[3] < 874) && (inval[3] > 784)) {kval = 24;}
 else if((inval[3] < 783) && (inval[3] > 696)) {kval = 23;}
 else if((inval[3] < 695) && (inval[3] > 605)) {kval = 2;}
 else if((inval[3] < 604) && (inval[3] > 507)) {kval = 21;}
 else if((inval[3] < 506) && (inval[3] > 395)) {kval = 1;}
 else if((inval[3] < 394) && (inval[3] > 261)) {kval = 13;}
 else if((inval[3] < 260) && (inval[3] > 94)) {kval = 12;}
 else if(inval[3] < 93 ) {kval = 11;}
 else {kval = 100;}
 if(kval < 99) {otime = millis();}              //Starts key debounce timer
 if((kval >= 0) && (kval <= 99)) {windex = 1;}  //windex showing true (1) indicates the return of a fresh key stroke
 return kval;
}

// Function to change operating Mode
void modeselect() {
  mode++;
  if(mode >=6) {mode = 0;}
  windex = 0;
}

// Default Mode: Enigma is a Typewriter
void mode0() {
  
  if((keyval >= 0) && (keyval <= 25)) {lampval = keyval;}
  lampita();
  marquee();
  lampitb();
  }

// Select the Rotors & the Reflector  
  void mode1() {
    int index;
    digitalWrite(led1, HIGH);
  if(windex == 1) {if((keyval == 43) || (keyval == 46)){ if(behavior != 2) {wheel[3][0]++;windex = 0; if (wheel[3][0] > 2) {wheel[3][0] = 1; reflect[0]++; if (reflect[0] > 2) {reflect[0] = 1;}} }
                        else {reflect[0]++; windex = 0; if (reflect[0] > 2) {reflect[0] = 1;} }}}
    if(windex == 1) {if(keyval == 47) { for(index = wheel[2][0];(index == wheel[1][0]) || (index == wheel[0][0]) || (index == wheel[2][0]);
                        index++) {if(index > 33) {index = 26;} }wheel[2][0] = index;  windex = 0;}}
    if(windex == 1) {if(keyval == 48) { for(index = wheel[1][0];(index == wheel[2][0]) || (index == wheel[0][0]) || (index == wheel[1][0]);
                        index++) {if(index > 33) {index = 26;} }wheel[1][0] = index;  windex = 0;}}
    if(windex == 1) {if(keyval == 49) { for(index = wheel[0][0];(index == wheel[2][0]) || (index == wheel[1][0]) || (index == wheel[0][0]);
                        index++) {if(index > 33) {index = 26;} }wheel[0][0] = index;  windex = 0;}}
    if(windex == 1) {if(keyval == 42) { for(index = wheel[2][0];(index == wheel[1][0]) || (index == wheel[0][0]) || (index == wheel[2][0]);
                        index--) {if(index < 28) {index = 35;} }wheel[2][0] = index;  windex = 0;}}
    if(windex == 1) {if(keyval == 41) { for(index = wheel[1][0];(index == wheel[2][0]) || (index == wheel[0][0]) || (index == wheel[1][0]);
                        index--) {if(index < 28) {index = 35;} }wheel[1][0] = index;  windex = 0;}}
    if(windex == 1) {if(keyval == 40) { for(index = wheel[0][0];(index == wheel[2][0]) || (index == wheel[1][0]) || (index == wheel[0][0]);
                        index--) {if(index < 28) {index = 35;} }wheel[0][0] = index;  windex = 0;}} 
    dig2 = wheel[2][0];  dig3 = wheel[1][0]; dig4 = wheel[0][0]; if(behavior != 2) { if( wheel[3][0] == 1 ) { dig1 = 1;} else {dig1 = 6;}} else{ dig1 = 36;} 
    lampval = reflect[0];
    lampita();
    nixisend();
    lampitb();
    dig1 =37; dig2 = 37; dig3 = 37; dig4 = 37;
    digitalWrite(led1, LOW);
  }

// Position the Inner setting of each Rotor  
  void mode2() {
    digitalWrite(led2, HIGH);
    if(windex == 1){
     if(behavior != 2) { if(keyval == 46) {wheel[3][1]++; if(wheel[3][1] > 25) {wheel[3][1] = 0;}}}
      if(keyval == 47) {wheel[2][1]++; if(wheel[2][1] > 25) {wheel[2][1] = 0;}}
      if(keyval == 48) {wheel[1][1]++; if(wheel[1][1] > 25) {wheel[1][1] = 0;}}
      if(keyval == 49) {wheel[0][1]++; if(wheel[0][1] > 25) {wheel[0][1] = 0;}}
      if(behavior != 2) {if(keyval == 43) {wheel[3][1]--; if(wheel[3][1] < 0) {wheel[3][1] = 25;}}}
      if(keyval == 42) {wheel[2][1]--; if(wheel[2][1] < 0) {wheel[2][1] = 25;}}
      if(keyval == 41) {wheel[1][1]--; if(wheel[1][1] < 0) {wheel[1][1] = 25;}}
      if(keyval == 40) {wheel[0][1]--; if(wheel[0][1] < 0) {wheel[0][1] = 25;}}
      windex = 0; }
    
    dig2 = wheel[2][1];  dig3 = wheel[1][1]; dig4 = wheel[0][1]; if(behavior != 2) {dig1 =wheel[3][1];} else {dig1 = 36;}
    nixisend();
    dig1 =37; dig2 = 37; dig3 = 37; dig4 = 37;
    digitalWrite(led2, LOW);
  }

// Position the Start character of each Wheel  
  void mode3() {
    digitalWrite(led3, HIGH);
    if(windex == 1){
     if(behavior != 2) { if(keyval == 46) {wheel[3][2]++; if(wheel[3][2] > 25) {wheel[3][2] = 0;}}}                                                                          // {reflect[1]++; if(reflect[1] > 25) {reflect[1] = 0;}}}
      if(keyval == 47) {wheel[2][2]++; if(wheel[2][2] > 25) {wheel[2][2] = 0;}}
      if(keyval == 48) {wheel[1][2]++; if(wheel[1][2] > 25) {wheel[1][2] = 0;}}
      if(keyval == 49) {wheel[0][2]++; if(wheel[0][2] > 25) {wheel[0][2] = 0;}}
      if(behavior != 2) {if(keyval == 43) {wheel[3][2]--; if(wheel[3][2] < 0) {wheel[3][2] = 25;}}}                                                                          // {reflect[1]--; if(reflect[1] < 0) {reflect[1] = 25;}}}
      if(keyval == 42) {wheel[2][2]--; if(wheel[2][2] < 0) {wheel[2][2] = 25;}}
      if(keyval == 41) {wheel[1][2]--; if(wheel[1][2] < 0) {wheel[1][2] = 25;}}
      if(keyval == 40) {wheel[0][2]--; if(wheel[0][2] < 0) {wheel[0][2] = 25;}}
      windex = 0; }
    
    dig2 = wheel[2][2];  dig3 = wheel[1][2]; dig4 = wheel[0][2]; dig1 = wheel[3][2];
    if (behavior == 2) {dig1 = 36;}
    nixisend();
    dig1 =37; dig2 = 37; dig3 = 37; dig4 = 37;
    digitalWrite(led3, LOW);
  }

// Define the Plugboard pairs  
  void mode4() {
    int index = 0;
    digitalWrite(led4, HIGH);
    if(pluguse <= 9) {
    if(plugval[0][paindex] == 1) {for(index = paindex;(index == paindex) || (index == pbindex) || (plugval[0][index] == 1); index++) {if(index > 24) {index = -1;}} paindex = index;}
    if(plugval[0][pbindex] == 1) {for(index = pbindex;(index == pbindex) || (index == paindex) || (plugval[0][index] == 1); index++) {if(index > 24) {index = -1;}} pbindex = index;}
    if(windex == 1)  { 
    if(keyval == 46) { for(index = paindex;(index == paindex) || (index == pbindex) || (plugval[0][index] == 1); index++) {if(index > 24) {index = -1;}} paindex = index; windex = 0; }
    if(keyval == 43) { for(index = paindex;(index == paindex) || (index == pbindex) || (plugval[0][index] == 1); index--) {if(index < 1) {index = 26;}} paindex = index; windex = 0;  }
    if(keyval == 49) { for(index = pbindex;(index == pbindex) || (index == paindex) || (plugval[0][index] == 1); index++) {if(index > 24) {index = -1;}} pbindex = index; windex = 0; }
    if(keyval == 40) { for(index = pbindex;(index == pbindex) || (index == paindex) || (plugval[0][index] == 1); index--) {if(index < 1) {index = 26;}} pbindex = index; windex = 0;  }
    if(keyval == 44) { plugval[0][paindex] = 1; plugval[1][paindex] = pbindex; plugval[0][pbindex] = 1; plugval[1][pbindex] = paindex; windex = 0; pluguse++;}
    }
    dig2 = 19; dig3 = 14; dig4 = pbindex; dig1 = paindex;
    nixisend();
    dig1 =37; dig2 = 37; dig3 = 37; dig4 = 37;
      }
      else {done();}
    digitalWrite(led4, LOW);
  }

// This is Normal Operation Mode to Encrypt/Decrypt  
  void mode5() {
    int pv = 0;
    digitalWrite(led5, HIGH);
    
    if((keyval >= 0) && (keyval <= 25)) {  if(windex == 1){procesvala = keyval;  indexwheels();}}
    windex = 0;
    procesval = procesvala;
    procesval = plugval[1][procesval];
   // Serial.print (procesval); Serial.print("   ");
    
    pv = (procesval + (wheel[0][2] - wheel[0][1]));
    if(pv < 0) {pv = pv + 26;}
    procesval = rotorvals[wheel[0][0] -27][pv]; 
    if(procesval >= 100) {procesval = procesval - 100;}
    procesval = (procesval - (wheel[0][2] - wheel[0][1]));
    if(procesval < 0) {procesval = procesval + 26;}if(procesval > 25) {procesval = procesval - 26;}
  //  Serial.print (procesval); Serial.print("   ");
    
    pv = (procesval + (wheel[1][2] - wheel[1][1]));
    if(pv < 0) {pv = pv + 26;}
    procesval = rotorvals[wheel[1][0] -27][pv]; 
    if(procesval >= 100) {procesval = procesval - 100;}
    procesval = (procesval - (wheel[1][2] - wheel[1][1]));
    if(procesval < 0) {procesval = procesval + 26;}if(procesval > 25) {procesval = procesval - 26;}
   // Serial.print (procesval); Serial.print("   ");
    
    pv = (procesval + (wheel[2][2] - wheel[2][1]));
    if(pv < 0) {pv = pv + 26;}
    procesval = rotorvals[wheel[2][0] -27][pv]; 
    if(procesval >= 100) {procesval = procesval - 100;}
    procesval = (procesval - (wheel[2][2] - wheel[2][1]));
    if(procesval < 0) {procesval = procesval + 26;}if(procesval > 25) {procesval = procesval - 26;}
   // Serial.print (procesval); Serial.print("   ");
   
   
    pv = (procesval + (wheel[3][2] - wheel[3][1]));
    if(pv < 0) {pv = pv + 26;}
    procesval = rotorvals[wheel[3][0] +7][pv]; 
    if(procesval >= 100) {procesval = procesval - 100;}
    procesval = (procesval - (wheel[3][2] - wheel[3][1]));
    if(procesval < 0) {procesval = procesval + 26;}if(procesval > 25) {procesval = procesval - 26;}
   // Serial.print (procesval); Serial.print("   ");
   
   
    procesval = rotorvals[reflect[0] + 9][procesval];
    //Serial.print (procesval); Serial.print("   ");
    
   
     pv = (procesval + (wheel[3][2] - wheel[3][1]));
    if(pv < 0) {pv = pv + 26;}
    procesval = rotorvali[wheel[3][0] +7][pv]; 
    if(procesval >= 100) {procesval = procesval - 100;}
    procesval = (procesval - (wheel[3][2] - wheel[3][1]));
    if(procesval < 0) {procesval = procesval + 26;}if(procesval > 25) {procesval = procesval - 26;}
   //Serial.print (procesval); Serial.print("   ");
    
   
  
    pv = (procesval + (wheel[2][2] - wheel[2][1]));
    if(pv < 0) {pv = pv + 26;}
    procesval = rotorvali[wheel[2][0] -27][pv]; 
    if(procesval >= 100) {procesval = procesval - 100;}
    procesval = (procesval - (wheel[2][2] - wheel[2][1]));
    if(procesval < 0) {procesval = procesval + 26;}if(procesval > 25) {procesval = procesval - 26;}
   //Serial.print (procesval); Serial.print("   ");
  
    pv = (procesval + (wheel[1][2] - wheel[1][1]));
    if(pv < 0) {pv = pv + 26;}
    procesval = rotorvali[wheel[1][0] -27][pv]; 
    if(procesval >= 100) {procesval = procesval - 100;}
    procesval = (procesval - (wheel[1][2] - wheel[1][1]));
    if(procesval < 0) {procesval = procesval + 26;}if(procesval > 25) {procesval = procesval - 26;}
   //Serial.print (procesval); Serial.print("   ");
   
    pv = (procesval + (wheel[0][2] - wheel[0][1]));
    if(pv < 0) {pv = pv + 26;}
    procesval = rotorvali[wheel[0][0] -27][pv]; 
    if(procesval >= 100) {procesval = procesval - 100;}
    procesval = (procesval - (wheel[0][2] - wheel[0][1]));
    if(procesval < 0) {procesval = procesval + 26;}if(procesval > 25) {procesval = procesval - 26;}
   
   // Serial.print (procesval); Serial.print("   ");
   
    procesval = plugval[1][procesval];
   
    lampval = procesval;
    //Serial.println(lampval);
    dig2 = wheel[2][2];  dig3 = wheel[1][2]; dig4 = wheel[0][2]; dig1 = wheel[3][2];
     if (behavior == 2) {dig1 = 36;}
    lampita();
    nixisend();
    lampitb();
    dig1 =37; dig2 = 37; dig3 = 37; dig4 = 37;
    digitalWrite(led5, LOW);
  }

// Helper Function to light the proper key 
 void lampita() {
   digitalWrite(lamparray[lampval][0],0);
   digitalWrite(lamparray[lampval][1],0);
 }
 void lampitb(){
   digitalWrite(lamparray[lampval][0],1);
   digitalWrite(lamparray[lampval][1],1);  
 }

// Help Function to illuminate "Nixies" 
 void nixisend() {
 sixteenSegWrite(0, dig1);
 sixteenSegWrite(1, dig2);
 sixteenSegWrite(2, dig3);
 sixteenSegWrite(3, dig4);
 }

void marquee() {
  
 time = millis();
 if( mtime < time) {
   mtime = time + 400;
 mdex++;}
dig1 = data[behavior][mdex];
dig2 = data[behavior][mdex + 1];
dig3 = data[behavior][mdex + 2];
dig4 = data[behavior][mdex + 3];
if (mdex >= 35) {mdex = 0;}
nixisend();
 
}
// Function that actually turns on each of 17 appropriate segments on each "Nixie"
void sixteenSegWrite(int digit, int character) {
  digitalWrite(anode[digit],0);
  for (int index = 0; index < 17; index++) {
  digitalWrite(segment[index], segmentvals[character][index]);
  }
  delay(7);
  for (int index =0; index <= 16; index++) {
    digitalWrite(segment[index], 1);
  }
  digitalWrite(anode[digit], 1);
  }

  void done() {
    dig1 = 3; dig2 = 14; dig3 = 13; dig4 = 4;
    nixisend();
    dig1 =37; dig2 = 37; dig3 = 37; dig4 = 37;
    }
    
  void indexwheels() {
    if (behavior > 0){ if(rotorvals[wheel[1][0]-27][wheel[1][2]] >= 100) {windex1 = 1;if (behavior < 2) { windex2 = 1;}}}
   // Serial.print(rotorvals[wheel[0][0]-27][wheel[0][2]]);
   // Serial.print("    "); Serial.print(wheel[0][0]-27); Serial.print("    "); Serial.println(wheel[0][2]);
    if(rotorvals[wheel[0][0]-27][wheel[0][2]] >= 100) {windex1 = 1;}
    wheel[0][2]++; if(wheel[0][2] > 25) {wheel[0][2] = 0;}
    windex = 0;
    if(windex1 == 1){
    if(rotorvals[wheel[1][0]-27][wheel[1][2]] >= 100) {windex2 = 1;}
    wheel[1][2]++; if(wheel[1][2] > 25) {wheel[1][2] = 0;}}
    windex1 = 0;
    if(windex2 == 1){
    wheel[2][2]++; if(wheel[2][2] > 25) {wheel[2][2] = 0;}
      windex2 = 0; } 
  }
 
