#!/bin/bash
#greetings

echo Login Page
echo "-------------"

#input username
echo Input Username :
read username

#input password
echo Input Password :
read -s password

data_input="$username-$password"

        if ! grep -q "$username" users.txt; then
                echo -e "\n-------------"
                echo "ERROR User not exist"
                echo "$(date '+%Y/%m/%d %H:%M:%S') LOGIN: ERROR User $username  not exist" >> log.txt
                echo -e "\n-------------"
        elif grep -q "^$data_input$" users.txt; then
                echo -e "\n------------"
                echo "Logged in" 
                echo "$(date '+%Y/%m/%d %H:%M:%S') LOGIN: INFO User $username logged in" >> log.txt
                echo -e "\n------------"
        else
                echo -e "\n------------"
                echo "ERROR Failed login" 
                echo "$(date '+%Y/%m/%d %H:%M:%S') LOGIN: ERROR Failed login attempt on user $username" >> log.txt
                echo -e "\n------------"
        fi
