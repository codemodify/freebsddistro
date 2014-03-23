#ifndef ICQEVENT_H
#define ICQEVENT_H

#include "pthread_rdwr.h"
#include "licq_constants.h"
#include "licq_message.h"

#include <stdlib.h>
#include <vector>

using std::vector;

class CPacket;
class CICQDaemon;
class CUserEvent;
class ICQUser;

//-----CExtendedAck----------------------------------------------------------

/*! \brief Information on file and chat request responses.

    This class will provide information on what the result of a chat or file
    transfer request was.
*/
class CExtendedAck
{
public:
  // Accessors

  //!Returns true if the remote end accepted the request, otherwise false.
  bool Accepted()         { return m_bAccepted; }
  //!The port to connect to if the request was accpeted.
  unsigned short Port()   { return m_nPort; }
  //!The reason for declining the request.
  const char *Response()  { return m_szResponse; }

  ~CExtendedAck();

protected:
  CExtendedAck(bool, unsigned short, char *);

  bool m_bAccepted;
  unsigned short m_nPort;
  char *m_szResponse;

friend class CICQDaemon;
};


//-----CSearchAck------------------------------------------------------------
// Values for the Status() field of the SearchAck
//! User is offline.
const unsigned short  SA_OFFLINE = 0;
//! User is online.
const unsigned short  SA_ONLINE = 1;
//! User has disabled online awareness.
const unsigned short  SA_DISABLED = 2;

/*! \brief The response to a search request.

    Each search result is passed to the plugin as a CSearchAck.  If there
    was no search results, then a single CSearchAck is passed on with
    Result() returning EVENT_SUCCESS, which also signifies the search is
    complete.
*/
class CSearchAck
{
public:
  // Accessors
  //! Returns the UIN of the search result.
  unsigned long Uin()      { return m_nUin; }
  //! Returns the alias (nickname) of the search result.
  const char *Alias()      { return m_szAlias; }
  //! Returns the first name of the search result.
  const char *FirstName()  { return m_szFirstName; }
  //! Returns the last name of the search result.
  const char *LastName()   { return m_szLastName; }
  //! Returns the e-mail address of the search result.
  const char *Email()      { return m_szEmail; }
  //! Retunrs the Id string
  const char *Id()         { return m_szId; }
  //! Returns the protocol plugin id
  unsigned long PPID()     { return m_nPPID; }
  //! If non-zero, the number of search results that were found that could not
  //! be displayed.  The server has a 40 user limit on search results.  This
  //! is valid when Result() is EVENT_SUCCESS.
  int More()     	   { return m_nMore; }
  //! The online status of the search result.
  char Status()            { return m_nStatus; }
  //! The gender of the search result.  Female = 1, Male = 2, Unknown = 3.
  char Gender()		   { return m_nGender; }
  //! The age of the serach result.
  char Age()		   { return m_nAge; }
  //! Non-zero if authorization is required to add this user.
  char Auth()		   { return m_nAuth; }

  ~CSearchAck();

protected:
  CSearchAck(unsigned long _nUin);
  CSearchAck(const char *_szId, unsigned long _nPPID);

  unsigned long m_nPPID;
  char *m_szId;
  unsigned long m_nUin;
  char *m_szAlias;
  char *m_szFirstName;
  char *m_szLastName;
  char *m_szEmail;
  int m_nMore;
  char m_nStatus;
  char m_nGender;
  char m_nAge;
  char m_nAuth;

friend class CICQDaemon;
};



//=====ICQEvent====================================================================================

enum ConnectType
{
  CONNECT_SERVER,
  CONNECT_USER,
  CONNECT_NONE
};

enum EventResult
{
  //! The event has been acked by the reciepient. Most events will
  //! return this.
  EVENT_ACKED,
  //! The event was succcessfully sent.  This is used by extended
  //! events.  Such as searching for users or updating user info.
  EVENT_SUCCESS,
  //! The event failed for some reason.  This should rarely happen.
  EVENT_FAILED,
  //! The event timed out while communicating with the remote socket.
  EVENT_TIMEDOUT,
  //! The event had some kind of error at some point.
  EVENT_ERROR,
  //! The event was concelled by the user and was not sent.
  //! A call to icqCancelEvent was made.
  EVENT_CANCELLED
};

/*! \brief Plugin event messages

    This class is the main event class for talking to the ICQ server and to
    plugins.  Internally all messages/urls... become ICQEvents with the
    relevant data fields set.  A plugin will receive an event in response
    to any asynchronous function call (such as icqSendMessage) eventually.
*/
class ICQEvent
{
public:
  // Accessors

  //!This is the result of the event.
  EventResult Result()         { return m_eResult; }

  //!This will be either ICQ_TCPxACK_ACCEPT if the event was accepted by
  //!the other side, ICQ_TCPxACK_REJECT if the event was rejected by the
  //!other side (should never happen really), or ICQ_TCPxACK_RETURN if the
  //!other side returned the event (meaning they are in occupied or dnd so
  //!the message would need to be sent urgent or to contact list).  This
  //!field is only relevant if the command was ICQ_CMDxTCP_START (ie the
  //!message was sent direct).
  int SubResult()              { return m_nSubResult; }

  //!This is used to identify what channel the event was sent on.  This is
  //!only non-zero for server events.
  unsigned char Channel()      { return m_nChannel; }

  //!The SNAC returned as an unsigned long.  The upper 2 bytes is the family
  //!and the lower 2 bytes is the subtype.  To compare SNAC's use the SNAC
  //!macro to convert it to an unsigned long: MAKESNAC(family, subtype).
  unsigned long SNAC()         { return m_nSNAC; }

  //!The command, for example ICQ_CMDxTCP_START.  This is only non-zero
  //!for direct connection events.
  unsigned short Command()     { return m_nCommand; }

  //!The subcommand, relevant only if this was a message/url/chat/file,
  //!in which case Command() will be ICQ_CMDxTCP_START or SNAC() will be
  //!MAKESNAC(ICQ_SNACxFAM_MESSAGE, ICQ_CMDxSND_THRU_SERVER) and this
  //!field will be ICQ_CMDxSUB_MSG...
  unsigned short SubCommand()  { return m_nSubCommand; }

  //!This is used to identify events internally, but is necessary for
  //!accepting/rejecting chat or file requests.
  unsigned short Sequence()     { return m_nSequence; }

  //!This is used to identify events internally, is are necessary for
  //!accepting/rejecting chat or file requests.
  unsigned short SubSequence() { return m_nSubSequence; }

  //!The uin of the user the event was destined for.  Only relevant if
  //!this was a message/url...
  unsigned long Uin()          { return m_nDestinationUin; }

  //!The user id that the event was destined for.  Only relevant if
  //!this was a message/url...
  char *Id()                   { return m_szId; }
  
  //!The protocol id of the user that the event was destined for.
  //!Only relevant if this was a message/url...
  unsigned long PPID()         { return m_nPPID; }

  //!Special structure containing information relevant if this is a
  //!search event.
  CSearchAck *SearchAck()      { return m_pSearchAck; }

  //!Special structure containing information relevant if this is a
  //!chat or file transfer accept or reject.
  CExtendedAck *ExtendedAck()  { return m_pExtendedAck; }
  
  //!Contains the actual CUserEvent containing the message/url...that was
  //!sent to Uin().  Can be used to resend the event.
  CUserEvent *UserEvent()      { return m_pUserEvent; }

  //!If the event was a user information update (basic/extended/meta) and
  //!the user does not exist on the contact list, this will return the user
  //!with the relevant fields set.  This is helpful in searches for example
  //!to avoid having to add the user to the list before checking their
  //!other information.
  ICQUser *UnknownUser()       { return m_pUnknownUser; }

  // Returns the event and transfers ownership to the calling function
  CUserEvent *GrabUserEvent();
  CSearchAck *GrabSearchAck();
  ICQUser *GrabUnknownUser();

  //!Compare this event to the id to see if the plugin matches a waiting
  //!event with the event that the daemon has signaled to the plugin.
  bool Equals(unsigned long) const;

  ~ICQEvent();

protected:
  ICQEvent(CICQDaemon *_xDaemon, int _nSocketDesc, CPacket *p, ConnectType _eConnect,
           unsigned long _nUin, CUserEvent *e);
  ICQEvent(CICQDaemon *_xDaemon, int _nSocketDesc, CPacket *p, ConnectType _eConnect,
           const char *_szId, unsigned long _nPPID, CUserEvent *e);
  ICQEvent(ICQEvent *);

  // Daemon only
  unsigned short SubType()     { return m_nSubType; }
  unsigned short ExtraInfo()   { return m_nExtraInfo; }

  // Compare this event to another one
  bool CompareEvent(int, unsigned short) const;
  bool CompareEvent(unsigned short) const;
  bool CompareSubSequence(unsigned long) const;
  unsigned long EventId() const;

  ConnectType    m_eConnect;
  EventResult    m_eResult;
  int            m_nSubResult;
  bool           m_bCancelled : 1;
  bool           m_Deleted : 1;
  bool           m_NoAck : 1;
  unsigned char  m_nChannel;
  unsigned long  m_nSNAC;
  unsigned short m_nCommand;
  unsigned short m_nSubCommand;
  unsigned long  m_nDestinationUin;
  unsigned short m_nSequence;
  unsigned short m_nSubSequence;
  unsigned short m_nSubType;
  unsigned short m_nExtraInfo;
  int            m_nSocketDesc;
  char           *m_szId;
  unsigned long  m_nPPID;
  CPacket        *m_pPacket;
  pthread_t      thread_send;
  bool           thread_running;
  pthread_t      thread_plugin;

  CUserEvent    *m_pUserEvent;
  CExtendedAck  *m_pExtendedAck;
  CSearchAck    *m_pSearchAck;
  ICQUser       *m_pUnknownUser;

  CICQDaemon    *m_pDaemon;

  unsigned long  m_nEventId;
  static unsigned long s_nNextEventId;

friend class CICQDaemon;
friend class CMSN;
friend void *ProcessRunningEvent_Client_tep(void *p);
friend void *ProcessRunningEvent_Server_tep(void *p);
friend void *MonitorSockets_tep(void *p);
};

//=====CICQSignal============================================================

/*
 * SIGNAL_UPDATExLIST -
 *   Indicates that the user list has changed in some way.  The sub-type
 *   will be one of the following.  In all cases the argument is 0.
 *     LIST_ADD - A user was added to the list.  The UIN will be that of
 *     the new user.
 *     LIST_REMOVE - A user was removed from the list.  The UIN will be
 *     that of the removed user.
 *     LIST_ALL - The entire list has been changed.  The UIN will be 0.
 *
 *  SIGNAL_UPDATExUSER - The user has been modified in some way.  The UIN
 *  is that of the relevant user, and the sub-type indicates what type of
 *  information was changed.  In all cases except the following the argument
 *  is 0:
 *    USER_STATUS - Indicates that the users status has changed.  The
 *    argument will be negative if the user went offline, positive if they
 *    went online, and 0 if they stayed online (for example went from NA
 *    to Away).
 *    USER_EVENTS - Indicates that the user has received or deleted an
 *    event (message, url...).  The argument will be the event id of the
 *    added event, or the negative of the event id if an event was deleted.
 *    The parameter will be the socket that the message was sent over.
 *    If the user checked our auto-response then the argument will be 0.
 *    USER_SECURITY - The users security status changed.  If the arg is
 *    1 the user is now secure, if it's 0, the user is no longer secure.
 *    USER_TYPING - Indicates the typing status of the user has changed.
 *
 *  SIGNAL_LOGOFF - Indicates that we logged off.  All parameters are 0.

 *
 *  SIGNAL_ONEVENT - FIXME: MISSING DESCRIPTION
 *-------------------------------------------------------------------------*/
const unsigned long SIGNAL_UPDATExLIST           = 0x00000001;
const unsigned long SIGNAL_UPDATExUSER           = 0x00000002;
//! Indicates that we have successfully logged on to the specified
//! network.  Argument is the Protocol ID.  All other arguments are
//! 0.
const unsigned long SIGNAL_LOGON                 = 0x00000004;
//! Indicates that we have logged off.  All parameters are 0.
const unsigned long SIGNAL_LOGOFF                = 0x00000008;
const unsigned long SIGNAL_ONEVENT               = 0x00000010;
//! The UIN is that of the user we want to view the oldest event of
//! or 0 if we just want to view the oldest pending event. All other
//! parameters are 0.
const unsigned long SIGNAL_UI_VIEWEVENT          = 0x00000020;
//! The UIN is that of the user we want to send a message.  All other
//! parameters are 0.
const unsigned long SIGNAL_UI_MESSAGE            = 0x00000040;
//! The UIN has been successfully added to the server list.  The
//! server side alias is set to the UIN.  The plugin may call 
//! icqRenameUser after receiving this.  Sub-type is 0.
const unsigned long SIGNAL_ADDxSERVERxLIST       = 0x00000080;
//! Indicates new protocol plugin has been successfully loaded.
//! The sub-type is the id of the plugin.  This is used as a parameter
//! for all functions dealing with this new protocol.  The UIN and all
//! other parameters are 0.
const unsigned long SIGNAL_NEWxPROTO_PLUGIN      = 0x00000100;
//! Returns the event tag of an event from a protocol plugin.
//! The UI plugin will use this event tag to keep track of 
//! all sent events.
const unsigned long SIGNAL_EVENTxID              = 0x00000200;
//! A user has joined a conversation. m_nArgument is the socket
//! of the conversation that the user has joined.
const unsigned long SIGNAL_CONVOxJOIN            = 0x00000400;
//! A user has left a conversation. m_nArgument is the socket
//! of the conversation that the user has left.
const unsigned long SIGNAL_CONVOxLEAVE           = 0x00000800;
//! A socket has been set for this user now.
const unsigned long SIGNAL_SOCKET                = 0x00010000;
//! A verification image is available for the gui to use
//! that is located at BASE_DIR + "verify.jpg"
const unsigned long SIGNAL_VERIFY_IMAGE          = 0x00020000;
//! A new user has been successfully registered.
const unsigned long SIGNAL_NEW_OWNER             = 0x00040000;
//! Used by a UI plugin to tell the daemon that all signals should be
//! sent to the UI plugin.
const unsigned long SIGNAL_ALL                   = 0xFFFFFFFF;

// logoff constants
//! The user requested to logoff and the action succeeded.
const unsigned long LOGOFF_REQUESTED             = 0x00000000;
//! The user requested to logon but failed.  The rate limit of the
//! server has been exceeded.
const unsigned long LOGOFF_RATE                  = 0x00000001;
//! The user requested to logon but failed.  The username and password
//! combination is invalid.
const unsigned long LOGOFF_PASSWORD              = 0x00000002;

// User information update constants
const unsigned long USER_STATUS                 = 1;
const unsigned long USER_EVENTS                 = 2;
const unsigned long USER_BASIC                  = 3;
const unsigned long USER_EXT                    = 4;
const unsigned long USER_GENERAL                = 5;
const unsigned long USER_MORE                   = 6;
const unsigned long USER_WORK                   = 7;
const unsigned long USER_ABOUT                  = 8;
const unsigned long USER_SECURITY               = 9;
const unsigned long USER_MORE2                  = 10;
const unsigned long USER_HP                     = 11;
const unsigned long USER_PHONExBOOK             = 12;
const unsigned long USER_PICTURE                = 13;
const unsigned long USER_TYPING                 = 14;

const unsigned long LIST_ADD                     = 1;
const unsigned long LIST_REMOVE                  = 2;
const unsigned long LIST_ALL                     = 3;

/*! \brief Plugin notification messages

    This class controls all asynchronous plugin notifications.  When a plugin
    registers with the Licq daemon it informs the daemon of what signals it
    is interested in.  From then on, at any time it may receive a signal
    from the following list.  Each signal contains the signal type, an
    optional sub-type, uin and signal specific argument.
*/
class CICQSignal
{
public:
  CICQSignal(unsigned long _nSignal, unsigned long _nSubSignal, unsigned long _nUin,
             int nArg = 0, unsigned long nCID = 0);
  CICQSignal(unsigned long _nSignal, unsigned long _nSubSignal, const char *_szId,
             unsigned long _nPPID, int nArg = 0, unsigned long nCID = 0);
  CICQSignal(CICQSignal *s);
  ~CICQSignal();

  //!Returns the signal being posted to the plugin.
  unsigned long Signal() { return m_nSignal; }
  //!Returns the sub-signal being posted to the plugin.
  unsigned long SubSignal() { return m_nSubSignal; }
  //!UIN that the signal is related.  See signals to understand how this
  //!value is set.
  unsigned long Uin()   { return m_nUin; }
  char *Id()            { return m_szId; }
  unsigned long PPID()  { return m_nPPID; }
  int Argument()        { return m_nArgument; }
  unsigned long CID()   { return m_nCID; }
protected:
  unsigned long m_nSignal;
  unsigned long m_nSubSignal;
  unsigned long m_nUin;
  char *m_szId;
  unsigned long m_nPPID;
  int m_nArgument;
  unsigned long m_nCID;
};

//! Signals that can be sent to protocol plugins.
enum SIGNAL_TYPE
{
  //! The user requested this protocol to logon.
  PROTOxLOGON = 1,
  //! The user requested this protocol to log off.
  PROTOxLOGOFF,
  //! The user requested this protocol to change status.
  PROTOxCHANGE_STATUS,
  //! The user requested this protocol to add a new user.
  PROTOxADD_USER,
  //! The user requested this protocol to remove a user.
  PROTOxREM_USER,
  //! The user requested this protocol to rename the user on the server's list.
  PROTOxRENAME_USER,
  //! The user requested this protocol to send a message.
  PROTOxSENDxMSG,
  //! The user has started typing and wants to let the remote user know about
  //! about it.
  PROTOxSENDxTYPING_NOTIFICATION,
  //! The user granted authorization for a user to add them to his
  //! contact list.
  PROTOxSENDxGRANTxAUTH,
  //! The user refused authorization for a user to add them to his
  //! contact list.
  PROTOxSENDxREFUSExAUTH,
  //! The user has requested the user's profile/information.
  PROTOxREQUESTxINFO,
  //! The user has requested to update the owner's profile/information.
  PROTOxUPDATExINFO,
  //! The user has requested this user be added to the Invisible/Block list.
  PROTOxBLOCKxUSER,
  //! The user has requested this user be removed from the Invisible/Block
  //! list,
  PROTOxUNBLOCKxUSER,
  //! The user has requested this user be added to the Visible/Accept list.
  PROTOxACCEPTxUSER,
  //! The user has requested this user be removed fromt he Visible/Accept
  //! list.
  PROTOxUNACCEPTxUSER,
  //! The user has requsted this user be put on the Ignore list.
  PROTOxIGNORExUSER,
  //! The user has requested this user be removed fromt he Ignore list.
  PROTOxUNIGNORExUSER,
  //! The user has requested to send a file to this user.
  PROTOxSENDxFILE,
  //! The user has requested to send a chat invitation to this user.
  PROTOxSENDxCHAT,
  //! The user is cancelling an event (chat, secure, or file for ICQ)
  PROTOxCANCELxEVENT,
  //! The user has requested to send an accept/refuse reply to a file/chat
  //! request
  PROTOxSENDxEVENTxREPLY,
  //! The user has opened a chat window with this user
  PROTOxOPENEDxWINDOW,
  //! The user has closed a chat window with this user
  PROTOxCLOSEDxWINDOW,
  //! The user has requested an SSL secure channel with this user
  PROTOxOPENxSECURE,
  //! The user has requested to close the SSL secure channel with this user
  PROTOxCLOSExSECURE
};

//! The class that gets passed to protocol plugins when a signal
//! is sent.
class CSignal
{
public:
  CSignal(SIGNAL_TYPE, const char *, unsigned long nCID = 0);
  CSignal(CSignal *);
  virtual ~CSignal();

  //! The signal is being sent to the plugin.
  SIGNAL_TYPE Type()  { return m_eType; }
  //! The user id that this signal is being used for.
  char *Id()          { return m_szId; }
  //! The conversation id to use (gets the socket).
  unsigned long CID() { return m_nCID; }
  //! The calling thread.
  pthread_t Thread() { return thread_plugin; }

private:
  char  *m_szId;
  SIGNAL_TYPE m_eType;
  unsigned long m_nCID;
  pthread_t thread_plugin;
};

class CLogonSignal : public CSignal
{
public:
  CLogonSignal(unsigned long);
  virtual ~CLogonSignal() { }
  //! The requested initial status.
  unsigned long LogonStatus() { return m_nLogonStatus; }

private:
  unsigned long m_nLogonStatus;
};

class CLogoffSignal : public CSignal
{
public:
  CLogoffSignal();
};

class CChangeStatusSignal : public CSignal
{
public:
  CChangeStatusSignal(unsigned long);
  //! The requested status.
  unsigned long Status()  { return m_nStatus; }

private:
  unsigned long m_nStatus;
};

class CAddUserSignal : public CSignal
{
public:
  CAddUserSignal(const char *, bool);
  virtual ~CAddUserSignal() { }
  //! True if authorization is required to add this user.
  bool AuthRequired() { return m_bAuthRequired; }

private:
  bool m_bAuthRequired;
};

class CRemoveUserSignal : public CSignal
{
public:
  CRemoveUserSignal(const char *);
};

class CRenameUserSignal : public CSignal
{
public:
  CRenameUserSignal(const char *);
};

/*! \brief Signal to a protocol plugin to send a messasge

    This is sent when ProtoSendMessage has been called. It
    contains the information necessary for a message to be
    sent by a protocol plugin
*/
class CSendMessageSignal : public CSignal
{
public:
  CSendMessageSignal(const char *szId, const char *szMsg, unsigned long nCID = 0);
  virtual ~CSendMessageSignal() { if (m_szMsg) free(m_szMsg); }
  //! The message to be sent
  char *Message() { return m_szMsg; }
  
private:
  char *m_szMsg;
};

class CTypingNotificationSignal : public CSignal
{
public:
  CTypingNotificationSignal(const char *szId, bool bActive, unsigned long nCID = 0);
  bool Active() { return m_bActive; }
private:
  bool m_bActive;
};

class CGrantAuthSignal : public CSignal
{
public:
  CGrantAuthSignal(const char *, const char *);
  virtual ~CGrantAuthSignal() { if (m_szMsg) free(m_szMsg); }
  char *Message() { return m_szMsg; }

private:
  char *m_szMsg;
};

class CRefuseAuthSignal : public CSignal
{
public:
  CRefuseAuthSignal(const char *, const char *);
  virtual ~CRefuseAuthSignal() { if (m_szMsg) free(m_szMsg); }

private:
  char *m_szMsg;
};

class CRequestInfo : public CSignal
{
public:
  CRequestInfo(const char *);
};

class CUpdateInfoSignal : public CSignal
{
public:
  CUpdateInfoSignal(const char *szAlias, const char *szFirstName,
    const char *szLastName, const char *szEmail,
    const char *szCity,
    const char *szState, const char *szPhoneNumber,
    const char *szFaxNumber, const char *szAddress,
    const char *szCellularNumber, const char *szZipCode);
  virtual ~CUpdateInfoSignal();

    char *Alias()        { return m_szAlias; }
    char *FirstName()    { return m_szFirstName; }
    char *LastName()     { return m_szLastName; }
    char *Email()        { return m_szEmail; }
    char *City()         { return m_szCity; }
    char *State()        { return m_szState; }
    char *PhoneNumber()  { return m_szPhoneNumber; }
    char *FaxNumber()    { return m_szFaxNumber; }
    char *Address()      { return m_szAddress; }
    char *CellNumber()   { return m_szCellNumber; }
    char *ZipCode()      { return m_szZipCode; }
    
private:
  char *m_szAlias,
       *m_szFirstName,
       *m_szLastName,
       *m_szEmail,
       *m_szCity,
       *m_szState,
       *m_szPhoneNumber,
       *m_szFaxNumber,
       *m_szAddress,
       *m_szCellNumber,
       *m_szZipCode;
};

class CBlockUserSignal : public CSignal
{
public:
  CBlockUserSignal(const char *);
};

class CUnblockUserSignal : public CSignal
{
public:
  CUnblockUserSignal(const char *);
};

class CAcceptUserSignal : public CSignal
{
public:
  CAcceptUserSignal(const char *);
};

class CUnacceptUserSignal : public CSignal
{
public:
  CUnacceptUserSignal(const char *);
};

class CIgnoreUserSignal : public CSignal
{
public:
  CIgnoreUserSignal(const char *);
};

class CUnignoreUserSignal : public CSignal
{
public:
  CUnignoreUserSignal(const char *);
};

class CSendFileSignal : public CSignal
{
public:
  CSendFileSignal(const char *_szUser, const char *_szFile,
                  const char *_szMessage, ConstFileList &_lFileList);
  virtual ~CSendFileSignal();

  char *GetFileName() { return m_szFile; }
  char *GetMessage()  { return m_szMessage; }
  ConstFileList GetFileList() { return m_lFileList; }
  
private:
  char *m_szFile,
       *m_szMessage;
  ConstFileList m_lFileList;
};

class CSendChatSignal : public CSignal
{
public:
  CSendChatSignal(const char *_szUser, const char *_szMessage);
  virtual ~CSendChatSignal() { if (m_szMessage) free(m_szMessage); }

  char *GetMessage() { return m_szMessage; }

private:
  char *m_szMessage;
};

class CCancelEventSignal : public CSignal
{
public:
  CCancelEventSignal(const char *_szUser, unsigned long _nFlag);

  unsigned long GetFlag() { return m_nFlag; }
    
private:
  unsigned long m_nFlag;
};

class CSendEventReplySignal : public CSignal
{
public:
  CSendEventReplySignal(const char *_szUser, const char *_szMessage,
                        bool _bAccepted, unsigned short nPort,
                        unsigned long nSequence = 0,
                        unsigned long nFlag1 = 0, unsigned long nFlag2 = 0,
                        bool bDirect = false);
  virtual ~CSendEventReplySignal() { if (m_szMessage) free(m_szMessage); }

  char *GetMessage() { return m_szMessage; }
  bool GetAccept()   { return m_bAccept; }
  bool GetDirect()   { return m_bDirect; }
  unsigned short GetPort()    { return m_nPort; }
  unsigned long GetSequence() { return m_nSequence; }
  unsigned long GetFlag()     { return m_nFlag; }
  unsigned long GetFlag2()    { return m_nFlag2; } 
  
private:
  char *m_szMessage;
  bool m_bAccept,
       m_bDirect;
  unsigned short m_nPort;
  unsigned long m_nSequence,
                m_nFlag,
                m_nFlag2;
};

class COpenedWindowSignal : public CSignal
{
public:
  COpenedWindowSignal(const char *);
};

class CClosedWindowSignal : public CSignal
{
public:
  CClosedWindowSignal(const char *);
};

class COpenSecureSignal : public CSignal
{
public:
  COpenSecureSignal(const char *);
};

class CCloseSecureSignal : public CSignal
{
public:
  CCloseSecureSignal(const char *);
};

#endif

