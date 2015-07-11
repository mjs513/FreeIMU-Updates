/** 
 * XBee ACD API Reader
 * by Rob Faludi and Daniel Shiffman
 * http://www.faludi.com
 * http://www.shiffman.net   
 * v 1.2  (added multiple samples)
 */

package xbee;

import java.util.ArrayList;

public class XBeeDataFrame {

    public static int MODEMSTATUS = 0x8A;
    public static int ATCOMMANDRESPONSE = 0x88;
    public static int TRANSMITSTATUS = 0x89;
    public static int PACKET = 0x81;         // 0x80 is for 64bit, but we usually us 16bit, 0x81.
    public static int SERIES1_IOPACKET = 0x83;       // 0x82 is for 64bit
    public static int ZNET_IOPACKET = 0x92;       

    public int apiId = -1;
    public int   sourceAddress16 = -1;
    public long  sourceAddress64 = -1;
    //public long 
    public int rssi = -1;

    public boolean addressBroadcast;
    public boolean panBroadcast;

    int totalSamples = -1;

    ArrayList digitalSamples;
    ArrayList analogSamples;

    //int[] digital;
    //int[] analog;


    public XBeeDataFrame() {
        digitalSamples = new ArrayList();
        analogSamples = new ArrayList();
    }

    public void setAddress16(int address) {
        sourceAddress16 = address;
    }

    public void setAddress64(long address) {
        sourceAddress64 = address;
    }


    public void setRSSI(int r) {
        rssi = -r;
    }

    public void setAddressBroadcast(boolean a) {
        addressBroadcast = a;
    }

    public void setPanBroadcast(boolean pan) {
        panBroadcast = pan;
    }

    public void setTotalSamples(int ts) {
        totalSamples = ts;
    }

    public void addDigital(int[] d) {
        digitalSamples.add(d);
        //digital = d;
    }

    public void addAnalog(int[] a) {
        analogSamples.add(a);
        //analog = a;
    }

    public void setApiID(int api_id) {
        apiId = api_id;
    }

    public int getApiID() {
        return apiId;
    }

    public int getAddress16() {
        // TODO Auto-generated method stub
        return sourceAddress16;
    }

    public long getAddress64() {
        // TODO Auto-generated method stub
        return sourceAddress64;
    }

    public int getRSSI() {
        return rssi;
    }

    public int[] getDigital() {
        return getDigital(0);
    }
    
    public int[] getAnalog() {
        return getAnalog(0);
    }
    
    public int[] getDigital(int index) {
        if (index < digitalSamples.size()) {
            return (int[]) digitalSamples.get(index);
        } else {
            return null;
        }
    }

    public int[] getAnalog(int index) {
        if (index < analogSamples.size()) {
            return (int[]) analogSamples.get(index);
        } else {
            return null;
        }
    }
    public int getTotalSamples() {
        return totalSamples;
    }

}
