// -*- c-basic-offset: 2 -*-
/* ----------------------------------------------------------------------------
 * Licq - A ICQ Client for Unix
 * Copyright (C) 1998 - 2003 Licq developers
 *
 * This program is licensed under the terms found in the LICENSE file.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <vector>

#ifdef HAVE_INET_ATON
#include <arpa/inet.h>
#endif

// Localization
#include "gettext.h"

#include "licq_constants.h"
#include "licq_user.h"
#include "licq_countrycodes.h"
#include "licq_occupationcodes.h"
#include "licq_languagecodes.h"
#include "licq_log.h"
#include "licq_packets.h"
#include "licq_icqd.h"
#include "licq_socket.h"
#include "support.h"
#include "pthread_rdwr.h"

using std::vector;

ICQUserCategory::ICQUserCategory(UserCat uc)
{
  used = 0;
  m_uc = uc;
}

ICQUserCategory::~ICQUserCategory()
{
  Clean();
}

void ICQUserCategory::Clean()
{      
  unsigned short i;

  for(i = 0; i < used; i++)
    free((void *)data[i].descr);
    
  used = 0;
}

bool ICQUserCategory::SaveToDisk(CIniFile &m_fConf,const char *const szN,
                             const char *const szCat,const char *const szDescr)
{      
  char buff[255];
  unsigned short i;

  if (!m_fConf.ReloadFile())
  {
    gLog.Error("%sError opening '%s' for reading.\n"
               "%sSee log for details.\n", L_ERRORxSTR, m_fConf.FileName(),
               L_BLANKxSTR);
      return false;
  }

  m_fConf.SetSection("user");
  m_fConf.WriteNum(szN, used);

  for(i = 0; i < used; i ++)
  {
    snprintf(buff, sizeof(buff), szCat, i);
    m_fConf.WriteNum(buff, data[i].id);
    snprintf(buff, sizeof(buff), szDescr, i);
    m_fConf.WriteStr(buff, data[i].descr);
  }

  if (!m_fConf.FlushFile())
  {
    gLog.Error("%sError opening '%s' for writing.\n"
               "%sSee log for details.\n", L_ERRORxSTR,
               m_fConf.FileName(), L_BLANKxSTR);
    return false;
  }

  m_fConf.CloseFile();
  return true;
}

bool ICQUserCategory::LoadFromDisk(CIniFile &m_fConf, const char *const szN,
       const char *const szCat, const char *const szDescr)
{
  unsigned short i, j, ret, n;
  char buff[255];
  char szTemp[MAX_LINE_LEN];

  Clean();
  m_fConf.SetSection("user");
  ret = m_fConf.ReadNum(szN, used, 0);

  if (used > MAX_CATEGORIES)
  {
    gLog.Warn("%sTrying to load more categories than the max limit."
              "Truncating.\n",L_WARNxSTR);
    used = MAX_CATEGORIES;
  }

  for(i = j = 0, n = used; i < n; i++)
  {
    snprintf(buff, sizeof(buff), szCat, i);
    ret = m_fConf.ReadNum(buff, data[j].id, 0);

    snprintf(buff, sizeof(buff), szDescr, i);
    if (ret)
    {
      ret = m_fConf.ReadStr(buff, szTemp);
      if (ret)
      {
        data[j].descr = strdup(szTemp);
        if (data[j].descr == NULL)
          ret = 0;
      }
      
    }

    /* this one failed loading. we ignore and keep trying */
    if (!ret)
      used--;
    else
      j++;
  }
   
  return true;
}

bool ICQUserCategory::AddCategory(unsigned short cat_, const char *descr_)
{
  bool nRet = true;

  if (used == MAX_CATEGORIES || descr_ == NULL)
    nRet =  false;
  else if (cat_ != 0)
  {
    data[used].id = cat_;
    data[used].descr = strdup(descr_);

    if (data[used].descr == NULL)
      nRet = false;
    else
      used++;
  }
   
  return nRet;
}

bool ICQUserCategory::Get(unsigned d,short unsigned *id, char const*  * descr) 
                                                                         const
{
  bool nRet = false;

  //assert(id && descr);

  if(nRet = (d < used ))
  {
    *id = data[d].id;
    *descr = (const char *)data[d].descr;
  }
  
  return nRet;
}

//===========================================================================

ICQUserPhoneBook::ICQUserPhoneBook()
{
}

ICQUserPhoneBook::~ICQUserPhoneBook()
{
  Clean();
}

void ICQUserPhoneBook::AddEntry(const struct PhoneBookEntry *entry)
{
  struct PhoneBookEntry new_entry = *entry;
  new_entry.szDescription = strdup(entry->szDescription);
  new_entry.szAreaCode = strdup(entry->szAreaCode);
  new_entry.szPhoneNumber = strdup(entry->szPhoneNumber);
  new_entry.szExtension = strdup(entry->szExtension);
  new_entry.szCountry = strdup(entry->szCountry);
  new_entry.szGateway = strdup(entry->szGateway);

  PhoneBookVector.push_back(new_entry);
}

void ICQUserPhoneBook::SetEntry(const struct PhoneBookEntry *entry,
                                unsigned long nEntry)
{
  if (nEntry >= PhoneBookVector.size())
  {
    AddEntry(entry);
    return;
  }

  free(PhoneBookVector[nEntry].szDescription);
  free(PhoneBookVector[nEntry].szAreaCode);
  free(PhoneBookVector[nEntry].szPhoneNumber);
  free(PhoneBookVector[nEntry].szExtension);
  free(PhoneBookVector[nEntry].szCountry);
  free(PhoneBookVector[nEntry].szGateway);

  PhoneBookVector[nEntry] = *entry;
  PhoneBookVector[nEntry].szDescription = strdup(entry->szDescription);
  PhoneBookVector[nEntry].szAreaCode = strdup(entry->szAreaCode);
  PhoneBookVector[nEntry].szPhoneNumber = strdup(entry->szPhoneNumber);
  PhoneBookVector[nEntry].szExtension = strdup(entry->szExtension);
  PhoneBookVector[nEntry].szCountry = strdup(entry->szCountry);
  PhoneBookVector[nEntry].szGateway = strdup(entry->szGateway);
}

void ICQUserPhoneBook::ClearEntry(unsigned long nEntry)
{
  if (nEntry >= PhoneBookVector.size())
    return;

  vector<struct PhoneBookEntry>::iterator i = PhoneBookVector.begin();
  for (;nEntry > 0; nEntry--, i++);

  free((*i).szDescription);
  free((*i).szAreaCode);
  free((*i).szPhoneNumber);
  free((*i).szExtension);
  free((*i).szCountry);
  free((*i).szGateway);

  PhoneBookVector.erase(i);
}

void ICQUserPhoneBook::Clean()
{
  while (PhoneBookVector.size() > 0)
    ClearEntry(PhoneBookVector.size() - 1);
}

void ICQUserPhoneBook::SetActive(long nEntry)
{
  vector<struct PhoneBookEntry>::iterator iter;
  long i;
  for (i = 0, iter = PhoneBookVector.begin(); iter != PhoneBookVector.end()
                                                 ; i++, iter++)
    (*iter).nActive = (i == nEntry);
}

bool ICQUserPhoneBook::Get(unsigned long nEntry,
                           const struct PhoneBookEntry **entry)
{
  if (nEntry >= PhoneBookVector.size())
    return false;

  *entry = &PhoneBookVector[nEntry];
  return true;
}

bool ICQUserPhoneBook::SaveToDisk(CIniFile &m_fConf)
{
  char buff[40];

  if (!m_fConf.ReloadFile())
  {
    gLog.Error("%sError opening '%s' for reading.\n"
               "%sSee log for details.\n", L_ERRORxSTR, m_fConf.FileName(),
               L_BLANKxSTR);
    return false;
  }

  m_fConf.SetSection("user");

  m_fConf.WriteNum("PhoneEntries", (unsigned long)PhoneBookVector.size());

  for (unsigned long i = 0 ; i < PhoneBookVector.size(); i++)
  {
    snprintf(buff, sizeof(buff), "PhoneDescription%lu", i);
    m_fConf.WriteStr(buff, PhoneBookVector[i].szDescription);

    snprintf(buff, sizeof(buff), "PhoneAreaCode%lu", i);
    m_fConf.WriteStr(buff, PhoneBookVector[i].szAreaCode);

    snprintf(buff, sizeof(buff), "PhoneNumber%lu", i);
    m_fConf.WriteStr(buff, PhoneBookVector[i].szPhoneNumber);

    snprintf(buff, sizeof(buff), "PhoneExtension%lu", i);
    m_fConf.WriteStr(buff, PhoneBookVector[i].szExtension);

    snprintf(buff, sizeof(buff), "PhoneCountry%lu", i);
    m_fConf.WriteStr(buff, PhoneBookVector[i].szCountry);

    snprintf(buff, sizeof(buff), "PhoneActive%lu", i);
    m_fConf.WriteNum(buff, PhoneBookVector[i].nActive);

    snprintf(buff, sizeof(buff), "PhoneType%lu", i);
    m_fConf.WriteNum(buff, PhoneBookVector[i].nType);

    snprintf(buff, sizeof(buff), "PhoneGateway%lu", i);
    m_fConf.WriteStr(buff, PhoneBookVector[i].szGateway);

    snprintf(buff, sizeof(buff), "PhoneGatewayType%lu", i);
    m_fConf.WriteNum(buff, PhoneBookVector[i].nGatewayType);

    snprintf(buff, sizeof(buff), "PhoneSmsAvailable%lu", i);
    m_fConf.WriteNum(buff, PhoneBookVector[i].nSmsAvailable);

    snprintf(buff, sizeof(buff), "PhoneRemoveLeading0s%lu", i);
    m_fConf.WriteNum(buff, PhoneBookVector[i].nRemoveLeading0s);

    snprintf(buff, sizeof(buff), "PhonePublish%lu", i);
    m_fConf.WriteNum(buff, PhoneBookVector[i].nPublish);
  }

  if (!m_fConf.FlushFile())
  {
    gLog.Error("%sError opening '%s' for writing.\n"
               "%sSee log for details.\n", L_ERRORxSTR,
               m_fConf.FileName(), L_BLANKxSTR);
    return false;
  }

  m_fConf.CloseFile();
  return true;
}

bool ICQUserPhoneBook::LoadFromDisk(CIniFile &m_fConf)
{
  char buff[40];
  char szDescription[MAX_LINE_LEN];
  char szAreaCode[MAX_LINE_LEN];
  char szPhoneNumber[MAX_LINE_LEN];
  char szExtension[MAX_LINE_LEN];
  char szCountry[MAX_LINE_LEN];
  char szGateway[MAX_LINE_LEN];
  struct PhoneBookEntry entry = {
                                  szDescription,
                                  szAreaCode,
                                  szPhoneNumber,
                                  szExtension,
                                  szCountry,
                                  0, 0,
                                  szGateway,
                                  0, 0, 0, 0
                                };

  Clean();
  m_fConf.SetSection("user");

  unsigned long nNumEntries;
  m_fConf.ReadNum("PhoneEntries", nNumEntries);
  for (unsigned long i = 0; i < nNumEntries; i++)
  {
    snprintf(buff, sizeof(buff), "PhoneDescription%lu", i);
    m_fConf.ReadStr(buff, entry.szDescription, "");

    snprintf(buff, sizeof(buff), "PhoneAreaCode%lu", i);
    m_fConf.ReadStr(buff, entry.szAreaCode, "");

    snprintf(buff, sizeof(buff), "PhoneNumber%lu", i);
    m_fConf.ReadStr(buff, entry.szPhoneNumber, "");

    snprintf(buff, sizeof(buff), "PhoneExtension%lu", i);
    m_fConf.ReadStr(buff, entry.szExtension, "");

    snprintf(buff, sizeof(buff), "PhoneCountry%lu", i);
    m_fConf.ReadStr(buff, entry.szCountry, "");

    snprintf(buff, sizeof(buff), "PhoneActive%lu", i);
    m_fConf.ReadNum(buff, entry.nActive, 0);

    snprintf(buff, sizeof(buff), "PhoneType%lu", i);
    m_fConf.ReadNum(buff, entry.nType, 0);

    snprintf(buff, sizeof(buff), "PhoneGateway%lu", i);
    m_fConf.ReadStr(buff, entry.szGateway, "");

    snprintf(buff, sizeof(buff), "PhoneGatewayType%lu", i);
    m_fConf.ReadNum(buff, entry.nGatewayType, 1);

    snprintf(buff, sizeof(buff), "PhoneSmsAvailable%lu", i);
    m_fConf.ReadNum(buff, entry.nSmsAvailable, 0);

    snprintf(buff, sizeof(buff), "PhoneRemoveLeading0s%lu", i);
    m_fConf.ReadNum(buff, entry.nRemoveLeading0s, 1);

    snprintf(buff, sizeof(buff), "PhonePublish%lu", i);
    m_fConf.ReadNum(buff, entry.nPublish, 2);

    AddEntry(&entry);
  }

  return true;
}
//===========================================================================


class CUserManager gUserManager;

const char *GroupsSystemNames[NUM_GROUPS_SYSTEM+1] = {
  "",
  "Online Notify",
  "Visible List",
  "Invisible List",
  "Ignore List",
  "New Users"
};

char *PPIDSTRING(unsigned long id)
{
  char *p = new char[5];
  p[0] = ((id & 0xFF000000) >> 24);
  p[1] = ((id & 0x00FF0000) >> 16);
  p[2] = ((id & 0x0000FF00) >> 8);
  p[3] = ((id & 0x000000FF));
  p[4] = '\0';
  return p;
}

/*---------------------------------------------------------------------------
 * ICQUser::Lock
 *-------------------------------------------------------------------------*/
void ICQUser::Lock(unsigned short _nLockType)
{
  switch (_nLockType)
  {
  case LOCK_R:
    pthread_rdwr_rlock_np (&mutex_rw);
    break;
  case LOCK_W:
    pthread_rdwr_wlock_np(&mutex_rw);
    break;
  default:
    break;
  }
  m_nLockType = _nLockType;
}


/*---------------------------------------------------------------------------
 * ICQUser::Unlock
 *-------------------------------------------------------------------------*/
void ICQUser::Unlock()
{
  unsigned short nLockType = m_nLockType;
  m_nLockType = LOCK_R;
  switch (nLockType)
  {
  case LOCK_R:
    pthread_rdwr_runlock_np(&mutex_rw);
    break;
  case LOCK_W:
    pthread_rdwr_wunlock_np(&mutex_rw);
    break;
  default:
    break;
  }
}



//=====CUserManager=============================================================
CUserManager::CUserManager() : m_hUsers(USER_HASH_SIZE)
{
  // Set up the basic all users and new users group
  pthread_rdwr_init_np(&mutex_grouplist, NULL);
  pthread_rdwr_init_np(&mutex_userlist, NULL);
  pthread_rdwr_init_np(&mutex_groupidlist, NULL);
  pthread_rdwr_init_np(&mutex_ownerlist, NULL);
  m_nOwnerListLockType = LOCK_N;
  m_nUserListLockType = m_nGroupListLockType = m_nGroupIDListLockType = LOCK_N;

  m_xOwner = NULL;
  m_nOwnerUin = 0;
}


CUserManager::~CUserManager()
{
  UserList::iterator iter;
  for (iter = m_vpcUsers.begin(); iter != m_vpcUsers.end(); iter++)
  {
    delete *iter;
  }

  GroupList::iterator g_iter;
  for (g_iter = m_vszGroups.begin(); g_iter != m_vszGroups.end(); g_iter++)
    free(*g_iter);

  OwnerList::iterator o_iter;
  for (o_iter = m_vpcOwners.begin(); o_iter != m_vpcOwners.end(); o_iter++)
    delete *o_iter;
}

void CUserManager::SetOwnerUin(unsigned long _nUin)
{
  char szUin[24];
  sprintf(szUin, "%lu", _nUin);
  AddOwner(szUin, LICQ_PPID);
}

void CUserManager::AddOwner(const char *_szId, unsigned long _nPPID)
{
  ICQOwner *o = new ICQOwner(_szId, _nPPID);

  if (_nPPID == LICQ_PPID)
    m_nOwnerUin = o->Uin();

  LockOwnerList(LOCK_W);
  m_vpcOwners.push_back(o);
  UnlockOwnerList();
}

/*---------------------------------------------------------------------------
 * CUserManager::Load
 *-------------------------------------------------------------------------*/
bool CUserManager::Load()
{
  gLog.Info(tr("%sUser configuration.\n"), L_INITxSTR);

  // Load the group info from licq.conf
  char filename[MAX_FILENAME_LEN];
  filename[MAX_FILENAME_LEN - 1] = '\0';
  snprintf(filename, MAX_FILENAME_LEN - 1, "%s/licq.conf", BASE_DIR);
  CIniFile licqConf(INI_FxERROR | INI_FxFATAL);
  licqConf.LoadFile(filename);

  unsigned short nOwners;
  licqConf.SetSection("owners");
  licqConf.ReadNum("NumOfOwners", nOwners, 0);

  m_bAllowSave = false;
  char sOwnerIDKey[MAX_KEYxNAME_LEN], sOwnerID[MAX_KEYxNAME_LEN],
       sOwnerPPIDKey[MAX_KEYxNAME_LEN], sOwnerPPID[MAX_KEYxNAME_LEN];
  unsigned long nPPID;

  //TODO Check for loaded plugins before the owner, so we can see
  //which owner(s) to load
  for (unsigned short i = 1; i <= nOwners; i++)
  {
    sprintf(sOwnerIDKey, "Owner%d.Id", i);
    licqConf.ReadStr(sOwnerIDKey, sOwnerID);
    sprintf(sOwnerPPIDKey, "Owner%d.PPID", i);
    licqConf.ReadStr(sOwnerPPIDKey, sOwnerPPID);
    nPPID = (sOwnerPPID[0] << 24) | (sOwnerPPID[1] << 16) |
            (sOwnerPPID[2] << 8) | (sOwnerPPID[3]);
    AddOwner(sOwnerID, nPPID);
  }

  unsigned short nGroups;
  licqConf.SetSection("groups");
  licqConf.ReadNum("NumOfGroups", nGroups);

  m_bAllowSave = false;
  char sGroupKey[MAX_KEYxNAME_LEN], sGroupIDKey[MAX_KEYxNAME_LEN],
       sGroupName[MAX_LINE_LEN];
  unsigned short nID;
  for (unsigned short i = 1; i <= nGroups; i++)
  {
     sprintf(sGroupKey, "Group%d.name", i);
     licqConf.ReadStr(sGroupKey, sGroupName);
     licqConf.ClearFlag( INI_FxFATAL );
     sprintf(sGroupIDKey, "Group%d.id", i);
     licqConf.ReadNum(sGroupIDKey, nID, 0);
     licqConf.SetFlag( INI_FxFATAL );
     char *szTempGroup = strdup(sGroupName);
     AddGroup(szTempGroup, nID);
     free(szTempGroup);
  }
  m_bAllowSave = true;

  licqConf.ReadNum("DefaultGroup", m_nDefaultGroup, 0);
  if(m_nDefaultGroup >=  1024)
      m_nDefaultGroup = 0;
  licqConf.ClearFlag(INI_FxFATAL);
  licqConf.ReadNum("NewUserGroup", m_nNewUserGroup, 0);
  licqConf.SetFlag(INI_FxFATAL);
  licqConf.CloseFile();

  // Load users from users.conf
  snprintf(filename, MAX_FILENAME_LEN - 1, "%s/users.conf", BASE_DIR);
  CIniFile usersConf(INI_FxFATAL | INI_FxERROR);
  usersConf.LoadFile(filename);

  unsigned short nUsers;
  usersConf.SetSection("users");
  usersConf.ReadNum("NumOfUsers", nUsers);
  gLog.Info(tr("%sLoading %d users.\n"), L_INITxSTR, nUsers);

  // TODO: We need to only load users of protocol plugins that are loaded!
  char sUserKey[MAX_KEYxNAME_LEN];
  char szFile[MAX_LINE_LEN];
  char szId[MAX_LINE_LEN];
  char *sz;
  ICQUser *u;
  usersConf.SetFlags(INI_FxWARN);
  for (unsigned short i = 1; i<= nUsers; i++)
  {
    sprintf(sUserKey, "User%d", i);
    if (!usersConf.ReadStr(sUserKey, szFile, ""))
    {
      gLog.Warn(tr("%sSkipping user %i, empty key.\n"), L_WARNxSTR, i);
      continue;
    }
    snprintf(filename, MAX_FILENAME_LEN - 1, "%s/%s/%s", BASE_DIR, USER_DIR,
             szFile);
    sz = strrchr(szFile, '.');
    if( !sz )
    {
      gLog.Error(tr("%sFatal error reading protocol information for User%d with ID '%s'.\n"
                    "%sPlease check \"%s/users.conf\".\n"), L_ERRORxSTR, i, szFile, 
                    L_BLANKxSTR, BASE_DIR);
      exit(1);
    }
    strncpy(szId, szFile, sz - szFile);
    szId[sz - szFile] = '\0';
    nPPID = (*(sz+1)) << 24 | (*(sz+2)) << 16 | (*(sz+3)) << 8 | (*(sz+4));
    u = new ICQUser(szId, nPPID, filename);
    u->AddToContactList();
    m_hUsers.Store(u, szId, nPPID);
    m_vpcUsers.push_back(u);
  }

  return true;
}

void CUserManager::AddUser(ICQUser *pUser, const char *_szId, unsigned long _nPPID)
{
  pUser->Lock(LOCK_W);

  if (!pUser->NotInList())
  {
    // Set this user to be on the contact list
    pUser->AddToContactList();
    //pUser->SetEnableSave(true);
    pUser->SaveLicqInfo();
    pUser->SaveGeneralInfo();
    pUser->SaveMoreInfo();
    pUser->SaveHomepageInfo();
    pUser->SaveWorkInfo();
    pUser->SaveAboutInfo();
    pUser->SaveInterestsInfo();
    pUser->SaveBackgroundsInfo();
    pUser->SaveOrganizationsInfo();
    pUser->SavePhoneBookInfo();
    pUser->SavePictureInfo();
    pUser->SaveExtInfo();
  }

  // Store the user in the hash table
  m_hUsers.Store(pUser, _szId, _nPPID);
  // Reorder the user to the correct place
  m_vpcUsers.push_back(pUser);
}

void CUserManager::RemoveUser(const char *_szId, unsigned long _nPPID)
{
  ICQUser *u = FetchUser(_szId, _nPPID, LOCK_W);
  if (u == NULL) return;
  if (!u->NotInList())
    u->RemoveFiles();
  LockUserList(LOCK_W);
  UserList::iterator iter = m_vpcUsers.begin();
  while (iter != m_vpcUsers.end() && u != (*iter)) iter++;
  if (iter == m_vpcUsers.end())
    gLog.Warn("%sInteral Error: CUserManager::RemoveUser():\n"
              "%sUser \"%s\" (%s) not found in vector.\n",
              L_WARNxSTR, L_BLANKxSTR, u->GetAlias(), u->IdString());
  else
    m_vpcUsers.erase(iter);
  DropUser(u);
  m_hUsers.Remove(_szId, _nPPID);
  UnlockUserList();
  delete u;
}

// Need to call CICQDaemon::SaveConf() after this
void CUserManager::RemoveOwner(unsigned long _nPPID)
{
  ICQOwner *o = FetchOwner(_nPPID, LOCK_W);
  if (o == NULL) return;
  o->RemoveFiles();
  LockOwnerList(LOCK_W);
  OwnerList::iterator iter = m_vpcOwners.begin();
  while (iter != m_vpcOwners.end() && o != (*iter)) iter++;
  if (iter == m_vpcOwners.end())
    gLog.Warn("%sInternal Error: CUserManager::RemoveOwner():\n"
              "%sOwner not found in vector.\n",
              L_WARNxSTR, L_BLANKxSTR);
  else
    m_vpcOwners.erase(iter);
  UnlockOwnerList();
  DropOwner(_nPPID);
  delete o;
}

ICQUser *CUserManager::FetchUser(const char *_szId, unsigned long _nPPID,
                                 unsigned short _nLockType)
{
  ICQUser *u = NULL;

  if (_szId == 0 || _nPPID == 0) return u;

  // Check for an owner first
  u = FindOwner(_szId, _nPPID);

  if (u == NULL)
    u = m_hUsers.Retrieve(_szId, _nPPID);

  if (u != NULL)
  {
    u->Lock(_nLockType);
    char *szId, *szIdString;
    ICQUser::MakeRealId(_szId, _nPPID, szId);
    ICQUser::MakeRealId(u->IdString(), u->PPID(), szIdString);
    if (strcmp(szId, szIdString))
      gLog.Error("%sInternal error: CUserManager::FetchUser(): Looked for %s, found %s.\n",
                 L_ERRORxSTR, szId, szIdString);
    delete [] szId;
    delete [] szIdString;
  }

  return u;
}

bool CUserManager::IsOnList(const char *_szId, unsigned long _nPPID)
{
  if (FindOwner(_szId, _nPPID)) return true;
  return m_hUsers.Retrieve(_szId, _nPPID) != NULL;
}

// This differs by FetchOwner by requiring an Id.  This isn't used to
// fetch an owner, but for interal use only to see if the given id and ppid
// is an owner.  (It can be used to fetch an owner by calling FetchUser with
// an owner's id and ppid.
ICQOwner *CUserManager::FindOwner(const char *_szId, unsigned long _nPPID)
{
/*
  // Strip spaces if ICQ protocol
  char *szId = new char[strlen(_szId)];
  if (_nPPID == LICQ_PPID)
  {
    for (int i = 0; i < strlen(_szId); i++)
      if (_szId[i] != ' ')
        *szId++ = _szId[i];
  }
  else
    strcpy(szId, _szId);
*/
  ICQOwner *o = NULL;

  LockOwnerList(LOCK_R);
  OwnerList::iterator iter;
  for (iter = m_vpcOwners.begin(); iter != m_vpcOwners.end(); iter++)
  {
    if (_nPPID == (*iter)->PPID() && 
        strcmp(_szId, (*iter)->IdString()) == 0/* || strcmp(szId, (*iter)->IdString()) == 0)*/)
    {
      o = *iter;
      break;
    }
  }
  UnlockOwnerList();

  //delete [] szId;

  return o;
}

/*---------------------------------------------------------------------------
 * CUserManager::AddUser
 *
 * The user is write locked upon return of this function
 *-------------------------------------------------------------------------*/
unsigned long CUserManager::AddUser(ICQUser *pUser)
{
  AddUser(pUser, pUser->IdString(), LICQ_PPID);
  return pUser->Uin();
}


/*---------------------------------------------------------------------------
 * CUserManager::RemoveUser
 *-------------------------------------------------------------------------*/
void CUserManager::RemoveUser(unsigned long _nUin)
{
  ICQUser *u = FetchUser(_nUin, LOCK_R);
  char *szId = u->IdString();
  unsigned long nPPID = u->PPID();
  DropUser(u);
  RemoveUser(szId, nPPID);
}


/*---------------------------------------------------------------------------
 * CUserManager::AddGroup
 *-------------------------------------------------------------------------*/
bool CUserManager::AddGroup(char *_szName, unsigned short nID)
{
  bool bNewGroup = true;

  if(_szName)
  {
    // Check to make sure it isn't a group name already
    LockGroupList(LOCK_W);

    GroupList::iterator iter;
    for (iter = m_vszGroups.begin(); iter != m_vszGroups.end(); iter++)
    {
      if (strcasecmp(*iter, _szName) == 0)
      {
        bNewGroup = false;
        break;
      }
    }

    // Don't allow a duplicate name
    if (!bNewGroup)
    {
      gLog.Warn(tr("%sGroup %s already on list.\n"), L_WARNxSTR, _szName);
    }
    else
    {
      AddGroupID(nID);

      m_vszGroups.push_back(strdup(_szName));
      SaveGroups();
    }

    UnlockGroupList();
  }

  if (bNewGroup && !nID && gLicqDaemon)
    gLicqDaemon->icqAddGroup(_szName);

  return bNewGroup;
}



/*---------------------------------------------------------------------------
 * CUserManager::RemoveGroup
 *
 * Removes a group and it's id (note groups are numbered 1 to NumGroups() )
 *-------------------------------------------------------------------------*/
void CUserManager::RemoveGroup(unsigned short n)
{
  // Don't delete the all users group
  if(n < 1 || n > NumGroups())
  {
    return;
  }

  GroupList *g = LockGroupList(LOCK_R);	

  // Must be called when there are no locks on GroupID and Group lists
  char szName[128];
  strncpy(szName, m_vszGroups[n-1], sizeof(szName));
  szName[sizeof(szName) - 1] = '\0';
  UnlockGroupList();
  gLicqDaemon->icqRemoveGroup(szName);

  // Lock it back up
  g = LockGroupList(LOCK_W);

  // Erase the group from the vector
  m_vszGroups.erase(m_vszGroups.begin()+n-1);

  unsigned short j;
  FOR_EACH_USER_START(LOCK_W)
  {
    for (j = n; j < g->size() + 1; j++)
      pUser->SetInGroup(GROUPS_USER, j, pUser->GetInGroup(GROUPS_USER, j + 1));
  }
  pUser->SetInGroup(GROUPS_USER, j, false);
  FOR_EACH_USER_END;
  if (m_nDefaultGroup >= n) m_nDefaultGroup--;
  if (m_nNewUserGroup >= n) m_nNewUserGroup--;

  RemoveGroupID(n);

  SaveGroups();
  UnlockGroupList();
}


/*---------------------------------------------------------------------------
 * CUserManager::SwapGroups
 *-------------------------------------------------------------------------*/
void CUserManager::SwapGroups(unsigned short g1, unsigned short g2)
{
  LockGroupList(LOCK_W);

  // validate the group numbers
  if (g1 > m_vszGroups.size() || g1 < 1 || g2 > m_vszGroups.size() || g2 < 1)
  {
    UnlockGroupList();
    return;
  }

  // move the actual group
  char *g = m_vszGroups[g1 - 1];
  m_vszGroups[g1 - 1] = m_vszGroups[g2 - 1];
  m_vszGroups[g2 - 1] = g;
  if (m_nDefaultGroup == g1) m_nDefaultGroup = g2;
  else if (m_nDefaultGroup == g2) m_nDefaultGroup = g1;
  if (m_nNewUserGroup == g1) m_nNewUserGroup = g2;
  else if (m_nNewUserGroup == g2) m_nNewUserGroup = g1;
  SaveGroups();
  UnlockGroupList();

  // adjust all the users
  bool bInG1;
  FOR_EACH_USER_START(LOCK_W)
  {
    bInG1 = pUser->GetInGroup(GROUPS_USER, g1);
    pUser->SetInGroup(GROUPS_USER, g1, pUser->GetInGroup(GROUPS_USER, g2));
    pUser->SetInGroup(GROUPS_USER, g2, bInG1);
  }
  FOR_EACH_USER_END

  // swap the group ids
  LockGroupIDList(LOCK_W);
  unsigned short nTmp = m_vnGroupsID[g1 - 1];
  m_vnGroupsID[g1 - 1] = m_vnGroupsID[g2 - 1];
  m_vnGroupsID[g2 - 1] = nTmp;
  SaveGroupIDs();
  UnlockGroupIDList();
}


/*---------------------------------------------------------------------------
 * CUserManager::RenameGroup
 *-------------------------------------------------------------------------*/
void CUserManager::RenameGroup(unsigned short n, const char *_sz, bool _bUpdate)
{
  if (n < 1 || n > NumGroups()) return;
  GroupList *g = LockGroupList(LOCK_W);
  free((*g)[n - 1]);
  (*g)[n - 1] = strdup(_sz);
  SaveGroups();
  UnlockGroupList();

  LockGroupIDList(LOCK_R);
  unsigned short nGSID = m_vnGroupsID[n-1];
  UnlockGroupIDList();

  // If we rename a group on logon, don't send the rename packet
  if (gLicqDaemon && _bUpdate)
    gLicqDaemon->icqRenameGroup(_sz, nGSID);
}


/*---------------------------------------------------------------------------
 * CUserManager::SaveGroups
 *
 * Assumes a lock on the group list
 *-------------------------------------------------------------------------*/
void CUserManager::SaveGroups()
{
  if (!m_bAllowSave) return;

  // Load the group info from licq.conf
  char filename[MAX_FILENAME_LEN];
  snprintf(filename, MAX_FILENAME_LEN, "%s/licq.conf", BASE_DIR);
  filename[MAX_FILENAME_LEN - 1] = '\0';
  CIniFile licqConf(INI_FxERROR | INI_FxFATAL);
  licqConf.LoadFile(filename);

  licqConf.SetSection("groups");
  licqConf.WriteNum("NumOfGroups", NumGroups());

  char sGroupKey[MAX_KEYxNAME_LEN], sGroupIDKey[MAX_KEYxNAME_LEN];
  //LockGroupList(LOCK_R);
  LockGroupIDList(LOCK_R);
  for (unsigned short i = 0; i < m_vszGroups.size(); i++)
  {
     sprintf(sGroupKey, "Group%d.name", i + 1);
     licqConf.WriteStr(sGroupKey, m_vszGroups[i]);

     sprintf(sGroupIDKey, "Group%d.id", i + 1);
     licqConf.WriteNum(sGroupIDKey, m_vnGroupsID[i]);
  }
  UnlockGroupIDList();
  //UnlockGroupList();

  licqConf.WriteNum("DefaultGroup", m_nDefaultGroup);
  licqConf.WriteNum("NewUserGroup", m_nNewUserGroup);
  licqConf.FlushFile();
  licqConf.CloseFile();
}

/*---------------------------------------------------------------------------
 * CUserManager::AddGroupID
 *-------------------------------------------------------------------------*/
void CUserManager::AddGroupID(unsigned short nID)
{
  LockGroupIDList(LOCK_W);
  m_vnGroupsID.push_back(nID);
  UnlockGroupIDList();
}

/*---------------------------------------------------------------------------
 * CUserManager::RemoveGroupID
 *-------------------------------------------------------------------------*/
void CUserManager::RemoveGroupID(unsigned short n)
{
  LockGroupIDList(LOCK_W);
  m_vnGroupsID.erase(m_vnGroupsID.begin()+n-1);
  SaveGroupIDs();
  UnlockGroupIDList();
}

/*---------------------------------------------------------------------------
 * CUserManager::GetIDFromGroup
 *-------------------------------------------------------------------------*/
unsigned short CUserManager::GetIDFromGroup(const char *_szName)
{
  unsigned short nID = 0;
  unsigned short nGroup = 0;

  LockGroupList(LOCK_R);
  LockGroupIDList(LOCK_R);
  for (GroupList::iterator i = m_vszGroups.begin(); i != m_vszGroups.end();
    ++i)
  {
    if (strcmp(_szName, *i) == 0)
    {			
      nID = m_vnGroupsID[nGroup];
      break;
    }
    nGroup++;
  }
  UnlockGroupIDList();
  UnlockGroupList();

  return nID;
}

/*---------------------------------------------------------------------------
 * CUserManager::GetGroupFromID
 *-------------------------------------------------------------------------*/
unsigned short CUserManager::GetGroupFromID(unsigned short nID)
{
  unsigned short nGroup = 0;
  
  LockGroupIDList(LOCK_R);
  for (GroupIDList::iterator i = m_vnGroupsID.begin(); i != m_vnGroupsID.end();
      ++i)
  {
    nGroup++;
    if (*i == nID)  break;
  }
  UnlockGroupIDList();

  return nGroup;
}

/*---------------------------------------------------------------------------
 * CUserManager::ModifyGroupID
 *-------------------------------------------------------------------------*/
void CUserManager::ModifyGroupID(char *szGroup, unsigned short nNewID)
{
  unsigned short nGroup = 0;

  LockGroupList(LOCK_R);
  LockGroupIDList(LOCK_W);

  for (GroupList::iterator i = m_vszGroups.begin(); i != m_vszGroups.end();
      ++i)
  {
    if (strcmp(*i, szGroup) == 0)
    {
      m_vnGroupsID[nGroup] = nNewID;
      break;
    }
    else
      nGroup++;
  }

  UnlockGroupIDList();

  SaveGroups();
  UnlockGroupList();
}

/*---------------------------------------------------------------------------
 * CUserManager::SaveGroupIDs
 *
 * Assumes a lock on the group id list
 *-------------------------------------------------------------------------*/
void CUserManager::SaveGroupIDs()
{
  if (!m_bAllowSave) return;

  // Load the group info from licq.conf
  char filename[MAX_FILENAME_LEN];
  snprintf(filename, MAX_FILENAME_LEN, "%s/licq.conf", BASE_DIR);
  filename[MAX_FILENAME_LEN - 1] = '\0';
  CIniFile licqConf(INI_FxWARN);
  licqConf.LoadFile(filename);

  licqConf.SetSection("groups");

  char sGroupKey[MAX_KEYxNAME_LEN];
  for (unsigned short i = 0; i < m_vnGroupsID.size(); i++)
  {
     sprintf(sGroupKey, "Group%d.id", i + 1);
     licqConf.WriteNum(sGroupKey, m_vnGroupsID[i]);
  }

  licqConf.FlushFile();
  licqConf.CloseFile();
}

/*---------------------------------------------------------------------------
 * CUserManager::GenerateSID
 *
 * Generate a random number.  Make sure no user has that random number.  If
 * a user does have that number, increment and check against all users.  When
 * a number is created that is not owned by a current user, check against all
 * the groups.  If it is owned by a group, increment and start back with checking
 * the users.
 *-------------------------------------------------------------------------*/
unsigned short CUserManager::GenerateSID()
{
  bool bCheckGroup, bDone;
  int nSID;
  unsigned short nOwnerPDINFO;

  ICQOwner *o = gUserManager.FetchOwner(LICQ_PPID, LOCK_R);
  nOwnerPDINFO = o->GetPDINFO();
  gUserManager.DropOwner(LICQ_PPID);

  // Generate a SID
  srand(time(NULL));
  nSID = 1+(int)(65535.0*rand()/(RAND_MAX+1.0));

  nSID &= 0x7FFF; // server limit it looks like

  // Make sure we have a unique number - a map would be better
  do
  {
    bDone = true;
    bCheckGroup = true;

    if (nSID == 0) nSID++;
    if (nSID == nOwnerPDINFO) nSID++;
    FOR_EACH_PROTO_USER_START(LICQ_PPID, LOCK_R)
    {
      if (pUser->GetSID() == nSID  || pUser->GetInvisibleSID() == nSID ||
          pUser->GetVisibleSID() == nSID)
      {
        if (nSID == 0x7FFF)
          nSID = 1;
        else
          nSID++;
        bDone = false;	// Restart
        bCheckGroup = false;	// Don't waste time now
        FOR_EACH_PROTO_USER_BREAK;
      }
    }
    FOR_EACH_PROTO_USER_END

    if (bCheckGroup)
    {
      // Check our groups too!
      GroupIDList *gID = gUserManager.LockGroupIDList(LOCK_R);
      for (unsigned short j = 0; j < gID->size(); j++)
      {
        if ((*gID)[j] == nSID)
        {
          if (nSID == 0x7FFF)
            nSID = 1;
          else
            nSID++;
          bDone = false;
          break;
        }
      }
      gUserManager.UnlockGroupIDList();
    }

  } while (!bDone);

  return nSID;
}

/*---------------------------------------------------------------------------
 * CUserManager::FetchUser
 *-------------------------------------------------------------------------*/
ICQUser *CUserManager::FetchUser(unsigned long _nUin, unsigned short _nLockType)
{
  char szUin[24];
  sprintf(szUin, "%lu", _nUin);
  ICQUser *u = FetchUser(szUin, LICQ_PPID, _nLockType);
  return u;
}

/*---------------------------------------------------------------------------
 * CUserManager::IsOnList
 *-------------------------------------------------------------------------*/
bool CUserManager::IsOnList(unsigned long nUin)
{
  char szUin[24];
  sprintf(szUin, "%lu", nUin);
  return IsOnList(szUin, LICQ_PPID);
}



/*---------------------------------------------------------------------------
 * CUserManager::DropUser
 *-------------------------------------------------------------------------*/
void CUserManager::DropUser(ICQUser *u)
{
  if (u == NULL) return;
  u->Unlock();
}

/*---------------------------------------------------------------------------
 * CUserManager::FetchOwner
 *-------------------------------------------------------------------------*/
ICQOwner *CUserManager::FetchOwner(unsigned short _nLockType)
{
  return FetchOwner(LICQ_PPID, _nLockType);
}

ICQOwner *CUserManager::FetchOwner(unsigned long _nPPID,
                                   unsigned short _nLockType)
{
  ICQOwner *o = NULL;

  LockOwnerList(LOCK_R);
  OwnerList::iterator iter;
  for (iter = m_vpcOwners.begin(); iter != m_vpcOwners.end(); iter++)
  {
    if ((*iter)->PPID() == _nPPID)
    {
      o = (*iter);
      o->Lock(_nLockType);
      break;
    }
  }
  UnlockOwnerList();

  return o;
}

/*---------------------------------------------------------------------------
 * CUserManager::DropOwner
 *-------------------------------------------------------------------------*/
void CUserManager::DropOwner()
{
  DropOwner(LICQ_PPID);
}

void CUserManager::DropOwner(unsigned long _nPPID)
{
  LockOwnerList(LOCK_R);
  OwnerList::iterator iter;
  for (iter = m_vpcOwners.begin(); iter != m_vpcOwners.end(); iter++)
  {
    if ((*iter)->PPID() == _nPPID)
    {
      (*iter)->Unlock();
      break;
    }
  }
  UnlockOwnerList();
}

/*---------------------------------------------------------------------------
 * CUserManager::SaveAllUsers
 *-------------------------------------------------------------------------*/
void CUserManager::SaveAllUsers()
{
  FOR_EACH_USER_START(LOCK_R)
  {
    if (!pUser->NotInList())
    {
      pUser->SaveLicqInfo();
      pUser->SaveGeneralInfo();
      pUser->SaveMoreInfo();
      pUser->SaveHomepageInfo();
      pUser->SaveWorkInfo();
      pUser->SaveAboutInfo();
      pUser->SaveInterestsInfo();
      pUser->SaveBackgroundsInfo();
      pUser->SaveOrganizationsInfo();
      pUser->SavePhoneBookInfo();
      pUser->SavePictureInfo();
      pUser->SaveExtInfo();
    }
  }
  FOR_EACH_USER_END
}

/*---------------------------------------------------------------------------
 * CUserManager::UpdateUsersInGroups
 *-------------------------------------------------------------------------*/
bool CUserManager::UpdateUsersInGroups()
{
  bool bDid = false;
  
  FOR_EACH_PROTO_USER_START(LICQ_PPID, LOCK_W)
  {
    unsigned short nGSID = pUser->GetGSID();
    if (nGSID)
    {
      unsigned short nInGroup = gUserManager.GetGroupFromID(nGSID);
      if (nInGroup != gUserManager.NumGroups())
      {
        pUser->AddToGroup(GROUPS_USER, nInGroup);
        bDid = true;
      }
    }
  }
  FOR_EACH_PROTO_USER_END
  
  return bDid;
}

/*---------------------------------------------------------------------------
 * CUserManager::NumUsers
 *-------------------------------------------------------------------------*/
unsigned short CUserManager::NumUsers()
{
  //LockUserList(LOCK_R);
  unsigned short n = m_vpcUsers.size();
  //UnlockUserList();
  return n;
}


/*---------------------------------------------------------------------------
 * CUserManager::NumOwners
 *-------------------------------------------------------------------------*/
unsigned short CUserManager::NumOwners()
{
  unsigned short n = m_vpcOwners.size();
  return n;
}

/*---------------------------------------------------------------------------
 * CUserManager::NumGroups
 *-------------------------------------------------------------------------*/
unsigned short CUserManager::NumGroups()
{
  //LockGroupList(LOCK_R);
  unsigned short n = m_vszGroups.size();
  //UnlockGroupList();
  return n;
}


/*---------------------------------------------------------------------------
 * LockUserList
 *
 * Locks the entire user list for iterating through...
 *-------------------------------------------------------------------------*/
UserList *CUserManager::LockUserList(unsigned short _nLockType)
{
  switch (_nLockType)
  {
  case LOCK_R:
    pthread_rdwr_rlock_np (&mutex_userlist);
    break;
  case LOCK_W:
    pthread_rdwr_wlock_np(&mutex_userlist);
    break;
  default:
    break;
  }
  m_nUserListLockType = _nLockType;
  return &m_vpcUsers;
}

/*---------------------------------------------------------------------------
 * CUserManager::UnlockUserList
 *-------------------------------------------------------------------------*/
void CUserManager::UnlockUserList()
{
  unsigned short nLockType = m_nUserListLockType;
  m_nUserListLockType = LOCK_R;
  switch (nLockType)
  {
  case LOCK_R:
    pthread_rdwr_runlock_np(&mutex_userlist);
    break;
  case LOCK_W:
    pthread_rdwr_wunlock_np(&mutex_userlist);
    break;
  default:
    break;
  }
}

/*---------------------------------------------------------------------------
 * LockGroupList
 *
 * Locks the entire group list for iterating through...
 *-------------------------------------------------------------------------*/
GroupList *CUserManager::LockGroupList(unsigned short _nLockType)
{
  switch (_nLockType)
  {
  case LOCK_R:
    pthread_rdwr_rlock_np (&mutex_grouplist);
    break;
  case LOCK_W:
    pthread_rdwr_wlock_np(&mutex_grouplist);
    break;
  default:
    break;
  }
  m_nGroupListLockType = _nLockType;
  return &m_vszGroups;
}



/*---------------------------------------------------------------------------
 * CUserManager::UnlockGroupList
 *-------------------------------------------------------------------------*/
void CUserManager::UnlockGroupList()
{
  unsigned short nLockType = m_nGroupListLockType;
  m_nGroupListLockType = LOCK_R;
  switch (nLockType)
  {
  case LOCK_R:
    pthread_rdwr_runlock_np(&mutex_grouplist);
    break;
  case LOCK_W:
    pthread_rdwr_wunlock_np(&mutex_grouplist);
    break;
  default:
    break;
  }
}


/*---------------------------------------------------------------------------
 * LockGroupIDList
 *
 * Locks the entire group id list for iterating through...
 *-------------------------------------------------------------------------*/
GroupIDList *CUserManager::LockGroupIDList(unsigned short _nLockType)
{
  switch (_nLockType)
  {
  case LOCK_R:
    pthread_rdwr_rlock_np (&mutex_groupidlist);
    break;
  case LOCK_W:
    pthread_rdwr_wlock_np(&mutex_groupidlist);
    break;
  default:
    break;
  }
  m_nGroupIDListLockType = _nLockType;
  return &m_vnGroupsID;
}



/*---------------------------------------------------------------------------
 * CUserManager::UnlockGroupIDList
 *-------------------------------------------------------------------------*/
void CUserManager::UnlockGroupIDList()
{
  unsigned short nLockType = m_nGroupIDListLockType;
  m_nGroupIDListLockType = LOCK_R;
  switch (nLockType)
  {
  case LOCK_R:
    pthread_rdwr_runlock_np(&mutex_groupidlist);
    break;
  case LOCK_W:
    pthread_rdwr_wunlock_np(&mutex_groupidlist);
    break;
  default:
    break;
  }
}

OwnerList *CUserManager::LockOwnerList(unsigned short _nLockType)
{
  switch (_nLockType)
  {
  case LOCK_R:
    pthread_rdwr_rlock_np(&mutex_ownerlist);
    break;
  case LOCK_W:
    pthread_rdwr_wlock_np(&mutex_ownerlist);
    break;
  default:
    break;
  }
  m_nOwnerListLockType = _nLockType;
  return &m_vpcOwners;
}

void CUserManager::UnlockOwnerList()
{
  unsigned short nLockType = m_nOwnerListLockType;
  m_nOwnerListLockType = LOCK_R;
  switch (nLockType)
  {
  case LOCK_R:
    pthread_rdwr_runlock_np(&mutex_ownerlist);
    break;
  case LOCK_W:
    pthread_rdwr_wunlock_np(&mutex_ownerlist);
    break;
  default:
    break;
  }
}

/*---------------------------------------------------------------------------
 * CUserManager::AddUserToGroup
 *-------------------------------------------------------------------------*/
void CUserManager::AddUserToGroup(const char *szId, unsigned long nPPID,
  unsigned short _nGroup)
{
  //TODO: For other protocols
  ICQUser *u = FetchUser(szId, nPPID, LOCK_W);
  if (u == NULL) return;
  u->AddToGroup(GROUPS_USER, _nGroup);
  int nGSID = u->GetGSID();
  DropUser(u);
  if (gLicqDaemon && nPPID == LICQ_PPID)
    gLicqDaemon->icqChangeGroup(szId, nPPID, _nGroup,
      nGSID, ICQ_ROSTxNORMAL, ICQ_ROSTxNORMAL);
}

void CUserManager::AddUserToGroup(unsigned long _nUin, unsigned short _nGroup)
{
  ICQUser *u = FetchUser(_nUin, LOCK_W);
  if (u == NULL) return;
  u->AddToGroup(GROUPS_USER, _nGroup);
  int nGSID = u->GetGSID();
  DropUser(u);
  if (gLicqDaemon)
    gLicqDaemon->icqChangeGroup(_nUin, _nGroup, nGSID, ICQ_ROSTxNORMAL,
      ICQ_ROSTxNORMAL);
}


/*---------------------------------------------------------------------------
 * CUserManager::RemoveUserFromGroup
 *-------------------------------------------------------------------------*/
void CUserManager::RemoveUserFromGroup(const char *szId, unsigned long nPPID,
  unsigned short _nGroup)
{
  ICQUser *u = FetchUser(szId, nPPID, LOCK_W);
  if (u == NULL) return;
  u->RemoveFromGroup(GROUPS_USER, _nGroup);
  DropUser(u);
}


void CUserManager::RemoveUserFromGroup(unsigned long _nUin, unsigned short _nGroup)
{
  ICQUser *u = FetchUser(_nUin, LOCK_W);
  if (u == NULL) return;
  u->RemoveFromGroup(GROUPS_USER, _nGroup);
  DropUser(u);
}


//=====CUserHashTable===========================================================
CUserHashTable::CUserHashTable(unsigned short _nSize) : m_vlTable(_nSize)
{
  pthread_rdwr_init_np(&mutex_rw, NULL);
  m_nLockType = LOCK_R;
}

ICQUser *CUserHashTable::Retrieve(const char *_szId, unsigned long _nPPID)
{
  Lock(LOCK_R);

  ICQUser *u = NULL;
  char *szRealId;
  ICQUser::MakeRealId(_szId, _nPPID, szRealId);
  UserList &l = m_vlTable[HashValue(szRealId)];
  char *szTempId = 0;
  unsigned long nPPID;
  UserList::iterator iter;
  for (iter = l.begin(); iter != l.end(); iter++)
  {
    nPPID = (*iter)->PPID();
    ICQUser::MakeRealId((*iter)->IdString(), nPPID, szTempId);
    if (_nPPID == nPPID && strcasecmp(szTempId, szRealId) == 0)
    {
      u = (*iter);
      break;
    }
    delete [] szTempId; szTempId = 0;
  }
  if (iter == l.end()) u = NULL;

  delete [] szRealId;
  if (szTempId) delete [] szTempId;

  Unlock();
  return u;
}

void CUserHashTable::Store(ICQUser *u, const char *_szId, unsigned long _nPPID)
{
  Lock(LOCK_W);
  char *szRealId;
  ICQUser::MakeRealId(_szId, _nPPID, szRealId);
  UserList &l = m_vlTable[HashValue(szRealId)];
  delete [] szRealId;
  l.push_front(u);
  Unlock();
}

void CUserHashTable::Remove(const char *_szId, unsigned long _nPPID)
{
  Lock(LOCK_W);

  char *szRealId;
  ICQUser::MakeRealId(_szId, _nPPID, szRealId);
  UserList &l = m_vlTable[HashValue(szRealId)];
  delete [] szRealId;
  char *szId;
  unsigned long nPPID;
  UserList::iterator iter;
  for (iter = l.begin(); iter != l.end(); iter++)
  {
    (*iter)->Lock(LOCK_R);
    szId = (*iter)->IdString();
    nPPID = (*iter)->PPID();
    (*iter)->Unlock();
    if (_nPPID == nPPID && strcmp(szId, _szId) == 0)
    {
      l.erase(iter);
      break;
    }
  }

  Unlock();
}

unsigned short CUserHashTable::HashValue(const char *_szId)
{
  int j = strlen(_szId);
  unsigned short nRet = 0;
  for (int i = 0; i < j; i++)
    nRet += (unsigned short)_szId[i];

  return (nRet % (USER_HASH_SIZE - 1));
}

ICQUser *CUserHashTable::Retrieve(unsigned long _nUin)
{
  char szUin[24];
  sprintf(szUin, "%lu", _nUin);
  return Retrieve(szUin, LICQ_PPID);
}

void CUserHashTable::Store(ICQUser *u, unsigned long _nUin)
{
  char szUin[24];
  sprintf(szUin, "%lu", _nUin);
  Store(u, szUin, LICQ_PPID);
}

void CUserHashTable::Remove(unsigned long _nUin)
{
  char szUin[24];
  sprintf(szUin, "%lu", _nUin);
  Remove(szUin, LICQ_PPID);
}

unsigned short CUserHashTable::HashValue(unsigned long _nUin)
{
  char szUin[24];
  sprintf(szUin, "%lu", _nUin);
  return HashValue(szUin);
}

void CUserHashTable::Lock(unsigned short _nLockType)
{
  switch (_nLockType)
  {
  case LOCK_R:
    pthread_rdwr_rlock_np (&mutex_rw);
    break;
  case LOCK_W:
    pthread_rdwr_wlock_np(&mutex_rw);
    break;
  default:
    break;
  }
  m_nLockType = _nLockType;
}

void CUserHashTable::Unlock()
{
  unsigned short nLockType = m_nLockType;
  m_nLockType = LOCK_R;
  switch (nLockType)
  {
  case LOCK_R:
    pthread_rdwr_runlock_np(&mutex_rw);
    break;
  case LOCK_W:
    pthread_rdwr_wunlock_np(&mutex_rw);
    break;
  default:
    break;
  }
}




















//=====CUser====================================================================

unsigned short ICQUser::s_nNumUserEvents = 0;
pthread_mutex_t ICQUser::mutex_nNumUserEvents = PTHREAD_MUTEX_INITIALIZER;


//-----ICQUser::constructor-----------------------------------------------------
ICQUser::ICQUser(unsigned long _nUin, char *_szFilename)
// Called when first constructing our known users
{
  char szUin[24];
  sprintf(szUin, "%lu", _nUin);
  Init(szUin, LICQ_PPID);
  m_fConf.SetFlags(INI_FxWARN);
  m_fConf.SetFileName(_szFilename);
  if (!LoadInfo())
  {
    gLog.Error("%sUnable to load user info from '%s'.\n%sUsing default values.\n",
               L_ERRORxSTR, _szFilename, L_BLANKxSTR);
    SetDefaults();
  }
  m_fConf.CloseFile();
  m_fConf.SetFlags(INI_FxWARN | INI_FxALLOWxCREATE);
}


ICQUser::ICQUser(unsigned long nUin)
{
  char szUin[24];
  sprintf(szUin, "%lu", nUin);
  Init(szUin, LICQ_PPID);

  SetDefaults();
  char szFilename[MAX_FILENAME_LEN];
  char *p = PPIDSTRING(LICQ_PPID);
  snprintf(szFilename, MAX_FILENAME_LEN, "%s/%s/%s.%s", BASE_DIR, USER_DIR,
           szUin, p);
  delete [] p;

  szFilename[MAX_FILENAME_LEN - 1] = '\0';
  m_fConf.SetFileName(szFilename);
  m_fConf.SetFlags(INI_FxWARN | INI_FxALLOWxCREATE);
}

ICQUser::ICQUser(const char *_szId, unsigned long _nPPID, char *_szFilename)
{
  Init(_szId, _nPPID);
  m_fConf.SetFlags(INI_FxWARN);
  m_fConf.SetFileName(_szFilename);
  if (!LoadInfo())
  {
    gLog.Error("%sUnable to load user info from '%s'.\n%sUsing default values.\n",
               L_ERRORxSTR, _szFilename, L_BLANKxSTR);
    SetDefaults();
  }
  m_fConf.CloseFile();
  m_fConf.SetFlags(INI_FxWARN | INI_FxALLOWxCREATE);
}

ICQUser::ICQUser(const char *_szId, unsigned long _nPPID, bool _bTempUser)
{
  Init(_szId, _nPPID);
  SetDefaults();
  m_bNotInList = _bTempUser;
  if (!m_bNotInList)
  {
    char szFilename[MAX_FILENAME_LEN];
    char *p = PPIDSTRING(_nPPID);
    snprintf(szFilename, MAX_FILENAME_LEN, "%s/%s/%s.%s", BASE_DIR, USER_DIR,
           _szId, p);
    delete [] p;
    szFilename[MAX_FILENAME_LEN - 1] = '\0';
    m_fConf.SetFileName(szFilename);
    m_fConf.SetFlags(INI_FxWARN | INI_FxALLOWxCREATE);
  }
}

void ICQUser::AddToContactList()
{
  m_bOnContactList = m_bEnableSave = true;

  // Check for old history file
  if (access(m_fHistory.FileName(), F_OK) == -1)
  {
    char szFilename[MAX_FILENAME_LEN];
    char *p = PPIDSTRING(m_nPPID);
    snprintf(szFilename, MAX_FILENAME_LEN, "%s/%s/%s.%s.%s", BASE_DIR, HISTORY_DIR, m_szId,
             p, HISTORYxOLD_EXT);
    delete [] p;

    szFilename[MAX_FILENAME_LEN - 1] = '\0';
    if (access(szFilename, F_OK) == 0)
    {
      if (rename(szFilename, m_fHistory.FileName()) == -1)
      {
        gLog.Warn(tr("%sFailed to rename old history file (%s):\n%s%s\n"), L_WARNxSTR,
            szFilename, L_BLANKxSTR, strerror(errno));
      }
    }
  }
}


//-----ICQUser::LoadInfo-----------------------------------------------------
bool ICQUser::LoadInfo()
{
  if (!m_fConf.ReloadFile()) return (false);
  m_fConf.SetFlags(0);
  m_fConf.SetSection("user");

  LoadGeneralInfo();
  LoadMoreInfo();
  LoadHomepageInfo();
  LoadWorkInfo();
  LoadAboutInfo();
  LoadInterestsInfo();
  LoadBackgroundsInfo();
  LoadOrganizationsInfo();
  LoadPhoneBookInfo();
  LoadPictureInfo();
  LoadLicqInfo();

  return true;
}


//-----ICQUser::LoadGeneralInfo----------------------------------------------
void ICQUser::LoadGeneralInfo()
{
  // read in the fields, checking for errors each time
  char szTemp[MAX_LINE_LEN];
  m_fConf.ReadStr("Alias", szTemp, tr("Unknown"));  SetAlias(szTemp);
  m_fConf.ReadStr("FirstName", szTemp, "");  SetFirstName(szTemp);
  m_fConf.ReadStr("LastName", szTemp, "");  SetLastName(szTemp);
  m_fConf.ReadStr("Email1", szTemp, "");  SetEmailPrimary(szTemp);
  m_fConf.ReadStr("Email2", szTemp, "");  SetEmailSecondary(szTemp);
  m_fConf.ReadStr("EmailO", szTemp, "");  SetEmailOld(szTemp);
  m_fConf.ReadStr("City", szTemp, "");  SetCity(szTemp);
  m_fConf.ReadStr("State", szTemp, "");  SetState(szTemp);
  m_fConf.ReadStr("PhoneNumber", szTemp, "");  SetPhoneNumber(szTemp);
  m_fConf.ReadStr("FaxNumber", szTemp, "");  SetFaxNumber(szTemp);
  m_fConf.ReadStr("Address", szTemp, "");  SetAddress(szTemp);
  m_fConf.ReadStr("CellularNumber", szTemp, "");  SetCellularNumber(szTemp);
  m_fConf.ReadStr("Zipcode", szTemp, "");  SetZipCode(szTemp);
  m_fConf.ReadNum("Country", m_nCountryCode, 0);
  m_fConf.ReadNum("Timezone", m_nTimezone, TIMEZONE_UNKNOWN);
  m_fConf.ReadBool("Authorization", m_bAuthorization, false);
  m_fConf.ReadBool("HideEmail", m_bHideEmail, false);
}


//-----ICQUser::LoadMoreInfo-------------------------------------------------
void ICQUser::LoadMoreInfo()
{
  // read in the fields, checking for errors each time
  char szTemp[MAX_LINE_LEN];
  m_fConf.ReadNum("Age", m_nAge, 0);
  m_fConf.ReadNum("Gender", m_nGender, GENDER_UNSPECIFIED);
  m_fConf.ReadStr("Homepage", szTemp, tr("<none>"));  SetHomepage(szTemp);
  m_fConf.ReadNum("BirthYear", m_nBirthYear, 0);
  m_fConf.ReadNum("BirthMonth", m_nBirthMonth, 0);
  m_fConf.ReadNum("BirthDay", m_nBirthDay, 0);
  m_fConf.ReadNum("Language1", m_nLanguage[0], 0);
  m_fConf.ReadNum("Language2", m_nLanguage[1], 0);
  m_fConf.ReadNum("Language3", m_nLanguage[2], 0);
}


//-----ICQUser::LoadHomepageInfo---------------------------------------------
void ICQUser::LoadHomepageInfo()
{
  // read in the fields, checking for errors each time
  char szTemp[MAX_LINE_LEN];
  m_fConf.ReadBool("HomepageCatPresent", m_bHomepageCatPresent, false);
  m_fConf.ReadNum("HomepageCatCode", m_nHomepageCatCode, 0);
  m_fConf.ReadStr("HomepageDesc", szTemp, "");  SetHomepageDesc(szTemp);
  m_fConf.ReadBool("ICQHomepagePresent", m_bICQHomepagePresent, false);
}


//-----ICQUser::LoadWorkInfo-------------------------------------------------
void ICQUser::LoadWorkInfo()
{
  // read in the fields, checking for errors each time
  char szTemp[MAX_LINE_LEN];
  m_fConf.ReadStr("CompanyCity", szTemp, "");  SetCompanyCity(szTemp);
  m_fConf.ReadStr("CompanyState", szTemp, "");  SetCompanyState(szTemp);
  m_fConf.ReadStr("CompanyPhoneNumber", szTemp, "");  SetCompanyPhoneNumber(szTemp);
  m_fConf.ReadStr("CompanyFaxNumber", szTemp, "");  SetCompanyFaxNumber(szTemp);
  m_fConf.ReadStr("CompanyAddress", szTemp, "");  SetCompanyAddress(szTemp);
  m_fConf.ReadStr("CompanyZip", szTemp, "");  SetCompanyZip(szTemp);
  m_fConf.ReadNum("CompanyCountry", m_nCompanyCountry, 0);
  m_fConf.ReadStr("CompanyName", szTemp, "");  SetCompanyName(szTemp);
  m_fConf.ReadStr("CompanyDepartment", szTemp, "");  SetCompanyDepartment(szTemp);
  m_fConf.ReadStr("CompanyPosition", szTemp, "");  SetCompanyPosition(szTemp);
  m_fConf.ReadNum("CompanyOccupation", m_nCompanyOccupation, 0);
  m_fConf.ReadStr("CompanyHomepage", szTemp, "");  SetCompanyHomepage(szTemp);
}


//-----ICQUser::LoadAboutInfo-------------------------------------------------
void ICQUser::LoadAboutInfo()
{
  // read in the fields, checking for errors each time
  char szTemp[MAX_LINE_LEN];
  m_fConf.SetSection("user");
  m_fConf.ReadStr("About", szTemp, ""); SetAbout(szTemp);
}

//-----ICQUser::LoadPhoneBookInfo--------------------------------------------
void ICQUser::LoadPhoneBookInfo()
{
  m_PhoneBook->LoadFromDisk(m_fConf);
}

//-----ICQUser::LoadPictureInfo----------------------------------------------
void ICQUser::LoadPictureInfo()
{
  m_fConf.SetSection("user");
  m_fConf.ReadBool("PicturePresent", m_bPicturePresent, false);
}

//-----ICQUser::LoadLicqInfo-------------------------------------------------
void ICQUser::LoadLicqInfo()
{
  // read in the fields, checking for errors each time
  char szTemp[MAX_LINE_LEN];
  unsigned short nNewMessages;
  unsigned long nLast;
  m_fConf.SetSection("user");
  m_fConf.ReadNum("Groups.System", m_nGroups[GROUPS_SYSTEM], 0);
  m_fConf.ReadNum("Groups.User", m_nGroups[GROUPS_USER], 0);
  m_fConf.ReadStr("Ip", szTemp, "0.0.0.0");
  struct in_addr in;
  m_nIp = inet_aton(szTemp, &in);
  if (m_nIp != 0) m_nIp = in.s_addr;
  m_fConf.ReadStr("IntIp", szTemp, "0.0.0.0");
  m_nIntIp = inet_aton(szTemp, &in);
  if (m_nIntIp != 0) m_nIntIp = in.s_addr;
  m_fConf.ReadNum("Port", m_nPort, 0);
  //m_fConf.ReadBool("NewUser", m_bNewUser, false);
  m_fConf.ReadNum("NewMessages", nNewMessages, 0);
  m_fConf.ReadNum("LastOnline", nLast, 0);
  m_nLastCounters[LAST_ONLINE] = nLast;
  m_fConf.ReadNum("LastSent", nLast, 0);
  m_nLastCounters[LAST_SENT_EVENT] = nLast;
  m_fConf.ReadNum("LastRecv", nLast, 0);
  m_nLastCounters[LAST_RECV_EVENT] = nLast;
  m_fConf.ReadNum("LastCheckedAR", nLast, 0);
  m_nLastCounters[LAST_CHECKED_AR] = nLast;
  m_fConf.ReadNum("AutoAccept", m_nAutoAccept, 0);
  m_fConf.ReadNum("StatusToUser", m_nStatusToUser, ICQ_STATUS_OFFLINE);
  if (User()) // Only allow to keep a modified alias for user uins
    m_fConf.ReadBool("KeepAliasOnUpdate", m_bKeepAliasOnUpdate, false);
  else
    m_bKeepAliasOnUpdate = false;
  m_fConf.ReadStr("CustomAutoRsp", szTemp, "");
  m_fConf.ReadBool("SendIntIp", m_bSendIntIp, false);
  SetCustomAutoResponse(szTemp);
  m_fConf.ReadStr( "UserEncoding", szTemp, "" );
  SetString( &m_szEncoding, szTemp );
  m_fConf.ReadStr("History", szTemp, "default");
  if (szTemp[0] == '\0') strcpy(szTemp, "default");
  SetHistoryFile(szTemp);
  m_fConf.ReadBool("AwaitingAuth", m_bAwaitingAuth, false);
  m_fConf.ReadNum("SID", m_nSID[NORMAL_SID], 0);
  m_fConf.ReadNum("InvisibleSID", m_nSID[INV_SID], 0);
  m_fConf.ReadNum("VisibleSID", m_nSID[VIS_SID], 0);
  m_fConf.ReadNum("GSID", m_nGSID, 0);
  m_fConf.ReadNum("ClientTimestamp", m_nClientTimestamp, 0);
  m_fConf.ReadNum("ClientInfoTimestamp", m_nClientInfoTimestamp, 0);
  m_fConf.ReadNum("ClientStatusTimestamp", m_nClientStatusTimestamp, 0);
  m_fConf.ReadNum("OurClientTimestamp", m_nOurClientTimestamp, 0);
  m_fConf.ReadNum("OurClientInfoTimestamp", m_nOurClientInfoTimestamp, 0);
  m_fConf.ReadNum("OurClientStatusTimestamp", m_nOurClientStatusTimestamp, 0);
  m_fConf.ReadNum("PhoneFollowMeStatus", m_nPhoneFollowMeStatus,
                  ICQ_PLUGIN_STATUSxINACTIVE);
  m_fConf.ReadNum("ICQphoneStatus", m_nICQphoneStatus,
                  ICQ_PLUGIN_STATUSxINACTIVE);
  m_fConf.ReadNum("SharedFilesStatus", m_nSharedFilesStatus,
                  ICQ_PLUGIN_STATUSxINACTIVE);
  m_fConf.ReadBool("UseGPG", m_bUseGPG, false );
  m_fConf.ReadStr("GPGKey", szTemp, "" );
  SetString( &m_szGPGKey, szTemp );
  m_bSupportsUTF8 = false;
  
  if (nNewMessages > 0)
  {
    HistoryList hist;
    if (GetHistory(hist))
    {
      HistoryListIter it;
      if (hist.size() < nNewMessages)
        it = hist.begin();
      else
      {
        it = hist.end();
        while (nNewMessages > 0 && it != hist.begin())
        {
          it--;
          nNewMessages--;
        }
      }
      while (it != hist.end())
      {
        m_vcMessages.push_back( (*it)->Copy() );
        incNumUserEvents();
        it++;
      }
    }
    ClearHistory(hist);
  }
}


//-----ICQUser::destructor------------------------------------------------------
ICQUser::~ICQUser()
{
  unsigned long nId;
  while (m_vcMessages.size() > 0)
  {
    nId = m_vcMessages[m_vcMessages.size() - 1]->Id();
    delete m_vcMessages[m_vcMessages.size() - 1];
    m_vcMessages.pop_back();
    decNumUserEvents();
    
    if (gLicqDaemon != NULL)
      gLicqDaemon->PushPluginSignal(new CICQSignal(SIGNAL_UPDATExUSER,
        USER_EVENTS, m_szId, m_nPPID, nId));
  }

  if ( m_szAutoResponse )
      free( m_szAutoResponse );
  if ( m_szEncoding )
      free( m_szEncoding );
  if ( m_szAlias )
      free( m_szAlias );
  if ( m_szFirstName )
      free( m_szFirstName );
  if ( m_szLastName )
      free( m_szLastName );
  if ( m_szEmailPrimary )
      free( m_szEmailPrimary );
  if ( m_szEmailSecondary )
      free( m_szEmailSecondary );
  if ( m_szEmailOld )
      free( m_szEmailOld );
  if ( m_szCity )
      free( m_szCity );
  if ( m_szState )
      free( m_szState );
  if ( m_szPhoneNumber )
      free( m_szPhoneNumber );
  if ( m_szFaxNumber )
      free( m_szFaxNumber );
  if ( m_szAddress )
      free( m_szAddress );
  if ( m_szCellularNumber )
      free( m_szCellularNumber );
  if ( m_szZipCode )
      free( m_szZipCode );
  if ( m_szHomepage )
      free( m_szHomepage );
  if ( m_szHomepageDesc )
      free( m_szHomepageDesc );
  if ( m_szCompanyCity )
      free( m_szCompanyCity );
  if ( m_szCompanyState )
      free( m_szCompanyState );
  if ( m_szCompanyPhoneNumber )
      free( m_szCompanyPhoneNumber );
  if ( m_szCompanyFaxNumber )
      free( m_szCompanyFaxNumber );
  if ( m_szCompanyAddress )
      free( m_szCompanyAddress );
  if ( m_szCompanyZip )
      free( m_szCompanyZip );
  if ( m_szCompanyName )
      free( m_szCompanyName );
  if ( m_szCompanyDepartment )
      free( m_szCompanyDepartment );
  if ( m_szCompanyPosition )
      free( m_szCompanyPosition );
  if ( m_szCompanyHomepage )
      free( m_szCompanyHomepage );
  if ( m_szAbout )
      free( m_szAbout );
  if ( m_szCustomAutoResponse )
      free( m_szCustomAutoResponse );
  if ( m_szClientInfo )
      free( m_szClientInfo );
  if ( m_szId )
      free( m_szId );
  if ( m_szGPGKey )
      free( m_szGPGKey );
  
  delete m_Interests;
  delete m_Organizations;
  delete m_Backgrounds;
  delete m_PhoneBook;
/*
  // Destroy the mutex
  int nResult = 0;
  do
  {
    pthread_mutex_lock(&mutex);
    pthread_mutex_unlock(&mutex);
    nResult = pthread_mutex_destroy(&mutex);
  } while (nResult != 0);
*/
}


//-----ICQUser::RemoveFiles-----------------------------------------------------
void ICQUser::RemoveFiles()
{
  remove(m_fConf.FileName());

  // Check for old history file and back up
  struct stat buf;
  if (stat(m_fHistory.FileName(), &buf) == 0 && buf.st_size > 0)
  {
    char szFilename[MAX_FILENAME_LEN];
    char *p = PPIDSTRING(m_nPPID);
    snprintf(szFilename, MAX_FILENAME_LEN, "%s/%s/%s.%s.%s", BASE_DIR, HISTORY_DIR,
             m_szId, p, HISTORYxOLD_EXT);
    delete [] p;

    szFilename[MAX_FILENAME_LEN - 1] = '\0';
    if (rename(m_fHistory.FileName(), szFilename) == -1)
    {
      gLog.Warn(tr("%sFailed to rename history file (%s):\n%s%s\n"), L_WARNxSTR,
          szFilename, L_BLANKxSTR, strerror(errno));
      remove(m_fHistory.FileName());
    }
  }
}


void ICQUser::Init(unsigned long _nUin)
{
  char szUin[24];
  sprintf(szUin, "%lu", _nUin);
  Init(szUin, LICQ_PPID);
}

void ICQUser::Init(const char *_szId, unsigned long _nPPID)
{
  //SetOnContactList(false);
  m_bOnContactList = m_bEnableSave = false;
  m_szAutoResponse = NULL;
  m_szEncoding = strdup("");
  m_bSecure = false;

  // General Info
  m_szAlias = NULL;
  m_szFirstName = NULL;
  m_szLastName = NULL;
  m_szEmailPrimary = NULL;
  m_szEmailSecondary = NULL;
  m_szEmailOld = NULL;
  m_szCity = NULL;
  m_szState = NULL;
  m_szPhoneNumber = NULL;
  m_szFaxNumber = NULL;
  m_szAddress = NULL;
  m_szCellularNumber = NULL;
  m_szZipCode = NULL;
  m_nCountryCode = COUNTRY_UNSPECIFIED;
  m_nTimezone = TIMEZONE_UNKNOWN;
  m_bAuthorization = false;
  m_nWebAwareStatus = 2; //Status unknown
  m_bHideEmail = false;
  m_nTyping = ICQ_TYPING_INACTIVEx0;
  m_bNotInList = false;
  
  // More Info
  m_nAge = 0xffff;
  m_nGender = 0;
  m_szHomepage = NULL;
  m_nBirthYear = 0;
  m_nBirthMonth = 0;
  m_nBirthDay = 0;
  m_nLanguage[0] = 0;
  m_nLanguage[1] = 0;
  m_nLanguage[2] = 0;

  // Homepage Info
  m_bHomepageCatPresent = false;
  m_nHomepageCatCode = 0;
  m_szHomepageDesc = NULL;
  m_bICQHomepagePresent = false;

  // More2
  m_Interests    = new ICQUserCategory(CAT_INTERESTS);
  m_Organizations = new ICQUserCategory(CAT_ORGANIZATION);
  m_Backgrounds  = new ICQUserCategory(CAT_BACKGROUND);

  // Work Info
  m_szCompanyCity = NULL;
  m_szCompanyState = NULL;
  m_szCompanyPhoneNumber = NULL;
  m_szCompanyFaxNumber = NULL;
  m_szCompanyAddress = NULL;
  m_szCompanyZip = NULL;
  m_nCompanyCountry = COUNTRY_UNSPECIFIED;
  m_szCompanyName = NULL;
  m_szCompanyDepartment = NULL;
  m_szCompanyPosition = NULL;
  m_nCompanyOccupation = OCCUPATION_UNSPECIFIED;
  m_szCompanyHomepage = NULL;

  // About
  m_szAbout = NULL;

  // Phone Book
  m_PhoneBook = new ICQUserPhoneBook();

  // Picture
  m_bPicturePresent = false;

  // GPG key
  m_szGPGKey = strdup("");

  if (_szId)
    m_szId = strdup(_szId);
  else
    m_szId = 0;
  m_nPPID = _nPPID;
  
  // gui plugin compat
  if (m_nPPID == LICQ_PPID && m_szId)
    m_nUin = strtoul(m_szId, (char **)NULL, 10);
  else
   m_nUin = 0;
  SetStatus(ICQ_STATUS_OFFLINE);
  SetAutoResponse("");
  SetSendServer(false);
  SetSendIntIp(false);
  SetShowAwayMsg(false);
  SetSequence(0xFFFFFFFF);
  SetOfflineOnDisconnect(false);
  ClearSocketDesc();
  m_nIp = m_nPort = m_nIntIp = 0;
  m_nMode = MODE_DIRECT;
  m_nVersion = 0;
  m_nCookie = 0;
  m_nClientTimestamp = 0;
  m_nClientInfoTimestamp = 0;
  m_nClientStatusTimestamp = 0;
  m_nOurClientTimestamp = 0;
  m_nOurClientInfoTimestamp = 0;
  m_nOurClientStatusTimestamp = 0;
  m_bUserUpdated = false;
  m_nPhoneFollowMeStatus = ICQ_PLUGIN_STATUSxINACTIVE;
  m_nICQphoneStatus = ICQ_PLUGIN_STATUSxINACTIVE;
  m_nSharedFilesStatus = ICQ_PLUGIN_STATUSxINACTIVE;
  Touch();
  for (unsigned short i = 0; i < 4; i++)
    m_nLastCounters[i] = 0;
  m_nOnlineSince = 0;
  m_nIdleSince = 0;
  m_nStatusToUser = ICQ_STATUS_OFFLINE;
  m_bKeepAliasOnUpdate = false;
  m_nStatus = ICQ_STATUS_OFFLINE;
  m_nAutoAccept = 0;
  m_szCustomAutoResponse = NULL;
  m_bConnectionInProgress = false;
  m_bAwaitingAuth = false;
  m_nSID[0] = m_nSID[1] = m_nSID[2] = 0;
  m_nGSID = 0;
  m_szClientInfo = NULL;

  snprintf(m_szUinString, 12, "%lu", m_nUin);
  m_szUinString[12] = '\0';

  pthread_rdwr_init_np (&mutex_rw, NULL);
}

//-----ICQUser::SetDefaults-----------------------------------------------------
void ICQUser::SetDefaults()
{
  char szTemp[12];
  SetAlias(IdString());
  SetHistoryFile("default");
  SetGroups(GROUPS_SYSTEM, 0);
  if (gUserManager.NewUserGroup())
    SetGroups(GROUPS_USER, (unsigned long)(1 << (gUserManager.NewUserGroup()-1)));
  else
    SetGroups(GROUPS_USER, 0);
  SetNewUser(true);
  SetAuthorization(false);
  SetNewUser(true);

  szTemp[0] = '\0';
  SetFirstName(szTemp);
  SetLastName(szTemp);
  SetEmailPrimary(szTemp);
  SetEmailSecondary(szTemp);
  SetEmailOld(szTemp);
  SetCity(szTemp);
  SetState(szTemp);
  SetPhoneNumber(szTemp);
  SetFaxNumber(szTemp);
  SetAddress(szTemp);
  SetCellularNumber(szTemp);
  SetHomepage(szTemp);
  SetHomepageDesc(szTemp);
  SetZipCode(szTemp);
  SetCompanyCity(szTemp);
  SetCompanyState(szTemp);
  SetCompanyPhoneNumber(szTemp);
  SetCompanyFaxNumber(szTemp);
  SetCompanyAddress(szTemp);
  SetCompanyZip(szTemp);
  SetCompanyName(szTemp);
  SetCompanyDepartment(szTemp);
  SetCompanyPosition(szTemp);
  SetCompanyHomepage(szTemp);
  SetAbout(szTemp);
  SetCustomAutoResponse(szTemp);
}



unsigned short ICQUser::Status()
// guarantees to return a unique status that switch can be run on
{
   if (StatusOffline()) return ICQ_STATUS_OFFLINE;
   else if (m_nStatus & ICQ_STATUS_DND) return ICQ_STATUS_DND;
   else if (m_nStatus & ICQ_STATUS_OCCUPIED) return ICQ_STATUS_OCCUPIED;
   else if (m_nStatus & ICQ_STATUS_NA) return ICQ_STATUS_NA;
   else if (m_nStatus & ICQ_STATUS_AWAY) return ICQ_STATUS_AWAY;
   else if (m_nStatus & ICQ_STATUS_FREEFORCHAT) return ICQ_STATUS_FREEFORCHAT;
   else if ((m_nStatus & 0xFF) == 0x00) return ICQ_STATUS_ONLINE;
   else return (ICQ_STATUS_OFFLINE - 1);
}

void ICQUser::SetStatusOffline()
{
  if (!StatusOffline())
  {
    m_nLastCounters[LAST_ONLINE] = time(NULL);
    SaveLicqInfo();
  }
  
  SetTyping(ICQ_TYPING_INACTIVEx0);
  SetUserUpdated(false);
  SetStatus(ICQ_STATUS_OFFLINE);
}



/* Birthday: checks to see if the users birthday is within the next nRange
   days.  Returns -1 if not, or the number of days until their bday */
int ICQUser::Birthday(unsigned short nRange)
{
  static const unsigned char nMonthDays[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

  time_t t = time(NULL);
  struct tm *ts = localtime(&t);
  int nDays = -1;

  if (GetBirthMonth() == 0 || GetBirthDay() == 0)
  {
    if (StatusBirthday() && User()) return 0;
    return -1;
  }

  if (nRange == 0)
  {
    if (ts->tm_mon + 1 == GetBirthMonth() && ts->tm_mday == GetBirthDay())
      nDays = 0;
  }
  else
  {
    unsigned char nMonth, nDayMin, nDayMax, nMonthNext, nDayMaxNext;

    nMonth = ts->tm_mon + 1;
    nMonthNext = nDayMaxNext = 0;
    nDayMin = ts->tm_mday;
    nDayMax = nDayMin + nRange;
    if (nDayMax > nMonthDays[nMonth])
    {
      nMonthNext = nMonth + 1;
      if (nMonthNext == 13) nMonth = 1;
      nDayMaxNext = nDayMax - nMonthDays[nMonth];
      nDayMax = nMonthDays[nMonth];
    }

    if (GetBirthMonth() == nMonth && GetBirthDay() >= nDayMin &&
         GetBirthDay() <= nDayMax)
    {
      nDays = GetBirthDay() - nDayMin;
    }
    else if (nMonthNext != 0 && GetBirthMonth() == nMonthNext &&
        GetBirthDay() <= nDayMaxNext)
    {
      nDays = GetBirthDay() + (nMonthDays[nMonth] - nDayMin);
    }

    /*struct tm tb = *ts;
    tm_mday = GetBirthDay() - 1;
    tm_mon = GetBirthMonth() - 1;
    mktime(&tb);
    nDays = tb.tm_yday - ts->tm_yday;*/
  }

  return nDays;
}


unsigned short ICQUser::Sequence(bool increment)
{
   if (increment)
      return (m_nSequence--);
   else
      return (m_nSequence);
}

void ICQUser::SetAlias(const char *s)
{
  if (s[0] == '\0')
  {
    if (m_szFirstName != NULL && m_szFirstName[0] != '\0')
      SetString(&m_szAlias, m_szFirstName);
    else
      SetString(&m_szAlias, m_szId);
  }
  else
    SetString(&m_szAlias, s);

  SaveGeneralInfo();
}


bool ICQUser::Away()
{
   unsigned short n = Status();
   return (n == ICQ_STATUS_AWAY || n == ICQ_STATUS_NA ||
           n == ICQ_STATUS_DND || n == ICQ_STATUS_OCCUPIED);
}

void ICQUser::SetHistoryFile(const char *s)
{
  m_fHistory.SetFile(s, m_szId, m_nPPID);
  SaveLicqInfo();
}


void ICQUser::SetIpPort(unsigned long _nIp, unsigned short _nPort)
{
  if ((SocketDesc(ICQ_CHNxNONE) != -1 || SocketDesc(ICQ_CHNxINFO) != -1
       || SocketDesc(ICQ_CHNxSTATUS) != -1) &&
      ( (Ip() != 0 && Ip() != _nIp) || (Port() != 0 && Port() != _nPort)) )
  {
    // Close our socket, but don't let socket manager try and clear
    // our socket descriptor
    if (SocketDesc(ICQ_CHNxNONE) != -1)
      gSocketManager.CloseSocket(SocketDesc(ICQ_CHNxNONE), false);
    if (SocketDesc(ICQ_CHNxINFO) != -1)
      gSocketManager.CloseSocket(SocketDesc(ICQ_CHNxINFO), false);
    if (SocketDesc(ICQ_CHNxSTATUS) != -1)
      gSocketManager.CloseSocket(SocketDesc(ICQ_CHNxSTATUS), false);
    ClearSocketDesc();
  }
  m_nIp = _nIp;
  m_nPort = _nPort;
  SaveLicqInfo();
}

int ICQUser::SocketDesc(unsigned char nChannel)
{
  switch (nChannel)
  {
  case ICQ_CHNxNONE:
    return m_nNormalSocketDesc;
  case ICQ_CHNxINFO:
    return m_nInfoSocketDesc;
  case ICQ_CHNxSTATUS:
    return m_nStatusSocketDesc;
  }
  gLog.Warn("%sUnknown channel type %u\n", L_WARNxSTR, nChannel);

  return 0;
}

void ICQUser::SetSocketDesc(TCPSocket *s)
{
  if (s->Channel() == ICQ_CHNxNONE)
    m_nNormalSocketDesc = s->Descriptor();
  else if (s->Channel() == ICQ_CHNxINFO)
    m_nInfoSocketDesc = s->Descriptor();
  else if (s->Channel() == ICQ_CHNxSTATUS)
    m_nStatusSocketDesc = s->Descriptor();
  m_nLocalPort = s->LocalPort();
  m_nConnectionVersion = s->Version();
  if (m_bSecure != s->Secure())
  {
    m_bSecure = s->Secure();
    if (gLicqDaemon != NULL && m_bOnContactList)
      gLicqDaemon->PushPluginSignal(new CICQSignal(SIGNAL_UPDATExUSER, USER_SECURITY,
        m_szId, m_nPPID, m_bSecure ? 1 : 0));
  }

  if (m_nIntIp == 0) m_nIntIp = s->RemoteIp();
  if (m_nPort == 0) m_nPort = s->RemotePort();
  SetSendServer(false);
}


void ICQUser::ClearSocketDesc()
{
  m_nNormalSocketDesc = m_nInfoSocketDesc = m_nStatusSocketDesc = -1;
  m_nLocalPort = 0;
  m_nConnectionVersion = 0;
  m_bSecure = false;

  if (gLicqDaemon != NULL && m_bOnContactList)
    gLicqDaemon->PushPluginSignal(new CICQSignal(SIGNAL_UPDATExUSER,
      USER_SECURITY, m_szId, m_nPPID, 0));
}

void ICQUser::ClearSocketDesc(unsigned char nChannel)
{
  switch (nChannel)
  {
  case ICQ_CHNxNONE:
    m_nNormalSocketDesc = -1;
    break;
  case ICQ_CHNxINFO:
    m_nInfoSocketDesc = -1;
    break;
  case ICQ_CHNxSTATUS:
    m_nStatusSocketDesc = -1;
    break;
  default:
    gLog.Info("%sUnknown channel %u\n", L_WARNxSTR, nChannel);
    return;
  }
  if (m_nStatusSocketDesc == m_nInfoSocketDesc == m_nNormalSocketDesc == -1)
    ClearSocketDesc();
  else if (gLicqDaemon != NULL && m_bOnContactList)
    gLicqDaemon->PushPluginSignal(new CICQSignal(SIGNAL_UPDATExUSER, USER_SECURITY, m_szId, m_nPPID, 0));
}

unsigned short ICQUser::ConnectionVersion()
{
  // If we are already connected, use that version
  if (m_nConnectionVersion != 0) return m_nConnectionVersion;
  // We aren't connected, see if we know their version
  return VersionToUse(m_nVersion);
}


int ICQUser::LocalTimeGMTOffset()
{
  return GetTimezone() * 1800;
}


int ICQUser::SystemTimeGMTOffset()
{
  time_t t = time(NULL);
  struct tm *tzone = localtime(&t);
#ifdef USE_GMTOFF
  return -(tzone->tm_gmtoff) + (tzone->tm_isdst == 1 ? 3600 : 0); // seconds _east_ of UTC
#elif defined(USE_TIMEZONE)
  return timezone;  // seconds _west_ of UTC
#else
#warning Unable to determine local timezone
  return 0;
#endif
}

char ICQUser::SystemTimezone()
{
  char nTimezone = SystemTimeGMTOffset() / 1800;
  if (nTimezone > 23)
    return 23 - nTimezone;
  return nTimezone;
}


int ICQUser::LocalTimeOffset()
{
  return SystemTimeGMTOffset() - LocalTimeGMTOffset();
}


time_t ICQUser::LocalTime()
{
  return time(NULL) + LocalTimeOffset();
}


SecureChannelSupport_et ICQUser::SecureChannelSupport()
{
  if ((m_nClientTimestamp & 0xFFFF0000) == LICQ_WITHSSL)
    return SECURE_CHANNEL_SUPPORTED;
  else if ((m_nClientTimestamp & 0xFFFF0000) == LICQ_WITHOUTSSL)
    return SECURE_CHANNEL_NOTSUPPORTED;
  else
    return SECURE_CHANNEL_UNKNOWN;
}



unsigned short ICQUser::LicqVersion()
{
  if ((m_nClientTimestamp & 0xFFFF0000) == LICQ_WITHSSL ||
       (m_nClientTimestamp & 0xFFFF0000) == LICQ_WITHOUTSSL)
    return m_nClientTimestamp & 0x0000FFFF;

  return LICQ_VERSION_UNKNOWN;
}


const char *ICQUser::StatusStr()
{
  return StatusToStatusStr(m_nStatus, StatusInvisible());
}

const char *ICQUser::StatusStrShort()
{
  return StatusToStatusStrShort(m_nStatus, StatusInvisible());
}


const char *ICQUser::StatusToStatusStr(unsigned short n, bool b)
{
  if (n == ICQ_STATUS_OFFLINE) return b ? tr("(Offline)") : tr("Offline");
  else if (n & ICQ_STATUS_DND) return b ? tr("(Do Not Disturb)") : tr("Do Not Disturb");
  else if (n & ICQ_STATUS_OCCUPIED) return b ? tr("(Occupied)") : tr("Occupied");
  else if (n & ICQ_STATUS_NA) return b ? tr("(Not Available)") : tr("Not Available");
  else if (n & ICQ_STATUS_AWAY) return b ? tr("(Away)") : tr("Away");
  else if (n & ICQ_STATUS_FREEFORCHAT) return b ? tr("(Free for Chat)") : tr("Free for Chat");
  else if (n << 24 == 0x00) return b ? tr("(Online)") : tr("Online");
  else return "Unknown";
}


const char *ICQUser::StatusToStatusStrShort(unsigned short n, bool b)
{
  if (n == ICQ_STATUS_OFFLINE) return b ? tr("(Off)") : tr("Off");
  else if (n & ICQ_STATUS_DND) return b ? tr("(DND)") : tr("DND");
  else if (n & ICQ_STATUS_OCCUPIED) return b ? tr("(Occ)") : tr("Occ");
  else if (n & ICQ_STATUS_NA) return b ? tr("(N/A)") : tr("N/A");
  else if (n & ICQ_STATUS_AWAY) return b ? tr("(Away)") : tr("Away");
  else if (n & ICQ_STATUS_FREEFORCHAT) return b ? tr("(FFC)") : tr("FFC");
  else if (n << 24 == 0x00) return b ? tr("(On)") : tr("On");
  else return "???";
}


char *ICQUser::IpStr(char *rbuf)
{
  char ip[32], buf[32];

  if (Ip() > 0)     		// Default to the given ip
    strcpy(ip, inet_ntoa_r(*(struct in_addr *)&m_nIp, buf));
  else				// Otherwise we don't know
    strcpy(ip, tr("Unknown"));

  if (StatusHideIp())
    sprintf(rbuf, "(%s)", ip);
  else
    sprintf(rbuf, "%s", ip);

  return rbuf;
}


char *ICQUser::PortStr(char *rbuf)
{
  if (Port() > 0)     		// Default to the given port
    sprintf(rbuf, "%d", Port());
  else				// Otherwise we don't know
    rbuf[0] = '\0';

  return rbuf;
}


char *ICQUser::IntIpStr(char *rbuf)
{
  char buf[32];
  int socket = SocketDesc(ICQ_CHNxNONE);
  if (socket < 0)
    socket = SocketDesc(ICQ_CHNxINFO);
  if (socket < 0)
    socket = SocketDesc(ICQ_CHNxSTATUS);

  if (socket > 0)		// First check if we are connected
  {
    INetSocket *s = gSocketManager.FetchSocket(socket);
    if (s != NULL)
    {
      strcpy(rbuf, s->RemoteIpStr(buf));
      gSocketManager.DropSocket(s);
    }
    else
      strcpy(rbuf, tr("Invalid"));
  }
  else
  {
    if (IntIp() > 0)		// Default to the given ip
      strcpy(rbuf, inet_ntoa_r(*(struct in_addr *)&m_nIntIp, buf));
    else			// Otherwise we don't know
      rbuf[0] = '\0';
  }

  return rbuf;
}


char *ICQUser::usprintf(const char *_szFormat, unsigned long nFlags)
{
  bool bLeft = false;
  unsigned long i = 0, j, nField = 0, nPos = 0;
  char szTemp[128];
  const char *sz;

  // Our secure string for escaping stuff
  bool bSecure = (_szFormat[0] == '|' && (nFlags & USPRINTF_PIPEISCMD)) ||
   (nFlags & USPRINTF_LINEISCMD);

  unsigned long bufSize = strlen(_szFormat) + 512;
  char *_sz = (char *)malloc(bufSize);
#define CHECK_BUFFER                     \
  if (nPos >= bufSize - 1)               \
  {                                      \
    bufSize *= 2;                        \
    _sz = (char *)realloc(_sz, bufSize); \
  }

  while(_szFormat[i] != '\0')
  {
    if (_szFormat[i] == '`')
    {
        _sz[nPos++] = '`';
        CHECK_BUFFER;
        i++;
        while(_szFormat[i] != '`' && _szFormat[i] != '\0')
        {
            _sz[nPos++] = _szFormat[i++];
            CHECK_BUFFER;
        }
        if (_szFormat[i] != '\0')
        {
          _sz[nPos++] = _szFormat[i];
          CHECK_BUFFER;
          i++;
        }
    }
    else if (_szFormat[i] == '%')
    {
      i++;
      if (!(nFlags & USPRINTF_NOFW))
      {
        if (_szFormat[i] == '-')
        {
          i++;
          bLeft = true;
        }
        j = nField = 0;
        while (isdigit(_szFormat[i]))
          szTemp[j++] = _szFormat[i++];
        szTemp[j] = '\0';
        if (j > 0) nField = atoi(szTemp);
      }
      else
      {
        if (isdigit(_szFormat[i]))
        {
          _sz[nPos++] = _szFormat[i - 1];
          CHECK_BUFFER;
          _sz[nPos++] = _szFormat[i++];
          CHECK_BUFFER;
          continue;
        }
      }

      switch(_szFormat[i])
      {
        case 'i':
          char buf[32];
          strcpy(szTemp, inet_ntoa_r(*(struct in_addr *)&m_nIp, buf));
          sz = szTemp;
          break;
        case 'p':
          sprintf(szTemp, "%d", Port());
          sz = szTemp;
          break;
        case 'P':
        {
          ProtoPluginsList pl;
          ProtoPluginsListIter it;
          gLicqDaemon->ProtoPluginList(pl);
          for (it = pl.begin(); it != pl.end(); it++)
          {
            if (m_nPPID == (*it)->PPID())
            {
              strcpy(szTemp, (*it)->Name());
              sz = szTemp;
              break;
            }
          }
          break;
        }
        case 'e':
          sz = GetEmailPrimary();
          if (sz[0] == '\0')
          {
            sz = GetEmailSecondary();
            if (sz[0] == '\0')
              sz = GetEmailOld();
          }
          break;
        case 'n':
          snprintf(szTemp, sizeof(szTemp), "%s %s", GetFirstName(), GetLastName());
          szTemp[sizeof(szTemp) - 1] = '\0';
          sz = szTemp;
          break;
        case 'f':
          sz = GetFirstName();
          break;
        case 'l':
          sz = GetLastName();
          break;
        case 'a':
          sz = GetAlias();
          break;
        case 'u':
          sz = IdString();
          break;
        case 'w':
          sz = GetHomepage();
          break;
        case 'h':
          sz = GetPhoneNumber();
          break;
        case 'c':
          sz = GetCellularNumber();
          break;
        case 'S':
          sz = StatusStrShort();
          break;
        case 's':
          sz = StatusStr();
          break;

        case 't':
        {
          time_t t = time(NULL);
          strftime(szTemp, 128, "%b %d %r", localtime(&t));
          sz = szTemp;
          break;
        }

        case 'T':
        {
          time_t t = time(NULL);
          strftime(szTemp, 128, "%b %d %R %Z", localtime(&t));
          sz = szTemp;
          break;
        }

        case 'o':
          if(m_nLastCounters[LAST_ONLINE] == 0)
          {
            strcpy(szTemp, tr("Never"));
            sz = szTemp;
            break;
          }
          strftime(szTemp, 128, "%b %d %R", localtime(&m_nLastCounters[LAST_ONLINE]));
          sz = szTemp;
          break;
        case 'O':
          if (m_nStatus == ICQ_STATUS_OFFLINE || m_nOnlineSince == 0)
          {
            strcpy(szTemp, tr("Unknown"));
            sz = szTemp;
            break;
          }
          strftime(szTemp, 128, "%b %d %R", localtime(&m_nOnlineSince));
          sz = szTemp;
          break;

        case 'I':
        {
          if (m_nIdleSince)
          {
            unsigned short nDays, nHours, nMinutes;
            char szTime[128];
            unsigned long nIdleTime = time(NULL) - m_nIdleSince;
            nDays = nIdleTime / ( 60 * 60 * 24);
            nHours = (nIdleTime % (60 * 60 * 24)) / (60 * 60);
            nMinutes = (nIdleTime % (60 * 60)) / 60;

            strcpy(szTemp, "");

            if (nDays)
            {
              if (nDays > 1)
                sprintf(szTime, tr("%d days "), nDays);
              else
                sprintf(szTime, tr("%d day "), nDays);
              strcat(szTemp, szTime);
            }

            if (nHours)
            {
              if (nHours > 1)
                sprintf(szTime, tr("%d hours "), nHours);
              else
                sprintf(szTime, tr("%d hour "), nHours);
              strcat(szTemp, szTime);
            }

            if (nMinutes)
            {
              if (nMinutes > 1)
                sprintf(szTime, tr("%d minutes"), nMinutes);
              else
                sprintf(szTime, tr("%d minute"), nMinutes);
              strcat(szTemp, szTime);
            }
          }
          else
            strcpy(szTemp, tr("Active"));

          sz = szTemp;

          break;
        }

        case 'm':
          sprintf(szTemp, "%d", NewMessages());
          sz = szTemp;
          break;
        case '%':
          strcpy(szTemp, "\%");
          sz = szTemp;
          break;
        default:
          gLog.Warn("%sWarning: Invalid qualifier in command: %%%c.\n",
                    L_WARNxSTR, _szFormat[i]);
          sprintf(szTemp, "%s%lu%%%c", (bLeft ? "-" : ""), nField, _szFormat[i]);
          sz = szTemp;
          bLeft = false;
          nField = 0;
          break;
      }

      // If we need to be secure, then quote the % string
      if (bSecure)
      {
        _sz[nPos++] = '\'';
        CHECK_BUFFER;
      }

// The only way to escape a ' inside a ' is to do '\'' believe it or not
#define PACK_STRING(x)                          \
  while(x)                                      \
  {                                             \
    if (bSecure && sz[j] == '\'')               \
    {                                           \
      if (nPos >= bufSize - 5)                  \
      {                                         \
        bufSize *= 2;                           \
        _sz = (char *)realloc(_sz, bufSize);    \
      }                                         \
      nPos += sprintf(&_sz[nPos], "'\\''");     \
      j++;                                      \
    }                                           \
    else                                        \
    {                                           \
      _sz[nPos++] = sz[j++];                    \
      CHECK_BUFFER;                             \
    }                                           \
  }

      // Now append sz to the string using the given field width and alignment
      if (nField == 0)
      {
        j = 0;
        PACK_STRING(sz[j] != '\0');
      }
      else
      {
        if (bLeft)
        {
          j = 0;
          PACK_STRING(sz[j] != '\0');
          while(j++ < nField)
          {
            _sz[nPos++] = ' ';
            CHECK_BUFFER;
          }
        }
        else
        {
          int nLen = nField - strlen(sz);
          if (nLen < 0)
          {
            j = 0;
            //while(j < nField) _sz[nPos++] = sz[j++];
            PACK_STRING(j < nField);
          }
          else
          {
            for (j = 0; j < (unsigned long)nLen; j++)
            {
              _sz[nPos++] = ' ';
              CHECK_BUFFER;
            }
            j = 0;
            PACK_STRING(sz[j] != '\0');
          }
        }
      }

      // If we need to be secure, then quote the % string
      if (bSecure)
      {
        _sz[nPos++] = '\'';
        CHECK_BUFFER;
      }

      if (_szFormat[i] != '\0') i++;
    }
    else
    {
      if (_szFormat[i] == '\n')
      {
        if (nFlags & USPRINTF_NTORN)
        {
          _sz[nPos++] = '\r';
          CHECK_BUFFER;
        }
        if (nFlags & USPRINTF_PIPEISCMD)
          bSecure = (_szFormat[i + 1] == '|');
      }
      _sz[nPos++] = _szFormat[i++];
      CHECK_BUFFER;
    }
  }
  _sz[nPos] = '\0';

  return _sz;
}

//Return value must be delete []'d
char *ICQUser::MakeRealId(const char *_szId, unsigned long _nPPID,
                                char *&szRealId)
{
  if (!(_szId == 0 || strlen(_szId) == 0))
    szRealId = new char[strlen(_szId) + 1];
  else
  {
    szRealId = new char[1];
    szRealId[0] = '\0';
    return szRealId;
  }

  int j = 0;
  if (_nPPID == LICQ_PPID && !isdigit(_szId[0]))
  {
    for (unsigned int i = 0; i < strlen(_szId); i++)
      if (_szId[i] != ' ')
        szRealId[j++] = tolower(_szId[i]);
    szRealId[j] = '\0';
  }
  else
    strcpy(szRealId, _szId);

  return szRealId;
}

//-----ICQUser::SaveGeneralInfo----------------------------------------------
void ICQUser::SaveGeneralInfo()
{
  if (!EnableSave()) return;

  if (!m_fConf.ReloadFile())
  {
     gLog.Error("%sError opening '%s' for reading.\n%sSee log for details.\n",
                L_ERRORxSTR, m_fConf.FileName(),  L_BLANKxSTR);
     return;
  }
  m_fConf.SetSection("user");
  m_fConf.WriteStr("Alias", m_szAlias);
  m_fConf.WriteBool("KeepAliasOnUpdate", m_bKeepAliasOnUpdate);
  m_fConf.WriteStr("FirstName", m_szFirstName);
  m_fConf.WriteStr("LastName", m_szLastName);
  m_fConf.WriteStr("Email1", m_szEmailPrimary);
  m_fConf.WriteStr("Email2", m_szEmailSecondary);
  m_fConf.WriteStr("EmailO", m_szEmailOld);
  m_fConf.WriteStr("City", m_szCity);
  m_fConf.WriteStr("State", m_szState);
  m_fConf.WriteStr("PhoneNumber", m_szPhoneNumber);
  m_fConf.WriteStr("FaxNumber", m_szFaxNumber);
  m_fConf.WriteStr("Address", m_szAddress);
  m_fConf.WriteStr("CellularNumber", m_szCellularNumber);
  m_fConf.WriteStr("Zipcode", m_szZipCode);
  m_fConf.WriteNum("Country", m_nCountryCode);
  m_fConf.WriteNum("Timezone", m_nTimezone);
  m_fConf.WriteBool("Authorization", m_bAuthorization);
  m_fConf.WriteBool("HideEmail", m_bHideEmail);

  if (!m_fConf.FlushFile())
  {
    gLog.Error("%sError opening '%s' for writing.\n%sSee log for details.\n",
               L_ERRORxSTR, m_fConf.FileName(), L_BLANKxSTR);
    return;
  }

  m_fConf.CloseFile();
}


//-----ICQUser::SaveMoreInfo----------------------------------------------
void ICQUser::SaveMoreInfo()
{
  if (!EnableSave()) return;

  if (!m_fConf.ReloadFile())
  {
     gLog.Error("%sError opening '%s' for reading.\n%sSee log for details.\n",
                L_ERRORxSTR, m_fConf.FileName(),  L_BLANKxSTR);
     return;
  }
  m_fConf.SetSection("user");
  m_fConf.WriteNum("Age", m_nAge);
  m_fConf.WriteNum("Gender", m_nGender);
  m_fConf.WriteStr("Homepage", m_szHomepage);
  m_fConf.WriteNum("BirthYear", m_nBirthYear);
  m_fConf.WriteNum("BirthMonth", m_nBirthMonth);
  m_fConf.WriteNum("BirthDay", m_nBirthDay);
  m_fConf.WriteNum("Language1", m_nLanguage[0]);
  m_fConf.WriteNum("Language2", m_nLanguage[1]);
  m_fConf.WriteNum("Language3", m_nLanguage[2]);

  if (!m_fConf.FlushFile())
  {
    gLog.Error("%sError opening '%s' for writing.\n%sSee log for details.\n",
               L_ERRORxSTR, m_fConf.FileName(), L_BLANKxSTR);
    return;
  }

  m_fConf.CloseFile();
}

//-----ICQUser::SaveHomepageInfo----------------------------------------------
void ICQUser::SaveHomepageInfo()
{
  if (!EnableSave()) return;

  if (!m_fConf.ReloadFile())
  {
     gLog.Error("%sError opening '%s' for reading.\n%sSee log for details.\n",
                L_ERRORxSTR, m_fConf.FileName(),  L_BLANKxSTR);
     return;
  }
  m_fConf.SetSection("user");
  m_fConf.WriteBool("HomepageCatPresent", m_bHomepageCatPresent);
  m_fConf.WriteNum("HomepageCatCode", m_nHomepageCatCode);
  m_fConf.WriteStr("HomepageDesc", m_szHomepageDesc);
  m_fConf.WriteBool("ICQHomepagePresent", m_bICQHomepagePresent);

  if (!m_fConf.FlushFile())
  {
    gLog.Error("%sError opening '%s' for writing.\n%sSee log for details.\n",
               L_ERRORxSTR, m_fConf.FileName(), L_BLANKxSTR);
    return;
  }

  m_fConf.CloseFile();
}

//-----ICQUser::SaveWorkInfo----------------------------------------------
void ICQUser::SaveWorkInfo()
{
  if (!EnableSave()) return;

  if (!m_fConf.ReloadFile())
  {
     gLog.Error("%sError opening '%s' for reading.\n%sSee log for details.\n",
                L_ERRORxSTR, m_fConf.FileName(),  L_BLANKxSTR);
     return;
  }
  m_fConf.SetSection("user");
  m_fConf.WriteStr("CompanyCity", m_szCompanyCity);
  m_fConf.WriteStr("CompanyState", m_szCompanyState);
  m_fConf.WriteStr("CompanyPhoneNumber", m_szCompanyPhoneNumber);
  m_fConf.WriteStr("CompanyFaxNumber", m_szCompanyFaxNumber);
  m_fConf.WriteStr("CompanyAddress", m_szCompanyAddress);
  m_fConf.WriteStr("CompanyZip", m_szCompanyZip);
  m_fConf.WriteNum("CompanyCountry", m_nCompanyCountry);
  m_fConf.WriteStr("CompanyName", m_szCompanyName);
  m_fConf.WriteStr("CompanyDepartment", m_szCompanyDepartment);
  m_fConf.WriteStr("CompanyPosition", m_szCompanyPosition);
  m_fConf.WriteNum("CompanyOccupation", m_nCompanyOccupation);
  m_fConf.WriteStr("CompanyHomepage", m_szCompanyHomepage);

  if (!m_fConf.FlushFile())
  {
    gLog.Error("%sError opening '%s' for writing.\n%sSee log for details.\n",
               L_ERRORxSTR, m_fConf.FileName(), L_BLANKxSTR);
    return;
  }

  m_fConf.CloseFile();
}


//-----ICQUser::SaveAboutInfo-------------------------------------------------
void ICQUser::SaveAboutInfo()
{
   if (!EnableSave()) return;

   if (!m_fConf.ReloadFile())
   {
      gLog.Error("%sError opening '%s' for reading.\n%sSee log for details.\n",
                 L_ERRORxSTR, m_fConf.FileName(), L_BLANKxSTR);
      return;
   }
   m_fConf.SetSection("user");
   m_fConf.WriteStr("About", m_szAbout);
   if (!m_fConf.FlushFile())
   {
     gLog.Error("%sError opening '%s' for writing.\n%sSee log for details.\n",
                L_ERRORxSTR, m_fConf.FileName(), L_BLANKxSTR);
     return;
   }

   m_fConf.CloseFile();
}

//-----ICQUser::Save<categories>Info()-----------------------------------------
static const char *const szN_int    = "InterestsN";
static const char *const szCat_int  = "InterestsCat%04X";
static const char *const szDesc_int = "InterestsDesc%04X";

static const char *const szN_bac    = "BackgroundsN";
static const char *const szCat_bac  = "BackgroundsCat%04X";
static const char *const szDesc_bac = "BackgroundsDesc%04X";

static const char *const szN_aff    = "OrganizationsN";
static const char *const szCat_aff  = "OrganizationsCat%04X";
static const char *const szDesc_aff = "OrganizationsDesc%04X";


void ICQUser::SaveInterestsInfo()
{
  if (!EnableSave())
    return;

  m_Interests->SaveToDisk(m_fConf, szN_int, szCat_int, szDesc_int);
}

void ICQUser::LoadInterestsInfo()
{
  m_Interests->LoadFromDisk(m_fConf, szN_int, szCat_int, szDesc_int);
}

void ICQUser::SaveBackgroundsInfo()
{
  if (!EnableSave())
    return;

  m_Backgrounds->SaveToDisk(m_fConf, szN_bac, szCat_bac, szDesc_bac);
}

void ICQUser::LoadBackgroundsInfo()
{
  m_Backgrounds->LoadFromDisk(m_fConf, szN_bac, szCat_bac, szDesc_bac);
}

void ICQUser::SaveOrganizationsInfo()
{
  if (!EnableSave())
    return;

  m_Organizations->SaveToDisk(m_fConf,szN_aff,szCat_aff, szDesc_aff);
}

void ICQUser::LoadOrganizationsInfo()
{
  m_Organizations->LoadFromDisk(m_fConf, szN_aff,szCat_aff, szDesc_aff);
}

//-----ICQUser::SavePhoneBookInfo--------------------------------------------
void ICQUser::SavePhoneBookInfo()
{
  if (!EnableSave()) return;

  m_PhoneBook->SaveToDisk(m_fConf);
}

//-----ICQUser::SavePictureInfo----------------------------------------------
void ICQUser::SavePictureInfo()
{
  if (!EnableSave()) return;

  if (!m_fConf.ReloadFile())
  {
     gLog.Error("%sError opening '%s' for reading.\n%sSee log for details.\n",
                L_ERRORxSTR, m_fConf.FileName(), L_BLANKxSTR);
     return;
  }
  m_fConf.SetSection("user");
  m_fConf.WriteBool("PicturePresent", m_bPicturePresent);
  if (!m_fConf.FlushFile())
  {
    gLog.Error("%sError opening '%s' for writing.\n%sSee log for details.\n",
               L_ERRORxSTR, m_fConf.FileName(), L_BLANKxSTR);
    return;
  }

  m_fConf.CloseFile();
}

//-----ICQUser::SaveLicqInfo-------------------------------------------------
void ICQUser::SaveLicqInfo()
{
   if (!EnableSave()) return;

   if (!m_fConf.ReloadFile())
   {
      gLog.Error("%sError opening '%s' for reading.\n%sSee log for details.\n",
                 L_ERRORxSTR, m_fConf.FileName(), L_BLANKxSTR);
      return;
   }
   char buf[64];
   m_fConf.SetSection("user");
   m_fConf.WriteStr("History", HistoryName());
   m_fConf.WriteNum("Groups.System", GetGroups(GROUPS_SYSTEM));
   m_fConf.WriteNum("Groups.User", GetGroups(GROUPS_USER));
   m_fConf.WriteStr("Ip", inet_ntoa_r(*(struct in_addr *)&m_nIp, buf));
   m_fConf.WriteStr("IntIp", inet_ntoa_r(*(struct in_addr *)&m_nIntIp, buf));
   m_fConf.WriteNum("Port", Port());
   m_fConf.WriteNum("NewMessages", NewMessages());
   m_fConf.WriteNum("LastOnline", (unsigned long)LastOnline());
   m_fConf.WriteNum("LastSent", (unsigned long)LastSentEvent());
   m_fConf.WriteNum("LastRecv", (unsigned long)LastReceivedEvent());
   m_fConf.WriteNum("LastCheckedAR", (unsigned long)LastCheckedAutoResponse());
   m_fConf.WriteNum("AutoAccept", m_nAutoAccept);
   m_fConf.WriteNum("StatusToUser", m_nStatusToUser);
   m_fConf.WriteStr("CustomAutoRsp", CustomAutoResponse());
   m_fConf.WriteBool("SendIntIp", m_bSendIntIp);
   m_fConf.WriteStr("UserEncoding", m_szEncoding);
   m_fConf.WriteBool("AwaitingAuth", m_bAwaitingAuth);
   m_fConf.WriteNum("SID", m_nSID[NORMAL_SID]);
   m_fConf.WriteNum("InvisibleSID", m_nSID[INV_SID]);
   m_fConf.WriteNum("VisibleSID", m_nSID[VIS_SID]);
   m_fConf.WriteNum("GSID", m_nGSID);
   m_fConf.WriteNum("ClientTimestamp", m_nClientTimestamp);
   m_fConf.WriteNum("ClientInfoTimestamp", m_nClientInfoTimestamp);
   m_fConf.WriteNum("ClientStatusTimestamp", m_nClientStatusTimestamp);
   m_fConf.WriteNum("OurClientTimestamp", m_nOurClientTimestamp);
   m_fConf.WriteNum("OurClientInfoTimestamp", m_nOurClientInfoTimestamp);
   m_fConf.WriteNum("OurClientStatusTimestamp", m_nOurClientStatusTimestamp);
   m_fConf.WriteNum("PhoneFollowMeStatus", m_nPhoneFollowMeStatus);
   m_fConf.WriteNum("ICQphoneStatus", m_nICQphoneStatus);
   m_fConf.WriteNum("SharedFilesStatus", m_nSharedFilesStatus);
   m_fConf.WriteBool("UseGPG", m_bUseGPG );
   m_fConf.WriteStr("GPGKey", m_szGPGKey );

   if (!m_fConf.FlushFile())
   {
     gLog.Error("%sError opening '%s' for writing.\n%sSee log for details.\n",
                L_ERRORxSTR, m_fConf.FileName(), L_BLANKxSTR);
     return;
   }

   m_fConf.CloseFile();
}



void ICQUser::SaveNewMessagesInfo()
{
   if (!EnableSave()) return;

   if (!m_fConf.ReloadFile())
   {
      gLog.Error("%sError opening '%s' for reading.\n%sSee log for details.\n",
                 L_ERRORxSTR, m_fConf.FileName(), L_BLANKxSTR);
      return;
   }
   m_fConf.SetSection("user");
   m_fConf.WriteNum("NewMessages", NewMessages());
   if (!m_fConf.FlushFile())
   {
     gLog.Error("%sError opening '%s' for writing.\n%sSee log for details.\n",
                L_ERRORxSTR, m_fConf.FileName(), L_BLANKxSTR);
     return;
   }

   m_fConf.CloseFile();
}


//-----ICQUser::SaveExtInfo--------------------------------------------------
void ICQUser::SaveExtInfo()
{
   if (!EnableSave()) return;

   if (!m_fConf.ReloadFile())
   {
      gLog.Error("%sError opening '%s' for writing.\n%sSee log for details.\n",
                 L_ERRORxSTR, m_fConf.FileName(), L_BLANKxSTR);
      return;
   }
   m_fConf.SetSection("user");
   m_fConf.WriteStr("Homepage", GetHomepage());
   m_fConf.WriteStr("City", GetCity());
   m_fConf.WriteStr("State", GetState());
   m_fConf.WriteNum("Country", GetCountryCode());
   m_fConf.WriteNum("Timezone", (signed short)GetTimezone());
   m_fConf.WriteStr("Zipcode", GetZipCode());
   m_fConf.WriteStr("PhoneNumber", GetPhoneNumber());
   m_fConf.WriteNum("Age", GetAge());
   m_fConf.WriteNum("Sex", (unsigned short)GetGender());
   m_fConf.WriteStr("About", GetAbout());
   if (!m_fConf.FlushFile())
   {
     gLog.Error("%sError opening '%s' for writing.\n%sSee log for details.\n",
                L_ERRORxSTR, m_fConf.FileName(), L_BLANKxSTR);
     return;
   }

   m_fConf.CloseFile();

}


//-----ICQUser::EventPush--------------------------------------------------------
void ICQUser::EventPush(CUserEvent *e)
{
  m_vcMessages.push_back(e);
  incNumUserEvents();
  SaveNewMessagesInfo();
  Touch();
  SetLastReceivedEvent();
  
  gLicqDaemon->PushPluginSignal(new CICQSignal(SIGNAL_UPDATExUSER,
    USER_EVENTS, m_szId, m_nPPID, e->Id(), e->ConvoId()));
}


void ICQUser::WriteToHistory(const char *_szText)
{
  m_fHistory.Append(_szText);
}



//-----ICQUser::GetEvent--------------------------------------------------------
CUserEvent *ICQUser::EventPeek(unsigned short index)
{
  if (index >= NewMessages()) return (NULL);
  return (m_vcMessages[index]);
}

CUserEvent *ICQUser::EventPeekId(int id)
{
  if (m_vcMessages.size() == 0) return NULL;
  CUserEvent *e = NULL;
  UserEventList::iterator iter;
  for (iter = m_vcMessages.begin(); iter != m_vcMessages.end(); iter++)
  {
    if ((*iter)->Id() == id)
    {
      e = *iter;
      break;
    }
  }
  return e;
}

//-----ICQUser::EventPeekLast----------------------------------------------------
CUserEvent *ICQUser::EventPeekLast()
{
  if (m_vcMessages.size() == 0) return (NULL);
  return (m_vcMessages[m_vcMessages.size() - 1]);
}

//-----ICQUser::EventPeekFirst----------------------------------------------------
CUserEvent *ICQUser::EventPeekFirst()
{
  if (m_vcMessages.size() == 0) return (NULL);
  return (m_vcMessages[0]);
}

//-----ICQUser::EventPop-----------------------------------------------------
CUserEvent *ICQUser::EventPop()
{
  if (m_vcMessages.size() == 0) return NULL;
  CUserEvent *e = m_vcMessages[0];
  for (unsigned short i = 0; i < m_vcMessages.size() - 1; i++)
    m_vcMessages[i] = m_vcMessages[i + 1];
  m_vcMessages.pop_back();
  decNumUserEvents();
  SaveNewMessagesInfo();
  
  gLicqDaemon->PushPluginSignal(new CICQSignal(SIGNAL_UPDATExUSER,
     USER_EVENTS, m_szId, m_nPPID, e->Id()));

  return e;
}

//-----ICQUser::EventClear----------------------------------------------------
void ICQUser::EventClear(unsigned short index)
{
  if (index >= m_vcMessages.size()) return;

  unsigned long id = m_vcMessages[index]->Id();

  delete m_vcMessages[index];
  for (unsigned short i = index; i < m_vcMessages.size() - 1; i++)
    m_vcMessages[i] = m_vcMessages[i + 1];
  m_vcMessages.pop_back();
  decNumUserEvents();
  SaveNewMessagesInfo();
  
  gLicqDaemon->PushPluginSignal(new CICQSignal(SIGNAL_UPDATExUSER,
    USER_EVENTS, m_szId, m_nPPID, -id));
}


void ICQUser::EventClearId(int id)
{
  UserEventList::iterator iter;
  for (iter = m_vcMessages.begin(); iter != m_vcMessages.end(); iter++)
  {
    if ((*iter)->Id() == id)
    {
      delete *iter;
      m_vcMessages.erase(iter);
      decNumUserEvents();
      SaveNewMessagesInfo();
      
      gLicqDaemon->PushPluginSignal(new CICQSignal(SIGNAL_UPDATExUSER,
         USER_EVENTS, m_szId, m_nPPID, -id));
      break;
    }
  }
}


bool ICQUser::GetInGroup(GroupType g, unsigned short _nGroup)
{
  if (_nGroup == 0) return true;
  return (GetGroups(g) & (unsigned long)(1 << (_nGroup - 1)));
}

void ICQUser::SetInGroup(GroupType g, unsigned short _nGroup, bool _bIn)
{
  if (_bIn)
    AddToGroup(g, _nGroup);
  else
    RemoveFromGroup(g, _nGroup);
}

void ICQUser::AddToGroup(GroupType g, unsigned short _nGroup)
{
  if (_nGroup == 0) return;
  SetGroups(g, GetGroups(g) | (unsigned long)(1 << (_nGroup - 1)));
}


void ICQUser::RemoveFromGroup(GroupType g, unsigned short _nGroup)
{
  if (_nGroup == 0) return;
  SetGroups(g, GetGroups(g) & (0xFFFFFFFF - (unsigned long)(1 << (_nGroup - 1))));
}


unsigned short ICQUser::getNumUserEvents()
{
  pthread_mutex_lock(&mutex_nNumUserEvents);
  unsigned short n = s_nNumUserEvents;
  pthread_mutex_unlock(&mutex_nNumUserEvents);
  return n;
}

void ICQUser::incNumUserEvents()
{
  pthread_mutex_lock(&mutex_nNumUserEvents);
  s_nNumUserEvents++;
  pthread_mutex_unlock(&mutex_nNumUserEvents);
}

void ICQUser::decNumUserEvents()
{
  pthread_mutex_lock(&mutex_nNumUserEvents);
  s_nNumUserEvents--;
  pthread_mutex_unlock(&mutex_nNumUserEvents);
}


//=====ICQOwner=================================================================

//-----ICQOwner::constructor----------------------------------------------------
ICQOwner::ICQOwner()
{
  gLog.Info(tr("%sOwner configuration.\n"), L_INITxSTR);
  char szTemp[MAX_LINE_LEN];
  char filename[MAX_FILENAME_LEN];
  m_bException = false;
  m_bSavePassword = true;
  m_szPassword = NULL;
  m_nPDINFO = 0;

  Init(0);
  //SetOnContactList(true);
  m_bOnContactList = true;

  // Get data from the config file
  snprintf(filename, MAX_FILENAME_LEN - 1, "%s/owner.Licq", BASE_DIR);
  filename[MAX_FILENAME_LEN - 1] = '\0';

  // Make sure owner.Licq is mode 0600
  if (chmod(filename, S_IRUSR | S_IWUSR) == -1)
  {
    gLog.Warn(tr("%sUnable to set %s to mode 0600.  Your ICQ password is vulnerable.\n"),
                 L_WARNxSTR, filename);
  }

  m_fConf.SetFileName(filename);
  LoadInfo();
  m_fConf.ReadNum("Uin", m_nUin, 0);
  snprintf(m_szUinString, 12, "%lu", m_nUin);
  m_szUinString[12] = '\0';
  if (m_szId)  free (m_szId);
  m_szId = strdup(m_szUinString);
  m_fConf.ReadStr("Password", szTemp, "", false);
  SetPassword(&szTemp[1]); // skip leading space since we didn't trim
  m_fConf.ReadBool("WebPresence", m_bWebAware, false);
  m_fConf.ReadBool("HideIP", m_bHideIp, false);
  m_fConf.ReadNum("RCG", m_nRandomChatGroup, ICQ_RANDOMxCHATxGROUP_NONE);
  m_fConf.ReadStr("AutoResponse", szTemp, "");
  m_fConf.ReadNum("SSTime", m_nSSTime, 0L);
  m_fConf.ReadNum("SSCount", m_nSSCount, 0);
  m_fConf.ReadNum("PDINFO", m_nPDINFO, 0);

  SetAutoResponse(szTemp);

  m_fConf.CloseFile();

  snprintf(filename, MAX_FILENAME_LEN - 1, "%s/%s/owner.history", BASE_DIR, HISTORY_DIR);
  SetHistoryFile(filename);

  if (m_nTimezone != SystemTimezone() && m_nTimezone != TIMEZONE_UNKNOWN)
  {
    gLog.Warn(tr("%sCurrent Licq GMT offset (%d) does not match system GMT offset (%d).\n"
              "%sUpdate general info on server to fix.\n"),
       L_WARNxSTR, m_nTimezone, SystemTimezone(), L_BLANKxSTR);
  }

  //if owner timestamps do not exist, set them to current time
  unsigned long nTime = time(NULL);

/*  if (m_nClientTimestamp == 0)
    m_nClientTimestamp = nTime;*/
  if (m_nClientInfoTimestamp == 0)
    m_nClientInfoTimestamp = nTime;
  if (m_nClientStatusTimestamp == 0)
    m_nClientStatusTimestamp = nTime;

  SetEnableSave(true);
}

ICQOwner::ICQOwner(const char *_szId, unsigned long _nPPID)
{
  char szTemp[MAX_LINE_LEN];
  char filename[MAX_FILENAME_LEN];
  m_bException = false;
  m_bSavePassword = true;
  m_szPassword = NULL;
  m_nPDINFO = 0;

  // Get data from the config file
  char *p = PPIDSTRING(_nPPID);
  snprintf(filename, MAX_FILENAME_LEN - 1, "%s/owner.%s", BASE_DIR, p);
  filename[MAX_FILENAME_LEN - 1] = '\0';

  // Make sure owner.Licq is mode 0600
  if (chmod(filename, S_IRUSR | S_IWUSR) == -1)
  {
    gLog.Warn(tr("%sUnable to set %s to mode 0600.  Your ICQ password is vulnerable.\n"),
                 L_WARNxSTR, filename);
  }

  // Get the id before init
  m_fConf.SetFileName(filename);
  m_fConf.SetFlags(INI_FxWARN | INI_FxALLOWxCREATE);
  m_fConf.ReloadFile();
  m_fConf.SetFlags(0);
  m_fConf.SetSection("user");
  m_fConf.ReadStr("Uin", szTemp, "", true);

  // Now we can init
  Init(_szId, _nPPID);
  m_bOnContactList = true;

  // And finally our favorite function
  LoadInfo();
  m_fConf.ReadStr("Password", szTemp, "", false);
  SetPassword(&szTemp[1]); // skip leading space since we didn't trim
  m_fConf.ReadBool("WebPresence", m_bWebAware, false);
  m_fConf.ReadBool("HideIP", m_bHideIp, false);
  m_fConf.ReadNum("RCG", m_nRandomChatGroup, ICQ_RANDOMxCHATxGROUP_NONE);
  m_fConf.ReadStr("AutoResponse", szTemp, "");
  m_fConf.ReadNum("SSTime", m_nSSTime, 0L);
  m_fConf.ReadNum("SSCount", m_nSSCount, 0);
  m_fConf.ReadNum("PDINFO", m_nPDINFO, 0);
  
  SetAutoResponse(szTemp);

  m_fConf.CloseFile();

  gLog.Info(tr("%sOwner configuration for %s.\n"), L_INITxSTR, m_szId);

  snprintf(filename, MAX_FILENAME_LEN - 1, "%s/%s/owner.%s.%s.history", BASE_DIR, HISTORY_DIR,
           m_szId, p);
    SetHistoryFile(filename);

  if (m_nTimezone != SystemTimezone() && m_nTimezone != TIMEZONE_UNKNOWN)
  {
    gLog.Warn(tr("%sCurrent Licq GMT offset (%d) does not match system GMT offset (%d).\n"
              "%sUpdate general info on server to fix.\n"),
       L_WARNxSTR, m_nTimezone, SystemTimezone(), L_BLANKxSTR);
  }
  SetEnableSave(true);

  delete [] p;
}

ICQOwner::~ICQOwner()
{
  // Save the current auto response
  if (!m_fConf.ReloadFile())
  {
     gLog.Error("%sError opening '%s' for reading.\n%sSee log for details.\n",
                L_ERRORxSTR, m_fConf.FileName(), L_BLANKxSTR);
     return;
  }
  m_fConf.SetSection("user");
  m_fConf.WriteStr("AutoResponse", AutoResponse());
  m_fConf.WriteNum("SSTime", m_nSSTime);
  m_fConf.WriteNum("SSCount", m_nSSCount);
  m_fConf.WriteNum("PDINFO", m_nPDINFO);
  if (!m_fConf.FlushFile())
  {
    gLog.Error("%sError opening '%s' for writing.\n%sSee log for details.\n",
               L_ERRORxSTR, m_fConf.FileName(), L_BLANKxSTR);
    return;
  }
  m_fConf.CloseFile();

  if ( m_szPassword )
    free( m_szPassword );
}



unsigned long ICQOwner::AddStatusFlags(unsigned long s)
{
  s &= 0x0000FFFF;

  if (WebAware())
    s |= ICQ_STATUS_FxWEBxPRESENCE;
  if (HideIp())
    s |= ICQ_STATUS_FxHIDExIP;
  if (Birthday() == 0)
    s |= ICQ_STATUS_FxBIRTHDAY;
  if (PhoneFollowMeStatus() != ICQ_PLUGIN_STATUSxINACTIVE)
    s |= ICQ_STATUS_FxPFM;
  if (PhoneFollowMeStatus() == ICQ_PLUGIN_STATUSxACTIVE)
    s |= ICQ_STATUS_FxPFMxAVAILABLE;

  return s;
}


//-----ICQOwner::saveInfo--------------------------------------------------------
void ICQOwner::SaveLicqInfo()
{
  if (!EnableSave()) return;
  
  ICQUser::SaveLicqInfo();
  
  if (!m_fConf.ReloadFile())
  {
     gLog.Error("%sError opening '%s' for reading.\n%sSee log for details.\n",
                L_ERRORxSTR, m_fConf.FileName(), L_BLANKxSTR);
     return;
  }
  m_fConf.SetSection("user");
  m_fConf.WriteStr("Uin", IdString());
  m_fConf.WriteBool("WebPresence", WebAware());
  m_fConf.WriteBool("HideIP", HideIp());
  m_fConf.WriteBool("Authorization", GetAuthorization());
  m_fConf.WriteNum("RCG", RandomChatGroup());
  m_fConf.WriteNum("SSTime", m_nSSTime);
  m_fConf.WriteNum("SSCount", m_nSSCount);
  m_fConf.WriteNum("PDINFO", m_nPDINFO);
  
  if (m_bSavePassword)
    m_fConf.WriteStr("Password", Password());
  else
    m_fConf.WriteStr("Password", "");

  if (!m_fConf.FlushFile())
  {
    gLog.Error("%sError opening '%s' for writing.\n%sSee log for details.\n",
               L_ERRORxSTR, m_fConf.FileName(), L_BLANKxSTR);
    return;
  }

  m_fConf.CloseFile();
}

void ICQOwner::SetStatusOffline()
{
  SetStatus(m_nStatus | ICQ_STATUS_OFFLINE);
}

//----ICQOwner::SetPicture---------------------------------------------------
void ICQOwner::SetPicture(const char *f)
{
  char szFilename[MAX_FILENAME_LEN];
  szFilename[MAX_FILENAME_LEN - 1] = '\0';
  snprintf(szFilename, MAX_FILENAME_LEN - 1, "%s/owner.pic", BASE_DIR);
  if (f == NULL)
  {
    SetPicturePresent(false);
    if (remove(szFilename) != 0 && errno != ENOENT)
    {
      gLog.Error("%sUnable to delete %s's picture file (%s):\n%s%s.\n",
                         L_ERRORxSTR, m_szAlias, szFilename, L_BLANKxSTR,
                         strerror(errno));
    }
  }
  else if (strcmp(f, szFilename) == 0)
  {
    SetPicturePresent(true);
    return;
  }
  else
  {
    int source = open(f, O_RDONLY);
    if (source == -1)
    {
      gLog.Error("%sUnable to open source picture file (%s):\n%s%s.\n",
                       L_ERRORxSTR, f, L_BLANKxSTR, strerror(errno));
      return;
    }

    int dest = open(szFilename, O_WRONLY | O_CREAT | O_TRUNC, 00664);
    if (dest == -1)
    {
      gLog.Error("%sUnable to open picture file (%s):\n%s%s.\n", L_ERRORxSTR,
                                     szFilename, L_BLANKxSTR, strerror(errno));
      return;
    }

    char buf[8192];
    ssize_t s;
    while (1)
    {
      s = read(source, buf, sizeof(buf));
      if (s == 0)
      {
        SetPicturePresent(true);
        break;
      }
      else if (s == -1)
      {
        gLog.Error("%sError reading from %s:\n%s%s.\n", L_ERRORxSTR, f,
                                         L_BLANKxSTR, strerror(errno));
        SetPicturePresent(false);
        break;
      }

      if (write(dest, buf, s) != s)
      {
        gLog.Error("%sError writing to %s:\n%s%s.\n", L_ERRORxSTR, f,
                                         L_BLANKxSTR, strerror(errno));
        SetPicturePresent(false);
        break;
      }
    }

    close(source);
    close(dest);
  }
}
 
