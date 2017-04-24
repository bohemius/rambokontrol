/*
 * This is a JavaScript Scratchpad.
 *
 * Enter some JavaScript, then Right Click or choose from the Execute Menu:
 * 1. Run to evaluate the selected text (Ctrl+R),
 * 2. Inspect to bring up an Object Inspector on the result (Ctrl+I), or,
 * 3. Display to insert the result in a comment after the selection. (Ctrl+L)
 */
           
           function get_eth_info() {
               rpcEthInfoFunc = new RPCVariable(mbed, "ethInfo");
               result = rpcEthInfoFunc.run(" ");
               console.log("Result: "+result);
           }

           function get_sd_root() {
               rpcSdRootFunc = new RPCVariable(mbed, "sdRoot");
               result = rpcSdRootFunc.run(" ");
               console.log("Result: "+result);
           }

           function send_request() 
            {
                var statusButton = document.getElementById("statusId");
                if(x == 1) {                    
                    console.log("Led ON, turning OFF");
                    statusButton.textContent = "OFF";
                    x = 0;
                }
                else if (x == 0) { 
                    console.log("Led OFF, turning ON");
                    statusButton.textContent = "ON";
                    x = 1; 
                } else {
                    console.log("Status unknown, turning OFF");
                    x = 0;
                }
                console.log(x);                
                RPCRequest.write(x);
            }
        

            

