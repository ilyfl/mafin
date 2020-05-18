
uint8_t get_str(FILE* stream, char* str);
uint8_t get_digit(FILE* stream, uint8_t left, uint8_t right);
float get_float(FILE* stream);
int get_number(FILE* stream, int left, int right);
uint8_t get_name(char *name);
uint8_t get_username(char **name);
uint8_t get_password(char *password);
double to_default_currency(double value);
