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

// written by Graham Roff <graham@licq.org>
// enhanced by Dirk A. Mueller <dmuell@gmx.net>
// -----------------------------------------------------------------------------

#include <qcheckbox.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qtextcodec.h>
#include <qregexp.h>

#include "licq_icqd.h"
#include "mledit.h"
#include "showawaymsgdlg.h"
#include "sigman.h"
#include "licq_user.h"
#include "usercodec.h"
#include <ctype.h>

// -----------------------------------------------------------------------------

ShowAwayMsgDlg::ShowAwayMsgDlg(CICQDaemon *_server, CSignalManager* _sigman,
  const char *szId, unsigned long nPPID, QWidget *parent)
  : LicqDialog(parent, "ShowAwayMessageDialog")
{
  m_szId = szId ? strdup(szId) : 0;
  m_nPPID = nPPID;
  sigman = _sigman;
  server = _server;

  QBoxLayout* top_lay = new QVBoxLayout(this, 10);
  mleAwayMsg = new MLEditWrap(true, this);
  // ICQ99b allows 37 chars per line, so we do the same
  //mleAwayMsg->setWordWrap(QMultiLineEditNew::FixedColumnWidth);
  //mleAwayMsg->setWrapColumnOrWidth(37);
  mleAwayMsg->setReadOnly(true);
  mleAwayMsg->setMinimumSize(230, 110);
  connect(mleAwayMsg, SIGNAL(signal_CtrlEnterPressed()), SLOT(accept()));
  top_lay->addWidget(mleAwayMsg);

  QBoxLayout* lay = new QHBoxLayout(top_lay, 10);

  chkShowAgain = new QCheckBox(tr("&Show Again"), this);
  lay->addWidget(chkShowAgain);

  lay->addStretch(1);
  lay->addSpacing(30);

  ICQUser *u = gUserManager.FetchUser(m_szId, m_nPPID, LOCK_R);
  QTextCodec * codec = UserCodec::codecForICQUser(u);
//  chkShowAgain->setChecked(u->ShowAwayMsg());

  setCaption(QString(tr("%1 Response for %2")).arg(u->StatusStr()).arg(QString::fromUtf8(u->GetAlias())));

  btnOk = new QPushButton(tr("&Ok"), this);
  btnOk->setMinimumWidth(75);
  btnOk->setDefault(true);
  btnOk->setFocus();
  connect(btnOk, SIGNAL(clicked()), SLOT(accept()));
  lay->addWidget(btnOk);

  // Check if this is an active request or not
  if (sigman == NULL || server == NULL)
  {
    mleAwayMsg->setText(codec->toUnicode(u->AutoResponse()));
    gUserManager.DropUser(u);
    icqEventTag = 0;
  }
  else
  {
    bool bSendServer = (u->SocketDesc(ICQ_CHNxNONE) <= 0 && u->Version() > 6);
    gUserManager.DropUser(u);
    mleAwayMsg->setEnabled(false);
    mleAwayMsg->setBackgroundMode(PaletteBackground);
    connect (sigman, SIGNAL(signal_doneUserFcn(ICQEvent *)), this, SLOT(doneEvent(ICQEvent *)));
    icqEventTag = server->icqFetchAutoResponse(szId, nPPID, bSendServer);
  }

  show();
}

ShowAwayMsgDlg::ShowAwayMsgDlg(CICQDaemon *_server, CSignalManager* _sigman, unsigned long _nUin, QWidget *parent)
{
  char szUin[13];
  snprintf(szUin, 12, "%lu", _nUin);
  szUin[12] = 0;
  
  ShowAwayMsgDlg(_server, _sigman, szUin, LICQ_PPID, parent);
}


// -----------------------------------------------------------------------------

ShowAwayMsgDlg::~ShowAwayMsgDlg()
{
  if (m_szId) free(m_szId);
}


// -----------------------------------------------------------------------------

void ShowAwayMsgDlg::accept()
{
  ICQUser *u = gUserManager.FetchUser(m_szId, m_nPPID, LOCK_W);
  u->SetShowAwayMsg(chkShowAgain->isChecked());
  gUserManager.DropUser(u);

  if (server != NULL && icqEventTag != 0)
  {
    server->CancelEvent(icqEventTag);
    icqEventTag = 0;
  }

  QDialog::accept();
}


// -----------------------------------------------------------------------------

void ShowAwayMsgDlg::doneEvent(ICQEvent *e)
{
  if ( !e->Equals(icqEventTag) )
    return;

  bool isOk = (e->Result() == EVENT_ACKED || e->Result() == EVENT_SUCCESS);

  QString title, result;
  if (e->ExtendedAck() && !e->ExtendedAck()->Accepted())
    result = tr("refused");
  else
  {
    switch (e->Result())
    {
    case EVENT_FAILED:
      result = tr("failed");
      break;
    case EVENT_TIMEDOUT:
      result = tr("timed out");
      break;
    case EVENT_ERROR:
      result = tr("error");
      break;
    default:
      break;
    }
  }

  if(!result.isEmpty())
  {
    title = " [" + result + "]";
    setCaption(caption() + title);
  }

  icqEventTag = 0;

  if (isOk && (e->Command() == ICQ_CMDxTCP_START ||
               e->SNAC() == MAKESNAC(ICQ_SNACxFAM_MESSAGE, ICQ_SNACxMSG_SENDxSERVER) ||
                e->SNAC() == MAKESNAC(ICQ_SNACxFAM_LOCATION, ICQ_SNACxLOC_INFOxREQ)))
  {
    ICQUser *u = gUserManager.FetchUser(m_szId, m_nPPID, LOCK_R);
    QTextCodec * codec = UserCodec::codecForICQUser(u);
    const char *szAutoResp = (e->ExtendedAck() && !e->ExtendedAck()->Accepted())?
                              e->ExtendedAck()->Response() : u->AutoResponse();
    if (m_nPPID == LICQ_PPID && isalpha(m_szId[0]))
    {
      // Strip HTML
      QString strResponse(codec->toUnicode(szAutoResp));
      QRegExp regExp("<.*>");
      regExp.setMinimal(true);
      strResponse.remove(regExp);
      mleAwayMsg->setText(strResponse);
    }
    else
      mleAwayMsg->setText(codec->toUnicode(szAutoResp));
    
    gUserManager.DropUser(u);
    mleAwayMsg->setEnabled(true);
    mleAwayMsg->setBackgroundMode(PaletteBase);
  }
}

#include "showawaymsgdlg.moc"
