#define EO_STRING_MARKER 0xFF

void eeprom_clear_completely(){
    for (int i = 0; i < EEPROM_SIZE; ++i) {
        EEPROM.write(i, 0x00);
    }
}

void eeprom_clear_at_index(uint8_t start_idx, uint8_t bytes_len){
    for (int i = start_idx; i < (start_idx+bytes_len); ++i) {
        EEPROM.write(i, 0x00);
    }
}

void eeprom_write_string(uint8_t start_idx, uint8_t bytes_len, String &text){
    eeprom_clear_at_index(start_idx, bytes_len);

    uint8_t loop_write_len = text.length();
    if (loop_write_len >= bytes_len-1){ // Check if len of String exceeds the specified max write len
        loop_write_len = bytes_len-1; // If so, only write String bytes until desired len is reached
    }

    uint8_t last_byte_idx = 0;
    for (unsigned int i = start_idx; i < (start_idx+loop_write_len); ++i){
        EEPROM.write(i, text[i-start_idx]);
        last_byte_idx = i;
    }
    EEPROM.write(last_byte_idx+1, EO_STRING_MARKER); // Writing EO_STRING_MARKER Byte to mark the end of String
}

void eeprom_read_string(uint8_t start_idx, uint8_t bytes_len, String &text){
    for (int i = start_idx; i < (start_idx+bytes_len); ++i){
        char character = char(EEPROM.read(i));
        if(character != EO_STRING_MARKER){
            if(character >= 0x20 && character <= 0x7E){ // only add Text-ASCII charcters
                text += character;
            }
        }
        else{
            break; // Stop reading the string
        }
    }
}


void eeprom_write_bytearr(uint8_t start_idx, uint8_t bytes_len, uint8_t *bytearr){
    for (int i=0; i < (bytes_len); ++i){
        EEPROM.write(i+start_idx, bytearr[i]);
    }
}

void eeprom_write_float(uint8_t start_idx, uint8_t bytes_len, float &float_value){
    uint8_t value_bytes[4];
    memcpy(&value_bytes, &float_value, 4); // converting float to bytearray
    eeprom_write_bytearr(start_idx, bytes_len, value_bytes);
}

void eeprom_read_bytearr(uint8_t start_idx, uint8_t bytes_len, uint8_t *bytearr){
    for (int i=0; i < (bytes_len); ++i){
        bytearr[i] = uint8_t(EEPROM.read(i+start_idx));
    }
}

void eeprom_read_float(uint8_t start_idx, uint8_t bytes_len, float *float_value){
    uint8_t value_bytes[4];
    eeprom_read_bytearr(start_idx, bytes_len, value_bytes);
    memcpy(&float_value, &value_bytes, 4); // converting bytearray to float
}