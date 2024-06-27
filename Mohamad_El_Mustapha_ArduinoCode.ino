     void setup() {       //initialize the serial communication
     Serial.begin(19200);  // setting the baud rate 
     pinMode(LED_BUILTIN, OUTPUT); // setting the pin mode of built-in LED
     }
     void loop() {
     if  (Serial.available() > 0) {    //checking availabilty of data on the serial port 
     String data = Serial.readStringUntil('\n');   // reading the received string until a nswline character 
     data.trim(); // Delete extra whitespace

     // Distinguish the positions of the commas
     int firstComma = data.indexOf(',');
     int secondComma = data.lastIndexOf(',');

      // Checking the commas are in valid positions
     if (firstComma != -1 && secondComma != -1 && firstComma != secondComma) {

      // Extract the numbers and the operation as a strings
      String first_numberStr = data.substring(0, firstComma);
      String op = data.substring(firstComma + 1, secondComma);
      String second_numberStr = data.substring(secondComma + 1);

      // Transfer strings to numbers 
      double first_number = first_numberStr.toDouble();
      double second_number = second_numberStr.toDouble();
      double result;
      bool validOp = true;

      // Performing required calculation 
      if (op == "+") {
        result = first_number + second_number;
      } else if (op == "-") {
        result = first_number - second_number;
      } else if (op == "*") {
        result = first_number * second_number;
      } else if (op == "/") {
        if (second_number != 0) {
          result = first_number / second_number;
        } else {
          Serial.println("Error: Division by zero");  // printing the error message in case of divisions by zero
          validOp = false;
        }
      } else {
        Serial.println("Error: Invalid operation");  // printing the error message in case of invalid operation
        validOp = false;
      }

      // Get the result back if the operation was valid
      if (validOp) {
        String resultStr = String(result);   // convert the result to a string 
        Serial.println(resultStr);           // send the string back as a result
      }
      } else {
      Serial.println("Error: Invalid input format");
    }
  }
}
