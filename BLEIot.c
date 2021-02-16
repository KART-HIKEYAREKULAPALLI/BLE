/** 
 *  @brief MQTT client
 *  
 *  an MQTT Client that Subscribes to  topic "config/ble"
 *  and configures bluetooth when commands are received on these MQTT topics.
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
   #define ADDRESS     "Enter your ADD"

   /*ClientId is required for device creation*/
   #define CLIENTID    "Enter your ID"

   /*Account username */
   #define USERNAME    "Enter your User name"

   /*Account password */
   #define PASSWORD    "Enter your password"
   
   /*Timeout*/
   #define TIMEOUT      10000L
  
   /*topic*/
   #define Topic      "config/ble"
    
   /* QOS of topics */
   #define  Qos         1



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
           
        pubmsg.payload = "Error";
        
        }else{
           
            pubmsg.payload = "Sucess";
        
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


    char *a=(char*)malloc((sizeof(char)*strlen(payloadptr))+2);
    strcpy(a,"./");
    strcat(a,payloadptr);
    k=system(a);
    /* publish the command is sucess or error to broker*/
    publish(topicName);
    /*free unwanted memory*/
    free(a);   
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

 
   printf("Subscribing to topic %s\nfor client %s using QoS %d\n",Topic, CLIENTID,Qos);
   
   
   printf("Press Q<Enter> to quit\n\n");
  
   

   /*subscribe a client to a list of topics*/
   MQTTClient_subscribe(client,Topic,Qos);
   
   char ch;
   do{
       
    ch = getchar();
    
     } while(ch!='Q' && ch != 'q');
    


    /*disconnect the client from the MQTT server*/
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}
