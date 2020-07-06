/** @file assignment_2.2/problem1.c
 *  @brief MQTT client
 *  
 *  an MQTT Client that Subscribes to 2 different topics "config/wifi" & "config/eth"
 *  and configures Wifi and Ethernet when commands are received on these MQTT topics.
 *
 *
 *  @author Rekulapalli Karthikeya
 *  @bug    No known bugs. 
 */

/*
 *#####################################################################
 *  Initialization block
 *  ---------------------
 *  This block contains initialization code for this particular file.
 *  It typically contains Includes, constants or global variables used
 *  throughout the file.
 *#####################################################################
 */

/*Host address to connect to broker which sets on port 1883*/
   #define ADDRESS     "192.168.0.2:1883"

   /*ClientId is required for device creation*/
   #define CLIENTID    "17H51A02A5"

   /*Account username on */
  #define USERNAME    "17H51A02A5"

   /*Account password on */
   #define PASSWORD   "17H51A02A5"
   
   /*Timeout*/
   #define TIMEOUT      10000L


/* --- Standard Includes --- */

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"

/* --- Project Includes --- */
   
#include "MQTTClient.h"

/* MQTTClient_deliveryToken instance*/
volatile MQTTClient_deliveryToken deliveredtoken;

/* 'k' is output of excuted cmd we can use for publishing */
int k;

/* Topic that you subcribe*/
char * const topic[2]= {
    
    "config/wifi",
    "config/eth"

};

/* QOS of topics */
int arr[] = { 1,1 }; 
/* pointer to arr */
int *qos=&arr[0];

/*MQTT Client object*/
 MQTTClient client;

/** 
 *  @brief Function to publish message as 
 *         a MQTT client to the server/broker.
 *  
 *  Takes argument topic and message about command is sucess or not
 *  
 *
 *  @return void, just prints the output. 
 */

void publish(char* topic) {

	/*Initializes new message*/
	MQTTClient_message pubmsg = MQTTClient_message_initializer;

        if(k==-1){
           
        pubmsg.payload = "Error in cmd";
        
        }else{
           
            pubmsg.payload = "Sucess cmd executed";
        
        }      
           

	/*sets the payload*/
	
	pubmsg.payloadlen = strlen(pubmsg.payload);

        /*sets the quality of security*/
	pubmsg.qos = 1;
	pubmsg.retained = 0;

	/*creates deliverytoken variable*/
	MQTTClient_deliveryToken token;

	/*finally publishes message*/
	MQTTClient_publishMessage(client, topic, &pubmsg, &token);
	MQTTClient_waitForCompletion(client, token, TIMEOUT);

	/*prints the delivery message*/
	printf("Message with delivery token %d delivered\n",token);

}

/** 
 *  @brief Function gives a delivery confirmation
 *  
 *  Gives a delivery confirmation with token value
 *
 *  @return void, just prints the output. 
 */

void delivered(void *context, MQTTClient_deliveryToken dt){

    /*prints the delivery message confirmation*/
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}


/** 
 *  @brief Function gets called when there is
 *         loss in connectivity.
 *  
 *  Also gives the reason of loss in connectivity.
 *
 *  @return void, just prints the output. 
 */

void connlost(void *context, char *cause){
    
    printf("\nConnection lost\n");

    /*prints the reason*/
    printf("     cause: %s\n", cause);
}
/** 
 *  @brief Function to check  message from topic and exec
 *         cammands  
 *         
 *  
 *  Takes mssg and topic name
 *  
 *
 *  @return void, just prints the output. 
 */
void Chk_Msg_exe( char *x , char *topic){

char *cmd;

cmd=(char*)malloc(50*sizeof(char));

int len=strlen(x);


int k;
/* check for eth on cmd*/
if(strcmp(x,"ethernet on")==0){

printf("Ethernet operation is being placed and you will get message of status");

cmd="sudo ifconfig eth0 up";

/*exec eth on*/
k=system(cmd);

}else if(strcmp(x,"ethernet off")==0){

printf("Ethernet operation is being placed and you will get message of status");


cmd="sudo ifconfig eth0 down";
/*exec eth off*/
k=system(cmd);

}else if(strcmp(x,"ethernet connect dhcp")==0){

printf("Ethernet operation is being placed and you will get message of status");

cmd="sudo dhclient eth0";

/*exec eth dhcp*/
k=system(cmd);

}else if(strncmp(x,"ethernet connect static ", 24)==0){

printf("Ethernet operation is being placed and you will get message of status");



/* sir i have written things in script to reduce complexity

#!/bin/bash

## this set up a ip address you need 
echo -e "auto eth0:1\niface eth0:1 inet static\n address $1 \n netmask  $2\n network  $3" >> /etc/network/interfaces

##For these new settings to take effect you need to restart networking services
sudo /etc/init.d/networking restart

*/
strcpy(cmd,"./eth ");

int i=0;

for(i=0;i+24 <= len ;i++){

*(cmd+6+i)=x[i+24];

} 

printf("\n%s\n",cmd);


/*exec eth static*/
k=system(cmd);

}else if(strncmp(x,"wifi connect ", 13)==0){

printf("WIFI operation is being placed and you will get message of status");

/* I Have Written a bash script to reduce complexity input it will connect to wifi*/

/* 
#! /bin/bash

sudo ifconfig wlan0 up
sudo iwconfig wlan0 essid $1 key $2
sudo dhclient wlan0 

*/

strcpy(cmd,"./wifi ");

int i=0;

for(i=0;i+12 <= len ;i++){

*(cmd+7+i)=x[i+13];

} 

printf("\n%s\n",cmd);

k=system(cmd);

}else if(strcmp(x,"wifi on")==0){

printf("WIFI operation is being placed and you will get message of status");

strcpy(cmd,"sudo ifconfig wlan0 up");

/*exec wifi on*/
k=system(cmd);

printf("%d",k);

}else if(strcmp(x,"wifi off")==0){

printf("WIFI operation is being placed and you will get message of status");



strcpy(cmd,"sudo ifconfig wlan0 down");


/*exec wifi off*/

k=system(cmd);



}else{

printf("Not valid ");

}

free(cmd);


publish(topic);


}



/** 
 *  @brief To perform I/O operations 
 *         make function call.
 *  
 *  Perform function subscribe message to the server/broker 
 *  and also give back message.
 *
 *  @return int 
 */

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message * message ){

    printf("   topic: %s\n", topicName);
    
   char* payloadptr = message->payload;
        
    /*prints received message acknowledgment*/
    printf("Received operation %s\n", payloadptr);
    
      /*check message cmmd*/
    Chk_Msg_exe(payloadptr,topicName);

    
    /*free unwanted memory*/
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}


/** 
 *  @brief To perform I/O operations 
 *         make function call.
 *  
 *  Perform function subscribe message to the server/broker 
 *  and also give back message.
 *
 *  @return int 
 */

int main(int argc, char* argv[]){
    

  /*This function creates an MQTT client ready for connection to the specified server */
    MQTTClient_create(&client, ADDRESS, CLIENTID,MQTTCLIENT_PERSISTENCE_NONE, NULL);
 
 
   /*initiates connections with username and alive session*/
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    conn_opts.keepAliveInterval = 40;
    conn_opts.cleansession = 1;
    conn_opts.username = USERNAME;
    conn_opts.password = PASSWORD;
    
    /*callback Whenever publish message arrives */
    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);
    
   
   /*check if connection is successful or not*/
   int rc = MQTTClient_connect(client,&conn_opts) ;
   
   if (rc  != MQTTCLIENT_SUCCESS) {
    
    printf("Failed to connect, return code %d\n", rc);
    
    exit(-1);
    
   }

   
   int i=0;
   for(i=0;i<2;i++) { 
       
   printf("Subscribing to topic %s\nfor client %s using QoS %d\n",topic[i], CLIENTID,qos[i]);
   
   }
   

   /*subscribe a client to a list of topics*/
   MQTTClient_subscribeMany(client,2,topic,qos);
   
   char ch;
   do{
       
    ch = getchar();
    
     } while(ch!='Q' && ch != 'q');
    


    /*disconnect the client from the MQTT server*/
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}
