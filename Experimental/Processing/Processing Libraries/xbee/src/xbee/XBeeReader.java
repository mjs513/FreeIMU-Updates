/** 
 * XBee ACD API Reader
 * by Rob Faludi and Daniel Shiffman
 * http://www.faludi.com
 * http://www.shiffman.net  
 * v 1.2  (added multiple samples)
 */

package xbee;

import java.lang.reflect.Method;
import processing.core.PApplet;
import processing.serial.Serial;

public class XBeeReader extends Thread {

    PApplet parent;
    Serial port; // We'll need the parent PApplet to make a Serial object for us
    Method xBeeMethod;

    private boolean running;   // Is the thread running?  Yes or no?
    boolean available = false;

    XBeeDataFrame data;

    static boolean DEBUG = false;

    // Constructor, create the thread
    // It is not running by default
    public XBeeReader(PApplet p, Serial s) {
        running = false;
        parent = p;
        port = s;

        try {
            xBeeMethod = parent.getClass().getMethod("xBeeEvent", new Class[] { 
                    XBeeReader.class             }
            );
        } 
        catch (Exception e) {
            System.out.println("You forgot to implement the xBeeEvent() method.");
        }
    }

    // Overriding "start()"


    public String startXBee(String command) {
        // System.out.println("Setting up XBee");
        // System.out.println("writing +++");
        port.write("+++"); 
        try {
            Thread.sleep(1100);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        //port.clear();

        // Waiting for a response, timeout after 10 seconds
        long saved = System.currentTimeMillis();
        while (port.available() <= 0) {
            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            if (System.currentTimeMillis() - saved > 10000) {
                // System.out.println("+++ Timeout error!");
                return "+++ Timeout Error";
            }
        }

        String input = "";
        while (port.available() > 0) {
            input += (char) port.read();
        }
        input = input.substring(input.length()-3,input.length()).trim();

        //System.out.println(input);
        if (!input.equals("OK")) {
            System.out.println("response error");
            return "Response Error";
        }

        port.write(command); //setup XBee
        // Waiting for a response, timeout after 10 seconds
        saved = System.currentTimeMillis();
        while (port.available() <= 0) {
            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            if (System.currentTimeMillis() - saved > 10000) {
                // System.out.println("Timeout error!");
                return "Command Timeout Error";
            }
        }

        input = "";
        while (port.available() > 0) {
            int val = port.read();
            if (val == 13) val = 32;
            input += (char) val;
        }

        //System.out.println("Finished setup.");
        port.write(",CN\r\n");

        running = true;
        super.start();

        return input;
    }

    public void startXBee() {
        running = true;
        super.start();
    }

    public void run () {
        while (running){
            data = getData();
            available = true;
            if (xBeeMethod != null && data != null && data.getApiID() != -1) {
                try {
                    xBeeMethod.invoke(parent, new Object[] { this });
                } catch (Exception e) {
                    System.err.println("Problem with xBeeEvent()");
                    e.printStackTrace();
                    xBeeMethod = null;
                }
            }

            try {
                sleep(1);  // Should we sleep?
            } catch (Exception e) {
                // Nothing for now. We'll get here if interrupt() is called
            }
        }
    }

    // Our method that quits the thread
    public void quit()
    {
        System.out.println("Quitting."); 
        running = false;  // Setting running to false ends the loop in run()
        super.interrupt();
    }

    public boolean available() {
        return available;
    }

    public XBeeDataFrame getXBeeReading() {
        available = false;
        return data;
    }

    private XBeeDataFrame getData() {

        XBeeDataFrame data = new XBeeDataFrame();

        port.clear(); // flush the buffer so that no old data comes in

        while (port.available() < 1) {
            ; // do nothing while we wait for the buffer to fill
        }
        if (port.available() >= 1) {
            int inByte = port.read();

            if (inByte == 0x7E) {
                while (port.available() < 2) {
                    ; // do nothing while we wait for the buffer to fill
                }
                if (port.available() >= 2) {
                    int lengthMSB = port.read(); // high byte for length of packet
                    int lengthLSB = port.read(); // low byte for length of packet
                    int length = (lengthLSB + (lengthMSB << 8)) + 1;

                    if (DEBUG) System.out.print("length: " + length);

                    while (port.available() < length) {
                        ; // do nothing while we wait for the buffer to fill
                    }

                    if (port.available() >= lengthLSB + (lengthMSB << 8)) {
                        int api_id = port.read();    // API Identifier indicates what type of packet this is

                        data.setApiID(api_id);

                        // FOR PROCESSING SERIES 1 PACKETS
                        if (api_id == XBeeDataFrame.SERIES1_IOPACKET) {

                            if (DEBUG) System.out.print("  API ID: " + (int) api_id);

                            // Get the address
                            int addrMSB = port.read();  // high byte of sender's 16-bit address
                            int addrLSB = port.read();  // low byte of sender's 16-bit address
                            int addr = (addrMSB << 8) + addrLSB;
                            data.setAddress16(addr);

                            // Get RSSI
                            int rssi = port.read();     // Received Signal Strength Indicator shows how strong a signal we received
                            data.setRSSI(rssi);

                            // Reading the options
                            int options = port.read();
                            boolean addressbroadcast = ((options >> 1) & 0x01) == 1;
                            data.setAddressBroadcast(addressbroadcast);

                            boolean pan = ((options >> 2) & 0x01) == 1;
                            data.setPanBroadcast(pan);

                            // now we get to the ADC data itself
                            int totalSamples = port.read(); // this is the number of sample packages that we're receiving

                            data.setTotalSamples(totalSamples);

                            /*if (totalSamples > 1) {
								System.out.println("This preliminary version of the XBee API library only works with a sample size of 1.");
								System.out.println("Set ATIT to 1 on your transmitting radio(s).");
								quit();
								return null;
							}*/

                            if (DEBUG) System.out.print("  Total Samples: " + (int) totalSamples);
                            int channelIndicatorHigh = port.read(); // this tells us which analog channels (pins) are in use (and one digital channel)
                            if (DEBUG) System.out.print("  CI High: " + PApplet.binary(channelIndicatorHigh));
                            int channelIndicatorLow = port.read();  // this tells us which digital channels (pins) are in use.
                            if (DEBUG) System.out.print("  CI Low: " + PApplet.binary(channelIndicatorLow));

                            // to calculate the checksum, first add all the bytes after the length byte together,
                            // except the checksum byte itself (we'll add the data bytes in a loop below)
                            int localChecksum = (api_id + addrMSB + addrLSB + rssi + options + totalSamples + channelIndicatorHigh + channelIndicatorLow);

                            // Process Digital

                            for (int n = 0; n < totalSamples; n++) {

                                int[] dataD = {-1,-1,-1,-1,-1,-1,-1,-1,-1};
                                int digitalChannels = channelIndicatorLow;
                                boolean digital = false;

                                // Is Digital on in any of the 8 bits?
                                for (int i=0; i < dataD.length-1; i++) { // add up the active channel indicators
                                    if ((digitalChannels & 1) == 1) { // by masking so we only see the last bit
                                        dataD[i] = 0;
                                        digital = true;
                                    }
                                    digitalChannels = digitalChannels >> 1;
                                }

                                // Is Digital on in the last weird extra bit?
                                if ((channelIndicatorHigh & 1) == 1) { // by masking so we only see the last bit
                                    dataD[8] = 0;
                                    digital = true;
                                }

                                //System.out.println(digital);

                                if (digital) {
                                    int digMSB = port.read();
                                    int digLSB = port.read();
                                    localChecksum += digMSB + digLSB; // add the data bytes to the existing local checksum calculation
                                    int dig = (int)((digMSB << 8) + digLSB);
                                    for (int i = 0; i < dataD.length; i++) {
                                        if (dataD[i] == 0) {
                                            dataD[i] = dig & 1;
                                        }
                                        dig = dig >> 1;
                                    }
                                }

                                // Put Digital Data in object
                                data.addDigital(dataD);

                                // Process Analog 
                                int[] dataADC = {-1,-1,-1,-1,-1,-1};
                                int analogChannels = (channelIndicatorHigh >> 1); // shift out the one digital channel indicator that doesn't interest us now
                                for (int i=0; i < dataADC.length; i++) { // add up the active channel indicators
                                    if ((analogChannels & 1) == 1) { // by masking so we only see the last bit
                                        int dataADCMSB = port.read();
                                        int dataADCLSB = port.read();
                                        localChecksum += dataADCMSB + dataADCLSB; // add the data bytes to the existing local checksum calculation
                                        dataADC[i] = (int)((dataADCMSB << 8) + dataADCLSB);
                                    }
                                    analogChannels = analogChannels >> 1; // then shifting over one bit at a time as we go
                                }

                                // Put Analog in object
                                data.addAnalog(dataADC);

                            }


                            // Some checksum checking
                            int checksum = port.read();
                            localChecksum = (byte) (0xFF -localChecksum);
                            if ( (byte) checksum - localChecksum != 0) {
                                if (DEBUG) System.out.print("\n\nchecksum error!  " +  ((byte) checksum - localChecksum) + "\n\n");
                            }

                            // FOR PROCESSING ZNET PACKETS
                        } else if (api_id == XBeeDataFrame.ZNET_IOPACKET) {

                            if (DEBUG) System.out.print("  API ID: " + (int) api_id);

                            int localChecksum = 0;

                            // Looping to get 64-bit address
                            long address = 0;
                            for (int i = 7; i >= 0; i--) {
                                long currentByte = port.read();
                                localChecksum += currentByte;
                                address += currentByte << i*8;
                            }
                            data.setAddress64(address);

                            // Get the 16 BIT address
                            int addrMSB = port.read();  // high byte of sender's 16-bit address
                            int addrLSB = port.read();  // low byte of sender's 16-bit address
                            int addr = (addrMSB << 8) + addrLSB;
                            data.setAddress16(addr);

                            // Reading the options
                            int options = port.read();
                            // DO NOTHING WITH OPTIONS FOR NOW

                            // now we get to the ADC data itself
                            int totalSamples = port.read(); // this is the number of sample packages that we're receiving

                            data.setTotalSamples(totalSamples);

                            if (totalSamples > 1) {
                                System.out.println("This preliminary version of the XBee API library only works with a sample size of 1.");
                                System.out.println("Set ATIT to 1 on your transmitting radio(s).");
                                quit();
                                return null;
                            }

                            if (DEBUG) System.out.print("  Total Samples: " + (int) totalSamples);
                            int digitalChannelIndicatorHigh = port.read(); // this tells us which analog channels (pins) are in use (and one digital channel)
                            int digitalChannelIndicatorLow = port.read();  // this tells us which digital channels (pins) are in use.
                            int analogChannelIndicator = port.read();

                            // to calculate the checksum, first add all the bytes after the length byte together,
                            // except the checksum byte itself (we'll add the data bytes in a loop below)
                            localChecksum += (api_id + addrMSB + addrLSB + options + totalSamples + digitalChannelIndicatorHigh + digitalChannelIndicatorLow + analogChannelIndicator);

                            for (int n = 0; n < totalSamples; n++) {

                                // Process Digital
                                int[] dataD = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
                                int digitalChannels = (digitalChannelIndicatorHigh << 8) + digitalChannelIndicatorLow;

                                boolean digital = false;

                                // Is Digital on in any of the 8 bits?
                                for (int i=0; i < dataD.length; i++) { // add up the active channel indicators
                                    if ((digitalChannels & 1) == 1) { // by masking so we only see the last bit
                                        dataD[i] = 0;  // 0 means it's on, unlike -1 it's off
                                        digital = true;
                                    }
                                    digitalChannels = digitalChannels >> 1;
                                }

                                //System.out.println(digital);

                                if (digital) {
                                    int digMSB = port.read();
                                    int digLSB = port.read();
                                    localChecksum += digMSB + digLSB; // add the data bytes to the existing local checksum calculation  
                                    int dig = (int)((digMSB << 8) + digLSB);
                                    for (int i = 0; i < dataD.length; i++) {
                                        if (dataD[i] == 0) {
                                            dataD[i] = dig & 1;
                                        }
                                        dig = dig >> 1;
                                    }
                                }

                                // Put Digital Data in object
                                data.addDigital(dataD);

                                // Process Analog 
                                int[] dataADC = {-1,-1,-1,-1};
                                int analogChannels = analogChannelIndicator; // Before, we shifted out a bit so just renaming var here
                                for (int i=0; i < dataADC.length; i++) { // add up the active channel indicators
                                    if ((analogChannels & 1) == 1) { // by masking so we only see the last bit
                                        int dataADCMSB = port.read();
                                        int dataADCLSB = port.read();
                                        localChecksum += dataADCMSB + dataADCLSB; // add the data bytes to the existing local checksum calculation
                                        dataADC[i] = (int)((dataADCMSB << 8) + dataADCLSB);
                                    }
                                    analogChannels = analogChannels >> 1; // then shifting over one bit at a time as we go
                                }

                                // Put Analog in object
                                data.addAnalog(dataADC);
                            }

                            // Some checksum checking
                            int checksum = port.read();
                            localChecksum = (byte) (0xFF -localChecksum);
                            if ( (byte) checksum - localChecksum != 0) {
                                if (DEBUG) System.out.print("\n\nchecksum error!  " +  ((byte) checksum - localChecksum) + "\n\n");
                            }

                        }
                    }
                } 
            }
            if (DEBUG) System.out.println();
        }

        return data;  
    }

}

