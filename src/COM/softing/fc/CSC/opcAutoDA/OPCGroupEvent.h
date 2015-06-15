
//////////////////////////////////////////////////////////////////////////////
// CProxyDIOPCGroupEvent
template <class T>
class CProxyDIOPCGroupEvent : public IConnectionPointImpl<T, &DIID_DIOPCGroupEvent, CComDynamicUnkArray>
{
public:
//methods:
//DIOPCGroupEvent : IDispatch
public:
   void Fire_DataChange(
      long TransactionID,
      long NumItems,
      SAFEARRAY * ClientHandles,
      SAFEARRAY * ItemValues,
      SAFEARRAY * Qualities,
      SAFEARRAY * TimeStamps)
   {
      VARIANTARG* pvars = new VARIANTARG[6];
      for (int i = 0; i < 6; i++)
         VariantInit(&pvars[i]);
      T* pT = (T*)this;
      pT->Lock();
      IUnknown** pp = m_vec.begin();
      while (pp < m_vec.end())
      {
         if (*pp != NULL)
         {
            pvars[5].vt = VT_I4;
            pvars[5].lVal= TransactionID;
            pvars[4].vt = VT_I4;
            pvars[4].lVal= NumItems;
            pvars[3].vt = VT_I4 | VT_ARRAY;
            pvars[3].parray= ClientHandles;
            pvars[2].vt = VT_VARIANT | VT_ARRAY;
            pvars[2].parray= ItemValues;
            pvars[1].vt = VT_I4 | VT_ARRAY;
            pvars[1].parray= Qualities;
            pvars[0].vt = VT_DATE | VT_ARRAY;
            pvars[0].parray= TimeStamps;
            DISPPARAMS disp = { pvars, NULL, 6, 0 };
            IDispatch* pDispatch = reinterpret_cast<IDispatch*>(*pp);
            pDispatch->Invoke(0x1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, NULL, NULL, NULL);
         }
         pp++;
      }
      pT->Unlock();
      delete[] pvars;
   }

   void Fire_AsyncReadComplete(
      long TransactionID,
      long NumItems,
      SAFEARRAY * ClientHandles,
      SAFEARRAY * ItemValues,
      SAFEARRAY * Qualities,
      SAFEARRAY * TimeStamps,
      SAFEARRAY * Errors)
   {
      VARIANTARG* pvars = new VARIANTARG[7];
      for (int i = 0; i < 7; i++)
         VariantInit(&pvars[i]);
      T* pT = (T*)this;
      pT->Lock();
      IUnknown** pp = m_vec.begin();
      while (pp < m_vec.end())
      {
         if (*pp != NULL)
         {
            pvars[6].vt = VT_I4;
            pvars[6].lVal= TransactionID;
            pvars[5].vt = VT_I4;
            pvars[5].lVal= NumItems;
            pvars[4].vt = VT_I4 | VT_ARRAY;
            pvars[4].parray= ClientHandles;
            pvars[3].vt = VT_VARIANT | VT_ARRAY;
            pvars[3].parray= ItemValues;
            pvars[2].vt = VT_I4 | VT_ARRAY;
            pvars[2].parray= Qualities;
            pvars[1].vt = VT_DATE | VT_ARRAY;
            pvars[1].parray= TimeStamps;
            pvars[0].vt = VT_I4 | VT_ARRAY;
            pvars[0].parray= Errors;
            DISPPARAMS disp = { pvars, NULL, 7, 0 };
            IDispatch* pDispatch = reinterpret_cast<IDispatch*>(*pp);
            pDispatch->Invoke(0x2, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, NULL, NULL, NULL);
         }
         pp++;
      }
      pT->Unlock();
      delete[] pvars;
   }

   void Fire_AsyncWriteComplete(
      long TransactionID,
      long NumItems,
      SAFEARRAY * ClientHandles,
      SAFEARRAY * Errors)
   {
      VARIANTARG* pvars = new VARIANTARG[4];
      for (int i = 0; i < 4; i++)
         VariantInit(&pvars[i]);
      T* pT = (T*)this;
      pT->Lock();
      IUnknown** pp = m_vec.begin();
      while (pp < m_vec.end())
      {
         if (*pp != NULL)
         {
            pvars[3].vt = VT_I4;
            pvars[3].lVal= TransactionID;
            pvars[2].vt = VT_I4;
            pvars[2].lVal= NumItems;
            pvars[1].vt = VT_I4 | VT_ARRAY;
            pvars[1].parray= ClientHandles;
            pvars[0].vt = VT_I4 | VT_ARRAY;
            pvars[0].parray= Errors;
            DISPPARAMS disp = { pvars, NULL, 4, 0 };
            IDispatch* pDispatch = reinterpret_cast<IDispatch*>(*pp);
            pDispatch->Invoke(0x3, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, NULL, NULL, NULL);
         }
         pp++;
      }
      pT->Unlock();
      delete[] pvars;
   }

   void Fire_AsyncCancelComplete(
      long TransactionID)
   {
      VARIANTARG* pvars = new VARIANTARG[1];
      for (int i = 0; i < 1; i++)
         VariantInit(&pvars[i]);
      T* pT = (T*)this;
      pT->Lock();
      IUnknown** pp = m_vec.begin();
      while (pp < m_vec.end())
      {
         if (*pp != NULL)
         {
            pvars[0].vt = VT_I4;
            pvars[0].lVal= TransactionID;
            DISPPARAMS disp = { pvars, NULL, 1, 0 };
            IDispatch* pDispatch = reinterpret_cast<IDispatch*>(*pp);
            pDispatch->Invoke(0x4, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, NULL, NULL, NULL);
         }
         pp++;
      }
      pT->Unlock();
      delete[] pvars;
   }

};


//////////////////////////////////////////////////////////////////////////////
// CProxyDIOPCServerEvent
template <class T>
class CProxyDIOPCServerEvent : public IConnectionPointImpl<T, &DIID_DIOPCServerEvent, CComDynamicUnkArray>
{
public:
//methods:
//DIOPCServerEvent : IDispatch
public:
   void Fire_ServerShutDown(
      BSTR Reason)
   {
      VARIANTARG* pvars = new VARIANTARG[1];
      for (int i = 0; i < 1; i++)
         VariantInit(&pvars[i]);
      T* pT = (T*)this;
      pT->Lock();
      IUnknown** pp = m_vec.begin();
      while (pp < m_vec.end())
      {
         if (*pp != NULL)
         {
            pvars[0].vt = VT_BSTR;
            pvars[0].bstrVal= Reason;
            DISPPARAMS disp = { pvars, NULL, 1, 0 };
            IDispatch* pDispatch = reinterpret_cast<IDispatch*>(*pp);
            pDispatch->Invoke(0x1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, NULL, NULL, NULL);
         }
         pp++;
      }
      pT->Unlock();
      delete[] pvars;
   }

};

//////////////////////////////////////////////////////////////////////////////
// CProxyDIOPCGroupsEvent
template <class T>
class CProxyDIOPCGroupsEvent : public IConnectionPointImpl<T, &DIID_DIOPCGroupsEvent, CComDynamicUnkArray>
{
public:
//methods:
//DIOPCGroupsEvent : IDispatch
public:
   void Fire_GlobalDataChange(
      long TransactionID,
      long GroupHandle,
      long NumItems,
      SAFEARRAY * ClientHandles,
      SAFEARRAY * ItemValues,
      SAFEARRAY * Qualities,
      SAFEARRAY * TimeStamps)
   {
      VARIANTARG* pvars = new VARIANTARG[7];
      for (int i = 0; i < 7; i++)
         VariantInit(&pvars[i]);
      T* pT = (T*)this;
      pT->Lock();
      IUnknown** pp = m_vec.begin();
      while (pp < m_vec.end())
      {
         if (*pp != NULL)
         {
            pvars[6].vt = VT_I4;
            pvars[6].lVal= TransactionID;
            pvars[5].vt = VT_I4;
            pvars[5].lVal= GroupHandle;
            pvars[4].vt = VT_I4;
            pvars[4].lVal= NumItems;
            pvars[3].vt = VT_I4 | VT_ARRAY;
            pvars[3].parray= ClientHandles;
            pvars[2].vt = VT_VARIANT | VT_ARRAY;
            pvars[2].parray= ItemValues;
            pvars[1].vt = VT_I4 | VT_ARRAY;
            pvars[1].parray= Qualities;
            pvars[0].vt = VT_DATE | VT_ARRAY;
            pvars[0].parray= TimeStamps;
            DISPPARAMS disp = { pvars, NULL, 7, 0 };
            IDispatch* pDispatch = reinterpret_cast<IDispatch*>(*pp);
            pDispatch->Invoke(0x1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, NULL, NULL, NULL);
         }
         pp++;
      }
      pT->Unlock();
      delete[] pvars;
   }

};

