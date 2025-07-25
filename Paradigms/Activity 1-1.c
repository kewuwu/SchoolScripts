#include <stdio.h>
#include <stdbool.h>

// Kevin Stephens Activity 1-1

struct userInput {
    int day;
    int month;
    int year;
    float tempF;
};

bool isValidDate(int day, int month, int year) {
    if (day > 31 || month > 12 || year > 2023) {
        return false;
    }
    return true;
}
bool isEarthTemp(float tempF, float minTemp, float maxTemp) {
    if (tempF < minTemp || tempF > maxTemp) {
        return false;
    }
    return true;    
}
void askUser(struct userInput *user) {
    printf("Enter the day: ");
    scanf("%d", &user -> day);
    printf("Enter the month: ");
    scanf("%d", &user -> month);
    printf("Enter the year: ");
    scanf("%d", &user -> year);
    printf("Enter the temperature in Fahrenheit: ");
    scanf("%f", &user -> tempF);
}
float convertToCelsius(float tempF) {
    return (tempF - 32) * 5 / 9;
}

int main() {
    struct userInput user;
    const float LOWER_TEMP = -128.6;
    const float UPPER_TEMP = 134;
    bool isValid = false;

    while(!isValid) {
        askUser(&user);
        if (isValidDate(user.day, user.month, user.year)) {
            isValid = true;
            if (isEarthTemp(user.tempF, LOWER_TEMP, UPPER_TEMP)) {
                printf("THOSE ARE EARTH TEMPS \n");
            }
            else {
                printf("THOSE ARE NOT EARTH TEMPS \n");
            } 
            float tempC = convertToCelsius(user.tempF);
            printf("The temperature in Fahrenheit is: %.2f\n", user.tempF);
            printf("The temperature in Celsius is: %.2f\n", tempC);
        }
        else {
            printf("THOSE ARE NOT VALID DATES \n");
            continue;
        }
    }
    return 0;
}
