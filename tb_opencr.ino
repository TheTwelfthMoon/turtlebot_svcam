#include <DynamixelWorkbench.h>
#include <ros.h>
#include <std_msgs/Bool.h>
#include <std_msgs/String.h>

#if defined(__OPENCM904__)
  #define DEVICE_NAME "3" //Dynamixel on Serial3(USART3)  <-OpenCM 485EXP
#elif defined(__OPENCR__)
  #define DEVICE_NAME ""
#endif   

#define BAUDRATE  1000000
#define DXL_ID1    1
#define DXL_ID2    2

DynamixelWorkbench dxl_wb;


bool result;

uint8_t dxl_id1 = DXL_ID1;
uint8_t dxl_id2 = DXL_ID2;
uint16_t model_number = 0;

int count;

bool rotate;
bool start;

std_msgs::Bool mov;
std_msgs::Bool st;


void messageCb(const std_msgs::Bool &mov) {
  rotate = mov.data;
}

void mesCb(const std_msgs::Bool &st) {
  start = st.data;
  
  
}

ros::NodeHandle nh;
ros::Subscriber<std_msgs::Bool> subCMD("move", messageCb);
ros::Subscriber<std_msgs::Bool> subCM("move_s", mesCb);
//scan_found = rclcpp::wait_for_message(message, this->shared_from_this(),"/scan",std::chrono::seconds(1));

void setup() {
  nh.initNode();
  nh.getHardware()->setBaud(BAUDRATE);
  nh.subscribe(subCMD);
  nh.subscribe(subCM);
  

  
  // put your setup code here, to run once:
  Serial.begin(57600);
//  while(!Serial); // Wait for Opening Serial Monitor

  const char *log;
  
  result = dxl_wb.init(DEVICE_NAME, BAUDRATE, &log);
  if (result == false)
  {
    Serial.println(log);
    Serial.println("Failed to init");
  }
  else
  {
    Serial.print("Succeeded to init : ");
    Serial.println(BAUDRATE);  
  }

  result = dxl_wb.ping(dxl_id1, &model_number, &log);
  if (result == false)
  {
    Serial.println(log);
    Serial.println("Failed to ping");
  }
  else
  {
    Serial.println("Succeeded to ping");
    Serial.print("id : ");
    Serial.print(dxl_id1);
    Serial.print(" model_number : ");
    Serial.println(model_number);
  }
  result = dxl_wb.ping(dxl_id2, &model_number, &log);
  if (result == false)
  {
    Serial.println(log);
    Serial.println("Failed to ping");
  }
  else
  {
    Serial.println("Succeeded to ping");
    Serial.print("id : ");
    Serial.print(dxl_id2);
    Serial.print(" model_number : ");
    Serial.println(model_number);
  }
  
  result = dxl_wb.wheelMode(dxl_id1, 0, &log);
  if (result == false)
  {
    Serial.println(log);
    Serial.println("Failed to change wheel mode");
  }
  result = dxl_wb.wheelMode(dxl_id2, 0, &log);
  if (result == false)
  {
    Serial.println(log);
    Serial.println("Failed to change wheel mode");
  }
}

void loop() {
//    Serial.print("rotate");
//    Serial.println(rotate);
//    Serial.print("start");
//    Serial.println(start);
//    Serial.print(pc);
  if (start == true){
    if(rotate == false){
      dxl_wb.goalVelocity(dxl_id1, (int32_t)50);
      dxl_wb.goalVelocity(dxl_id2, (int32_t)-50);
    }
    else if (rotate == true){
      dxl_wb.goalVelocity(dxl_id1, (int32_t)50);
      dxl_wb.goalVelocity(dxl_id2, (int32_t)50);
    }
    
  }
  else if(start == false){
      dxl_wb.goalVelocity(dxl_id1, (int32_t)0);
      dxl_wb.goalVelocity(dxl_id2, (int32_t)0);
  }

  nh.spinOnce();
  delay(10);
    
}
