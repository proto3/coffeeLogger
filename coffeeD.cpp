#include <MFRC522.h>
#include "PersistentCoffeeData.h"

#define MASTER_KEY 0x257AEE75
#define TICKS_TO_WAIT 5
#define TICK_MS_DURATION 30

MFRC522 mfrc522(10, 9);
PersistentCoffeeData *users;
long int last_uid = 0;
int ticks = 0;

//----------------------------------------------------------------------------//
void add_coffee(long int uid)
{
    int user = users->find(uid);
    if(user < 0)
    {
        user = users->create_user(uid);
        if(user < 0)
            return;
    }
    users->add_coffee(user);

    char name[10];
    users->get_name(user, name);
    int coffee = users->get_coffee(user);
    Serial.print(name);
    Serial.print(" : ");
    Serial.println(coffee);
}
//----------------------------------------------------------------------------//
void setup()
{
    SPI.begin();
    mfrc522.PCD_Init();
    Serial.begin(9600);
    users = new PersistentCoffeeData();
}
//----------------------------------------------------------------------------//
void loop()
{
    if(!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
    {
        if(ticks < TICKS_TO_WAIT)
        {
            ticks++;
        }
        else if(last_uid != 0)
        {
            Serial.println("ready");
            last_uid = 0;
        }
        delay(TICK_MS_DURATION);
        return;
    }

    long int uid;
    memcpy((void*)&uid, (void*)(&(mfrc522.uid.uidByte)), 4);

    if(uid != last_uid)
    {
        if(ticks == TICKS_TO_WAIT)
        {
            if(uid == MASTER_KEY)
            {
                Serial.println("RESET");
                users->coffee_reset();
            }
            else
            {
                add_coffee(uid);
            }
            last_uid = uid;
            ticks = 0;
        }
        else
        {
            ticks++;
        }
    }
    else
    {
        ticks = 0;
    }

    delay(TICK_MS_DURATION);
}
//----------------------------------------------------------------------------//
