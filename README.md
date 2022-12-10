# ECEN 5813 - Principles of Embedded Software Torque_demand_calc
#### As a part of my final project for the ECEN 5813, I wish to demonstrate the torque demand calculation for the modern vehicle, although automotive systems are more complex with a lot of complex modules integrated to build a domain centralized system and due to the limitations of these complex setups and hardware resources I have demonstrated the torque demands of our vehicle with the help of the onboard LED by controlling the PWM.
The following components are integrated into virtual vehicle control systems:
>Acceleration sensor
>>mma8451 interfaced with the i2c0 inbuilt capabilities of our FRDM kl25z board, this module primarily intends to read the acceleration sensor value for roll and pitch which are two of the most important components of our project, we derive the elevation angle with the acceleration sensor module.

```c
/*a table consisting of various command received by the user*/
static const command_table_t commands[] =
{
		{"Nileshkartik", "Password",handle_user,"Performs the user authentication"},						/*command author, callback function for author and help string*/
};
```
> UART-based Command Processor
>>To simplify the process of a user entering the key and turning on the ignition, I have simulated the same with our UART-based command processor that enables the user to enter a username and password, if the user wishes to add more secondary users following can be done by appending the table {command_table_t commands[]}.

>Touch Sensor Interface
>>The user can switch to the park brake mode with the help of the TSI module inbuilt into kl25z.

>LED PWM 
>> To simplify our board driving the complex motors in vehicle control systems, we have the LED on board which is controlled with the help of the PWM.

>SYSTICK Module 
>> To derive the system timing and display the Incliniation every 1 sec, The systick interrupt is generated every 63ms.

>State Machine
>>![image](https://user-images.githubusercontent.com/112504087/206822886-d7b3450b-c828-4212-a793-69923f5e348f.png)


The dashboard will display the following Message:
1. DOWNHILL DRIVE mode
2. UPHILL DRIVE mode
3. CRITICAL DRIVE mode
4. PARK BRAKE AND THE RELATIVE TIME WHEN THE PARK BRAKE WAS REQUESTED
5. BUMP DETECTED
6. CURRENT INCLINATION OF THE VEHICLE

Note: To avoid our UART based dashboard to continously print the Inclination, which degrades the readability I have prinited the inclination every 1 sec.

Following are the states and various drive modes:
![image](https://user-images.githubusercontent.com/112504087/206809179-d6024188-f050-4fce-afaa-652504d3ea58.png)

Following are tha various test criteria:
![image](https://user-images.githubusercontent.com/112504087/206827929-420fe928-113c-40f6-9378-b1c973a1c5fd.png)


User Access:
Access granted

![image](https://user-images.githubusercontent.com/112504087/206811508-668146ca-6d1c-4d8b-a7df-d62527608ff0.png)

Access Denied

![image](https://user-images.githubusercontent.com/112504087/206811579-d3667d34-27ab-4c19-87e2-4e0f62880c1d.png)

Downhill --> Critical Drive Condition --> Downhill --> Plainterrain

![image](https://user-images.githubusercontent.com/112504087/206827347-2d4c4fe6-3c8d-4283-89f1-ab2a9004a88e.png)

Uphill --> Critical Drive Condition --> Uphill --> Plainterrain

![image](https://user-images.githubusercontent.com/112504087/206827393-715c07ce-40e0-4b39-bbce-97359fc3ef67.png)

Uphill --> Bump --> Plainterrain

![image](https://user-images.githubusercontent.com/112504087/206827455-c88be50d-6d0d-48a0-b8ef-8a44772f7dca.png)

Roll Stability Support

![image](https://user-images.githubusercontent.com/112504087/206827905-00cf6663-a932-40d7-82ae-5538f381bdbe.png)

parkBrake from Downhill condition

![image](https://user-images.githubusercontent.com/112504087/206827979-25b2665c-1352-468d-ab67-b6c5144fe603.png)



