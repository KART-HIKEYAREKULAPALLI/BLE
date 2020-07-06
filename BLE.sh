## @file assingment_2.1/problem2.sh
 #  
 #  @brief Bluetooth 
 #       to show gatt service 
 #  
 #   
 #  @author Rekulapalli Karthikeya
 #  @bug no bugs
 #

#
 ######################################################################
 #  
 #
 ######################################################################
 #

#!/bin/bash



#########################################################
#
# Gatt_Show() - shows primary 
#                list of all the available “services” 
#                running on the low energy device.  
#
#
#########################################################

Gatt_Show(){

##interacting with selected device ,connecting to it and 
##see all services and exit from that program
gatttool -b $1 -I << EOF

connect 

primary

exit

EOF

exit

} 
#########################################################
#
# Gatt_Write() -  write to a handle gatt
#
#
#########################################################

Gatt_Write(){
  
  # Printing all handels 
  sudo gatttool -b $1 --char-desc   
  
  echo "Enter Handel Number"
  
  read han
  
  echo "Enter Value"
  
  read value
  
  sudo gatttool -b $1 --char-write -a $han -n $value
  
  exit
}
#########################################################
#
# Gatt_Read() -  read a handle gatt
#
#
#########################################################

Gatt_Read(){

  # Printing all handels 
  sudo gatttool -b $1 --char-desc   
  
  echo "Enter Handel Number"
  
  read han
  
  sudo gatttool -b $1 --char-read -a $han
  
  exit
}
#First kill all unblocks
sudo rfkill unblock all

#checking Bluetooth Status
if [[ $(hciconfig -a | grep UP) ]]; then
		echo "Bluetooth is enabled"
		
else
    sudo hciconfig hci0 up

fi


#To Enable Visibility
sudo hciconfig hci0 piscan


# scanning for low energy devices
sudo hcitool scanon  

read -p "Which Device You want to see gatt Services?(Enter Line No ) " -n 1 device
				let "device += 1"
				echo -e "\n"

#MAC Address of selected Device 
ADDR=$(hcitool scan | sed -n '/'"$device"'/p' | cut -c 2-19);


#To See The Gatt Services on Device 

if [[ "$1" == "show" && "$2" == "gatt" ]] ; then
   
   echo " All Gatt Service "
   Gatt_Show $ADDR

fi


# Read a Service


if [[ "$1" == "read" && "$2" == "gatt" ]] ;then
    echo "Read Service "
        Gatt_Read $ADDR

fi


# Write values to Service 



if [[ "$1" == "write" && "$2" == "gatt" ]]  ;then
    echo "Write To Service"
         Gatt_Write $ADDR

fi

