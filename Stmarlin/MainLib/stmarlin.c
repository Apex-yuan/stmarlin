#include "stmarlin.h"

///ͷ�ļ��İ���˳�򣺰��ȶ��Ե�����˳�����У���Խ�ȶ���Խ�ײ㣬�����޸ģ���ͷ�ļ�Խ���������
/* STMARLIN */
#include "planner.h"
#include "stepper.h"
#include "temperature.h"
#include "serial.h"
#include "language.h"
#include "ConfigurationStore.h"
#include "motion_control.h"
/* CHIP */
#include "timer.h"
#include "delay.h"
#include "usart.h"

/*  // G/M����Ľ���
//Implemented Codes
//-------------------
// G0  -> G1
// G1  - Coordinated Movement X Y Z E
// G2  - CW ARC
// G3  - CCW ARC
// G4  - Dwell S<seconds> or P<milliseconds>
// G10 - retract filament according to settings of M207
// G11 - retract recover filament according to settings of M208
// G28 - Home all Axis
// G90 - Use Absolute Coordinates
// G91 - Use Relative Coordinates
// G92 - Set current position to cordinates given

//RepRap M Codes
// M0   - Unconditional stop - Wait for user to press a button on the LCD (Only if ULTRA_LCD is enabled)
// M1   - Same as M0
// M104 - Set extruder target temp
// M105 - Read current temp
// M106 - Fan on
// M107 - Fan off
// M109 - Wait for extruder current temp to reach target temp.
// M114 - Display current position

//Custom M Codes
// M15  - update flash data (font data ; icon data and so on)
// M16  - screen_adjust
// M17  - Enable/Power all stepper motors 
// M18  - Disable all stepper motors; same as M84
// M20  - List SD card
// M21  - Init SD card
// M22  - Release SD card
// M23  - Select SD file (M23 filename.g)
// M24  - Start/resume SD print
// M25  - Pause SD print
// M26  - Set SD position in bytes (M26 S12345)
// M27  - Report SD print status
// M28  - Start SD write (M28 filename.g)
// M29  - Stop SD write
// M30  - Delete file from SD (M30 filename.g)
// M31  - Output time since last M109 or SD card start to serial
// M42  - Change pin status via gcode Use M42 Px Sy to set pin x to value y, when omitting Px the onboard led will be used.
// M80  - Turn on Power Supply
// M81  - Turn off Power Supply
// M82  - Set E codes absolute (default)
// M83  - Set E codes relative while in Absolute Coordinates (G90) mode
// M84  - Disable steppers until next move,
//        or use S<seconds> to specify an inactivity timeout, after which the steppers will be disabled.  S0 to disable the timeout.
// M85  - Set inactivity shutdown timer with parameter S<seconds>. To disable set zero (default)
// M92  - Set axis_steps_per_unit - same syntax as G92
// M114 - Output current position to serial port
// M115 - Capabilities string
// M117 - display message
// M119 - Output Endstop status to serial port
// M126 - Solenoid Air Valve Open (BariCUDA support by jmil)
// M127 - Solenoid Air Valve Closed (BariCUDA vent to atmospheric pressure by jmil)
// M128 - EtoP Open (BariCUDA EtoP = electricity to air pressure transducer by jmil)
// M129 - EtoP Closed (BariCUDA EtoP = electricity to air pressure transducer by jmil)
// M140 - Set bed target temp
// M190 - Wait for bed current temp to reach target temp.
// M200 - Set filament diameter
// M201 - Set max acceleration in units/s^2 for print moves (M201 X1000 Y1000)
// M202 - Set max acceleration in units/s^2 for travel moves (M202 X1000 Y1000) Unused in marlin_main!!
// M203 - Set maximum feedrate that your machine can sustain (M203 X200 Y200 Z300 E10000) in mm/sec
// M204 - Set default acceleration: S normal moves T filament only moves (M204 S3000 T7000) im mm/sec^2  also sets minimum segment time in ms (B20000) to prevent buffer underruns and M20 minimum feedrate
// M205 -  advanced settings:  minimum travel speed S=while printing T=travel only,  B=minimum segment time X= maximum xy jerk, Z=maximum Z jerk, E=maximum E jerk
// M206 - set additional homeing offset
// M207 - set retract length S[positive mm] F[feedrate mm/sec] Z[additional zlift/hop]
// M208 - set recover=unretract length S[positive mm surplus to the M207 S*] F[feedrate mm/sec]
// M209 - S<1=true/0=false> enable automatic retract detect if the slicer did not support G10/11: every normal extrude-only move will be classified as retract depending on the direction.
// M218 - set hotend offset (in mm): T<extruder_number> X<offset_on_X> Y<offset_on_Y>
// M220 S<factor in percent>- set speed factor override percentage
// M221 S<factor in percent>- set extrude factor override percentage
// M240 - Trigger a camera to take a photograph
// M280 - set servo position absolute. P: servo index, S: angle or microseconds
// M300 - Play beepsound S<frequency Hz> P<duration ms>
// M301 - Set PID parameters P I and D
// M302 - Allow cold extrudes
// M303 - PID relay autotune S<temperature> sets the target temperature. (default target temperature = 150C)
// M304 - Set bed PID parameters P I and D
// M400 - Finish all moves
// M500 - stores paramters in EEPROM
// M501 - reads parameters from EEPROM (if you need reset them after you changed them temporarily).
// M502 - reverts to the default "factory settings".  You still need to store them in EEPROM afterwards if you want to.
// M503 - print the current settings (from memory not from eeprom)
// M540 - Use S[0|1] to enable or disable the stop SD card print on endstop hit (requires ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED)
// M600 - Pause for filament change X[pos] Y[pos] Z[relative lift] E[initial retract] L[later retract distance for removal]
// M907 - Set digital trimpot motor current using axis codes.
// M908 - Control digital trimpot directly.
// M350 - Set microstepping mode.
// M351 - Toggle MS1 MS2 pins directly.
// M928 - Start SD logging (M928 filename.g) - ended by M29
// M999 - Restart after being stopped by error
*/

//Stepper Movement Variables


//===========================================================================
//=============================public variables=============================
//===========================================================================
float homing_feedrate[] = HOMING_FEEDRATE;
bool axis_relative_modes[] = AXIS_RELATIVE_MODES;
int feedmultiply=100; //100->1 200->2
int saved_feedmultiply;
int extrudemultiply=100; //100->1 200->2
float current_position[NUM_AXIS] = { 0.0, 0.0, 0.0, 0.0 };

float stop_position[NUM_AXIS] = { 0.0, 0.0, 0.0, 0.0 };

float add_homeing[3]={0,0,0};
float min_pos[3] = { X_MIN_POS, Y_MIN_POS, Z_MIN_POS };  //�����ӡ����С
float max_pos[3] = { X_MAX_POS, Y_MAX_POS, Z_MAX_POS };
bool axis_known_position[3] = {false, false, false};

static const float  base_min_pos[3] = { X_MIN_POS, Y_MIN_POS, Z_MIN_POS };  
static float  base_max_pos[3] = { X_MAX_POS, Y_MAX_POS, Z_MAX_POS };
static const float  base_home_pos[3] = { X_HOME_POS, Y_HOME_POS, Z_HOME_POS };
static float  max_length[3] = { X_MAX_LENGTH, Y_MAX_LENGTH, Z_MAX_LENGTH };
static const float  home_retract_mm[3] = { X_HOME_RETRACT_MM, Y_HOME_RETRACT_MM, Z_HOME_RETRACT_MM };
static const signed char home_dir[3] = { X_HOME_DIR, Y_HOME_DIR, Z_HOME_DIR }; //���巽��

// Extruder offset, only in XY plane
#if EXTRUDERS > 1
float extruder_offset[2][EXTRUDERS] = {
#if defined(EXTRUDER_OFFSET_X) && defined(EXTRUDER_OFFSET_Y)
  EXTRUDER_OFFSET_X, EXTRUDER_OFFSET_Y
#endif
};
#endif
uint8_t active_extruder = 0;
int fanSpeed=0;
#ifdef BARICUDA  //���ǻ��
int ValvePressure=0;
int EtoPPressure=0;
#endif

#ifdef FWRETRACT //���ǻ��
  bool autoretract_enabled=true;
  bool retracted=false;
  float retract_length=3, retract_feedrate=17*60, retract_zlift=0.8;
  float retract_recover_length=0, retract_recover_feedrate=8*60;
#endif
//===========================================================================
//=============================private variables=============================
//===========================================================================
const char axis_codes[NUM_AXIS] = {'X', 'Y', 'Z', 'E'};  //�����
 float destination[NUM_AXIS] = {  0.0, 0.0, 0.0, 0.0};
float offset[3] = {0.0, 0.0, 0.0};
static bool home_all_axis = true;
static float feedrate = 1500.0, next_feedrate, saved_feedrate;
static long gcode_N, gcode_LastN, Stopped_gcode_LastN = 0;

volatile static bool relative_mode = false;  //Determines Absolute or Relative Coordinates

static char cmdbuffer[BUFSIZE][MAX_CMD_SIZE];
volatile static bool fromsd[BUFSIZE];
static int bufindr = 0;
static int bufindw = 0;
static int buflen = 0;
//static int i = 0;
static char serial_char;
static int serial_count = 0;
static bool comment_mode = false;
static char *strchr_pointer; // just a pointer to find chars in the cmd string like X, Y, Z, E, etc

//Inactivity shutdown variables
static unsigned long previous_millis_cmd = 0;
static unsigned long max_inactive_time = 0;
static unsigned long stepper_inactive_time = DEFAULT_STEPPER_DEACTIVE_TIME*1000L;

unsigned long starttime=0;
unsigned long stoptime=0;
static uint8_t tmp_extruder;


bool Stopped=false;


//===========================================================================
//=============================ROUTINES=============================
//===========================================================================

void get_arc_coordinates(void);
bool setTargetedHotend(int code);


void enquecommand(const char *cmd) {
  if(buflen < BUFSIZE) {
    //this is dangerous if a mixing of serial and this happsens
    strcpy(&(cmdbuffer[bufindw][0]),cmd);
    SERIAL_ECHO_START;
    printf("enqueing \"%s\"",cmdbuffer[bufindw]);
    bufindw= (bufindw + 1)%BUFSIZE;
    buflen += 1;
  }
}

void setup(void) {	
	printf("start\n");
  SERIAL_ECHO_START;
  printf(MSG_STMARLIN); //��ӡStmarlin
  printf(VERSION_STRING); //��ӡ�汾��
  printf("\n");
  #ifdef STRING_VERSION_CONFIG_H
    #ifdef STRING_CONFIG_H_AUTHOR
      SERIAL_ECHO_START;
      printf(MSG_CONFIGURATION_VER);
      printf(STRING_VERSION_CONFIG_H);
      printf(MSG_AUTHOR);
      printf(STRING_CONFIG_H_AUTHOR);
      printf("Compiled: ");
      printf(__DATE__);
      printf("\n");
    #endif
  #endif
  SERIAL_ECHO_START;
  //printf(MSG_FREE_MEMORY);
  //printf(freeMemory());
  printf(MSG_PLANNER_BUFFER_BYTES);
  printf("%d",(int)sizeof(block_t)*BLOCK_BUFFER_SIZE);
  printf("\n");
  for(int8_t i = 0; i < BUFSIZE; i++) {
    fromsd[i] = false;
  }
  // loads data from EEPROM if available else uses defaults (and resets step acceleration rate)
  //Config_RetrieveSettings();
  Config_ResetDefault();
  Config_PrintSettings();
  
  st_init();    // Initialize stepper, this enables interrupts
  tp_init();    // Initialize temperature loop
  plan_init();  // Initialize planner;
	//lcd_init();
}

//����Marlin 1.0.0�汾��ʽ 2018/2/2
void loop(void) {
	if(buflen < (BUFSIZE-1)) { 
			 get_command();
  }
  
  #ifdef SDSUPPORT
    if(SD_CD) {
      card.cardOK = false;
    }
  card_checkautostart(false); 
  #endif
    
  if(buflen) 
  {
    #ifdef SDSUPPORT						  
      if(card.saving) {	 
        if(strstr(cmdbuffer[bufindr], PSTR("M29")) == NULL) {
          card_write_command(cmdbuffer[bufindr]);
          printf(MSG_OK);
        }
        else {
          card_closefile();
          printf(MSG_FILE_SAVED);
          printf("\n");
        }         
      }
      else {	
        process_commands();
      }
    #else
      process_commands();  
    #endif //SDSUPPORT	
      
    buflen = (buflen-1);
    bufindr = (bufindr + 1)%BUFSIZE;
  } 
  //check heater every n milliseconds
  manage_heater();
  manage_inactivity();
  checkHitEndstops();
  //lcd_update();
}

//����Marlin 1.0.0�汾��ʽ 2018/2/2
void get_command(void)  //��ȡ������Ϣ
{ 
  while( MYSERIAL_available() > 0  && buflen < BUFSIZE)
  {	
    serial_char = MYSERIAL_read(); //�Ӵ��ڻ�������ȡ�����ַ�
    
    if(serial_char == '\n' ||
       serial_char == '\r' ||
       (serial_char == ':' && comment_mode == false) ||
       serial_count >= (MAX_CMD_SIZE - 1) ) {
      if(!serial_count) {
        comment_mode = false; //for new command
        return;
      }
      cmdbuffer[bufindw][serial_count] = 0; //terminate string
      if(!comment_mode)  //comment_mode ע�ͣ��������ע��
	    {
        comment_mode = false; //for new command
        fromsd[bufindw] = false;
        if(strchr(cmdbuffer[bufindw], 'N') != NULL) {  //������ַ��������ַ���N��NΪ���� //extern char *strchr(const char *s,char c);���ܣ������ַ���s���״γ����ַ�c��λ�� ˵���������״γ���c��λ�õ�ָ�룬���s�в�����c�򷵻�NULL                                                             
          strchr_pointer = strchr(cmdbuffer[bufindw], 'N'); //�����ַ���N�����ַ�������ָ��
          gcode_N = (strtol(&cmdbuffer[bufindw][strchr_pointer - cmdbuffer[bufindw] + 1], NULL, 10)); //��ȡN��������� //long int strtol (const char* str, char** endptr, int base);���ַ���ת��Ϊ��������(long) ˵����str ΪҪת�����ַ�����endstr Ϊ��һ������ת�����ַ���ָ�룬base Ϊ�ַ��� str �����õĽ��ơ�
          if(gcode_N != gcode_LastN+1 && (strstr(cmdbuffer[bufindw], PSTR("M110")) == NULL) ) {  //�����ֲ�������һ�����ּ�һ�������ַ�����û�С�M110���򷵻ؿ�ָ�롣 //extern char *strstr(char *str1, char *str2); �ҳ�str2�ַ�����str1�ַ����е�һ�γ��ֵ�λ�ã�������str2�Ĵ���������˵�������ظ�λ�õ�ָ�룬���Ҳ��������ؿ�ָ�롣                                                                                   
            SERIAL_ERROR_START;  //���ڴ�ӡ��ERROR��
            printf(MSG_ERR_LINE_NO);  //��ӡ�кŴ�����Ϣ
            printf("%ld\n",gcode_LastN);  //��ӡ��һ�е��к�
            FlushSerialRequestResend();  //���������Ϣ���·���
            serial_count = 0;  //�������ַ�����
            return;
          }
          
          if(strchr(cmdbuffer[bufindw], '*') != NULL) {  //��ǰ�ַ������С�*��// *��Ϊ�����  
            uint8_t checksum = 0;
            uint8_t count = 0;
            
            while(cmdbuffer[bufindw][count] != '*') checksum = checksum^cmdbuffer[bufindw][count++];  //������������
            strchr_pointer = strchr(cmdbuffer[bufindw], '*');  //���ء�*�����ַ����е�λ��         
            if((int)(strtod(&cmdbuffer[bufindw][strchr_pointer - cmdbuffer[bufindw] + 1], NULL)) != checksum) {  //*�ź������ֵ�����ڱ�������ֵ //���� strtod() �������ַ���ת����˫���ȸ�����(double)				                                                               
              SERIAL_ERROR_START;  //���ڴ�ӡ��ERROR��
              printf(MSG_ERR_CHECKSUM_MISMATCH); //��ӡ����벻ƥ����Ϣ        
              printf("%ld\n",gcode_LastN);  //��ӡ��һ�е��к�
              FlushSerialRequestResend(); //���������Ϣ���·���
              serial_count = 0;
              return;
            }
            //if no errors, continue parsing //���û�д��������﷨����
          }
          else
          {
            SERIAL_ERROR_START;  //���ڴ�ӡ��ERROR��
            printf(MSG_ERR_NO_CHECKSUM);  //��ӡ ������û�б����Ĵ�����Ϣ
            printf("%ld",gcode_LastN);  //��ӡ��һ�е�����
            FlushSerialRequestResend();  //���������Ϣ���·���
            serial_count = 0;
            return;
          }
          gcode_LastN = gcode_N;  //��ֵ����¼������ʷֵ
          //if no errors, continue parsing
        }
        else {  // if we don't receive 'N' but still see '*'//���û���յ����롮N��������Ȼ�յ��˱���롮*��
          if((strchr(cmdbuffer[bufindw], '*') != NULL)) {  //��ǰ�ַ������С�*��    
            SERIAL_ERROR_START;  //���ڴ�ӡ��ERROR��
            printf(MSG_ERR_NO_LINENUMBER_WITH_CHECKSUM);  //��ӡû�������б����Ĵ�����Ϣ
            printf("%ld\n",gcode_LastN);
            serial_count = 0;
            return;
          }
        }
        if((strchr(cmdbuffer[bufindw], 'G') != NULL)) //��ǰ�ַ��������ַ���G��
		    {
          strchr_pointer = strchr(cmdbuffer[bufindw], 'G');  //�����ַ�G����λ�õ�ָ��
          switch((int)((strtod(&cmdbuffer[bufindw][strchr_pointer - cmdbuffer[bufindw] + 1], NULL)))) {  //��ȡ�ַ�G���������						
							case 0:
							case 1:
							case 2:
							case 3:
								if(Stopped == false) { // If printer is stopped by an error the G[0-3] codes are ignored. //�����ӡ���������ֹͣ״̬��G[0-3]����ᱻ���Ե���
                  #ifdef SDSUPPORT
                    if(card.saving)
                      break;
							    #endif //SDSUPPORT
									printf(MSG_OK);  //��ӡ��OK��
									printf("\n");
								}
								else {
									printf(MSG_ERR_STOPPED);  //��ӡ ���ڴ���ֹͣ����Ϣ "Printer stopped due to errors. Fix the error and use M999 to restart. (Temperature is reset. Set it after restarting)"
								  //LCD_MESSAGEPGM(MSG_STOPPED);  //lcd����ʾֹͣ��Ϣ
                }
								break;
							default:
								break;
             }
        }
        bufindw = (bufindw + 1)%BUFSIZE;
        buflen += 1;
      }
      serial_count = 0; //reset buffer  //��λ������
    }
    else {
      if(serial_char == ';') comment_mode = true; //���ڶ�ȡ���ֺ�,�ֺź���Ϊע����Ϣ
      if(!comment_mode) cmdbuffer[bufindw][serial_count++] = serial_char; //�������ע�����ݣ���������Ϣ�洢���������
    }
  }
  #ifdef SUPPORTSD
  /* ��SD����ȡ���� */
  #endif
}

float code_value(void)  //��strchr_pointerָ�����ַ����е����ְ�ʮ������ת��Ϊ��ֵ
{
  return (strtod(&cmdbuffer[bufindr][strchr_pointer - cmdbuffer[bufindr] + 1], NULL));
}

long code_value_long(void)  //��strchr_pointerָ�����ַ����е����ְ�ʮ������ת��Ϊ˫������ֵ
{
  return (strtol(&cmdbuffer[bufindr][strchr_pointer - cmdbuffer[bufindr] + 1], NULL, 10));
}

bool code_seen(char code)  //����ַ���code�����ҵ�������ֵΪtrue
{
  strchr_pointer = strchr(cmdbuffer[bufindr], code);
  return (strchr_pointer != NULL);  //Return True if a character was found
}


static void axis_is_at_home(int axis)  //�趨��ĵ�ǰλ��Ϊԭ��λ�ã�֮���ԭ���λ�ã�
{
  current_position[axis] = base_home_pos[axis] + add_homeing[axis];
  min_pos[axis] =          base_min_pos[axis] + add_homeing[axis];
  max_pos[axis] =          base_max_pos[axis] + add_homeing[axis];
}

static void homeaxis(int axis)  //�������ԭ��
{
  #define HOMEAXIS_DO(LETTER) (( LETTER##_HOME_DIR==-1) || (LETTER##_HOME_DIR==1))
  
  if(axis==X_AXIS ? HOMEAXIS_DO(X) :
     axis==Y_AXIS ? HOMEAXIS_DO(Y) :
     axis==Z_AXIS ? HOMEAXIS_DO(Z) : 
     0) {
    ///��ǰ����1.5��������󳤶ȵľ�����ԭ���ƶ�����������λ���ػ�ֹͣ   
    current_position[axis] = 0;
    plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
    destination[axis] = 1.5 * max_length[axis] * home_dir[axis];
    feedrate = homing_feedrate[axis];
    plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate/60, active_extruder);
    st_synchronize();

    //��ǰ����ԭ�㷴�����ƶ�home_retract_mm[axis]�ľ���   
    current_position[axis] = 0;
    plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
    destination[axis] = -home_retract_mm[axis] * home_dir[axis];
    plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate/60, active_extruder);
    st_synchronize();

    //��ǰ����ԭ���ƶ� 2*home_retract_mm[axis]�ľ��룬��������λ���ػ�ֹͣ�˶�  
    destination[axis] = 2*home_retract_mm[axis] * home_dir[axis];
    feedrate = homing_feedrate[axis]/2 ;
    plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate/60, active_extruder);
    st_synchronize();

    axis_is_at_home(axis);  //�趨��ĵ�ǰλ��Ϊԭ��λ��
    destination[axis] = current_position[axis]; //Ŀ��λ������
    feedrate = 0.0;  //������������
    endstops_hit_on_purpose(); //��λ��λ���ش�����������ص�����ԭ�����޷������˶�
    axis_known_position[axis] = true;
  }
}
#define HOMEAXIS(LETTER) homeaxis(LETTER##_AXIS)  //�Ժ����ʽ�����������ԭ��ĺ���

void gcode_G0_G1(void) {  
  if(Stopped == false) 
  {
    get_coordinates(); // For X Y Z E F
    prepare_move();
    return;
  } 
}
void gcode_G2_G3(bool clockwise) {
  if(Stopped == false) 
  {
    get_arc_coordinates();
    prepare_arc_move(clockwise);
    return;
  }
}
inline void gcode_G4(void) {
  unsigned long dwell_ms = 0;
  if(code_seen('P')) dwell_ms = code_value(); // milliseconds to wait  //����ȴ�
  if(code_seen('S')) dwell_ms = code_value() * 1000; // seconds to wait  //��ȴ�
  st_synchronize();  //synchronizeʹ�������ģ�ͬ��
  dwell_ms += millis();  // keep track of when we started waiting  //����ʼ�ȴ�ʱ���ֹ켣
  previous_millis_cmd = millis(); //��¼��ʷֵ
  while(millis()  < dwell_ms )
  {
    manage_heater();
    manage_inactivity();
  }
}
void gcode_G28(void) {
  saved_feedrate = feedrate;
  saved_feedmultiply = feedmultiply;
  feedmultiply = 100;
  previous_millis_cmd = millis();

  enable_endstops(true);

  for(char i=0; i < NUM_AXIS; i++) {
    destination[i] = current_position[i];
  }
  feedrate = 0.0;
  home_all_axis = !((code_seen(axis_codes[0])) || (code_seen(axis_codes[1])) || (code_seen(axis_codes[2])));

  #if Z_HOME_DIR > 0                      // If homing away from BED do Z first
  if((home_all_axis) || (code_seen(axis_codes[Z_AXIS]))) {
    HOMEAXIS(Z);
  }
  #endif

  #ifdef QUICK_HOME
  if((home_all_axis)||( code_seen(axis_codes[X_AXIS]) && code_seen(axis_codes[Y_AXIS])) )  //first diagonal move
  {
    current_position[X_AXIS] = 0;current_position[Y_AXIS] = 0;

    plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
    destination[X_AXIS] = 1.5 * X_MAX_LENGTH * X_HOME_DIR;destination[Y_AXIS] = 1.5 * Y_MAX_LENGTH * Y_HOME_DIR;
    feedrate = homing_feedrate[X_AXIS];
    if(homing_feedrate[Y_AXIS]<feedrate)
      feedrate =homing_feedrate[Y_AXIS];
    plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate/60, active_extruder);
    st_synchronize();

    axis_is_at_home(X_AXIS);
    axis_is_at_home(Y_AXIS);
    plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
    destination[X_AXIS] = current_position[X_AXIS];
    destination[Y_AXIS] = current_position[Y_AXIS];
    plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate/60, active_extruder);
    feedrate = 0.0;
    st_synchronize();
    endstops_hit_on_purpose();
  }
  #endif

  if((home_all_axis) || (code_seen(axis_codes[X_AXIS])))
  {
    HOMEAXIS(X);
  }

  if((home_all_axis) || (code_seen(axis_codes[Y_AXIS]))) {
    HOMEAXIS(Y);
  }

  #if Z_HOME_DIR < 0                      // If homing towards BED do Z last
  if((home_all_axis) || (code_seen(axis_codes[Z_AXIS]))) {
    HOMEAXIS(Z);
  }
  #endif

  if(code_seen(axis_codes[X_AXIS]))
  {
    if(code_value_long() != 0) {
      current_position[X_AXIS]=code_value()+add_homeing[0];
    }
  }

  if(code_seen(axis_codes[Y_AXIS])) {
    if(code_value_long() != 0) {
      current_position[Y_AXIS]=code_value()+add_homeing[1];
    }
  }

  if(code_seen(axis_codes[Z_AXIS])) {
    if(code_value_long() != 0) {
      current_position[Z_AXIS]=code_value()+add_homeing[2];
    }
  }
  plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);

  #ifdef ENDSTOPS_ONLY_FOR_HOMING
    enable_endstops(false);
  #endif

  feedrate = saved_feedrate;
  feedmultiply = saved_feedmultiply;
  previous_millis_cmd = millis();
  endstops_hit_on_purpose();
}
void gcode_G92(void) {
  if(!code_seen(axis_codes[E_AXIS]))
    st_synchronize();
    for(char i=0; i < NUM_AXIS; i++) {
      if(code_seen(axis_codes[i])) {
         if(i == E_AXIS) {
           current_position[i] = code_value();
           plan_set_e_position(current_position[E_AXIS]);
         }
         else {
           current_position[i] = code_value()+add_homeing[i];
           plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
         }
      }
    }
}

/**
 * M17: Enable power on all stepper motors
 */
void gcode_M17() {
  enable_x();
  enable_y();
  enable_z();
  enable_e0();
  enable_e1();
}
#ifdef SDSUPPORT
  /**
    * M20: List SD card to serial output
    */
  void gcode_M20(void) {
    printf(MSG_BEGIN_FILE_LIST);
    printf("\n");
    card_ls();
    printf(MSG_END_FILE_LIST);
    printf("\n");
  }
  
  /**
   * M21: Init SD Card
   */
  void gcode_M21(void) { card_initsd(); }

  /**
   * M22: Release SD Card
   */
  void gcode_M22(void) { card_release(); }

  /**
   * M23: Open a file
   */
  void gcode_M23(void) {
    starpos = (strchr(strchr_pointer + 4,'*'));
    if(starpos!=NULL) *(starpos-1)='\0';
    card_openFile(strchr_pointer + 4,true);
  }

  /**
   * M24: Start or Resume SD Print
   */
  void gcode_M24(void) {
    card_startFileprint();
    starttime=millis();
  }

  /**
   * M25: Pause SD Print
   */
  void gcode_M25(void) {
    card_pauseSDPrint();
  }

  /**
   * M26: Set SD Card file index
   */
  void gcode_M26(void) {
    if(card.cardOK && code_seen('S')) {
      card_setIndex(code_value_long());
    }
  }

  /**
   * M27: Get SD Card status
   */
  inline void gcode_M27() { card_getStatus(); }

  /**
   * M28: Start SD Write
   */
  void gcode_M28(void) { 
    starpos = (strchr(strchr_pointer + 4,'*'));
    if(starpos != NULL) {
      char* npos = strchr(cmdbuffer[bufindr], 'N');
      strchr_pointer = strchr(npos,' ') + 1;
      *(starpos-1) = '\0';
    }
    card_openFile(strchr_pointer+4,false);
  }

  /**
   * M29: Stop SD Write
   * Processed in write to file routine above
   */
  void gcode_M29(void) {
    // card.saving = false;
  }

  /**
   * M30 <filename>: Delete SD Card file
   */
  void gcode_M30(void) {
    if (card.cardOK) {
      card_closefile();
      starpos = (strchr(strchr_pointer + 4,'*'));
      if(starpos != NULL){
        char* npos = strchr(cmdbuffer[bufindr], 'N');
        strchr_pointer = strchr(npos,' ') + 1;
        *(starpos-1) = '\0';
      }
      card_removeFile(strchr_pointer + 4);
    }
  }  
#endif // SDSUPPORT
  
/**
 * M31: Get the time since the start of SD Print (or last M109)
 */
void gcode_M31(void) {
  unsigned long t=(stoptime-starttime)/1000;
  int sec,min;
  stoptime=millis();
  min=t/60;
  sec=t%60;
  SERIAL_ECHO_START;
  printf("Print time: %d min, %d sec", min, sec);
  autotempShutdown();
}  

/**
 * M104: Set hot end temperature
 */
void gcode_M104(void) {
  if(setTargetedHotend(104)) return;
  if(code_seen('S')) 
  {	
    setTargetHotend(code_value(), tmp_extruder);
  }
  setWatch();
}

/**
 * M140: Set bed temperature
 */
void gcode_M140(void) {
  if (code_seen('S')) 
    setTargetBed(code_value());
}

/**
 * M105: Read hot end and bed temperature
 */
void gcode_M105(void) {
  if(setTargetedHotend(105)) return;
  #if defined(TEMP_0_PIN)
    printf("ok T:%.1f /%.1f",degHotend(tmp_extruder),degTargetHotend(tmp_extruder));
    #if defined(TEMP_BED_PIN)
    printf(" B:%.1f /%.1f",degBed(),degTargetBed());
    #endif //TEMP_BED_PIN
  #else
    SERIAL_ERROR_START;
    printf(MSG_ERR_NO_THERMISTORS);
  #endif //TEMP_0_PIN

  printf(" @:%d",getHeaterPower(tmp_extruder));
  printf(" B@:%d\n",getHeaterPower(-1));
}

/**
 * M109: Sxxx Wait for extruder(s) to reach temperature. Waits only when heating.
 *       Rxxx Wait for extruder(s) to reach temperature. Waits when heating and cooling.
 */
void gcode_M109(void) {
  unsigned long codenum;
  bool target_direction; //���ڼ�¼���Ȼ�����ȴ
  long residencyStart;
  if(setTargetedHotend(109)) return;
  #ifdef AUTOTEMP
     autotemp_enabled=false;
  #endif
  if (code_seen('S')) setTargetHotend(code_value(), tmp_extruder);
  #ifdef AUTOTEMP
    if (code_seen('S')) autotemp_min=code_value();
    if (code_seen('B')) autotemp_max=code_value();	
    if (code_seen('F')) {
      autotemp_factor=code_value();
      autotemp_enabled=true;
    }
  #endif
  setWatch();
  codenum = millis();
  /* See if we are heating up or cooling down */ //�����ڼ��Ȼ�������ȴ 1Ϊ���� 0Ϊ��ȴ
  target_direction = isHeatingHotend(tmp_extruder); // true if heating, false if cooling
  #ifdef TEMP_RESIDENCY_TIME
  residencyStart = -1;
  /* continue to loop until we have reached the target temp _and_ until TEMP_RESIDENCY_TIME hasn't passed since we reached it */
  while((residencyStart == -1) || (residencyStart >= 0 
                               && (((unsigned int) (millis() - residencyStart)) < (TEMP_RESIDENCY_TIME * 1000UL))) ) { 
    if( (millis() - codenum) > 1000UL ) { //Print Temp Reading and remaining time every 1 second while heating up/cooling down
      printf("T:%.1f E:%d",degHotend(tmp_extruder),tmp_extruder);
      #ifdef TEMP_RESIDENCY_TIME
        printf(" W:");
        if(residencyStart > -1) {
           codenum = ((TEMP_RESIDENCY_TIME * 1000UL) - (millis() - residencyStart)) / 1000UL;
           printf("%ld\n", codenum );
        }
        else {
           printf("?\n");
        }
      #else
        printf("\n");
      #endif  //TEMP_RESIDENCY_TIME
      codenum = millis();
    }
    manage_heater();
    manage_inactivity();
    //lcd_update();
    #ifdef TEMP_RESIDENCY_TIME
      /* start/restart the TEMP_RESIDENCY_TIME timer whenever we reach target temp for the first time
        or when current temp falls outside the hysteresis after target temp was reached */
     if((residencyStart == -1 &&  target_direction && (degHotend(tmp_extruder) >= (degTargetHotend(tmp_extruder)-TEMP_WINDOW))) ||
          (residencyStart == -1 && !target_direction && (degHotend(tmp_extruder) <= (degTargetHotend(tmp_extruder)+TEMP_WINDOW))) ||
          (residencyStart > -1 && labs(degHotend(tmp_extruder) - degTargetHotend(tmp_extruder)) > TEMP_HYSTERESIS) ) {
       residencyStart = millis();
     }
    #endif //TEMP_RESIDENCY_TIME
  }
#else
  while ( target_direction ? (isHeatingHotend(tmp_extruder)) : (isCoolingHotend(tmp_extruder)&&(CooldownNoWait==false)) ) { 
    if( (millis() - codenum) > 1000UL ) { //Print Temp Reading and remaining time every 1 second while heating up/cooling down
      printf("T:%.1f E:%d",degHotend(tmp_extruder),tmp_extruder);
      #ifdef TEMP_RESIDENCY_TIME
        printf(" W:");
        if(residencyStart > -1)
        {
           codenum = ((TEMP_RESIDENCY_TIME * 1000UL) - (millis() - residencyStart)) / 1000UL;
           printf("%ld\n", codenum );
        }
        else
        {
           printf("?\n");
        }
      #else
        printf("\n");
      #endif
      codenum = millis();
    }
    manage_heater();
    manage_inactivity();
    //lcd_update();
   #ifdef TEMP_RESIDENCY_TIME
      /* start/restart the TEMP_RESIDENCY_TIME timer whenever we reach target temp for the first time
        or when current temp falls outside the hysteresis after target temp was reached */
     if ((residencyStart == -1 &&  target_direction && (degHotend(tmp_extruder) >= (degTargetHotend(tmp_extruder)-TEMP_WINDOW))) ||
          (residencyStart == -1 && !target_direction && (degHotend(tmp_extruder) <= (degTargetHotend(tmp_extruder)+TEMP_WINDOW))) ||
          (residencyStart > -1 && labs(degHotend(tmp_extruder) - degTargetHotend(tmp_extruder)) > TEMP_HYSTERESIS) )
      {
        residencyStart = millis();
      }
    #endif //TEMP_RESIDENCY_TIME
  }
  #endif //TEMP_RESIDENCY_TIME
  starttime=millis();
  previous_millis_cmd = millis();
}

/**
 * M190: Sxxx Wait for bed current temp to reach target temp. Waits only when heating
 *       Rxxx Wait for bed current temp to reach target temp. Waits when heating and cooling
 */
void gcode_M190(void) {
  unsigned long codenum;
  #if defined(TEMP_BED_PIN)
    if (code_seen('S')) setTargetBed(code_value());
    codenum = millis();
    while(isHeatingBed()) {
      if(( millis() - codenum) > 1000 ) { //Print Temp Reading every 1 second while heating up.
        float tt=degHotend(active_extruder);
        printf("T:%.1f E:%d B:%.1f\n",tt,active_extruder,degBed());
        codenum = millis();
      }
      manage_heater();
      manage_inactivity();
		  //lcd_update();
    }
    previous_millis_cmd = millis();
  #endif
}
/**
 * M106: Set Fan Speed
 *
 *  S<int>   Speed between 0-255
 *  P<index> Fan index, if more than one fan
 *
 * With EXTRA_FAN_SPEED enabled:
 *
 *  T<int>   Restore/Use/Set Temporary Speed:
 *           1     = Restore previous speed after T2
 *           2     = Use temporary speed set with T3-255
 *           3-255 = Set the speed for use with T2
 */
void gcode_M106(void) {
  if (code_seen('S')){
    fanSpeed = constrain(code_value(),0,255);
  }
  else {
    fanSpeed = 255;
  }
}

/**
 * M107: Fan Off
 */
void gcode_M107(void) {
  fanSpeed = 0;
}

/**
 * M82: Set E codes absolute (default)
 */
inline void gcode_M82() { axis_relative_modes[E_AXIS] = false; }

/**
 * M83: Set E codes relative while in Absolute Coordinates (G90) mode
 */
inline void gcode_M83() { axis_relative_modes[E_AXIS] = true; }

/**
 * M18, M84: Disable stepper motors
 */
void gcode_M18_M84(void) {
  if(code_seen('S')){
    stepper_inactive_time = code_value() * 1000;
  }
  else {
    bool all_axis = !((code_seen(axis_codes[0])) || (code_seen(axis_codes[1])) || (code_seen(axis_codes[2]))|| (code_seen(axis_codes[3])));
    if(all_axis){
      st_synchronize();
      disable_e0();
      disable_e1();
      //disable_e2();
      finishAndDisableSteppers();
    }
    else {
      st_synchronize();
      if(code_seen('X')) disable_x();
      if(code_seen('Y')) disable_y();
      if(code_seen('Z')) disable_z();
      if(code_seen('E')) {
        disable_e0();
        disable_e1();
        //disable_e2();
      }
    }
  }
}

/**
 * M85: Set inactivity shutdown timer with parameter S<seconds>. To disable set zero (default)
 */
void gcode_M85(void) {
   if(code_seen('S')) max_inactive_time = code_value() * 1000;
}

/**
 * M92: Set axis steps-per-unit for one or more axes, X, Y, Z, and E.
 *      (Follows the same syntax as G92)
 *
 *      With multiple extruders use T to specify which one.
 */
void gcode_M92() {
  for(char i=0; i < NUM_AXIS; i++) {
    if(code_seen(axis_codes[i])) {
      if(i == E_AXIS) { 
        float value = code_value();
        if(value < 20.0) {
          float factor = axis_steps_per_unit[i] / value; // increase e constants if M92 E14 is given for netfab.
          max_e_jerk *= factor;
          max_feedrate[i] *= factor;
          axis_steps_per_sqr_second[i] *= factor;
        }
        axis_steps_per_unit[i] = value;
      }
      else {
        axis_steps_per_unit[i] = code_value();
      }
    }
  }
}

/**
 * M115: Capabilities string
 */
void gcode_M115(void) {
  printf(MSG_M115_REPORT);
}

/**
 * M119: Output endstop states to serial output
 */
void gcode_M119() {  }

/**
 * M120: Enable endstops and set non-homing endstop state to "enabled"
 */
void gcode_M120() { enable_endstops(true); }

/**
 * M121: Disable endstops and set non-homing endstop state to "disabled"
 */
void gcode_M121() { enable_endstops(false); }

/**
 * M201: Set max acceleration in units/s^2 for print moves (M201 X1000 Y1000)
 *
 *       With multiple extruders use T to specify which one.
 */
void gcode_M201() {
  for( char i=0; i < NUM_AXIS; i++) {
    if(code_seen(axis_codes[i])) {
      max_acceleration_units_per_sq_second[i] = code_value();
    }
  }
  // steps per sq second need to be updated to agree with the units per sq second (as they are what is used in the planner)
  reset_acceleration_rates();
}

/**
 * M203: Set maximum feedrate that your machine can sustain (M203 X200 Y200 Z300 E10000) in units/sec
 *
 *       With multiple extruders use T to specify which one.
 */
void gcode_M203() {
  for( char i=0; i < NUM_AXIS; i++) {
    if(code_seen(axis_codes[i])) 
      max_feedrate[i] = code_value();
  }
}

/**
 * M204: Set Accelerations in units/sec^2 (M204 P1200 R3000 T3000)
 *
 *    P = Printing moves
 *    R = Retract only (no X, Y, Z) moves
 *    T = Travel (non printing) moves
 *
 *  Also sets minimum segment time in ms (B20000) to prevent buffer under-runs and M20 minimum feedrate
 */
void gcode_M204() {
  if(code_seen('S')) acceleration = code_value() ;
  if(code_seen('T')) retract_acceleration = code_value() ;	    
}

/**
 * M205: Set Advanced Settings
 *
 *    S = Min Feed Rate (units/s)
 *    T = Min Travel Feed Rate (units/s)
 *    B = Min Segment Time (��s)
 *    X = Max X Jerk (units/sec^2)
 *    Y = Max Y Jerk (units/sec^2)
 *    Z = Max Z Jerk (units/sec^2)
 *    E = Max E Jerk (units/sec^2)
 */
void gcode_M205() {
  if(code_seen('S')) minimumfeedrate = code_value();
  if(code_seen('T')) mintravelfeedrate = code_value();
  if(code_seen('B')) minsegmenttime = code_value() ;
  if(code_seen('X')) max_xy_jerk = code_value() ;
  if(code_seen('Z')) max_z_jerk = code_value() ;
  if(code_seen('E')) max_e_jerk = code_value() ;
}

void gcode_M206(void) {
  for( char i=0; i < 3; i++){
    if(code_seen(axis_codes[i])) add_homeing[i] = code_value();
  }
}

/**
 * M220: Set speed percentage factor, aka "Feed Rate" (M220 S95)
 */
void gcode_M220(void) {
  if(code_seen('S')) {
    feedmultiply = code_value() ;
  }
}

/**
 * M221: Set extrusion percentage (M221 T0 S95)
 */
void gcode_M221(void) {
  if(code_seen('S')) {
    extrudemultiply = code_value() ;
  }
}

/**
   * M302: Allow cold extrudes, or set the minimum extrude temperature
   *
   *       S<temperature> sets the minimum extrude temperature
   *       P<bool> enables (1) or disables (0) cold extrusion
   *
   *  Examples:
   *
   *       M302         ; report current cold extrusion state
   *       M302 P0      ; enable cold extrusion checking
   *       M302 P1      ; disables cold extrusion checking
   *       M302 S0      ; always allow extrusion (disables checking)
   *       M302 S170    ; only allow extrusion above 170
   *       M302 S170 P1 ; set min extrude temp to 170 but leave disabled
   */
void gcode_M302() {
  //allow_cold_extrudes(true);
  float temp = .0;
  if (code_seen('S')) temp = code_value();
  set_extrude_min_temp(temp);
}

/**
 * M303: PID relay autotune
 *
 *       S<temperature> sets the target temperature. (default 150C)
 *       E<extruder> (-1 for the bed) (default 0)
 *       C<cycles>
 *       U<bool> with a non-zero value will apply the result to current settings
 */
void gcode_M303() {
  float temp = 150.0;
  int e=0;
  int c=5;
  if (code_seen('E')) e=code_value();
  if (e<0) temp=70;
  if (code_seen('S')) temp=code_value();
  if (code_seen('C')) c=code_value();
  PID_autotune(temp, e, c);
}

/**
 * M400: Finish all moves
 */
void gcode_M400() { st_synchronize(); }

/**
 * M500: Store settings in EEPROM
 */
void gcode_M500() {
  Config_StoreSettings();
}

/**
 * M501: Read settings from EEPROM
 */
void gcode_M501() {
  Config_RetrieveSettings();
}

/**
 * M502: Revert to default settings
 */
void gcode_M502() {
   Config_ResetDefault();
}

/**
 * M503: print settings currently in memory
 */
void gcode_M503() {
  Config_PrintSettings();
}

/**
 * M600: Pause for filament change
 *
 *  E[distance] - Retract the filament this far (negative value)
 *  Z[distance] - Move the Z axis by this distance
 *  X[position] - Move to this X position, with Y
 *  Y[position] - Move to this Y position, with X
 *  U[distance] - Retract distance for removal (negative value) (manual reload)
 *  L[distance] - Extrude distance for insertion (positive value) (manual reload)
 *  B[count]    - Number of times to beep, -1 for indefinite (if equipped with a buzzer)
 *
 *  Default values are used for omitted arguments.
 *
 */
void gcode_M600(void) {
  float target[4];
  float lastpos[4];

  target[X_AXIS]=current_position[X_AXIS];
  target[Y_AXIS]=current_position[Y_AXIS];
  target[Z_AXIS]=current_position[Z_AXIS];
  target[E_AXIS]=current_position[E_AXIS];
  lastpos[X_AXIS]=current_position[X_AXIS];
  lastpos[Y_AXIS]=current_position[Y_AXIS];
  lastpos[Z_AXIS]=current_position[Z_AXIS];
  lastpos[E_AXIS]=current_position[E_AXIS];
  //retract by E
  if(code_seen('E')) {
    target[E_AXIS]+= code_value();
  }
  else {
    #ifdef FILAMENTCHANGE_FIRSTRETRACT
      target[E_AXIS]+= FILAMENTCHANGE_FIRSTRETRACT ;
    #endif
  }
  plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], feedrate/60, active_extruder);

  //lift Z
  if(code_seen('Z')) {
    target[Z_AXIS]+= code_value();
  }
  else {
    #ifdef FILAMENTCHANGE_ZADD
      target[Z_AXIS]+= FILAMENTCHANGE_ZADD ;
    #endif
  }
  plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], feedrate/60, active_extruder);

  //move xy
  if(code_seen('X')) {
    target[X_AXIS]+= code_value();
  }
  else {
    #ifdef FILAMENTCHANGE_XPOS
      target[X_AXIS]= FILAMENTCHANGE_XPOS ;
    #endif
  }
  if(code_seen('Y')) {
    target[Y_AXIS]= code_value();
  }
  else {
    #ifdef FILAMENTCHANGE_YPOS
      target[Y_AXIS]= FILAMENTCHANGE_YPOS ;
    #endif
  }

  plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], feedrate/60, active_extruder);

  if(code_seen('L')) {
    target[E_AXIS]+= code_value();
  }
  else {
    #ifdef FILAMENTCHANGE_FINALRETRACT
      target[E_AXIS]+= FILAMENTCHANGE_FINALRETRACT ;
    #endif
  }

  plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], feedrate/60, active_extruder);

  //finish moves
  st_synchronize();
  //disable extruder steppers so filament can be removed
  disable_e0();
  disable_e1();
  //disable_e2();
  delay_ms(100);
  
  //return to normal
  if(code_seen('L')) {
    target[E_AXIS]+= -code_value();
  }
  else {
    #ifdef FILAMENTCHANGE_FINALRETRACT
      target[E_AXIS]+=(-1)*FILAMENTCHANGE_FINALRETRACT ;
    #endif
  }
  current_position[E_AXIS]=target[E_AXIS]; //the long retract of L is compensated by manual filament feeding
  plan_set_e_position(current_position[E_AXIS]);
  plan_buffer_line(target[X_AXIS], target[Y_AXIS], target[Z_AXIS], target[E_AXIS], feedrate/60, active_extruder); //should do nothing
  plan_buffer_line(lastpos[X_AXIS], lastpos[Y_AXIS], target[Z_AXIS], target[E_AXIS], feedrate/60, active_extruder); //move xy back
  plan_buffer_line(lastpos[X_AXIS], lastpos[Y_AXIS], lastpos[Z_AXIS], target[E_AXIS], feedrate/60, active_extruder); //move z back
  plan_buffer_line(lastpos[X_AXIS], lastpos[Y_AXIS], lastpos[Z_AXIS], lastpos[E_AXIS], feedrate/60, active_extruder); //final untretract
}


void gcode_M907(void) {
  char i;
  for(i=0;i<NUM_AXIS;i++) if(code_seen(axis_codes[i])) digipot_current(i,code_value());
  if(code_seen('B')) digipot_current(4,code_value());
  if(code_seen('S')) for(i=0;i<=4;i++) digipot_current(i,code_value());
}

void gcode_M908(void) {
  uint8_t channel,current;
  if(code_seen('P')) channel=code_value();
  if(code_seen('S')) current=code_value();
  digipot_current(channel, current);
}

void gcode_M350(void) {
  char i;
  if(code_seen('S')) for(i=0;i<=4;i++) microstep_mode(i,code_value());
  for( i=0;i<NUM_AXIS;i++) if(code_seen(axis_codes[i])) microstep_mode(i,(uint8_t)code_value());
  if(code_seen('B')) microstep_mode(4,code_value());
  microstep_readings();
}

void gcode_M351(void) {
  char i;
  if(code_seen('S')) switch((int)code_value())
  {
    case 1:
      for(i=0;i<NUM_AXIS;i++) if(code_seen(axis_codes[i])) microstep_ms(i,code_value(),-1,-1);
      if(code_seen('B')) microstep_ms(4,code_value(),-1,-1);
      break;
    case 2:
      for(i=0;i<NUM_AXIS;i++) if(code_seen(axis_codes[i])) microstep_ms(i,-1,code_value(),-1);
      if(code_seen('B')) microstep_ms(4,-1,code_value(),-1);
      break;
    case 3:
      for(i=0;i<NUM_AXIS;i++) if(code_seen(axis_codes[i])) microstep_ms(i,-1,-1,code_value());
      if(code_seen('B')) microstep_ms(4,-1,-1,code_value());
      break;
  }
  microstep_readings();
}

void gcode_M999(void) {
  Stopped = false;
  //lcd_reset_alert_level();
  gcode_LastN = Stopped_gcode_LastN;
  FlushSerialRequestResend();
}

void process_commands(void)  //����G�����M����
{ 
  //unsigned long codenum;
  //char *starpos = NULL;
  //int8_t i;
  if(code_seen('G')) {  //��ȡ�����ַ�G
    switch((int)code_value()) {  //��ȡG�������ֵ
      case 0: // G0 -> G1
      case 1: // G1
        gcode_G0_G1();
        break;
      case 2: // G2  - CW ARC 
      case 3: // G3  - CCW ARC
        gcode_G2_G3(code_value() == 2);
        break;
      case 4: // G4 dwell
        gcode_G4();
        break;
      #ifdef FWRETRACT  //���ϻس�
      case 10: // G10 retract  //�س�
        break;
      case 11: // G10 retract_recover  //�س�ָ�
        break;
      #endif //FWRETRACT
      case 28: //G28 Home all Axis one at a time  //���������̹�λ
        gcode_G28();
        break;
      case 90: // G90
        relative_mode = false;
        break;
      case 91: // G91
        relative_mode = true;
        break;
      case 92: // G92
        gcode_G92();
        break;
    }
  }
  else if(code_seen('M')) {	 //��ȡ�����ַ�M
    switch((int)code_value())  //��ȡM�������ֵ
    {  
	    case 0: // M0 - Unconditional stop - Wait for user button press on LCD
	    case 1: // M1 - Conditional stop - Wait for user button press on LCD
			  break;
			case 15:
				break;
			case 16:
				break;
	    case 17:
        gcode_M17();
        break;
		  #ifdef SDSUPPORT
        case 20:
          gcode_M20();
          break;
        case 21: 	
          gcode_M21();		
          break;
        case 22: 
          gcode_M22();
          break;
        case 23:
          gcode_M23();
          break;
        case 24:
          gcode_M24();
          break;
        case 25: 
          gcode_M25();
          break;
        case 26: 
          gcode_M26();
          break;
        case 27: 
          gcode_M27();
          break;
        case 28:
          gcode_M28();  
          break;
        case 29:
          gcode_M29();  
          break;
        case 30: 
          gcode_M30();
          break;
	    #endif //SDSUPPORT
        
	    case 31: //M31 take time since the start of the SD print or an M109 command
		    gcode_M31();  
		    break;
      case 42: //M42 -Change pin status via gcode
        break;
      case 104: // M104: Set hot end temperature
        gcode_M104();    
        break;
      case 140: // M140: Set bed temperature
        gcode_M140();    
        break;
      case 105 : // M105: Report current temperature
        gcode_M105();      
        break;
      case 109: // M109: Wait for extruder heater to reach target.
				gcode_M109();
			  break;
      case 190: // M190: Wait for bed heater to reach target.
        gcode_M190();
		    break;
      #if defined(FAN_PIN) 
        case 106: // M106 Fan On //����ģ��ɢ�ȷ���
          gcode_M106();
          break;        
        case 107: // M107 Fan Off //�ر�ģ��ɢ�ȷ���
          gcode_M107();
        break;
		  #endif //FAN_PIN
		  case 80: // M80: Turn on Power Supply
	      break;
	    case 81: // M81: Turn off Power, including Power Supply, if possible
		  	break;
	    case 82: // M82: Set E axis normal mode (same as other axes)
	      gcode_M82();
	      break;
	     case 83: // M83: Set E axis relative mode
	       gcode_M83();
	       break;
	     case 18: // M18 => M84
	     case 84: // M84: Disable all steppers or set timeout
	       gcode_M18_M84();
			 break;
		  case 85: // M85: Set inactivity stepper shutdown timeout
		    gcode_M85();
		    break;
		  case 92: // M92: Set the steps-per-unit for one or more axes
		    gcode_M92();  
			  break;
      case 114: // M114: Report current position 
		    break; 
	    case 115: // M115: Report capabilities
        gcode_M115(); 
		    break;
	    case 117: // M117: Set LCD message text, if possible
		    break;
      case 119: // M119: Report endstop states
		    break;
		  case 120: // M120: Enable endstops
		    gcode_M120();    
		    break;
		  case 121: // M121: Disable endstops
		    gcode_M121();
		    break;	   
			case 200: // M200: Set filament diameter, E to cubic units
        break;
		  case 201: // M201: Set max acceleration for print moves (units/s^2)	 
		    gcode_M201(); 
		    break;
	    case 203: // M203: Set max feedrate (units/sec)
		    gcode_M203();
		    break; 
	    case 204: // M204: Set acceleration
		    gcode_M204();  
	      break;
	    case 205: // M205: Set advanced settings --- minimum travel speed S=while printing T=travel only,  B=minimum segment time X= maximum xy jerk, Z=maximum Z jerk
		    gcode_M205();		      		    
	    	break;
	    case 206: // M206: Set home offsets
		    gcode_M206();  
		    break;
      #ifdef FWRETRACT
        case 207: //M207 - set retract length S[positive mm] F[feedrate mm/sec] Z[additional zlift/hop]
          break;
        case 208: // M208 - set retract recover length S[positive mm surplus to the M207 S*] F[feedrate mm/sec]
          break;
        case 209: // M209 - S<1=true/0=false> enable automatic retract detect if the slicer did not support G10/11: every normal extrude-only move will be classified as retract depending on the direction.
          break;
      #endif // FWRETRACT
		  #if EXTRUDERS > 1
        case 218: // M218 - set hotend offset (in mm), T<extruder_number> X<offset_on_X> Y<offset_on_Y>
          break;
		  #endif
		  case 220: // M220: Set Feedrate Percentage: S<percent> ("FR" on your LCD)e
		    break;
		  case 221: // M221: Set Flow Percentage
		    break;
      
		  #if NUM_SERVOS > 0
		    case 280: // M280 - set servo position absolute. P: servo index, S: angle or microseconds
		      break;
		  #endif // NUM_SERVOS > 0
        
		  case 300: // M300: Play beep tone
		    break;
      
      #ifdef PIDTEMP
        case 301: // M301
          break;
	    #endif /*PIDTEMP*/
        
	    #ifdef PIDTEMPBED
        case 304: // M304
          break;
	    #endif /*PIDTEMPBED*/
        
	    case 240: // M240  Triggers a camera by emulating a Canon RC-1 : http://www.doc-diy.net/photo/rc-1_hacked/
		    break;
      #ifdef PREVENT_DANGEROUS_EXTRUDE
        case 302: // M302: Allow cold extrudes (set the minimum extrude temperature)
          gcode_M302();
          break;
      #endif
		  case 303: // M303: PID autotune
        gcode_M303();
		    break;
		  case 400: // M400 finish all moves
		    gcode_M400();
		    break;
		  case 500: // M500 Store settings in EEPROM
		    gcode_M500();
		    break;
		  case 501: // M501 Read settings from EEPROM
		    gcode_M501();
		    break;
		  case 502: // M502 Revert to default settings
		    gcode_M502();
		    break;
		  case 503: // M503 print settings currently in memory
        gcode_M503();
		    break;
	    #ifdef FILAMENTCHANGEENABLE
		  case 600: //Pause for filament change X[pos] Y[pos] Z[relative lift] E[initial retract] L[later retract distance for removal]
		  gcode_M600(); 
		  break;
	    #endif //FILAMENTCHANGEENABLE
	    case 907: // M907 Set digital trimpot motor current using axis codes.
        gcode_M907();
        break;
	    case 908: // M908 Control digital trimpot directly.
        gcode_M908();
        break;
	    case 350: // M350 Set microstepping mode. Warning: Steps per unit remains unchanged. S code sets stepping mode for all drivers.
        gcode_M350();
        break;
	    case 351: // M351 Toggle MS1 MS2 pins directly, S# determines MS1 or MS2, X# sets the pin high/low.
        gcode_M351();
        break;
	    case 999: // M999: Restart after being stopped
        gcode_M999();  
        break;		
    }//end switch(int) code_value();
  }
  else if(code_seen('T')) { 
    tmp_extruder = code_value();
    if(tmp_extruder >= EXTRUDERS) {
      SERIAL_ECHO_START;
      printf("T%d",tmp_extruder);
      printf(MSG_INVALID_EXTRUDER);
    }
	  else {
      volatile bool make_move = false;
      if(code_seen('F')) {
        make_move = true;
        next_feedrate = code_value();
        if(next_feedrate > 0.0) {
          feedrate = next_feedrate;
        }
      }
	    #if EXTRUDERS > 1
        if(tmp_extruder != active_extruder) 
        {
          // Save current position to return to after applying extruder offset
          memcpy(destination, current_position, sizeof(destination));
          // Offset extruder (only by XY)
          for(i = 0; i < 2; i++) {
            current_position[i] = current_position[i] - extruder_offset[i][active_extruder] + extruder_offset[i][tmp_extruder];
          }
          // Set the new active extruder and position
          active_extruder = tmp_extruder;
          plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
          // Move to the old position if 'F' was in the parameters
          if(make_move && Stopped == false) {
            prepare_move();
          }
        }	//end  
	    #endif
      SERIAL_ECHO_START;
      printf(MSG_ACTIVE_EXTRUDER);
      printf("%d",active_extruder);
      printf("\n");
    }
  }//end else if(code_seen('T'))
  else { 
    SERIAL_ECHO_START;
    printf(MSG_UNKNOWN_COMMAND);
    printf("%s",cmdbuffer[bufindr]);
    printf("\"");
  }
  ClearToSend();
}

void FlushSerialRequestResend() {  //��������������·���
  MYSERIAL_flush();  //���������Ϣ
  printf(MSG_RESEND); //��ӡ���·���
  printf("%d\n",gcode_LastN + 1); //��ӡ��ǰ�к�
  ClearToSend();
}

void ClearToSend() {  //���������
  previous_millis_cmd = millis(); //�洢��ǰ�������е�ʱ�䳤��	
	#ifdef SDSUPPORT
    if(fromsd[bufindr])
      return;
  #endif //SDSUPPORT	
  printf(MSG_OK);  //��ӡ��OK��
  printf("\n");    
}

void get_coordinates() {  //��ȡGָ���е�����ֵ 
  volatile bool seen[4]={false,false,false,false};
  for(int8_t i=0; i < NUM_AXIS; i++) {
    if(code_seen(axis_codes[i])) {  //�����ȡ���ַ�X
      destination[i] = (float)code_value() + (axis_relative_modes[i] || relative_mode)*current_position[i]; //Ŀ��λ�� = ��ȡ������ֵ + ��ǰλ��
      seen[i]=true;
    }
    else destination[i] = current_position[i]; //Are these else lines really needed? //��ǰ����ֵ��ֵ��Ŀ������ֵ
  }
  if(code_seen('F')) {  //�����ȡ���ַ�F
    next_feedrate = code_value(); //�´ν������� Ϊ�ַ�F�������ֵ
    if(next_feedrate > 0.0) feedrate = next_feedrate;  //���´ν������ʸ�ֵ����������
  }

  #ifdef FWRETRACT  //���ϻس�(�ǻ)
  if(autoretract_enabled)
  if( !(seen[X_AXIS] || seen[Y_AXIS] || seen[Z_AXIS]) && seen[E_AXIS])
  {
    float echange=destination[E_AXIS]-current_position[E_AXIS];
    if(echange<-MIN_RETRACT) //retract
    {
      if(!retracted)
      {

      destination[Z_AXIS]+=retract_zlift; //not sure why chaninging current_position negatively does not work.
      //if slicer retracted by echange=-1mm and you want to retract 3mm, corrrectede=-2mm additionally
      float correctede=-echange-retract_length;
      //to generate the additional steps, not the destination is changed, but inversely the current position
      current_position[E_AXIS]+=-correctede;
      feedrate=retract_feedrate;
      retracted=true;
      }

    }
    else
      if(echange>MIN_RETRACT) //retract_recover
    {
      if(retracted)
      {
      //current_position[Z_AXIS]+=-retract_zlift;
      //if slicer retracted_recovered by echange=+1mm and you want to retract_recover 3mm, corrrectede=2mm additionally
      float correctede=-echange+1*retract_length+retract_recover_length; //total unretract=retract_length+retract_recover_length[surplus]
      current_position[E_AXIS]+=correctede; //to generate the additional steps, not the destination is changed, but inversely the current position
      feedrate=retract_recover_feedrate;
      retracted=false;
      }
    }

  }
  #endif //FWRETRACT
}
void get_arc_coordinates(void) {  //��ȡGָ���е�Բ������
  #ifdef SF_ARC_FIX
   bool relative_mode_backup = relative_mode;
   relative_mode = true;
  #endif
   get_coordinates();
  #ifdef SF_ARC_FIX
   relative_mode=relative_mode_backup;
  #endif

  if(code_seen('I')) {  //�����ȡ����I
   offset[0] = code_value();
  }
  else {
   offset[0] = 0.0;
  }
  if(code_seen('J')) {  // �����ȡ����J
   offset[1] = code_value();
  }
  else {
   offset[1] = 0.0;
  }
}

void clamp_to_software_endstops(float target[3]) {  //������λ
  if (min_software_endstops) {
    if (target[X_AXIS] < min_pos[X_AXIS]) target[X_AXIS] = min_pos[X_AXIS];
    if (target[Y_AXIS] < min_pos[Y_AXIS]) target[Y_AXIS] = min_pos[Y_AXIS];
    if (target[Z_AXIS] < min_pos[Z_AXIS]) target[Z_AXIS] = min_pos[Z_AXIS];
  }

  if (max_software_endstops) {
    if (target[X_AXIS] > max_pos[X_AXIS]) target[X_AXIS] = max_pos[X_AXIS];
    if (target[Y_AXIS] > max_pos[Y_AXIS]) target[Y_AXIS] = max_pos[Y_AXIS];
    if (target[Z_AXIS] > max_pos[Z_AXIS]) target[Z_AXIS] = max_pos[Z_AXIS];
  }
}

void prepare_move(void) {   //׼���ƶ�
  clamp_to_software_endstops(destination); //������λ
  previous_millis_cmd = millis();  //��¼������ǰ���е�ʱ�䳤��
  
  // Do not use feedmultiply for E or Z only moves  //ֻ��E��Z���ƶ�ʱ��Ҫʹ��feedmultiply
  if( (current_position[X_AXIS] == destination [X_AXIS]) && (current_position[Y_AXIS] == destination [Y_AXIS])) {
      plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate/60, active_extruder);
  }
  else {
    plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate*feedmultiply/60/100.0, active_extruder);
  }
  
  for(int8_t i=0; i < NUM_AXIS; i++) {
    current_position[i] = destination[i]; //��¼�������ʷֵ
  }
}

void prepare_arc_move(u8 isclockwise) {  //׼��Բ���˶�  clockwise˳ʱ�뷽���
  float r = hypot(offset[X_AXIS], offset[Y_AXIS]); // Compute arc radius for mc_arc //����Բ���뾶

  // Trace the arc  //���Բ���켣
  mc_arc(current_position, destination, offset, X_AXIS, Y_AXIS, Z_AXIS, feedrate*feedmultiply/60/100.0, r, isclockwise, active_extruder);

  // As far as the parser���������� is concerned, the position is now == target. In reality the
  // motion control system might still be processing the action and the real tool position
  // in any intermediate���м�ģ� location.
  for(int8_t i=0; i < NUM_AXIS; i++) {
    current_position[i] = destination[i]; //��¼�������ʷֵ
  }
  previous_millis_cmd = millis(); //��¼������ǰ����ʱ�䳤�ȵ���ʷֵ
}

#if defined(CONTROLLERFAN_PIN)  //(�ǻ)

#if defined(FAN_PIN)
  #if CONTROLLERFAN_PIN == FAN_PIN
    #error "You cannot set CONTROLLERFAN_PIN equal to FAN_PIN"
  #endif
#endif

unsigned long lastMotor = 0; //Save the time for when a motor was turned on last
unsigned long lastMotorCheck = 0;

void controllerFan()
{
  if ((millis() - lastMotorCheck) >= 2500) //Not a time critical function, so we only check every 2500ms
  {
    lastMotorCheck = millis();

    if(!READ(X_ENABLE_PIN) || !READ(Y_ENABLE_PIN) || !READ(Z_ENABLE_PIN) || (soft_pwm_bed > 0)
    #if EXTRUDERS > 2
       || !READ(E2_ENABLE_PIN)
    #endif
    #if EXTRUDER > 1
      #if defined(X2_ENABLE_PIN) && X2_ENABLE_PIN > -1
       || !READ(X2_ENABLE_PIN)
      #endif
       || !READ(E1_ENABLE_PIN)
    #endif
       || !READ(E0_ENABLE_PIN)) //If any of the drivers are enabled...
    {
      lastMotor = millis(); //... set time to NOW so the fan will turn on
    }

    if ((millis() - lastMotor) >= (CONTROLLERFAN_SECS*1000UL) || lastMotor == 0) //If the last time any driver was enabled, is longer since than CONTROLLERSEC...
    {
        digitalWrite(CONTROLLERFAN_PIN, 0);
        analogWrite(CONTROLLERFAN_PIN, 0);
    }
    else
    {
        // allows digital or PWM fan output to be used (see M42 handling)
        digitalWrite(CONTROLLERFAN_PIN, CONTROLLERFAN_SPEED);
        analogWrite(CONTROLLERFAN_PIN, CONTROLLERFAN_SPEED);
    }
  }
}
#endif

#ifdef TEMP_STAT_LEDS  //���ǻ��
static bool blue_led = false;
static bool red_led = false;
static uint32_t stat_update = 0;

void handle_status_leds(void) {
  float max_temp = 0.0;
  if(millis() > stat_update) {
    stat_update += 500; // Update every 0.5s
    for (int8_t cur_extruder = 0; cur_extruder < EXTRUDERS; ++cur_extruder) {
       max_temp = max(max_temp, degHotend(cur_extruder));
       max_temp = max(max_temp, degTargetHotend(cur_extruder));
    }
    #if defined(TEMP_BED_PIN) && TEMP_BED_PIN > -1
      max_temp = max(max_temp, degTargetBed());
      max_temp = max(max_temp, degBed());
    #endif
    if((max_temp > 55.0) && (red_led == false)) {
      digitalWrite(STAT_LED_RED, 1);
      digitalWrite(STAT_LED_BLUE, 0);
      red_led = true;
      blue_led = false;
    }
    if((max_temp < 54.0) && (blue_led == false)) {
      digitalWrite(STAT_LED_RED, 0);
      digitalWrite(STAT_LED_BLUE, 1);
      red_led = false;
      blue_led = true;
    }
  }
}
#endif

//��������Ծ״̬
//����Ϊ���ڴ�ӡ��ɺ�رո������в������ӽ��ܽǶȿ��ǣ�������ӡ�����в��ó���ִ�иú�����״����
//Ŀǰ�ڼ���ʱ����ָ�״��������취���ʵ��Ӵ�stepper_inactive_time��ֵ
void manage_inactivity(void) {
  //����ָ��ڲ���Ծ״̬��ʱ��
	if( (millis() - previous_millis_cmd) >  max_inactive_time ) {
    if(max_inactive_time) kill();
	}
	//����������ڲ���Ծ״̬��ʱ��
	if(stepper_inactive_time) {
		if( (millis() - previous_millis_cmd) >  stepper_inactive_time ) {
			if(blocks_queued() == false) {
				disable_x();
				disable_y();
				disable_z();
				disable_e0();
				disable_e1();
		    //disable_e2();
		  }
	  }
	}
  #if defined(KILL_PIN)  //���ǻ��
    if( 0 == READ(KILL_PIN) )
      kill();
  #endif
  #if defined(CONTROLLERFAN_PIN) //���ǻ��
    controllerFan(); //Check if fan should be turned on to cool stepper drivers down
  #endif
	check_axes_activity(); //�����Ļ�Ծ��
}

//�ر��жϣ��رռ��ȣ��رղ������
void kill(void) {
  CRITICAL_SECTION_START; // Stop interrupts
  disable_heater();

  disable_x();
  disable_y();
  disable_z();
  disable_e0();
  disable_e1();
 // disable_e2();

#if defined(PS_ON_PIN)  //���ǻ��
  //pinMode(PS_ON_PIN,INPUT);
#endif  
  SERIAL_ERROR_START;
  printf(MSG_ERR_KILLED);
  printf("\n");
  //suicide();
  while(1) { /* Intentionally left empty */ } // Wait for reset
}

void Stop(void) {
  disable_heater();
  if(Stopped == false) {
    Stopped = true;
    Stopped_gcode_LastN = gcode_LastN; // Save last g_code for restart
    SERIAL_ERROR_START;
    printf(MSG_ERR_STOPPED);
  }
}

bool IsStopped(void) { return Stopped; }

bool setTargetedHotend(int code) {  //�趨�ȶ˵�Ŀ���¶�
  tmp_extruder = active_extruder;
  if(code_seen('T')) {  //��ȡ�����ַ�T 
    tmp_extruder = code_value(); //��ȡT��������֣���ΪҪ�趨�¶ȵ���ͷ���
    if(tmp_extruder >= EXTRUDERS) {
      SERIAL_ECHO_START;
      switch(code) {
        case 104:
          printf(MSG_M104_INVALID_EXTRUDER);
          break;
        case 105:
          printf(MSG_M105_INVALID_EXTRUDER);
          break;
        case 109:
          printf(MSG_M109_INVALID_EXTRUDER);
          break;
        case 218:
          printf(MSG_M218_INVALID_EXTRUDER);
          break;
      }
      printf("%d\n",tmp_extruder);
      return true;
    }
  }
  return false ;
}