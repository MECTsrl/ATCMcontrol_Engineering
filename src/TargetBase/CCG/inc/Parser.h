/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/inc/Parser.h 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: Parser.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/inc/Parser.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / CCG
 *
 * =COMPONENT			NCCBase
 *
 * =CURRENT 	 $Date: 28.02.07 19:04 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#if !defined(AFX_PARSER_H__0FA35BA7_29D3_11D5_A05C_0050BF490551__INCLUDED_)
#define AFX_PARSER_H__0FA35BA7_29D3_11D5_A05C_0050BF490551__INCLUDED_

const int   undefined   = -1,
            IP_CODE     =  0,
            NC_CODE     =  1;

const int   LSloadAkku  = 1,
            LSloadReg   = 2,
            LSstoreAkku = 3;


enum    {ftin1, ftin2, ftout};

class CParser  
{
    struct CFType {
        ASM_Opcodes   op;
        unsigned char in1;
        unsigned char in2;
        unsigned char out;
    };
static
    CFType         FType[92];


    CIpCodeList    IpCodeList;              // IP-Code
    CNccCodeList   NccCodeList;             // Ncc-Code
    CCodec         Codec;
                                            // Suchbeschleunigung
    CMapPtrToPtr   MapIpOffset;        
    CMapPtrToPtr   MapIpLabel;        
    CMapPtrToPtr   MapNccLabel;        

    int            urlnumber;
   
    int            index_j;                 // Speicher akt. Sprungindex


    friend int NCCBase(int argc, TCHAR *argv[], TCHAR *envp[]); // member initializer

    unsigned long  m_IP_LoadAddress;        // code module IP load address
    unsigned long  m_NC_LoadAddress;        // code module NCC load address

    unsigned short m_StackAlignment;	    // Stack Alignment
    unsigned short m_InstAlignment; 	    // Instance data Alignment
    unsigned long  m_FirmwareVersion;	    // Firmaware Version
	BOOL		   m_b16BitProc;			// 16 bit processor
	int			   m_iList;					// Flag Listoption
	int            m_iTrashBreakpoints;     // if true remove all breakpoints
	int			   m_iDebugNames;			// Flag Debugnames benutzen 
											// default: bin -> bin, bin(old) -> ip
											//			dbp -> dbp
											// debug:   bin -> ncc
											//			dbp -> dbp_
    unsigned long  m_CodeSegSize;	        // Code Segment Size
 
    // vm dependent constants
    unsigned       m_max_calls;
    unsigned       m_max_index_entries;
    unsigned       m_max_object_entries;
public:
    void pars(CFile& NccFile, CStdioFile& ListFile, XDownHeader& DownHeader, unsigned char *pBuffer, Cxxx_dbp& xxx_dbp);
    CParser();
    virtual ~CParser();
private:
    int     scanner(CStdioFile& ListFile, XDownHeader& DownHeader, unsigned char *pNcc, Cxxx_dbp& xxx_dbp);
    int     checkBlock(int index_b, int index_e);
    int     generateBlock(int index_0, int index);
    int     checkJmp(void);
    int     ASM_getFType(int opcode, int pos);
    bool    exchangeTest(int ft1, int ft2);
    int     parsRPN(int index_0, int index, CNccCodeList& CodeList);
    void    add(CNccCodeList& CodeList, CNccCodeElem& elem);
    int     generateNCC(CNccCodeList& CodeList);
    void    segmentedNCC(int index, unsigned long& ulsize);
    int     linkNCC(CStdioFile& ListFile, Cxxx_dbp& xxx_dbp);
    int     getURL(void) {return ++urlnumber;}
    bool    isKommutativ(unsigned short opcode);
    void    generateLS (unsigned char dtype, unsigned short segment, CNccCodeElem& NccCodeElem, int LStype);
    void    generateLSI(unsigned char dtype, CNccCodeElem& NccCodeElem, int LsType);
    void    generateLSII(unsigned char dtype, CNccCodeElem& NccCodeElem, int LsType);
    void    generateLSV(unsigned char dtype, unsigned short segment, CNccCodeElem& NccCodeElem, int LStype);
	void	generateLSS(unsigned char dtype, CNccCodeElem& NccCodeElem, int LsType);
	void	generateLSID(unsigned char dtype, CNccCodeElem& NccCodeElem, int LsType);
	void	generateLEAID(CNccCodeElem& NccCodeElem);
	void	generateLEAI(CNccCodeElem& NccCodeElem);
    bool    generateLoadAkku(CNccCodeElem& NccCodeElem);
    bool    generateLoadReg (CNccCodeElem& NccCodeElem, int ftype);
    int     skipEmptyIpBlock(int  index);
    int     skipEmptyNccBlock(int  index);
};

#endif

/* ---------------------------------------------------------------------------- */
