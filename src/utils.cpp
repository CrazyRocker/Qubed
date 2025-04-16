#include "utils.hpp"
#include <iostream>
#include <string>

namespace owo{

uint8_t input_get_uint8_t(const std::string& prompt, uint8_t defaultValue){
    std::string input;
    while(true){
    uint16_t value = 0;     //This will be returned at the end after casting to a uint8_t if \n, \r or \t is not entered and the input is a properly formatted number
    std::cout<<prompt;
    std::getline(std::cin, input);
    const auto firstDigitCharIndex = input.find_first_not_of(" \n\r\t");

    if (firstDigitCharIndex == std::string::npos){       //For handling empty input
        return defaultValue;
    }

    if(std::isdigit(input[firstDigitCharIndex])){
        value += input[firstDigitCharIndex] - '0';      //Getting the first number inside value after converting the character to a number
        bool validInput = true;

        if(input.length() == firstDigitCharIndex+1){
            return value;
        }
        for(int i = firstDigitCharIndex + 1; i < input.length(); i++){        //Starting at +1 index, since we have already checked if input[firstUsefulChar] is a digit
            char ch = input[i];
            if(std::isdigit(ch)) {
                value = value * 10 + (ch - '0');

                if(value>UINT8_MAX){
                    std::cout<<"Oops! The number you have entered is too large, only numbers from 0-255 are allowed, please try again!\n";
                    validInput=false; break;
                }
            }
            else if(ch==' ') {
                return value;
            } 
            else {
                std::cout<<"Oops! Only numbers from 0-255 are allowed, please try again!\n";
                validInput=false; break;
            }
            if(validInput)
                return value;
        }
    }
    else{
        std::cout<<"You've entered a character instead of a number, please try again!\n";
        continue;
    }
    }

    //No need to return here, because the while loop above can only be exitted with returning something
}

uint8_t input_get_uint8_t_and_confirm(const std::string& prompt, uint8_t defaultValue){
    while(true){
        uint8_t value = input_get_uint8_t(prompt, defaultValue);

        std::cout<<"Are you sure your choice is "<<value<<"? (Y/n) :";

        std::string input;
        std::getline(std::cin, input);

        const auto charIndex = input.find_first_not_of(" \n\r\t");
        if(charIndex == std::string::npos || charIndex == 'y' || charIndex == 'Y'){
            return value;
        }

        std::cout<<"Alright! Trying again!\n";
    }
}

}