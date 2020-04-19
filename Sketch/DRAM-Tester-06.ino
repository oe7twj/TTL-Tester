/*
  Code für DRAM Tester - by Slabbi
  C 2020 - Thomas Winkler 


  The circuit:
 * LCD RS pin to digital pin D13
 * LCD Enable pin to digital pin D14
 * LCD D4 pin to digital pin A3
 * LCD D5 pin to digital pin A2
 * LCD D6 pin to digital pin A1
 * LCD D7 pin to digital pin A0
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V


well tested chips:
14pin: 7400,7402,7408,7410,7414,7421,7430,7432,7474,7486,74125,74126,
       74164,
  (7404,7405  -> 7414)
  (74132 -> 7400)
16pin: 74138,74139,74157,74163,74165,74595
20pin: 74541,74573

  
*/

//
// IO definitions
//
#define O_POWER         52
#define I_SEL           50
#define I_OK            51


// size of chip (pins)
#define PINS            20
const int pinTab[] = {49,47,45,43,41,39,37,35,33,31,  30,32,34,36,38,40,42,44,46,-1};


#define DEBUG           0

//
// TEST SEQUENZ definitions
//
#define  TEST_SEQUENZ(p)     if(test_seq(p)) return 0
#define  PLAY_SEQUENZ(p)     play_seq(p)
typedef struct {
  char*     cmd;
  uint32_t  expected;
} probe_t;


// ============ TTL probes ============
probe_t p7400[] = {
  "7400: 4x NAND", 0,
  "HH-HH-L-HH-HH+", 0x3B1Bl,
  "LL-LL-L-LL-LL+", 0x24A4l,
  "HH-LL-L-HH-LL+", 0x2723l,
  "HL-LH-L-LL-HH+", 0x38B5L,
  "LL-HH-L-LH-HL+", 0x2E9Cl,
  "LH-HL-L-HL-LH+", 0x35AEl,
  NULL
};
probe_t p7402[] = {
  "7402: 4x NOR", 0,
  "-HH-HHLHH-HH-+", 0x2DB6,
  "-LL-HHLLL-LL-+", 0x3231,
  "-HL-LLLHL-HL-+", 0x248A,
  "-LH-HLLLH-LH-+", 0x2914,
  "-LL-LHLLL-LL-+", 0x3221,
  NULL
};
probe_t p7408[] = {
  "7408: 4x AND", 0,
  "HH-HH-L-HH-HH+", 0x3FBF,
  "LL-LL-L-LL-LL+", 0x2000,
  "HL-LL-L-HH-LH+", 0x3381,
  "LH-LH-L-LH-HL+", 0x2A12,
  "LL-HL-L-HL-HH+", 0x3D08,
  NULL
};
probe_t p7410[] = {
  "7410: 3x NAND3", 0,
  "HHHHH-L-HHH-H+", 0x371F,
  "HLHHL-L-HHL-H+", 0x3BAD,
  "LHHLH-L-HLH-H+", 0x3DB6,
  "HHLHH-L-LHH-L+", 0x2EBB,
  "LLLHL-L-HLL-L+", 0x29A8,
  NULL
};
probe_t p7414[] = {
  "7414: 6 Inverter", 0,          // same as 7404,7405,7406
  "H-H-L-L-L-L-H+",0x32A5,
  "L-L-L-L-L-L-L+",0x2AAA,
  "H-L-H-L-L-H-H+",0x3499,
  "L-H-H-L-H-H-L+",0x2D16,
  "H-H-H-L-H-H-H+",0x3515,
  NULL
};
probe_t p7421[] = {
  "7421: 2x AND4", 0,
  "HH-HH-L-HH-HH+", 0x3FBF,
  "LH-HH-L-HH-HL+", 0x2F1E,
  "HL-HH-L-HH-LH+", 0x371D,
  "HH-LH-L-HL-HH+", 0x3D17,
  "HH-HL-L-LH-HH+", 0x3E0F,
  "LL-LL-L-HL-LH+", 0x3504,
  NULL
};
probe_t p7430[] = {
  "7430: NAND-8", 0,
  "HHHHHHL-llHHl+", 0x3F3F,
  "HHHHHLL-llHHl+", 0x3F9F,
  "HHHHLHL-llHHl+", 0x3FAF,
  "HHHLHHL-llHHl+", 0x3FB7,
  "HHLHHHL-llHHl+", 0x3FBB,
  "HLHHHHL-llHHl+", 0x3FBD,
  "LHHHHHL-llHHl+", 0x3FBE,
  "HHHHHHL-llLHl+", 0x3BBF,
  "HHHHHHL-llHLl+", 0x37BF,
  NULL
};
probe_t p7432[] = {
  "7432: 4x OR", 0,
  "!LLlLL-L-HH-LH+", 0x3780,
  "!HLlLL-L-HH-LH+", 0x3785,
  "HL-LL-L-HH-LH+", 0x3785,
  "LH-LH-L-LH-HL+", 0x2EB6,
  "LL-HL-L-HL-HH+", 0x3DA8,
  "LL-LL-L-LL-LL+", 0x2000,
  "HH-HH-L-HH-HH+", 0x3FBF,
  NULL
};
probe_t p7474[] = {
  "7474:2xD-FlipFlop", 0,
  "Lhhh--L--Lhhh+", 0x3D2E,
  "!hhhL--L--hhhL+", 0x2E97,
  "!hhhh--L--hhhh+", 0x3E9F,
  "!hhLh--L--hLhh+", 0x3A9B,
  "!hhHh--L--hHhh+", 0x3F1F,
  "!hLHh--L--hHLh+", 0x371D,
  "!hLLh--L--hLLh+", 0x3319,
  "!hLHh--L--hHLh+", 0x36AD,
  NULL
};
probe_t p7486[] = {
  "7486: 4x ExOR", 0,
  "HH-HH-L-HH-HH+", 0x3B1B,
  "LL-LL-L-LL-LL+", 0x2000,
  "HH-LL-L-HH-LL+", 0x2303,
  "HL-LH-L-LL-HH+", 0x3835,
  "LL-HH-L-LH-HL+", 0x2E98,
  "LH-HL-L-HL-LH+", 0x35AE,
  NULL
};
probe_t p74125[] = {
  "74125:4x3ST-Buffer", 0,
  "!LLlLLlLlHHlLH+", 0x3780,
  "!HLlLL-L-HL-LL+", 0x2181,
  "LL-LH-L-LL-HL+", 0x2C30,
  NULL
};
probe_t p74126[] = {
  "74126:4x3ST-Buffer", 0,
  "!LLlLLlLlHHlLH+", 0x33A4,
  "!HLlLL-L-HL-LL+", 0x25A1,
  "LL-LH-L-LL-HL+", 0x2CB4,
  NULL
};
probe_t p74138[] = {
  "74138:3to8 decoder", 0,
  "HHHLLH-L-------+",0xFF27,
  "LLLLLH-L-------+",0xBF60,
  "HLLLLH-L-------+",0xDF61,
  "LHLLLH-L-------+",0xEF62,
  "LLHLLH-L-------+",0xFB64,
  "HHHHLH-L-------+",0xFF6F,
  "LLLLHH-L-------+",0xFF70,
  "HHHLLL-L-------+",0xFF47,
  NULL
};
probe_t p74139[] = {
  "74139:2x2to4 decoder", 0,
  "LLL----L----HHL+",0xBE70,
  "LHL----L----HLL+",0x9D6A,
  "LLH----L----LHL+",0xAB5C,
  "LHH----L----LLL+",0x873E,
  "HHH----L----LLH+",0xCF7F,
  NULL
};
probe_t p74157[] = {
  "74157:4x Multiplexer", 0,
  "LLH-LH-L-LH-LHL+", 0xAD24l,
  "LLH-LH-L-LH-LHH+", 0xE424l,
  "HLH-LH-L-LH-LHL+", 0xA46Dl,
  "HHL-HL-L-HL-HLL+", 0x9B13l,
  "LHL-LH-L-LH-HLH+", 0xD422l,
  NULL
};
probe_t p74163[] = {
  "74163:4-Bit Counter Synchronous Presettable", 0,
  "?HL-----LL------+", 0x0,
  "!HH-----LL------+", 0xFE7F,
  "?HLLHLH-LL------+", 0x0,
  "!HHLHLH-LL------+", 0x966B,
  "?HLLHLH-LH------+", 0x0,
  "!LLLHLH-LH------+", 0x9768,
  "!LHLHLH-LH------+", 0x836A,
  "!HLLHLHHLHH-----+", 0x8369,
  "!HHLHLHHLHH-----+", 0xA36B,
  "!HLLHLHHLHH-----+", 0xA369,
  "!HLHHLHHLHH-----+", 0xA36D,
  NULL
};
probe_t p74164[] = {
  "74164:8bit shift register serial-in parallel-out", 0,
  "!hh----LLL----+",0x2003,
  "!HH----LLH----+",0x2103,
  "!HH----LHH----+",0x2187,
  "!HH----LLH----+",0x2107,
  "!HH----LHH----+",0x218F,
  "!HH----LLH----+",0x210F,
  "!HH----LHH----+",0x219F,
  "!LH----LLH----+",0x211E,
  "!LH----LHH----+",0x21BA,
  "!HL----LLH----+",0x2139,
  "!HL----LHH----+",0x23B1,
  "!LL----LLH----+",0x2330,
  "!LL----LHH----+",0x27A0,
  "!HH----LLH----+",0x2723,
  "!HH----LHH----+",0x2F87,
  "!HH----LLH----+",0x2F07,
  "!HH----LHH----+",0x3D8F,
  NULL
};
probe_t p74165[] = {
  "74165:8bit shift register parallel-in serial-out", 0,
  "!LLhhhh-L-LhhhhL+",0xBD3C,
  "!LLLHHL-L-LHLLHL+",0xA458,
  "!HLLHHL-L-LHLLHL+",0xA459,
  "!HHLHHL-L-LHLLHL+",0xA51B,
  "!HLLHHL-L-LHLLHL+",0xA519,
  "!HHLHHL-L-LHLLHL+",0xA51B,
  "!HLLHHL-L-LHLLHL+",0xA519,
  "!HLLHHL-L-LHLLHH+",0xE459,
  "!HLLHHL-L-LHLLHL+",0xA459,
  "!HLLHHL-L-LHLLHH+",0xE519,
  "!HLLHHL-L-LHLLHL+",0xA519,
  "!HLLHHL-L-LHLLHH+",0xE459,
  NULL
};
probe_t p74541[] = {
  "74541:Octal buffer/line driver; 3-state", 0,
  "LLhhhhhhhL--------L+",0x9FDFC,
  "LLhLhhhhhL--------L+",0x97DF4,
  "LHLHLLLLLL--------L+",0xA800A,
  "HHLHLLLLLL--------L+",0xBFC0B,
  "HHLHLLLLLL---L--L-L+",0xADC0B,
  "HHLHLLLLLL----LL--H+",0xF3C0B,
  "LhhhhhhhhL--------H+",0xFFDFE,
  "LHLHLHLHLL--------L+",0xAA8AA,
  NULL
};
probe_t p74573[] = {
  "74573:Octal D-type transparent latch; 3-state", 0,
  "LhhLhhhhhLh--------+",0xEFDF6,
  "LhhhhLLhhLh--------+",0xF9D9E,
  "LLLhLhhLLLh--------+",0x96468,
  "HLLhLhhLLLh--------+",0xFFC69,
  "HhhhhhhhhLh--LL-LL-+",0xC9DFF,
  "HLHLHLHLHLh--------+",0xFFD55,
  "!LLHLHLHLHLh--------+",0xAAD54,
  "!LLHLHLHLHLL--------+",0xAA954,
  "LLLLLLLLLLL--------+",0xAA800,
  NULL
};
probe_t p74595[] = {
  "74595:8bit shift register with latch 3-state", 0,
  "?-------L-LLLLh-+",0x0,
  "!-------L-HLHLL-+",0x8A00,       // latch clock!
  "!-------L-HHHLL-+",0x8E00,       // shift clock
  "!-------L-HLHLH-+",0xAA00,   
  "!-------L-HHHLH-+",0xAE00,       // shift clock
  "!-------L-HLHLH-+",0xAA00,   
  "!-------L-HHHLH-+",0xAE00,       // shift clock
  "!-------L-HLLLH-+",0xA200,   
  "!-------L-HHHLH-+",0xEE03,       // shift & latch clock
  "!-------L-HLLLL-+",0xC203,   
  "!-------L-HHHLH-+",0xAE07,       // shift & latch clock
  "!-------L-HLLLL-+",0x8207,   
  "!-------L-HHHLH-+",0xAE0E,       // shift & latch clock
  "!-------L-HHHHH-+",0xFE7F, 
  NULL
};





// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 13, en = 12, d4 = A3, d5 = A2, d6 = A1, d7 = A0;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);



#define LINE_BUF_SIZE 128   //Maximum input string length
char line[LINE_BUF_SIZE];




// ==================
// setup function
void setup() 
{
  pinMode(O_POWER, OUTPUT);
  digitalWrite(O_POWER, HIGH);       // turn off Poewer for Chip

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("## TTL Tester ##");

  Serial.begin(115200);
  Serial.println("### TTL Tester ###  (Slabbi Hardware rev 4.1)");

  setZIFin();

  line[0] = 0;
}




// ==================
// write one IO port
void writePin(char cmd, int io) 
{
  int pin;

  if((pin = pinTab[io]) < 0)
    cmd = '+';
    
  switch(cmd)
  {
    case 'l':
      pinMode(pin, INPUT);
      digitalWrite(pin, LOW);
      break;
    case 'h':
      pinMode(pin, INPUT);
      digitalWrite(pin, HIGH);
      break;
    case 'L':
      pinMode(pin, OUTPUT);
      digitalWrite(pin, LOW);
      break;
    case 'H':
      pinMode(pin, OUTPUT);
      digitalWrite(pin, HIGH);
      break;
    case '+':
      break;
    default:
      Serial.print('-');     
      pinMode(pin, INPUT);
      digitalWrite(pin, HIGH);
      return;
  }
  Serial.print(cmd);     
}


// ==================
// read one IO port
int readPin(int io) 
{
  int pin = pinTab[io];
  if(pin < 0 ? 1 : digitalRead(pin))
  {
    Serial.print("1");
    return 1;
  }
  Serial.print("0");
  return 0;
}


// ==================
// write IO ports of ZIF socket
void writeIO(char *cmd, int pins) 
{
  uint32_t rc = 0l, b;
  int pin;
  char c;
  char cExp;
  bool f;

  //Serial.println(cmd);
  for(int i=0; i<pins; ++i)
  {
    writePin(*(cmd +i), i);
  }
  for(int i=0; i<pins; ++i)
  {
    writePin(*(cmd +i +pins), PINS -pins +i);
  }
  Serial.println();     
}


// ==================
// read IO ports of ZIF socket
uint32_t readIO(int hpins) 
{
  uint32_t rc = 0l, b;
  int ii;
  bool f;

  Serial.print("<<< ");
   //for(int i=0, b=1l; i<hpins; ++i, b=(b<<1l))
  for(int i=0; i<hpins; ++i)
  {
    if(readPin(i))
      rc |= (1l << i);
  }
  for(int i=0, ii=PINS-hpins; i<hpins; ++i, ++ii)
  {
    if(readPin(ii))
      rc |= (1l << (i+hpins));
  }
  Serial.print(" -- ");
  Serial.println(rc, 16);
  return rc;
}




// ==================
// set IO ports of ZIF socket
void setZIFin() 
{
  // all IO to input mode
  Serial.print("--- ");
  for(int i=0; i<PINS; ++i)
  {
    writePin('-', i);
  }
  Serial.println();
}



// ==================
// set IO ports of ZIF socket
uint32_t setZIF(char *cmd) 
{
  uint32_t rc = 0l, b;
  int pin;
  int hpins;
  char c;
  char cExp;
  bool f;

  hpins = strlen(cmd) /2;
/*
  Serial.print("pins: ");
  for(int i=0; i<PINS; ++i)
  {
    pin = pinTab[i];
    Serial.print(pin);
    Serial.print(";");
  }  
  Serial.println();
*/
  
  Serial.print(">>> ");
  writeIO(cmd, hpins);

  rc = readIO(hpins);

  setZIFin();
  return rc;
}




// ==================
// set IO ports of ZIF socket
uint32_t debugZIF(char *cmd) 
{
  uint32_t rc = 0l, b;
  int hpins;

  hpins = strlen(cmd) /2;

  Serial.print("!!! ");
  writeIO(cmd, hpins);

  rc = readIO(hpins);
  return rc;
}



// ==================
// send command
uint32_t sendCmd(char *cmd) 
{
  if(*cmd == '!')
    return debugZIF(cmd +1);

  if(*cmd == '?')
  {
    debugZIF(cmd +1);
    return 0;
  }

  return setZIF(cmd);
}



// ==================
// test sequence
int found(char *s) 
{
    int l = strlen(s);
    
    lcd.setCursor(0,1);
    lcd.print(s);
    while(l++ <16)
      lcd.print(" ");
    Serial.print("found chip: ");
    Serial.println(s);
    return 0;
}



// ==================
// test sequence
int test_seq(probe_t p[]) 
{
  uint32_t rc;

  Serial.print("+--+ ");
  Serial.println(p[0].cmd);

  for(int i=1; p[i].cmd != NULL; ++i)
  {
    if((rc = sendCmd(p[i].cmd)) != p[i].expected) return 0;    
  }
  found(p[0].cmd);
  return 1;
}
void play_seq(probe_t p[]) 
{
  uint32_t rc;

  Serial.print("===== test sequence: ");
  Serial.println(p[0].cmd);
  for(int i=1; p[i].cmd != NULL; ++i)
  {
    sendCmd(p[i].cmd);    
  }
  Serial.println("=====================");
}




// ==================
// test sequence 14 Pin
int test_chip_14() 
{
  uint32_t rc;

  //Serial.println("testing 14 pin chip ...");

  rc = setZIF("hhhhhhLhhhhhh+");
  if(rc == 0x3B1B)    TEST_SEQUENZ(p7400);
  if(rc == 0x2DB6)    TEST_SEQUENZ(p7402);
  if(rc == 0x3FBF)    TEST_SEQUENZ(p7408);
  if(rc == 0x371F)    TEST_SEQUENZ(p7410);
  if(rc == 0x3515)    TEST_SEQUENZ(p7414);  //  if(test_7414()) return 0;
  if(rc == 0x3FBF)    TEST_SEQUENZ(p7421);
  if(rc == 0x3F3F)    TEST_SEQUENZ(p7430);
  if(rc == 0x3FBF)    TEST_SEQUENZ(p7432);
  if(rc == 0x3F1F)    TEST_SEQUENZ(p7474);
  if(rc == 0x3B1B)    TEST_SEQUENZ(p7486);
  if(rc == 0x3FBF)    TEST_SEQUENZ(p74125);
  if(rc == 0x3FBF)    TEST_SEQUENZ(p74126);
  
  //PLAY_SEQUENZ(p7486);

  return 1;
}

// ==================
// test sequence other
int test_chip_other() 
{
  uint32_t rc;
  uint32_t rcLL;
  uint32_t rcHH;

  rc = setZIF("hhhhhhLhhhhhhhh+");      // test 14 pin
  rcHH = setZIF("hhhhhhhLhhhhhhh+");      // FF7F
  if(rcHH == 0xFF7F && rc != 0xFF3F)
  {
    // suppose 14 pin
    if(test_chip_14() == 0) return 0;
  }

  rcLL = setZIF("lllllllLlllllll+");      // 

  //if(rc == 0x3B1B)    TEST_SEQUENZ(p7400);
  TEST_SEQUENZ(p74157);
  TEST_SEQUENZ(p74138);
  TEST_SEQUENZ(p74139);
  TEST_SEQUENZ(p74163);
  TEST_SEQUENZ(p74164);
  TEST_SEQUENZ(p74165);
  TEST_SEQUENZ(p74541);
  TEST_SEQUENZ(p74573);
  TEST_SEQUENZ(p74595);

  return 1;
}

// ==================
// test pin size 
void test_chip() 
{
  lcd.setCursor(0,1);
  lcd.print("testing chip ...");
  Serial.println("testing chip ...");

  //PLAY_SEQUENZ(p74573);
  while(1)
  {
    if(test_chip_other()) break; return;
    if(test_chip_14()) break; return;
    break;
  }
  
  lcd.setCursor(0,1);
  lcd.print("unknown chip    ");
  Serial.println("-- unknown chip --");
}


// ==================
// read line from serial
//
void read_line(){
    String line_string;
 
    if(Serial.available()){
        line_string = Serial.readStringUntil("\n");
        if(line_string.length() < LINE_BUF_SIZE)
        {
          int l;
          line_string.toCharArray(line, LINE_BUF_SIZE);
          l = strlen(line);
          if(line[0] == '<')
          {
            readIO(PINS /2);
          }
          else if(l > 14 && (l&1) == 0)
          {
            line[l -1] = 0;
            if(line[0] == '>')
            {
              digitalWrite(O_POWER, LOW);        // turn on Poewer for Chip
              delay(50);                   
              setZIF(line +1);
              digitalWrite(O_POWER, HIGH);       // turn off Poewer for Chip
            }
            else if(line[0] == '!')
            {
              digitalWrite(O_POWER, LOW);        // turn on Poewer for Chip
              debugZIF(line +1);
            }
            else
              Serial.println("## command starts with '>', '<' or '!'");
          }
          else
          {
            Serial.print("## send command has a length of 15,17,19,21");
            Serial.print(PINS +1);
            Serial.print(" chars!    (has ");
            Serial.print(strlen(line)-1);
            Serial.println(")");
          }
        }
        else{
//          Serial.println("Input string too long.");
        }
    }
}


// ==================
// loop function
void loop() 
{
  static byte l_Sel=0;
  static byte l_Ok=0;

  if(digitalRead(I_SEL))
  {
    if(l_Sel == 0)
    {
      l_Sel++;
      lcd.setCursor(0,1);
      lcd.print("-SEL- ");
      Serial.println("SEL Taste");
    }
  }
  else l_Sel = 0;
  
  if(digitalRead(I_OK))
  {
    if(l_Ok == 0)
    {
      l_Ok++;
      lcd.setCursor(0,1);
      lcd.print("-OK-  ");
      Serial.println("OK Taste");
      digitalWrite(O_POWER, LOW);        // turn on Poewer for Chip
      delay(50);                   
      test_chip();
      digitalWrite(O_POWER, HIGH);       // turn off Poewer for Chip
    }
  }
  else l_Ok = 0;

  read_line();
}
