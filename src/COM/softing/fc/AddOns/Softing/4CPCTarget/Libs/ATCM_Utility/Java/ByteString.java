package COM.softing.fc.CC.utility;
import COM.softing.fc.CC.system.StructVar;
import COM.softing.fc.CC.system.IntVar;
import COM.softing.fc.CC.util.*;

/**
 * class ByteString
 */
public class ByteString extends StructVar
{
    private byte[] byteArray;

    /**
     * getByteString returns the underlying byte array.
     * the array is resized if the member "size" has changed.
     * @return byte[]
     */
    public synchronized byte[] getByteString()
    {
      int actSize = ((FcInt)(locVars[0])).getInt(); //size
      if (actSize != byteArray.length)
        {
          byte[] newArray = new byte[actSize];
          int minSize = actSize<byteArray.length ? actSize : byteArray.length;
          System.arraycopy(byteArray,0,newArray,0,minSize);
          byteArray = newArray;
        }
      return byteArray;
    }

    /**
     * setByteString overwrites the underlying byte array.
     * the member "size" will be changed to reflect the new array length.
     * @param newArray  --  the new byte array (not copied!)
     * @return void
     */
    public synchronized void setByteString(byte[] newArray)
    {
      ((FcInt)(locVars[0])).putInt(newArray.length); //size
      byteArray = newArray;
    }

    /**
     * setByteAt sets the specified byte.
     * @param index : position
     * @param value : byte-value
     */
    public synchronized void setByteAt(int index, int value)
    {
      byte[] bstr = getByteString();
      bstr[index] = (byte)value;
    }

    /**
     * getByteAt gets the specified byte.
     * @param index : position
     * @return int : byte-value
     */
    public int getByteAt(int index)
    {
      byte[] bstr = getByteString();
      return bstr[index] & 0xFF;
    }

    /**
     * initFromString initializes the ByteString from
     * the given String. the value should only consist
     * of ASCII-Characters in the range 1-127.
     * this function handles C-like \-quotes correctly.
     * @param val : ASCII-STRING
     * @see ByteString.handleEscape()
     */
    public void initFromString(String val)
    {
      int len = handleEscape(val, null);
      byte[] bstr = new byte[len];
      handleEscape(val, bstr);
      setByteString(bstr);
    }

    /**
     * convert this to a human-readable string.
     * non-printable characters will be escaped like "\xFF"
     * @return String : a quoted string.
     */
    public String toString()
    {
      byte[] bstr = getByteString();
      StringBuffer wstr = new StringBuffer(bstr.length);
      for (int i=0; i<bstr.length; i++)
        {
          char b=(char)(bstr[i] & 0xFF);
          if (b>=32 && b<127)
            {
              wstr.append(b);
              if (b == '\\')
                wstr.append(b);
            }
          else
            {
              wstr.append('\\');
              wstr.append('x');
              wstr.append(Character.forDigit((b>>>4)&0xF, 16));
              wstr.append(Character.forDigit(b      &0xF, 16));
            }
        }
      return wstr.toString();
    }

    /**
     * replaces ObjectDirectoryEx::assignFrom().
     */
    public void assignFrom(ObjectDirectoryEx other_in)
    {
      ByteString other = (ByteString)other_in;
      super.assignFrom(other);
      if (this != other)
        {
          this.byteArray = new byte[other.byteArray.length];
          System.arraycopy(other.byteArray, 0,
                           this.byteArray, 0,
                           this.byteArray.length);
        }
    }

    /**
     * replaces ObjectDirectoryEx::newClone().
     * @return a new empty ByteString
     */
    public ObjectDirectoryEx newClone()
    {
        ByteString myClone = new ByteString();
        myClone.locVars = new FcObject[1];
        myClone.locVars[0] = new IntVar();
        myClone.byteArray = new byte[0];
        return myClone;
    }

    /**
     * replaces ObjectDirectory::init(ObjectDescriptor, FcObject, ObjectDirectory).
     */
    public boolean init(ObjectDescriptor objDescr, FcObject parent, ObjectDirectory oldObj)
    {
      boolean ok = super.init(objDescr, parent, oldObj);
      if (ok)
      {
        if (oldObj == null)
          {
            int actSize = ((FcInt)(locVars[0])).getInt();
            byteArray = new byte[actSize];
          }
        else if (oldObj != this)
          {
            byteArray = ((ByteString)oldObj).byteArray;
          }
      }
      return ok;
    }

    private static final int handleEscape(String in, byte[] out)
    {
      int raw_len = 0;
      int state = 0;
      int d, value = 0;
      for (int i=0; i<in.length(); i++)
      {
        char c = in.charAt(i);
        next: for(;;) {
        switch(state)
        {
          case 0:
            if (c == '\\') {
              state = 1;
              break next;
            }
            if (out!=null)
              out[raw_len] = (byte)(c & 0x7F);
            raw_len++;
            break next;
          case 1:
            switch (c)
            {
              case 'x':
                value = 0;
                state = 3;
                break next;
              case '0': case '1': case '2': case '3':
              case '4': case '5': case '6': case '7':
                value = c-'0';
                state = 2;
                break next;
              case 'a': c='\007'; break; //BEL=7
              case 'b': c='\b'; break;
              case 'f': c='\f'; break;
              case 'n': c='\n'; break;
              case 'r': c='\r'; break;
              case 't': c='\t'; break;
              case 'v': c='\013'; break; //VT=11
            }
            if (out!=null)
              out[raw_len] = (byte)(c & 0x7F);
            raw_len++;
            state = 0;
            break next;
          case 2:
            if (c>='0' && c<='7' && value<=037) {
              value *= 8;
              value += c-'0';
              break next;
            }
            state = 5;
            continue next;
          case 3: case 4:
            d = Character.digit(c, 16);
            if (d == -1) {
              state = 5;
              continue next;
            }
            value *= 16;
            value += d;
            state++;
            break next;
          case 5:
            if (out!=null)
              out[raw_len] = (byte)value;
            raw_len++;
            state = 0;
            continue next;
        } // case state
        } // next
      } // for i

      if (state>=2)
      {
        if (out!=null)
          out[raw_len] = (byte)value;
        raw_len++;
      }
      return raw_len;
    }
}
