import com.digi.xbee.api.listeners.IDataReceiveListener;
import com.digi.xbee.api.models.XBeeMessage;
import com.digi.xbee.api.utils.HexUtils;

/**
 * Class to manage the XBee received data that was sent by other modules in the 
 * same network.
 * 
 * <p>Acts as a data listener by implementing the 
 * {@link IDataReceiveListener} interface, and is notified when new 
 * data for the module is received.</p>
 * 
 * @see IDataReceiveListener
 *
 */
public class MyDataReceiveListener implements IDataReceiveListener {
  /*
   * (non-Javadoc)
   * @see com.digi.xbee.api.listeners.IDataReceiveListener#dataReceived(com.digi.xbee.api.models.XBeeMessage)
   */
  @Override
  public void dataReceived(XBeeMessage xbeeMessage) {
    //System.out.format("From %s >> %s | %s%n", xbeeMessage.getDevice().get64BitAddress(), 
    //    HexUtils.prettyHexString(HexUtils.byteArrayToHexString(xbeeMessage.getData())), 
    //    new String(xbeeMessage.getData()));
    if (xbeeMessage != null) {
        inputStringArr = split(new String(xbeeMessage.getData()), ",");
        decodePacket();
    }
  }
}

