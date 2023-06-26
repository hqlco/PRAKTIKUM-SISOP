#!/bin/bash
#greetings
echo Hello Buddy!! 
echo Welcome To The Register
echo "-------------"

#input username
echo Input Username :
read username

#input password
echo Input Password :
read -s password

#check if users.txt and log.txt file already exist
if [ ! -f "users.txt" ]; then
touch users.txt
fi
if [ ! -f "log.txt" ]; then
touch log.txt
fi

#check if user has ever registered before, program will stopped if username already exist
if grep -q "$username" users.txt; then
        echo -e "\n-------------"
        echo "ERROR User already exist"
        echo "$(date '+%Y/%m/%d %H:%M:%S') REGISTER: ERROR User already exists" >> log.txt
        echo -e "\n-------------"
elif echo "$username" | grep -q  " "; then
        echo -e "\n-------------"
        echo "ERROR Username can not contains space"
        echo "$(date '+%Y/%m/%d %H:%M:%S') REGISTER: ERROR Username can not contains space" >> log.txt
        echo -e "\n-------------"
else
#check if password not same as username 
        if [[ ( "^$password$" =~ "^$username$") ]]; then
        echo -e "\n-------------"
        echo "ERROR Password cannot like username, please try again with another username" 
        echo "$(date '+%Y/%m/%d %H:%M:%S') REGISTER: ERROR User $username password cannot like username" >> log.txt
        echo -e "\n------------"

        else
                #if password valid
                if [[ "$password" =~ [0-9A-Za-z]{8,} ]]; then
                        if [[ ! "$password" =~ [0-9] || ! "$password" =~ [A-Z] || ! "$password" =~ [a-z] ]]; then
                                echo -e "\n-------------"
                                echo "Password not valid!"
                                echo "ERROR: Password must contain at least one numeric, uppercase and lowercase letter"
                                echo "$(date '+%Y/%m/%d %H:%M:%S') REGISTER: ERROR User $username password must contain at least one numeric, uppercase and lowercase letter" >> log.txt
                                echo -e "\n-------------"
                        elif echo "$password" | grep -q -i -E "chicken|ernie"; then
                                echo -e "\n-------------"
                                echo "Password not valid!"
                                echo "ERROR: Password must not contain "ernie or chicken"!"
                                echo "$(date '+%Y/%m/%d %H:%M:%S') REGISTER: ERROR User $username password must not contain "ernie or chicken"!" >> log.txt
                                echo -e "\n-------------"
                        else
                                echo -e "\n-------------"
                                echo "Registered successfully"
                                echo "$username""-""$password" >> users.txt
                                echo "$(date '+%Y/%m/%d %H:%M:%S') REGISTER: INFO User $username registered successfully" >> log.txt
                                echo -e "\n-------------"
                        fi
                    #if password not valid
                else
                        echo -e "\n-------------"
                        echo "Password not valid!"
                        echo "ERROR: Password must contain at least 8 characters!"
                        echo "$(date '+%Y/%m/%d %H:%M:%S') REGISTER: INFO User $username password must contain at least 8 characters!" >> log.txt
                        echo -e "\n-------------"
                fi
        fi
fi
