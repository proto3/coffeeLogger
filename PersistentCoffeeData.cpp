#include "PersistentCoffeeData.h"
#include <stdio.h>
#include <string.h>
#include <EEPROM.h>

#define MAX_USERS 48

#define NAME_LENGTH 10

#define USER_SIZE            16
#define EXIST_PADDING        0
#define UID_PADDING          1
#define NAME_PADDING         5
#define COFFEE_PADDING 15

//----------------------------------------------------------------------------//
void PersistentCoffeeData::hard_reset()
{
    for(int i=0;i<MAX_USERS;i++)
        if(get_exist(i))
            set_exist(i, false);
}
//----------------------------------------------------------------------------//
void PersistentCoffeeData::coffee_reset()
{
    for(int i=0;i<MAX_USERS;i++)
        if(get_exist(i))
            set_coffee(i, 0);
}
//----------------------------------------------------------------------------//
int PersistentCoffeeData::find(long int uid)
{
    for(int i=0;i<MAX_USERS;i++)
    {
        if(get_exist(i) && get_uid(i) == uid)
            return i;
    }
    return -1;
}
//----------------------------------------------------------------------------//
int PersistentCoffeeData::create_user(long int uid)
{
    for(int i=0;i<MAX_USERS;i++)
    {
        if(!get_exist(i))
        {
            char default_name[NAME_LENGTH] = "user_";
            char index_str[4];
            sprintf(index_str, "%d", i+1);
            strcat(default_name, index_str);

            set_uid(i, uid);
            set_name(i, default_name);
            set_coffee(i, 0);
            set_exist(i, true);

            return i;
        }
    }
    return -1;
}
//----------------------------------------------------------------------------//
void PersistentCoffeeData::add_coffee(int user_index)
{
    if(EEPROM[user_index*USER_SIZE + COFFEE_PADDING] < 255)
        EEPROM[user_index*USER_SIZE + COFFEE_PADDING]++;
}
//----------------------------------------------------------------------------//
bool PersistentCoffeeData::get_exist(int user_index)
{
    return EEPROM[user_index*USER_SIZE + EXIST_PADDING];
}
//----------------------------------------------------------------------------//
long int PersistentCoffeeData::get_uid(int user_index)
{
    long int uid;
    unsigned char* buffer = (unsigned char*)&uid;
    for(int i=0;i<4;i++)
        buffer[i] =EEPROM[user_index*USER_SIZE + UID_PADDING + i];
    return uid;
}
//----------------------------------------------------------------------------//
void PersistentCoffeeData::get_name(int user_index, char* name)
{
    //unsigned char* buffer = (unsigned char*)name;
    for(int i=0;i<NAME_LENGTH;i++)
        name[i] = EEPROM[user_index*USER_SIZE + NAME_PADDING + i];
}
//----------------------------------------------------------------------------//
unsigned char PersistentCoffeeData::get_coffee(int user_index)
{
    return EEPROM[user_index*USER_SIZE + COFFEE_PADDING];
}
//----------------------------------------------------------------------------//
void PersistentCoffeeData::set_exist(int user_index, bool exist)
{
    EEPROM[user_index*USER_SIZE + EXIST_PADDING] = exist;
}
//----------------------------------------------------------------------------//
void PersistentCoffeeData::set_uid(int user_index, long int uid)
{
    unsigned char* buffer = (unsigned char*)&uid;
    for(int i=0;i<4;i++)
        EEPROM[user_index*USER_SIZE + UID_PADDING + i] = buffer[i];
}
//----------------------------------------------------------------------------//
void PersistentCoffeeData::set_name(int user_index, char* name)
{
    //TODO check if buffer is necessary or directly name is ok
    //unsigned char* buffer = (unsigned char*)name;
    for(int i=0;i<NAME_LENGTH;i++)
        EEPROM[user_index*USER_SIZE + NAME_PADDING + i] = name[i];
}
//----------------------------------------------------------------------------//
void PersistentCoffeeData::set_coffee(int user_index, unsigned char coffee)
{
    EEPROM[user_index*USER_SIZE + COFFEE_PADDING] = coffee;
}
//----------------------------------------------------------------------------//
