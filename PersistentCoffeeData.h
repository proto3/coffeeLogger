class PersistentCoffeeData
{
public:
    PersistentCoffeeData();
    void reset();

    bool get_exist(int user_index);
    long int get_uid(int user_index);
    void get_name(int user_index, char* name);
    unsigned char get_coffee(int user_index);

    void set_exist(int user_index, bool exist);
    void set_uid(int user_index, long int uid);
    void set_name(int user_index, char* name);
    void set_coffee(int user_index, unsigned char coffee);
};
