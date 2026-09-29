#include <LiquidCrystal_I2C.h> // Library for LCD

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 columns and 2 rows

#define TdsSensorPin A1
#define VREF 5.0              // Analog reference voltage (Volt) of the ADC
#define SCOUNT 30             // Number of sample points

int analogBuffer[SCOUNT];     // Store analog values read from ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0, copyIndex = 0;

float averageVoltage = 0, tdsValue = 0, temperature = 25;

void setup()
{
    Serial.begin(115200);

    pinMode(TdsSensorPin, INPUT);

    lcd.init();               // Initialize the LCD
    lcd.backlight();

    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
}

void loop()
{
    static unsigned long analogSampleTimepoint = millis();

    // Read analog value every 40 milliseconds
    if (millis() - analogSampleTimepoint > 40U)
    {
        analogSampleTimepoint = millis();

        analogBuffer[analogBufferIndex] = analogRead(TdsSensorPin);

        analogBufferIndex++;

        if (analogBufferIndex == SCOUNT)
            analogBufferIndex = 0;
    }

    static unsigned long printTimepoint = millis();

    // Calculate and display TDS every 800 milliseconds
    if (millis() - printTimepoint > 800U)
    {
        printTimepoint = millis();

        for (copyIndex = 0; copyIndex < SCOUNT; copyIndex++)
        {
            analogBufferTemp[copyIndex] = analogBuffer[copyIndex];
        }

        // Median filtering and conversion to voltage
        averageVoltage =
            getMedianNum(analogBufferTemp, SCOUNT)
            * (float)VREF / 1024.0;

        // Temperature compensation
        float compensationCoefficient =
            1.0 + 0.02 * (temperature - 25.0);

        float compensationVoltage =
            averageVoltage / compensationCoefficient;

        // Convert voltage to TDS value
        tdsValue =
            (133.42 * compensationVoltage * compensationVoltage * compensationVoltage
            - 255.86 * compensationVoltage * compensationVoltage
            + 857.39 * compensationVoltage) * 0.5;

        // Serial Monitor output
        Serial.print("TDS Value:");
        Serial.print(tdsValue, 0);
        Serial.println("ppm");

        // LED indication
        if (tdsValue > 1 && tdsValue < 50)
        {
            digitalWrite(6, HIGH);
            delay(2000);
        }
        else if (tdsValue == 0)
        {
            digitalWrite(9, HIGH);
            delay(2000);
        }
        else if (tdsValue >= 50)
        {
            digitalWrite(7, HIGH);
            delay(2000);
        }
        else
        {
            digitalWrite(8, HIGH);
            delay(2000);
        }

        // LCD display
        lcd.setCursor(0, 0);
        lcd.print("__TDS Value : ");
        lcd.print(tdsValue, 0);

        lcd.setCursor(0, 1);
        lcd.print("Water Quality");
    }
}

// Median filtering function
int getMedianNum(int bArray[], int iFilterLen)
{
    int bTab[iFilterLen];

    for (byte i = 0; i < iFilterLen; i++)
        bTab[i] = bArray[i];

    int i, j, bTemp;

    for (j = 0; j < iFilterLen - 1; j++)
    {
        for (i = 0; i < iFilterLen - j - 1; i++)
        {
            if (bTab[i] > bTab[i + 1])
            {
                bTemp = bTab[i];

                bTab[i] = bTab[i + 1];

                bTab[i + 1] = bTemp;
            }
        }
    }

    if ((iFilterLen & 1) > 0)
    {
        bTemp = bTab[(iFilterLen - 1) / 2];
    }
    else
    {
        bTemp =
            (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
    }

    return bTemp;
}
