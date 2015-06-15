package COM.softing.fc.CC.commFB;

import java.util.Hashtable;
import java.lang.Integer;
import COM.softing.fc.CC.util.*;
import COM.softing.fc.CC.utility.*;

public class CommFBFactory
{
    private static final int CFB_Serial = 0;
    private static Hashtable m_commFBMap = new Hashtable();   

    private static int m_nextCommID = 0;

    public static CommFB createCommFB(int iCommType)
    {
        CommFB comm = null; 
        switch (iCommType)
        {
        case CFB_Serial:
            comm = new SerialCommFB();
            // DebugManager.print("SerialCommFB created!\n");
            break;
        default:
            break;
        }
        return comm;
    }

    public synchronized static int addCommFBToMap(CommFB comm)
    {
        int iCommID = m_nextCommID;

        m_nextCommID++;
        Integer ID = new Integer(iCommID);
        m_commFBMap.put(ID, comm);

        return iCommID;       
    }

    public synchronized static void removeCommFB(int iCommID, int iCommType)
    {
        Integer ID = new Integer(iCommID);
        CommFB comm = getCommFB(iCommID, iCommType);
        if (comm != null)
        {
            m_commFBMap.remove(ID);
            // DebugManager.print("CommFB removed!\n");
        }
    }

    public synchronized static CommFB lookUpCommFB(int iCommID, int iCommType)
    {
        return (getCommFB(iCommID, iCommType));
    }

    private static CommFB getCommFB(int iCommID, int iCommType)
    {
        Integer ID = new Integer(iCommID);
        CommFB comm;

        comm = (CommFB)m_commFBMap.get(ID);
        switch(iCommType)
        {
        case CFB_Serial:
            if (!(comm instanceof SerialCommFB))
                comm = null;
            break;
        default:
            comm = null;
            break;
        }
        return comm;
    }
}

