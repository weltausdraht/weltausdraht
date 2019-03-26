#include <Arduino.h>

const uint8_t m_outputsCount = 8;
const uint8_t m_outputs[m_outputsCount] = {12, 11, 10, 9, 8, 7, 6, 5};

const uint8_t m_inputsCount = 2;
const uint8_t m_inputs[m_inputsCount] = {5, 4};
      int     m_inputStates[m_inputsCount];

      uint8_t m_countValue;
      uint8_t m_delay;


/*  lookup table for controlling seven segement led display
 *
 *     a
 *    ---
 * f | g | b
 *    ---
 * e |   | c
 *    --- 
 *     d
 * 
 * 
 * digit     a b c d e f g  
 *  0     -> 1 1 1 1 1 1 0
 *  1     -> 0 1 1 0 0 0 0
 *  2     -> 1 1 0 1 1 0 1
 *  3     -> 1 1 1 1 0 0 1
 *  4     -> 0 1 1 0 0 1 1
 *  5     -> 1 0 1 1 0 1 1
 *  6     -> 1 0 1 1 1 1 1
 *  7     -> 1 1 1 0 0 0 0
 *  8     -> 1 1 1 1 1 1 1
 *  9     -> 1 1 1 1 0 1 1
 */

const uint8_t m_sevenLedLUT[10] = {0b01111110,  // 0
                                   0b00110000,  // 1
                                   0b01101101,  // 2
                                   0b01111001,  // 3
                                   0b00110011,  // 4
                                   0b01011011,  // 5
                                   0b01011111,  // 6
                                   0b01110000,  // 7
                                   0b01111111,  // 8
                                   0b01111011}; // 9
                                    


void setup()
{

  for(uint8_t i = 0; i < m_outputsCount; i++) {
    pinMode(m_outputs[i], OUTPUT);
  }

  for(uint8_t i = 0; i < m_inputsCount; i++) {
    pinMode(m_inputs[i], INPUT);
  }

  m_countValue = 0;
}

void digitToSevenLed(const uint8_t digit)
{
  if(digit > 9) {
    return;
  }

  uint8_t pattern = m_sevenLedLUT[digit];

  for(uint8_t i = 0; i < 7; i++) {

    if(pattern & 0x01 == 1) {
      digitalWrite(m_outputs[i], HIGH);
    } else {
      digitalWrite(m_outputs[i], LOW);
    }

    pattern = pattern >> 1;
  }
}

void loop()
{
  uint8_t curValue;
  
  int i0_new = digitalRead(m_inputs[0]);
  int i1_new = digitalRead(m_inputs[1]);

  if(m_inputStates[0] != i0_new && m_inputStates[1] != i1_new) {

    m_inputStates[0] = i0_new;
    m_inputStates[1] = i1_new;

    m_delay++;
    if(m_delay == 0) {
      m_countValue++;
    }

    if(m_inputStates[0] == LOW && m_inputStates[1] == LOW) {
      curValue = m_countValue / 1000;
    } else if(m_inputStates[0] == HIGH && m_inputStates[1] ==  LOW) {
      curValue = m_countValue / 100;
    } else if(m_inputStates[0] ==  LOW && m_inputStates[1] == HIGH) {
      curValue = m_countValue / 10;
    } else if(m_inputStates[0] == HIGH && m_inputStates[1] == HIGH) {
      curValue = m_countValue;
    }

    digitToSevenLed(curValue % 10);
  } 
}