#include <Wire.h>
#include "ds3231.h"

#define BUFF_MAX 128

uint8_t time[8];
char recv[BUFF_MAX];
unsigned int recv_size = 0;

void setup()
{
    Serial.begin(9600);
    Wire.begin();
    DS3231_init(DS3231_CONTROL_INTCN);
    memset(recv, 0, BUFF_MAX);
}

void loop()
{
    delay(5000);
    
    char buff[BUFF_MAX];
    char buff2[BUFF_MAX];
    struct ts t;
    
    DS3231_get(&t);

#ifdef CONFIG_UNIXTIME
        snprintf(buff, BUFF_MAX, "%02d/%02d/%d %ld", t.mday, t.mon, t.year t.unixtime);
        snprintf(buff2, BUFF_MAX, "%02d:%02d %ld",t.hour, t.min t.unixtime);
#else
        snprintf(buff, BUFF_MAX, "%02d/%02d/%d ", t.mday, t.mon, t.year);
        snprintf(buff2, BUFF_MAX, "%02d:%02d", t.hour, t.min);
#endif
     Serial.println(buff);
     Serial.println(buff2);
}
