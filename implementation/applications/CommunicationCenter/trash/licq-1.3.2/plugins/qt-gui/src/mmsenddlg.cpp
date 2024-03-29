// -*- c-basic-offset: 2 -*-
/*
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or   
    (at your option) any later version.                              
                                       
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of 
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
    GNU General Public License for more details.                 
                                                
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software      
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  

*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <qpushbutton.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qprogressbar.h>
#include <qvgroupbox.h>
#include <qtextcodec.h>
#include <qregexp.h>

#include <unistd.h>
#include <stdio.h>

#include "mmsenddlg.h"
#include "mmlistview.h"
#include "sigman.h"
#include "licq_user.h"
#include "licq_translate.h"
#include "licq_icqd.h"
#include "licq_events.h"
#include "usercodec.h"

CMMSendDlg::CMMSendDlg(CICQDaemon *_server, CSignalManager *sigman,
  CMMUserView *_mmv, QWidget *p)
  : LicqDialog(p, "MMSendDialog", true, WDestructiveClose)
{
  m_nUin = 0;
  m_szId = 0;
  m_nPPID = 0;
  icqEventTag = 0;
  mmv = _mmv;
  server = _server;

  QVBoxLayout *v = new QVBoxLayout(this, 10, 5);

  grpSending = new QVGroupBox(this);
  barSend = new QProgressBar(grpSending);

  btnCancel = new QPushButton(tr("&Cancel"), this);
  btnCancel->setFixedWidth(btnCancel->sizeHint().width());

  v->addWidget(grpSending);
  v->addWidget(btnCancel);

  connect(btnCancel, SIGNAL(clicked()), SLOT(slot_cancel()));
  connect(sigman, SIGNAL(signal_doneUserFcn(ICQEvent *)), SLOT(slot_done(ICQEvent *)));

  mmvi = (CMMUserViewItem *)mmv->firstChild();
  barSend->setTotalSteps(mmv->childCount());
  barSend->setProgress(0);

  setMinimumWidth(300);
}


int CMMSendDlg::go_message(QString msg)
{
  m_nEventType = ICQ_CMDxSUB_MSG;
  s1 = msg;

  setCaption(tr("Multiple Recipient Message"));

  // Start
  SendNext();
  show();
  return 0;
}


int CMMSendDlg::go_url(QString url, QString desc)
{
  m_nEventType = ICQ_CMDxSUB_URL;
  s1 = desc;
  s2 = url;

  setCaption(tr("Multiple Recipient URL"));

  // Start
  SendNext();
  show();
  return result();
}


int CMMSendDlg::go_contact(UserStringList &_users)
{
  m_nEventType = ICQ_CMDxSUB_CONTACTxLIST;
  users = &_users;

  setCaption(tr("Multiple Recipient Contact List"));

  // Start
  SendNext();
  show();
  return result();
}




void CMMSendDlg::slot_done(ICQEvent *e)
{
  if ( !e->Equals(icqEventTag) )
    return;

  bool isOk = (e != NULL && e->Result() == EVENT_ACKED);

  icqEventTag = 0;

  if (!isOk)
  {
    grpSending->setTitle(grpSending->title() + tr("failed"));
    btnCancel->setText(tr("&Close"));
    return;
  }

  // Send next message
  barSend->setProgress(barSend->progress() + 1);
  CMMUserViewItem *i = mmvi;
  mmvi = (CMMUserViewItem *)mmvi->nextSibling();
  delete i;

  SendNext();
}


void CMMSendDlg::SendNext()
{
  if (mmvi == NULL)
  {
    accept();
    return;
  }

  if (m_szId)
  {
    free(m_szId);
    m_szId = 0;
  }

  m_szId = mmvi->Id() ? strdup(mmvi->Id()) : 0;
  m_nPPID = mmvi->PPID();

  if (m_szId == 0) return;

  switch (m_nEventType)
  {
    case ICQ_CMDxSUB_MSG:
    {
      ICQUser *u = gUserManager.FetchUser(m_szId, m_nPPID, LOCK_R);
      if (u == NULL) return;
      QTextCodec * codec = UserCodec::codecForICQUser(u);
      grpSending->setTitle(tr("Sending mass message to %1...").arg(QString::fromUtf8(u->GetAlias())));
      gUserManager.DropUser(u);

      // create initial strings (implicit copying, no allocation impact :)
      char *tmp = gTranslator.NToRN(codec->fromUnicode(s1));
      QCString wholeMessageRaw(tmp);
      delete [] tmp;
      unsigned int wholeMessagePos = 0;

      bool needsSplitting = false;
      // If we send through server (= have message limit), and we've crossed the limit
      if ((wholeMessageRaw.length() - wholeMessagePos) > MAX_MESSAGE_SIZE)
      {
        needsSplitting = true;
      }

      QString message;
      QCString messageRaw;

      while (wholeMessageRaw.length() > wholeMessagePos)
      {
        if (needsSplitting)
        {
          // This is a bit ugly but adds safety. We don't simply search
          // for a whitespace to cut at in the encoded text (since we don't
          // really know how spaces are represented in its encoding), so
          // we take the maximum length, then convert back to a Unicode string
          // and then search for Unicode whitespaces.
          messageRaw = wholeMessageRaw.mid(wholeMessagePos, MAX_MESSAGE_SIZE);
          tmp = gTranslator.RNToN(messageRaw);
          messageRaw = tmp;
          delete [] tmp;
          message = codec->toUnicode(messageRaw);

          if ((wholeMessageRaw.length() - wholeMessagePos) > MAX_MESSAGE_SIZE)
          {
            // We try to find the optimal place to cut
            // (according to our narrow-minded Latin1 idea of optimal :)
            // prefer keeping sentences intact 1st
            int foundIndex = message.findRev(QRegExp("[\\.\\n]"));
            // slicing at 0 position would be useless
            if (foundIndex <= 0)
              foundIndex = message.findRev(QRegExp("\\s"));

            if (foundIndex > 0)
            {
              message.truncate(foundIndex);
              messageRaw = codec->fromUnicode(message);
            }
          }
        }
        else
        {
          messageRaw = codec->fromUnicode(s1);
        }

        icqEventTag = server->ProtoSendMessage(m_szId, m_nPPID,
          messageRaw.data(), false, ICQ_TCPxMSG_NORMAL, true);

        tmp = gTranslator.NToRN(messageRaw);
        wholeMessagePos += strlen(tmp);
        delete [] tmp;
      }

      break;
    }
    case ICQ_CMDxSUB_URL:
    {
      ICQUser *u = gUserManager.FetchUser(m_szId, m_nPPID, LOCK_R);
      if (u == NULL) return;
      grpSending->setTitle(tr("Sending mass URL to %1...").arg(QString::fromUtf8(u->GetAlias())));
      QTextCodec *codec = UserCodec::codecForICQUser(u);
      gUserManager.DropUser(u);

      icqEventTag = server->ProtoSendUrl(m_szId, m_nPPID, s2.latin1(), codec->fromUnicode(s1),
        false, ICQ_TCPxMSG_NORMAL, true);
      break;
    }
    case ICQ_CMDxSUB_CONTACTxLIST:
    {
      ICQUser *u = gUserManager.FetchUser(m_szId, m_nPPID, LOCK_R);
      if (u == NULL) return;
      grpSending->setTitle(tr("Sending mass list to %1...").arg(QString::fromUtf8(u->GetAlias())));
      gUserManager.DropUser(u);

      icqEventTag = server->icqSendContactList(m_szId, *users, false,
        ICQ_TCPxMSG_NORMAL);
      break;
    }
  }

  if (icqEventTag == 0) slot_done(NULL);
}


CMMSendDlg::~CMMSendDlg()
{
  if (icqEventTag != 0)
  {
    server->CancelEvent(icqEventTag);
    icqEventTag = 0;
  }

  if (m_szId) free(m_szId);
}


void CMMSendDlg::slot_cancel()
{
  if (icqEventTag != 0)
  {
    server->CancelEvent(icqEventTag);
    icqEventTag = 0;
  }
  //disconnect (sigman, SIGNAL(signal_doneUserFcn(ICQEvent *)), this, SLOT(slot_done(ICQEvent *)));

  reject();
}



#include "mmsenddlg.moc"
