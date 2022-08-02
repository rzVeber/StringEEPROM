#include <EEPROM.h>

// #define FLAG_CHAR_ 19
// #define CLEAR_CHAR_ 12



class SEEPROM {
  private:
    char FLAG_CHAR_ = 19;
    char CLEAR_CHAR_ = 12;
    uint32_t lenth;
    uint32_t start_pos;
    uint32_t p;
    uint32_t pos;
    uint32_t cell;
  public:
    SEEPROM(uint32_t LENTH = 512, uint32_t START_POS = 8,  uint32_t CELL = 5) { //(количество байт eeprom[по умлчанию 512],количество зарезервированных ячеек для переменных[по умолчанию 8],количество ячеек под слова[по умолчанию 6])
      start_pos = START_POS;
      pos = start_pos;
      p = start_pos - 1;
      lenth = LENTH;
      cell = CELL;
    }
    String read(uint32_t g) {
   	  begin();
      String slowar[cell];
      p = start_pos-1;
      for (uint32_t z = 0; z < cell; z++)
        slowar[z] = read_eeprom();
      end();
      return slowar[g];
    }
    String internal_read(uint32_t g) {
      String slowar[cell];
      p = start_pos-1;
      for (uint32_t z = 0; z < cell; z++)
        slowar[z] = read_eeprom();
      return slowar[g];
    }
    void write(String slovo , uint32_t g) {
      begin();
      String vocabulary[cell];
      for (uint32_t i = 0; i < cell; i++)
        vocabulary[i] = internal_read(i);
      vocabulary[g] = slovo;
      pos = start_pos;
      // for (uint32_t i = start_pos+1; i < lenth ; i++) {
      //   EEPROM.write(i, 255);	
      // }
      for (uint32_t i = 0; i < cell; i++) {
        write_eeprom(vocabulary[i]);
      }
      end();
    }
    String read_eeprom() {  //считывает данные из eeprom
      String val = "";  //обнуляем строку
      bool c = false; //для определения ошибки или того что слово полученно
      while (true) {  //главный цыкл
        p++;                              //алгоритм поиска флажка
        p = constrain(p, start_pos-1, lenth - 1);   //
        if (p == lenth - 1) {           //если флажек не найдет возвращает сообщение о ошибке
          val = "Error";                //сообщение о ошибке
          break;                        //
        }                               //
        if (EEPROM.read(p) == FLAG_CHAR_) {        //ищет флажок начала и запускает алгоритм считывания если флажек найдет
          p++;                            //
          p = constrain(p, 0, lenth - 1); //
          while (true) {  //цыкл алгоритма считывания
            if (p == (lenth-1)) { //при переполнении выходим из цыкла
              break;        //
            }               //
            if (EEPROM.read(p) == FLAG_CHAR_) {  //ищет флажек концы и выходит из цыкла и ставит флажек что слово полученно
              c = true;                 //поднимает флажек получения
              p--;                      //
              break;                    //
            }                           //
            val += (char)EEPROM.read(p) ;   //считывает слово
            p++;                            //
            p = constrain(p, 0, lenth - 1); //
          }               //
        }
        if (c) {  //если находит флажек получения выходит из главного цыкла
          break;  //
        }         //
      }
      return val; //возвращает строку
    }
    bool write_eeprom(String slowo) { //функцыя принимает слово и записывает eeprom
      if (pos != start_pos) {           //для правильной работы флажков
        pos--;                          //
        pos = constrain(pos, start_pos, lenth); //
      }                                 //
      EEPROM.write(pos, FLAG_CHAR_);           //флажок в начале строки
      pos++;                          //
      pos = constrain(pos, 0, lenth); //
      for (uint32_t i = 0; i < slowo.length(); i++) {  //главный цыкл, разбирает слово на эллементы и записывает в eeprom
        byte var = (byte)slowo[i];                //
        EEPROM.write(pos, var);                   //
        pos++;                                    //
        pos = constrain(pos, 0, lenth);           //
      }                                           //
      EEPROM.write(pos, FLAG_CHAR_);           //добавляет флажок в конец слова
      pos++;                          //                 
      pos = constrain(pos, 0, lenth); //
      if (pos == (lenth-1)) //возвращает 1 при переполнении и 0 если переполнения нет
        return true;  //
      else            //
        return false; //
    }
    bool write_eeprom() { //функцыя предназначена для проверки переполнения
      if (pos == (lenth-1)) //возвращает 1 при переполнении и 0 если переполнения нет
        return true;  //
      else            //
        return false; //
    }
    bool restart() { //функцыя обновления eeprom, принимает количесво однотипных слов которое будет записанно в eeprom (по умолчанию 0)
    begin();
      const char RESTART[] = "NULL";  //слово которое будет записанно в eeprom n-ное количество раз
      for (uint32_t i = start_pos+1; i < lenth ; i++) {//стирает все из eeprom
        EEPROM.write(i, CLEAR_CHAR_);         //
      }
      if (cell != 0) {
        for (uint32_t i = 0; i < cell; i++) { //основной цыкл, вылоняется n-ное количество раз
          EEPROM.write(pos, FLAG_CHAR_);              //добавляет флажок в начало строки
          pos++;                              //
          pos = constrain(pos, 0, lenth - 1); //
          for (uint32_t o = 0; o < sizeof(RESTART) - 1; o++) { //разбирает строку на отдельные эллементы и записывает в eeprom
            char buf=RESTART[o];
            EEPROM.write(pos, buf);               //
            pos++;                                        //
            pos = constrain(pos, 0, lenth - 1);           //
          }
        }                             //
        EEPROM.write(pos, FLAG_CHAR_);                //добавляет флажок в конец записанной строки
        pos++;                                //
        pos = constrain(pos, 0, lenth - 1);   //
      }
      end();
      if (pos == (lenth-1)) //возвращает 1 при переполнении и 0 если переполнения нет
        return true;  //
      else            //
        return false; //
    }
    void writeInt(int16_t eep, int16_t addr = 0) {
  		addr = constrain(addr, 0, EEPROM.length());
  		int8_t eep0 = (int8_t) eep;
  		int8_t eep1 = (int8_t) (eep >> 8);
  		EEPROM.write(0 + addr, eep0);
  		EEPROM.write(1 + addr, eep1);
	}
	int32_t readInt(int16_t addr = 0) {
		begin();
  		addr = constrain(addr, 0, EEPROM.length());
  		int16_t eep;
  		int16_t eep0 = EEPROM.read(0 + addr);
  		int16_t eep1 = EEPROM.read(1 + addr);
  		eep = (eep1 << 8) | eep0;
  		return eep;
	}	
    void writeLong(int32_t eep, int16_t addr = 0) {
  		addr = constrain(addr, 0, EEPROM.length());
  		int16_t eep0 = (int16_t) eep;
  		int16_t eep1 = (int16_t) (eep >> 16);
  		int8_t eep00 = (int8_t) eep0;
  		int8_t eep01 = (int8_t) (eep0 >> 8);
  		int8_t eep10 = (int8_t) eep1;
  		int8_t eep11 = (int8_t) (eep1 >> 8);
  		EEPROM.write(0 + addr, eep00);
  		EEPROM.write(1 + addr, eep01);
  		EEPROM.write(2 + addr, eep10);
  		EEPROM.write(3 + addr, eep11);
	}
	int32_t readLong(int16_t addr = 0) {
		begin();
  		addr = constrain(addr, 0, EEPROM.length());
  		int32_t eep;
  		int32_t eep0;
  		int32_t eep1;
  		int16_t eep00 = EEPROM.read(0 + addr);
  		int16_t eep01 = EEPROM.read(1 + addr);
  		int16_t eep10 = EEPROM.read(2 + addr);
  		int16_t eep11 = EEPROM.read(3 + addr);
  		eep0 = (eep01 << 8) | eep00;
  		eep1 = (eep11 << 8) | eep10;
  		eep = (eep1 << 16) | eep0;
  		return eep;
	}
    void begin(){
    	EEPROM.begin(lenth);
    }
    void submit(){
    	EEPROM.end();
    	EEPROM.begin(lenth);
    }
    void end(){
    	EEPROM.end();
    }
};