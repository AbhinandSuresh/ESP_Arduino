#pragma once
typedef enum msgType	//type of messages used as enum
{
	JOIN_REQUEST=0x01,	//used in pairing mode for join request
	JOIN_ACCEPT,		//used in pairing mode for join accept
	JOIN_CONFIRM,		//used in pairing mode for join confirmation
	CONFIG_REQ,			//used in configuration mode for requesting configuration message
	CONFIG_MSG,			//used in configuration mode for sending configuration message
	CONFIG_ACK,			//used in configuration mode for acknowledging configuration successfull
	SENSOR_DATA,		//used in monitoring mode for sending sensor data and battery level
	ACK,				//used for acknowledging a message received
	MOTOR_STATUS,		//used for acknowleging status of motor
	ALARM,				//used to send warning messages/error messages
	INIT_NOTIFY			//used to notify level monitor that the controller is recently initiated
}msgType; 

typedef enum devType {
	MONITOR = 0x01,
	CONTROLLER,
	RELAY_UNIT
}devType;

typedef enum response {
	SUCCESS = 0x00,
	JOIN_ERROR,
	CONFIG_ERROR
}response;

typedef enum motorState {
	DEVICE_ON = 0x00,
	DEVICE_OFF,
	DRY_RUN
}motorState;

/*
* Message type: Join Request
* devType: Device type
*/
typedef struct joinRequest {
	unsigned char devType;
}joinRequest;

/*
* Message type: Join Accept
* devID: Device ID, 
* secretKey: AES Secret Key
*/
typedef struct joinAccept {
	unsigned char devID[4];
	unsigned char secretKey[16];
	}joinAccept;

/*
* Message type: config Request
* devType: Device type
*/
typedef struct configReq {
	unsigned char devType;
}configReq;

/*
* Message type: Configuration Message
* lowThreshold: Height of lower threshold value
* highThreshold: Height of higher threshold value
* sendFrequency: Number of messages to be send in an hour
* backoffTries: Number of tries untill going back to normal mode
*/
typedef struct configmsg {
	float lowThreshold;
	float highThreshold;
	int sendFrequency;
	int backoffTries;
}configmsg;

/*
* Message type: sensorData
* level: height of water level from the sensor
* batteryInfo: Battery voltage information
*/
typedef struct sensorData {
	float level;
	float batteryInfo;
}sensorData;

/*
* Message type: acknowledgment
* response : Acknowledgment response
*/
typedef struct ack {
	unsigned char response;
}ack;
